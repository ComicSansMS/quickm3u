#include <quickm3u/ui/m3u_file_model.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251 5054)
#endif
#include <QDataStream>
#include <QMessageBox>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include <QMimeData>

#include <limits>
#include <unordered_set>

namespace ui {

M3UFileModel::M3UFileModel(QObject* parent)
    :QAbstractListModel(parent), m_lastInsertIndex(-1)
{}

Qt::ItemFlags M3UFileModel::flags(QModelIndex const& /* index */) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
        Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemNeverHasChildren;
}

int M3UFileModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_file.entries.size());
}

QVariant M3UFileModel::data(QModelIndex const& index, int role) const
{
    int const row_index = index.row();
    if (index.isValid() && (row_index < static_cast<int>(m_file.entries.size()))) {
        if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
            return QString::fromStdU16String(m_file.entries[row_index].path.u16string());
        }
    }
    return {};
}

bool M3UFileModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    int const row_index = index.row();
    if (index.isValid() && (row_index < static_cast<int>(m_file.entries.size()))) {
        if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
            if (m_lastInsertIndex == index.row()) {
                // call is part of a drag-n-drop action. drop state previously saved by insertRows().
                m_undoHistory.pop_back();
            }
            saveState();
            m_file.entries[row_index] = M3UEntry{};
            m_file.entries[row_index].path = value.toString().toStdU16String();
            m_file.entries[row_index].path.make_preferred();
            emit dataChanged(index, index);
        }
    }
    return false;
}

bool M3UFileModel::insertRows(int row, int count, QModelIndex const& parent)
{
    if (parent.isValid() || count < 1 || row < 0 || row > static_cast<int>(m_file.entries.size())) {
        return false;
    }
    if (m_file.entries.size() + count >= static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        // limit number of entries to stay addressable by Qt indices
        return false;
    }

    saveState();
    m_lastInsertIndex = row;
    beginInsertRows(QModelIndex(), row, row + count - 1);
    m_file.entries.insert(begin(m_file.entries) + row, count, M3UEntry{});
    endInsertRows();
    return true;
}

bool M3UFileModel::removeRows(int row, int count, QModelIndex const& parent)
{
    if (parent.isValid() || row < 0 || row + count > static_cast<int>(m_file.entries.size())) {
        return false;
    }

    saveState();
    beginRemoveRows(parent, row, row + count - 1);
    auto const it_erase_begin = begin(m_file.entries) + row;
    auto const it_erase_end = it_erase_begin + count;
    m_file.entries.erase(it_erase_begin, it_erase_end);
    endRemoveRows();
    return true;
}

Qt::DropActions M3UFileModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList M3UFileModel::mimeTypes() const
{
    QStringList types;
    types << QAbstractListModel::mimeTypes();
    types << "application/quickm3u.m3uentry";
    return types;
}

QMimeData* M3UFileModel::mimeData(QModelIndexList const& indices) const
{
    QMimeData* mimeData = new QMimeData;

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    std::size_t n_indices = std::count_if(indices.begin(), indices.end(),
                                          [](QModelIndex const& index) { return index.isValid(); });
    stream << static_cast<quint32>(n_indices);
    for (QModelIndex const& index : indices) {
        if (index.isValid()) {
            stream << static_cast<qint32>(index.row());
        }
    }

    mimeData->setData("application/quickm3u.m3uentry", encodedData);
    return mimeData;
}

bool M3UFileModel::dropMimeData(QMimeData const* data, Qt::DropAction action,
                                int row, int column, QModelIndex const& parent)
{
    if ((action == Qt::CopyAction) || (action == Qt::MoveAction)) {
        if (parent.isValid()) {
            row = parent.row();
        } else if (row == -1) {
            row = static_cast<int>(m_file.entries.size());
        }
        QByteArray raw_data = data->data("application/quickm3u.m3uentry");
        if (!raw_data.isEmpty()) {
            QDataStream stream(&raw_data, QIODevice::ReadOnly);
            quint32 n_indices;
            stream >> n_indices;
            std::vector<int> source_rows(n_indices);
            for (quint32 i = 0; i < n_indices; ++i) {
                qint32 tmp;
                stream >> tmp;
                source_rows[i] = tmp;
            }
            return gatherRows(source_rows.data(), source_rows.size(), row);
        }
        bool ret = QAbstractListModel::dropMimeData(data, action, row, column, QModelIndex{});
        return ret;
    }
    return false;
}

bool M3UFileModel::gatherRows(int* source_rows_ptr, std::size_t source_rows_size, int destination_row)
{
    saveState();
    std::size_t const n_elements = m_file.entries.size();
    std::vector<int> indices(n_elements);
    std::iota(begin(indices), end(indices), 0);
    std::unordered_set<int> source_indices;
    for (std::size_t i = 0; i < source_rows_size; ++i) { source_indices.insert(source_rows_ptr[i]); }
    std::stable_partition(begin(indices), begin(indices) + destination_row,
        [&source_indices](int i) { return !source_indices.contains(i); });
    std::stable_partition(begin(indices) + destination_row, end(indices),
        [&source_indices](int i) { return source_indices.contains(i); });

    /// @todo consider use of beginMoveRows() here
    beginResetModel();
    auto const old_entries = std::move(m_file.entries);
    m_file.entries.clear();
    m_file.entries.reserve(n_elements);
    for (auto const& i : indices) {
        m_file.entries.push_back(std::move(old_entries[i]));
    }
    endResetModel();

    return true;
}

QString M3UFileModel::getFilename() const
{
    return QString::fromStdU16String(m_file.filename.filename().u16string());
}

QString M3UFileModel::getFullPath() const
{
    return QString::fromStdU16String(m_file.filename.u16string());
}

void M3UFileModel::newFile(QString const& path)
{
    beginResetModel();
    m_file = M3UFile{};
    m_file.filename = path.toStdU16String();
    m_file.filename.make_preferred();
    endResetModel();
    m_undoHistory.clear();
    m_redoHistory.clear();
    emit pathChanged();
    emit historyChange(false, false);
}

void M3UFileModel::openFile(QString const& path)
{
    M3UFile loaded_file;
    try {
        loaded_file = m3u_load(path.toStdU16String());
    } catch (...) {
        QMessageBox msg(QMessageBox::Critical, tr("Error opening file."),
            tr("File %1 could not be opened.").arg(path), QMessageBox::Ok);
        msg.exec();
        return;
    }
    if (loaded_file.entries.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        QMessageBox msg(QMessageBox::Critical, tr("Error opening file."),
            tr("File %1 contains too many entries to be opened.").arg(path), QMessageBox::Ok);
        msg.exec();
        return;
        return;
    }
    beginResetModel();
    m_file = std::move(loaded_file);
    endResetModel();
    m_undoHistory.clear();
    m_redoHistory.clear();
    emit pathChanged();
    emit historyChange(false, false);
}

void M3UFileModel::saveFile()
{
    try {
        m3u_save(m_file);
    } catch (...) {
        QMessageBox msg(QMessageBox::Critical, tr("Error saving file."),
            tr("File %1 could not be saved.").arg(m_file.filename.u16string()), QMessageBox::Ok);
        msg.exec();
        return;
    }
}

void M3UFileModel::convertToRelativePaths()
{
    saveState();
    beginResetModel();
    m3u_convert_to_relative_paths(m_file);
    endResetModel();
}

void M3UFileModel::convertToAbsolutePaths()
{
    saveState();
    beginResetModel();
    m3u_convert_to_absolute_paths(m_file);
    endResetModel();
}

void M3UFileModel::undo()
{
    m_redoHistory.emplace_back(std::move(m_file));
    beginResetModel();
    m_file = std::move(m_undoHistory.back());
    endResetModel();
    m_undoHistory.pop_back();
    emit historyChange(!m_undoHistory.empty(), true);
}

void M3UFileModel::redo()
{
    m_undoHistory.emplace_back(std::move(m_file));
    beginResetModel();
    m_file = std::move(m_redoHistory.back());
    endResetModel();
    m_redoHistory.pop_back();
    emit historyChange(true, !m_redoHistory.empty());
}

void M3UFileModel::copyFilesToDirectory(QString destination_path)
{
    auto file = m_file;
    m3u_convert_to_absolute_paths(file);
    int const n_files = static_cast<int>(file.entries.size());
    int n_digits = [](int n) {
        int digits = 0;
        while (n > 0) {
            n /= 10;
            ++digits;
        }
        return digits;
    }(n_files);
    int count = 0;
    std::filesystem::path dest_dir = destination_path.toStdU16String();
    std::vector<std::tuple<std::filesystem::path, std::filesystem::path>> to_copy;
    bool overwrite = false;
    bool skipped = false;
    for (auto const& f : file.entries) {
        auto const destination_file = dest_dir /
            QString("%1 %2").arg(count, n_digits, 10, QLatin1Char('0')).arg(f.path.filename().u16string()).toStdU16String();
        if (std::filesystem::exists(destination_file)) {
            overwrite = true;
        }
        if (!std::filesystem::is_regular_file(f.path)) {
            skipped = true;
        } else {
            to_copy.emplace_back(f.path, destination_file);
        }
        ++count;
    }
    if (overwrite) {
        QMessageBox msg(QMessageBox::Question, tr("Some files will get overwritten."),
            tr("Some existing files in the destination directory will get overwritten.\nDo you want to continue?"),
            QMessageBox::Yes | QMessageBox::No);
        if (msg.exec() == QMessageBox::No) {
            return;
        }
    }
    if (skipped) {
        QMessageBox msg(QMessageBox::Question, tr("Some files cannot be copied."),
            tr("Some entries cannot be copied.\nDo you want to continue?"),
            QMessageBox::Yes | QMessageBox::No);
        if (msg.exec() == QMessageBox::No) {
            return;
        }
    }
    bool errors_occurred = false;
    for (auto const& [source, dest] : to_copy) {
        std::error_code ec;
        std::filesystem::copy_file(source, dest, std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) {
            errors_occurred = true;
        }
    }
    if (errors_occurred) {
        QMessageBox msg(QMessageBox::Warning, tr("Some files could not be copied."),
            tr("Some files could not be copied to the destination."), QMessageBox::Ok);
        msg.exec();
    } else {
        QMessageBox msg(QMessageBox::Information, tr("All files have been copied."),
            tr("All files have been copied successfully."), QMessageBox::Ok);
        msg.exec();
    }
}

void M3UFileModel::saveState()
{
    if (!m_undoHistory.empty() && (m_file == m_undoHistory.back())) {
        return;
    }
    m_lastInsertIndex = -1;
    m_undoHistory.push_back(m_file);
    m_redoHistory.clear();
    emit historyChange(true, false);
}
}

#include <quickm3u/ui/m3u_file_model.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QDataStream>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include <QMimeData>

#include <unordered_set>

namespace ui {

M3UFileModel::M3UFileModel(QObject* parent)
    :QAbstractListModel(parent)
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
    if (index.isValid() && (row_index < m_file.entries.size())) {
        if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
            return QString::fromStdU16String(m_file.entries[row_index].path.u16string());
        }
    }
    return {};
}

bool M3UFileModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    int const row_index = index.row();
    if (index.isValid() && (row_index < m_file.entries.size())) {
        if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
            m_file.entries[row_index] = M3UEntry{};
            m_file.entries[row_index].path = value.toString().toStdU16String();
            emit dataChanged(index, index);
        }
    }
    return false;
}

bool M3UFileModel::insertRows(int row, int count, QModelIndex const& parent)
{
    if (parent.isValid() || count < 1 || row < 0 || row > m_file.entries.size()) {
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);
    m_file.entries.insert(begin(m_file.entries) + row, count, M3UEntry{});
    endInsertRows();
    return true;
}

bool M3UFileModel::removeRows(int row, int count, QModelIndex const& parent)
{
    if (parent.isValid() || row < 0 || row + count > m_file.entries.size()) {
        return false;
    }
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
    stream << n_indices;
    for (QModelIndex const& index : indices) {
        if (index.isValid()) {
            stream << index.row();
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
            std::size_t n_indices;
            stream >> n_indices;
            std::vector<int> source_rows(n_indices);
            for (int i = 0; i < n_indices; ++i) {
                stream >> source_rows[i];
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
    emit pathChanged();
}

void M3UFileModel::openFile(QString const& path)
{
    beginResetModel();
    m_file = m3u_load(path.toStdU16String());
    endResetModel();
    emit pathChanged();
}

void M3UFileModel::saveFile()
{
    m3u_save(m_file);
}

void M3UFileModel::convertToRelativePaths()
{
    beginResetModel();
    m3u_convert_to_relative_paths(m_file);
    endResetModel();
}

void M3UFileModel::convertToAbsolutePaths()
{
    beginResetModel();
    m3u_convert_to_absolute_paths(m_file);
    endResetModel();
}

}

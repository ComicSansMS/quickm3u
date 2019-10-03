#include <quickm3u/ui/m3u_file_model.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QDataStream>
#pragma warning(pop)
#include <QMimeData>

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
    for (QModelIndex const& index : indices) {
        if (index.isValid()) {
            M3UEntry const& entry = m_file.entries[index.row()];
            stream << QString::fromStdU16String(entry.path.u16string());
            stream << entry.runtime.count();
            stream << QString::fromStdString(entry.artist);
            stream << QString::fromStdString(entry.title);
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
            row = m_file.entries.size();
        }
        bool ret = QAbstractListModel::dropMimeData(data, action, row, column, QModelIndex{});
        return ret;
    }
    return false;
}

void M3UFileModel::openFile(QString const& path)
{
    beginResetModel();
    m_file = m3u_load(path.toStdU16String());
    endResetModel();
}

void M3UFileModel::newFile()
{
    beginResetModel();
    m_file = M3UFile{};
    endResetModel();
}

}

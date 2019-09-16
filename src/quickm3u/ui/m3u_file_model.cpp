#include <quickm3u/ui/m3u_file_model.hpp>

namespace ui {

M3UFileModel::M3UFileModel(QObject* parent)
    :QAbstractListModel(parent)
{}

int M3UFileModel::rowCount(QModelIndex const& index) const
{
    return {};
}

QVariant M3UFileModel::data(QModelIndex const& index, int role) const
{
    return {};
}

void M3UFileModel::openFile(QString const& path)
{
    m_file = m3u_load(path.toStdU16String());
}

}

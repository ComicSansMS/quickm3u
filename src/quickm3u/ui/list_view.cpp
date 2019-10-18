#include <quickm3u/ui/list_view.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QKeyEvent>
#pragma warning(pop)
#include <QMimeData>

#include <algorithm>

namespace ui {

ListView::ListView(QWidget* parent)
    :QListView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void ListView::keyPressEvent(QKeyEvent* evt)
{
    if (evt->key() == Qt::Key_Delete) {
        removeSelectedItems();
    }
    return QListView::keyPressEvent(evt);
}

void ListView::removeSelectedItems()
{
    QModelIndexList indices = selectedIndexes();
    std::sort(indices.begin(), indices.end(), [](QModelIndex const& lhs, QModelIndex const& rhs) {
        return lhs.row() < rhs.row();
    });
    for (auto it = indices.rbegin(); it != indices.rend(); ++it) {
        model()->removeRow(it->row());
    }
}

void ListView::invertSelection()
{
    /// @todo
    QItemSelectionModel* const smodel = selectionModel();
    QItemSelection selection = smodel->selection();
    QAbstractItemModel* const imodel = model();
    selection.select(imodel->index(0, 0), imodel->index(imodel->rowCount(), 0));
    smodel->select(selection, QItemSelectionModel::SelectionFlag::Select);
}

void ListView::dragEnterEvent(QDragEnterEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        evt->acceptProposedAction();
    } else {
        return QListView::dragEnterEvent(evt);
    }
}

void ListView::dragMoveEvent(QDragMoveEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        evt->acceptProposedAction();
    } else {
        return QListView::dragMoveEvent(evt);
    }
}

void ListView::dragLeaveEvent(QDragLeaveEvent* evt)
{
    return QListView::dragLeaveEvent(evt);
}

void ListView::dropEvent(QDropEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        for (auto const& url : evt->mimeData()->urls()) {
            addItem(url.toLocalFile());
        }
    }
    return QListView::dropEvent(evt);
}

void ListView::addItem(QString item)
{
    QAbstractItemModel* m = model();
    int const row = m->rowCount();
    m->insertRow(row);
    m->setData(m->index(row, 0), item, Qt::DisplayRole);
}

}

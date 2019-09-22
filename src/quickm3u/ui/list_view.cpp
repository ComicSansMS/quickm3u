#include <quickm3u/ui/list_view.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QKeyEvent>
#pragma warning(pop)
#include <QMimeData>

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
    /// @todo
    /*
    for (QListViewItem* to_remove : selectedItems()) {
        delete to_remove;
    }
    */
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
        /// @todo
        for (auto const& url : evt->mimeData()->urls()) {
            //addItem(url.toLocalFile());
        }
    }
    return QListView::dropEvent(evt);
}

}

#include <quickm3u/ui/list_widget.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QKeyEvent>
#pragma warning(pop)
#include <QMimeData>

namespace ui {

ListWidget::ListWidget(QWidget* parent)
    :QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    addItem("Foo");
    addItem("Bar");
    addItem("Baz");
}

void ListWidget::keyPressEvent(QKeyEvent* evt)
{
    if (evt->key() == Qt::Key_Delete) {
        removeSelectedItems();
    }
    return QListWidget::keyPressEvent(evt);
}

void ListWidget::removeSelectedItems()
{
    for (QListWidgetItem* to_remove : selectedItems()) {
        delete to_remove;
    }
}

void ListWidget::dragEnterEvent(QDragEnterEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        evt->acceptProposedAction();
    } else {
        return QListWidget::dragEnterEvent(evt);
    }
}

void ListWidget::dragMoveEvent(QDragMoveEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        evt->acceptProposedAction();
    } else {
        return QListWidget::dragMoveEvent(evt);
    }
}

void ListWidget::dragLeaveEvent(QDragLeaveEvent* evt)
{
    return QListWidget::dragLeaveEvent(evt);
}

void ListWidget::dropEvent(QDropEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        for (auto const& url : evt->mimeData()->urls()) {
            addItem(url.toLocalFile());
        }
    }
    return QListWidget::dropEvent(evt);
}

}

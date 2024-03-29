#ifndef INCLUDE_GUARD_QUICKM3U_UI_LIST_WIDGET_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_LIST_WIDGET_HPP_

#pragma warning(push)
#pragma warning(disable: 4251 5054)
#include <QListWidget>
#pragma warning(pop)

namespace ui {

class ListWidget : public QListWidget {
    Q_OBJECT
private:
public:
    ListWidget(QWidget* parent);

    void keyPressEvent(QKeyEvent* evt) override;

    void removeSelectedItems();

protected:
    void dragEnterEvent(QDragEnterEvent* evt) override;
    void dragMoveEvent(QDragMoveEvent* evt) override;
    void dragLeaveEvent(QDragLeaveEvent* evt) override;
    void dropEvent(QDropEvent* evt) override;
};

}

#endif

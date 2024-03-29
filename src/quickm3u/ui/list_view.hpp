#ifndef INCLUDE_GUARD_QUICKM3U_UI_LIST_VIEW_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_LIST_VIEW_HPP_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251 5054)
#endif
#include <QListView>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace ui {

class ListView : public QListView {
    Q_OBJECT
private:
public:
    ListView(QWidget* parent);

    void keyPressEvent(QKeyEvent* evt) override;

public slots:
    void addItem(QString item);
    void removeSelectedItems();
    void invertSelection();

protected:
    void dragEnterEvent(QDragEnterEvent* evt) override;
    void dragMoveEvent(QDragMoveEvent* evt) override;
    void dragLeaveEvent(QDragLeaveEvent* evt) override;
    void dropEvent(QDropEvent* evt) override;
};

}

#endif

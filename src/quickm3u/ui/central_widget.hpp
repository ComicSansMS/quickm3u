#ifndef INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP

#pragma warning(push)
#pragma warning(disable: 4251 5054)
#include <QBoxLayout>
#include <QWidget>
#pragma warning(pop)

namespace ui {

class ListWidget;

class CentralWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout m_layout;
    ListWidget* m_list;
public:
    CentralWidget(QWidget* parent);
};

}

#endif

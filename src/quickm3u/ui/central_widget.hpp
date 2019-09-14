#ifndef INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP

#pragma warning(push)
#pragma warning(disable: 5054)
#include <QWidget>
#pragma warning(pop)

namespace ui {

class CentralWidget : public QWidget {
    Q_OBJECT
private:
public:
    CentralWidget(QWidget* parent);
};

}

#endif

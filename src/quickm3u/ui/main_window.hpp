#ifndef INCLUDE_GUARD_QUICKM3U_UI_MAIN_WINDOW_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_MAIN_WINDOW_HPP

#pragma warning(push)
#pragma warning(disable: 5054)
#include <QMainWindow>
#pragma warning(pop)

namespace ui {

class CentralWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    CentralWidget* m_centralWidget;
public:
    MainWindow();
};

}

#endif

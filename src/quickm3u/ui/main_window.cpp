#include <quickm3u/ui/main_window.hpp>

#include <quickm3u/ui/central_widget.hpp>

namespace ui {

MainWindow::MainWindow()
    :m_centralWidget(new CentralWidget(this))
{
    setWindowTitle("QuickM3U");
    show();
}

}

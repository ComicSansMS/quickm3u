#include <quickm3u/ui/main_window.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QApplication>
#pragma warning(pop)

int main(int argc, char* argv[])
{
    QApplication the_app(argc, argv);

    ui::MainWindow main_window;
    main_window.show();

    return the_app.exec();
}

#include <quickm3u/ui/main_window.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QApplication>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

int main(int argc, char* argv[])
{
    QApplication the_app(argc, argv);

    ui::MainWindow main_window;
    main_window.show();

    return the_app.exec();
}

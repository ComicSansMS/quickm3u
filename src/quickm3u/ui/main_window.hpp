#ifndef INCLUDE_GUARD_QUICKM3U_UI_MAIN_WINDOW_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_MAIN_WINDOW_HPP

#pragma warning(push)
#pragma warning(disable: 5054)
#include <QMainWindow>
#pragma warning(pop)

class QToolBar;

namespace ui {

class CentralWidget;
class M3UFileModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    CentralWidget* m_centralWidget;
    QToolBar* m_toolbar;
    M3UFileModel* m_model;
    struct Actions {
        QAction* save = nullptr;
        QAction* copy_to_directory = nullptr;
        void setEnabled(bool enabled);
    } m_actions;
public:
    MainWindow();

public slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onCopyToDirectory();
    void onPathChanged();

protected:
    void dragEnterEvent(QDragEnterEvent* evt) override;
    void dragMoveEvent(QDragMoveEvent* evt) override;
    void dragLeaveEvent(QDragLeaveEvent* evt) override;
    void dropEvent(QDropEvent* evt) override;
private:
    bool isM3UDrop(QDropEvent* evt);
    void doOpenFile(QString const& path);
    void createActions();
};

}

#endif

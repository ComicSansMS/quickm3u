#include <quickm3u/ui/main_window.hpp>

#include <quickm3u/ui/central_widget.hpp>
#include <quickm3u/ui/m3u_file_model.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QDropEvent>
#include <QFileInfo>
#include <QMenuBar>
#include <QMimeData>
#include <QStyle>
#include <QToolBar>
#pragma warning(pop)

namespace ui {

MainWindow::MainWindow()
    :m_centralWidget(new CentralWidget(this)), m_toolbar(new QToolBar(this)),
     m_model(new M3UFileModel(this))
{
    setWindowTitle("QuickM3U");
    addToolBar(m_toolbar);

    m_centralWidget->setModel(m_model);
    setCentralWidget(m_centralWidget);
    m_centralWidget->setEnabled(false);

    createActions();

    resize(400, 200);

    setAcceptDrops(true);
}

void MainWindow::onNewFile()
{
    m_centralWidget->setEnabled(true);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* evt)
{
    if (isM3UDrop(evt)) {
        evt->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent* evt)
{
    if (isM3UDrop(evt)) {
        evt->acceptProposedAction();
    } else {
        evt->ignore();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* evt)
{
}

void MainWindow::dropEvent(QDropEvent* evt)
{
    if (isM3UDrop(evt)) {
        evt->acceptProposedAction();
        m_model->openFile(evt->mimeData()->urls().first().toLocalFile());
        m_centralWidget->setEnabled(true);
    } else {
        evt->ignore();
    }
}

bool MainWindow::isM3UDrop(QDropEvent* evt)
{
    if (evt->mimeData()->hasUrls()) {
        QList<QUrl> urls = evt->mimeData()->urls();
        if (urls.size() == 1) {
            QUrl const& url = urls.first();
            if (url.isLocalFile()) {
                QFileInfo const finfo(url.toLocalFile());
                auto const file_extension = finfo.suffix();
                if ((file_extension == "m3u") || (file_extension == "m3u8")) {
                    return true;
                }
            }
        }
    }
    return false;
}

void MainWindow::createActions()
{
    QIcon const icon_new = QIcon::fromTheme("document-new");
    QAction* action_new = new QAction(icon_new, tr("&New"), this);
    action_new->setShortcuts(QKeySequence::New);
    action_new->setStatusTip(tr("Create a new M3U file"));
    connect(action_new, &QAction::triggered, this, &MainWindow::onNewFile);
    QAction* action_exit = new QAction(tr("E&xit"), this);
    connect(action_exit, &QAction::triggered, this, &MainWindow::close);
    QAction* action_convert_relative = new QAction(tr("Convert to &Relative Paths"), this);
    connect(action_convert_relative, &QAction::triggered, m_model, &M3UFileModel::convertToRelativePaths);
    QAction* action_convert_absolute = new QAction(tr("Convert to &Absolute Paths"), this);
    connect(action_convert_absolute, &QAction::triggered, m_model, &M3UFileModel::convertToAbsolutePaths);
    //fileMenu->addAction(newAct);
    m_toolbar->addAction(action_new);
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(action_new);
    fileMenu->addSeparator();
    fileMenu->addAction(action_exit);
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(action_convert_relative);
    editMenu->addAction(action_convert_absolute);

}

}

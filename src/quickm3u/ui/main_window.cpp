#include <quickm3u/ui/main_window.hpp>

#include <quickm3u/ui/central_widget.hpp>
#include <quickm3u/ui/m3u_file_model.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QStyle>
#include <QToolBar>
#pragma warning(pop)

namespace ui {

MainWindow::MainWindow()
    :m_centralWidget(new CentralWidget(this)), m_toolbar(new QToolBar(this)), m_model(new M3UFileModel(this))
{
    setWindowTitle("QuickM3U");
    addToolBar(m_toolbar);

    m_centralWidget->setModel(m_model);
    setCentralWidget(m_centralWidget);
    m_centralWidget->setEnabled(false);

    createActions();

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
    //fileMenu->addAction(newAct);
    m_toolbar->addAction(action_new);
}

}

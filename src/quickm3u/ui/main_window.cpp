#include <quickm3u/ui/main_window.hpp>

#include <quickm3u/ui/about_dialog.hpp>
#include <quickm3u/ui/central_widget.hpp>
#include <quickm3u/ui/m3u_file_model.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMimeData>
#include <QStyle>
#include <QToolBar>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace ui {

void MainWindow::Actions::setEnabled(bool enabled) {
    save->setEnabled(enabled);
    copy_to_directory->setEnabled(enabled);
}

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

    resize(800, 400);

    setAcceptDrops(true);

    connect(m_model, &M3UFileModel::pathChanged, this, &MainWindow::onPathChanged);
    connect(m_model, &M3UFileModel::historyChange, this, [this](bool can_undo, bool can_redo) {
            m_actions.undo->setEnabled(can_undo);
            m_actions.redo->setEnabled(can_redo);
        });
}

void MainWindow::openFile(QString const& path)
{
    doOpenFile(path);
}

void MainWindow::onNewFile()
{
    auto const target_file = QFileDialog::getSaveFileName(this, tr("New M3U File"), QString(),
                                                          tr("M3U File") + " (*.m3u *.m3u8)");
    m_model->newFile(target_file);
}

void MainWindow::onOpenFile()
{
    auto const target_file = QFileDialog::getOpenFileName(this, tr("Open M3U File"), QString(),
                                                          tr("M3U File") + " (*.m3u *.m3u8)");
    if (!target_file.isEmpty()) {
        doOpenFile(target_file);
    }
}

void MainWindow::onSaveFile()
{
    m_model->saveFile();
}

void MainWindow::onCopyToDirectory()
{
    QString const destination = QFileDialog::getExistingDirectory(this, tr("Choose Destination Directory"), QString());
    m_model->copyFilesToDirectory(destination);
}

void MainWindow::onUndo()
{
    m_model->undo();
}

void MainWindow::onRedo()
{
    m_model->redo();
}

void MainWindow::onPathChanged()
{
    QString new_path = m_model->getFullPath();
    if (new_path.isEmpty()) {
        setWindowTitle("QuickM3U");
        m_centralWidget->setEnabled(false);
        m_actions.setEnabled(false);
        m_centralWidget->setFilePath(new_path);
    } else {
        setWindowTitle("QuickM3U - " + m_model->getFilename());
        m_centralWidget->setEnabled(true);
        m_actions.setEnabled(true);
        m_centralWidget->setFilePath(new_path);
    }
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
    return QMainWindow::dragLeaveEvent(evt);
}

void MainWindow::dropEvent(QDropEvent* evt)
{
    if (isM3UDrop(evt)) {
        evt->acceptProposedAction();
        doOpenFile(evt->mimeData()->urls().first().toLocalFile());
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

void MainWindow::doOpenFile(QString const& path)
{
    m_model->openFile(path);
}

void MainWindow::createActions()
{
    QIcon const icon_new = QIcon::fromTheme("document-new");
    QAction* action_new = new QAction(icon_new, tr("&New..."), this);
    action_new->setShortcuts(QKeySequence::New);
    action_new->setStatusTip(tr("Create a new M3U file"));
    connect(action_new, &QAction::triggered, this, &MainWindow::onNewFile);

    QIcon const icon_open = QIcon::fromTheme("document-open");
    QAction* action_open = new QAction(icon_open, tr("&Open..."), this);
    action_open->setShortcuts(QKeySequence::Open);
    action_open->setStatusTip(tr("Open an existing M3U file"));
    connect(action_open, &QAction::triggered, this, &MainWindow::onOpenFile);

    QIcon const icon_save = QIcon::fromTheme("document-save");
    QAction* action_save = new QAction(icon_save, tr("&Save"), this);
    action_save->setShortcut(QKeySequence::Save);
    action_save->setStatusTip(tr("Save all changes"));
    connect(action_save, &QAction::triggered, this, &MainWindow::onSaveFile);
    m_actions.save = action_save;
    action_save->setEnabled(false);

    QAction* action_copy_to_directory = new QAction(tr("Copy to &Directory..."), this);
    action_copy_to_directory->setStatusTip("Copy all files from the list to a single directory");
    connect(action_copy_to_directory, &QAction::triggered, this, &MainWindow::onCopyToDirectory);
    m_actions.copy_to_directory = action_copy_to_directory;
    action_copy_to_directory->setEnabled(false);

    QAction* action_exit = new QAction(tr("E&xit"), this);
    action_exit->setShortcut(QKeySequence::Quit);
    connect(action_exit, &QAction::triggered, this, &MainWindow::close);

    QAction* action_undo = new QAction(tr("&Undo"), this);
    action_undo->setShortcut(QKeySequence::Undo);
    connect(action_undo, &QAction::triggered, this, &MainWindow::onUndo);
    m_actions.undo = action_undo;

    QAction* action_redo = new QAction(tr("&Redo"), this);
    action_redo->setShortcut(QKeySequence::Redo);
    connect(action_redo, &QAction::triggered, this, &MainWindow::onRedo);
    m_actions.redo = action_redo;

    QAction* action_convert_relative = new QAction(tr("Convert to &Relative Paths"), this);
    connect(action_convert_relative, &QAction::triggered, m_model, &M3UFileModel::convertToRelativePaths);
    QAction* action_convert_absolute = new QAction(tr("Convert to &Absolute Paths"), this);
    connect(action_convert_absolute, &QAction::triggered, m_model, &M3UFileModel::convertToAbsolutePaths);
    QAction* action_about = new QAction(tr("&About..."));
    connect(action_about, &QAction::triggered, this, [this]() { AboutDialog dlg(this); dlg.exec(); });

    m_toolbar->addAction(action_new);
    m_toolbar->hide();
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(action_new);
    fileMenu->addAction(action_open);
    fileMenu->addAction(action_save);
    fileMenu->addSeparator();
    fileMenu->addAction(action_copy_to_directory);
    fileMenu->addSeparator();
    fileMenu->addAction(action_exit);
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(action_undo);
    editMenu->addAction(action_redo);
    editMenu->addSeparator();
    editMenu->addAction(action_convert_relative);
    editMenu->addAction(action_convert_absolute);
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(action_about);
}

}

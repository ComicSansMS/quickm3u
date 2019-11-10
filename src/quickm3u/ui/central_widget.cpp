#include <quickm3u/ui/central_widget.hpp>

#include <quickm3u/ui/list_view.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QFileDialog>
#include <QInputDialog>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include <QLabel>
#include <QPushButton>

namespace ui {

CentralWidget::CentralWidget(QWidget* parent)
    :QWidget(parent), m_labelFilePath(new QLabel(this)), m_list(new ListView(this)),
     m_buttonAddFiles(new QPushButton(tr("&Add Files..."), this)),
     m_buttonAddCustom(new QPushButton(tr("Add &Custom Entry..."), this)),
     m_buttonRemoveSelected(new QPushButton(tr("Remove Selected"), this)),
     m_buttonInvertSelection(new QPushButton(tr("&Invert Selection"), this))
{
    m_layout.addWidget(m_labelFilePath);
    m_layout.addWidget(m_list);

    m_layoutButtons.addWidget(m_buttonAddFiles);
    m_layoutButtons.addWidget(m_buttonAddCustom);
    m_layoutButtons.addWidget(m_buttonRemoveSelected);
    m_layoutButtons.addWidget(m_buttonInvertSelection);
    m_layout.addLayout(&m_layoutButtons);

    connect(m_buttonAddFiles, &QPushButton::clicked, this, &CentralWidget::onAddFiles);
    connect(m_buttonAddCustom, &QPushButton::clicked, this, &CentralWidget::onAddCustom);
    connect(m_buttonRemoveSelected, &QPushButton::clicked, this, &CentralWidget::onRemoveSelected);
    connect(m_buttonInvertSelection, &QPushButton::clicked, this, &CentralWidget::onInvertSelection);

    setLayout(&m_layout);
}

void CentralWidget::setModel(QAbstractItemModel* model)
{
    m_list->setModel(model);
}

void CentralWidget::setFilePath(QString const& file_path)
{
    m_labelFilePath->setText(file_path);
}

ListView* CentralWidget::fileList() const
{
    return m_list;
}

void CentralWidget::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select one or more files to add"));
    for (auto const& f : files) {
        m_list->addItem(f);
    }
}

void CentralWidget::onAddCustom()
{
    bool is_ok = false;
    QString const e = QInputDialog::getText(this, tr("Add Custom Entry"), tr("Entry:"),
                                            QLineEdit::Normal, QString(), &is_ok);
    if (is_ok) {
        m_list->addItem(e);
    }
}

void CentralWidget::onRemoveSelected()
{
    m_list->removeSelectedItems();
}

void CentralWidget::onInvertSelection()
{
    m_list->invertSelection();
}

}

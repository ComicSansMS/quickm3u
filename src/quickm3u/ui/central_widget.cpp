#include <quickm3u/ui/central_widget.hpp>

#include <quickm3u/ui/list_view.hpp>

#include <QLabel>

namespace ui {

CentralWidget::CentralWidget(QWidget* parent)
    :QWidget(parent), m_labelFilePath(new QLabel(this)), m_list(new ListView(this))
{
    m_layout.addWidget(m_labelFilePath);
    m_layout.addWidget(m_list);

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

}

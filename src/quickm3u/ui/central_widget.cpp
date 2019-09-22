#include <quickm3u/ui/central_widget.hpp>

#include <quickm3u/ui/list_view.hpp>

namespace ui {

CentralWidget::CentralWidget(QWidget* parent)
    :QWidget(parent), m_list(new ListView(this))
{
    m_layout.addWidget(m_list);

    setLayout(&m_layout);
}

void CentralWidget::setModel(QAbstractItemModel* model)
{
    m_list->setModel(model);
}

ListView* CentralWidget::fileList() const
{
    return m_list;
}

}

#include <quickm3u/ui/central_widget.hpp>

#include <quickm3u/ui/list_widget.hpp>

namespace ui {

CentralWidget::CentralWidget(QWidget* parent)
    :QWidget(parent), m_list(new ListWidget(this))
{
    m_layout.addWidget(m_list);

    setLayout(&m_layout);
}

}

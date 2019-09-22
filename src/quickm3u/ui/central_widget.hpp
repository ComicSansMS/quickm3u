#ifndef INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP

#pragma warning(push)
#pragma warning(disable: 4251 5054)
#include <QBoxLayout>
#include <QWidget>
#pragma warning(pop)

class QAbstractItemModel;

namespace ui {

class ListView;

class CentralWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout m_layout;
    ListView* m_list;
public:
    CentralWidget(QWidget* parent);

    void setModel(QAbstractItemModel* model);
    ListView* fileList() const;
};

}

#endif

#ifndef INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP

#pragma warning(push)
#pragma warning(disable: 4251 5054)
#include <QBoxLayout>
#include <QWidget>
#pragma warning(pop)

class QAbstractItemModel;
class QLabel;
class QPushButton;

namespace ui {

class ListView;

class CentralWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout m_layout;
    QLabel* m_labelFilePath;
    ListView* m_list;
    QHBoxLayout m_layoutButtons;
    QPushButton* m_buttonAddFile;
    QPushButton* m_buttonRemoveSelected;
    QPushButton* m_buttonInvertSelection;
public:
    CentralWidget(QWidget* parent);

    void setModel(QAbstractItemModel* model);
    void setFilePath(QString const& file_path);
    ListView* fileList() const;

public slots:
    void onAddFile();
    void onRemoveSelected();
    void onInvertSelection();
};

}

#endif

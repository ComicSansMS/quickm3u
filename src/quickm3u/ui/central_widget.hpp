#ifndef INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_QUICKM3U_UI_CENTRAL_WIDGET_HPP

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251 5054)
#endif
#include <QBoxLayout>
#include <QWidget>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

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
    QPushButton* m_buttonAddFiles;
    QPushButton* m_buttonAddCustom;
    QPushButton* m_buttonRemoveSelected;
    QPushButton* m_buttonInvertSelection;
public:
    CentralWidget(QWidget* parent);

    void setModel(QAbstractItemModel* model);
    void setFilePath(QString const& file_path);
    ListView* fileList() const;

public slots:
    void onAddFiles();
    void onAddCustom();
    void onRemoveSelected();
    void onInvertSelection();
};

}

#endif

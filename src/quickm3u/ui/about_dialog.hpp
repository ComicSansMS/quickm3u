#ifndef INCLUDE_GUARD_QUICKM3U_UI_ABOUT_DIALOG_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_ABOUT_DIALOG_HPP_

#pragma warning(push)
#pragma warning(disable: 5054)
#include <QDialog>
#pragma warning(pop)

#include <QLabel>
#include <QPushButton>
#pragma warning(push)
#pragma warning(disable: 4251)
#include <QBoxLayout>
#include <QPlainTextEdit>
#pragma warning(pop)

namespace ui {

class AboutDialog : public QDialog {
    Q_OBJECT;
private:
    QVBoxLayout m_layout;
    QHBoxLayout m_layoutLabels;
    QLabel* m_labelImage;
    QLabel* m_labelText;
    QPlainTextEdit* m_editLicense;
    QHBoxLayout m_layoutButtons;
    QPushButton* m_buttonOk;
    QPushButton* m_buttonAboutQt;
public:
    AboutDialog(QWidget* parent = nullptr);
public slots:
    void showAboutQt();
    void onLinkClicked(QString const& href);
};

}

#endif

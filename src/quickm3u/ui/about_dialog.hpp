#ifndef INCLUDE_GUARD_QUICKM3U_UI_ABOUT_DIALOG_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_ABOUT_DIALOG_HPP_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5054)
#endif
#include <QDialog>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <QLabel>
#include <QPushButton>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QBoxLayout>
#include <QPlainTextEdit>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace ui {

class AboutDialog : public QDialog {
    Q_OBJECT
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

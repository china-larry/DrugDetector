#ifndef CDETECTORPAGETITLEWIDGET_H
#define CDETECTORPAGETITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
class CDetectorPageTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageTitleWidget(QWidget *parent = nullptr);

signals:    
    void SigGoDetectorPage();
    void SigGoHistoryPage();
    void SigGoSettingPage();
    void SigMinWindow();
    void SigCloseWindow();


public slots:


private:
    void InitWidget();
    void InitLayout();

private:
    QLabel *m_pUserInfoLabel;
    //
    QPushButton *m_pSettingPageButton;
    QPushButton *m_pHistoryPageButton;
    QPushButton *m_pDetectorPageButton;
    //
    QPushButton *m_pMinWindowButton;
    QPushButton *m_pCloseWindowButton;
};

#endif // CDETECTORPAGETITLEWIDGET_H

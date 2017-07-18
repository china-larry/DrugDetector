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
    void SignalGoDetectorPage();
    void SignalGoHistoryPage();
    void SignalGoSettingPage();
    void SignalMinWindow();
    void SignalCloseWindow();


public slots:


private:
    void _InitWidget();
    void _InitLayout();

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

#ifndef CDETECTORPAGESTATUSBAR_H
#define CDETECTORPAGESTATUSBAR_H

#include <QWidget>
#include <QLabel>
class CDetectorPageStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageStatusBar(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _InitWidget();
    void _InitLayout();

private:
    QLabel *m_pTestStatusLabel;// 测试状态
    QLabel *m_pProgressStatusLabel;// 进度条
};

#endif // CDETECTORPAGESTATUSBAR_H

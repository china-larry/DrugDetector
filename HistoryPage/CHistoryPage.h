#ifndef CHISTORYPAGE_H
#define CHISTORYPAGE_H

#include <QWidget>
#include <QLabel>
class CHistoryPage : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *m_pLabel;
};

#endif // CHISTORYPAGE_H

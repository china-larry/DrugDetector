#ifndef CDETECTORPAGE_H
#define CDETECTORPAGE_H

#include <QWidget>
#include <QLabel>
class CDetectorPage : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *m_pLabel;
};

#endif // CDETECTORPAGE_H

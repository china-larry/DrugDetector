/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 设置页面Widget，版本信息，调光，校正等操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CSETTINGPAGE_H
#define CSETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = nullptr);

signals:

public slots:

private:
    void _LoadQss();
    QGroupBox *_CreateVersionGroup();
private:
    QLabel *m_pSoftwareLabel;
    QLabel *m_pFirmewareLabel;
};

#endif // CSETTINGPAGE_H

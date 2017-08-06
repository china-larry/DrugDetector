#ifndef CSTANDARDMODELWIDGET_H
#define CSTANDARDMODELWIDGET_H
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QGraphicsView>
#include "CommonDataWidget/CLabelLineEditWidget.h"

class CStandardModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CStandardModelWidget(QWidget *parent = nullptr);

signals:

public slots:
public:

private:
    QGroupBox *_CreateBrightnessValueGroup();
    //
    void _InitWidget();
    void _InitLayout();


private:
    // brightness value
    CHLabelLineEditWidget *m_pNo1LineEditWidget;
    CHLabelLineEditWidget *m_pNo2LineEditWidget;
    CHLabelLineEditWidget *m_pNo3LineEditWidget;
    CHLabelLineEditWidget *m_pNo4LineEditWidget;
    CHLabelLineEditWidget *m_pNo5LineEditWidget;
    CHLabelLineEditWidget *m_pNo6LineEditWidget;
    CHLabelLineEditWidget *m_pNo7LineEditWidget;
    CHLabelLineEditWidget *m_pNo8LineEditWidget;
    //
    QPushButton *m_pConfirmButton;
    // photo
    QLabel *m_pPhotoNameLabel;
    QLabel *m_pPhotoShowLabel;
    // curve valuesx
    QLabel *m_pCurveValuesLabel;
    QGraphicsView *m_pCurveGraphicsView;
    //
    QPushButton *m_pDeriveButton;
};

#endif // CSTANDARDMODELWIDGET_H

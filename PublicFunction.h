#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H
#include <QString>
#include <QDate>
#include <QDateTime>
// 测试页所有非结果数据(用户)结构体，便于数据传送
struct DetectorPageUserData
{
    // donor details
    QString strDonorFirstName;
    QString strDonorLastName;
    QDateTime qTestDateTime;
    QDate qBirthDate;
    QString strDonorID;
    QString strTestSite;
    // reason
    bool bPreEmployment;
    bool bRandom;
    bool bReasonableSuspicionCause;
    bool bPostAccident;
    bool bReturnToDuty;
    bool bFollowUp;
    bool bOtherReason;
    QString strOtherReasonComments;
    // product details
    bool bTemperatureNormal;
    QString strProductDefinition;
    QDate qExpriationDate;
    QString strProductLot;
    QString strProductID;
};


#endif // PUBLICFUNCTION_H

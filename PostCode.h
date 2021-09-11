#ifndef POSTCODE_H
#define POSTCODE_H

#include <QObject>

#include "DAOLib.h"

class PostCode
{
public:
    PostCode();

    void setPrimKey(qint64 val);
    qint64 getPrimKey() const;

    void setCode(const QString &val);
    QString getCode() const;

    void setCity (const QString &val);
    QString getCity() const;

    void setTimeStamp(const QString &val);
    QString getTimeStamp() const;

private:
    qint64 primKey;
    QString code;
    QString city;
    QString timeStamp;

};

#endif // POSTCODE_H

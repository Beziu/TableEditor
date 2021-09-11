#include "PostCode.h"

PostCode::PostCode()
{

}

void PostCode::setPrimKey(qint64 val)
{
    primKey = val;
}

qint64 PostCode::getPrimKey() const
{
    return primKey;
}

void PostCode::setCode(const QString &val)
{
    this->code = val;
}

QString PostCode::getCode() const
{
    return code;
}

void PostCode::setCity(const QString &val)
{
    this->city = val;
}

QString PostCode::getCity() const
{
    return city;
}

void PostCode::setTimeStamp(const QString &val)
{
    timeStamp = val;
}

QString PostCode::getTimeStamp() const
{
    return timeStamp;
}

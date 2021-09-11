#include "PostCodesDAO.h"

PostCodesDAO::PostCodesDAO()
{

}

bool PostCodesDAO::insertPostCode(const QString &code, const QString &city)
{
    QString sql = "INSERT INTO POSTCODES";
    sql += "(CODE, CITY) VALUES (";
    sql += DAOLib::dbString(code) + ", ";
    sql += DAOLib::dbString(city) + ")";

    return DAOLib::executeNonQuery(sql) > 0;

}

bool PostCodesDAO::postCodeExists(const QString &code, const QString &city)
{
    QString sql = "SELECT COUNT(*) FROM POSTCODES";
    sql += "WHERE CODE = " + DAOLib::dbString(code);
    sql += " AND CITY = " + DAOLib::dbString(city);

    bool ok;

    QVariant count = DAOLib::executeScalar(sql, ok);

    if (!ok)
        return false;
    else
        return count.toInt() > 0;
}

void PostCodesDAO::deleteTable()
{
    QString sql = "TRUNCATE TABLE POSTCODES";

    DAOLib::executeNonQuery(sql);

}

int PostCodesDAO::getRowCount()
{
    QString sql = "SELECT COUNT(*) FROM POSTCODES";

    bool ok;

    QVariant count = DAOLib::executeScalar(sql, ok);

    if (!ok)
        return 0;
    else
        return count.toInt();

}

bool PostCodesDAO::deletePostCode(qint64 key)
{
    QString sql = "DELETE FROM POSTCODES ";
    sql += "WHERE PRIMARYKEY = " + QString::number(key);

    return DAOLib::executeNonQuery(sql) > 0;

}









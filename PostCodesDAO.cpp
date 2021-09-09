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



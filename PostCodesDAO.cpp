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

QSqlTableModel *PostCodesDAO::readPostCodeInToTableModel()
{
    QSqlTableModel *tableModel = new QSqlTableModel(nullptr, DAOLib::getDatabaseConnection());

    tableModel->setTable("PostCodes");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->select();

    return tableModel;

}

PostCode *PostCodesDAO::readPostCode(qint64 key)
{
    PostCode *code = nullptr;
    bool ok;

    QString sql = "SELECT PRIMARYKEY, CODE, CITY, TIMESTAMP FROM POSTCODES ";
    sql += "WHERE PRIMARYKEY = " + QString::number(key);

    QSqlQuery *query = DAOLib::executeQuery(sql, ok);

    if (!ok)
    {
        delete query;
        return code;
    }

    query->first();

    code = new PostCode();
    code->setPrimKey(query->value("PRIMARYKEY").toLongLong());
    code->setCode(query->value("CODE").toString());
    code->setCity(query->value("CITY").toString());
    code->setTimeStamp(query->value("TIMESTAMP").toString());

    delete query;
    return code;

}

bool PostCodesDAO::deletePostCode(qint64 key)
{
    QString sql = "DELETE FROM POSTCODES ";
    sql += "WHERE PRIMARYKEY = " + QString::number(key);

    return DAOLib::executeNonQuery(sql) > 0;

}









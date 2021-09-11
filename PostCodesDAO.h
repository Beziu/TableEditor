#ifndef POSTCODESDAO_H
#define POSTCODESDAO_H

#include <QObject>
#include <QtWidgets>
#include <QSqlTableModel>

#include "DAOLib.h"
#include "PostCode.h"

class PostCodesDAO
{
public:
    static bool insertPostCode(const QString &code, const QString &city);
    static bool postCodeExists(const QString &code, const QString &city);
    static bool updatePostCode(qint64 key, const QString &code, const QString &city);
    static bool deletePostCode(qint64 key);
    static void deleteTable();

    static int getRowCount();
    static qint64 getLastIdentity();

    static QSqlTableModel *readPostCodeInToTableModel();
    static PostCode *readPostCode(qint64 key);

private:
    PostCodesDAO();

};

#endif // POSTCODESDAO_H

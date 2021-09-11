#ifndef POSTCODESDAO_H
#define POSTCODESDAO_H

#include "DAOLib.h"

class PostCodesDAO
{
public:
    static bool insertPostCode(const QString &code, const QString &city);
    static bool postCodeExists(const QString &code, const QString &city);
    static void deleteTable();
    static int getRowCount();
    static bool deletePostCode(qint64 key);


private:
    PostCodesDAO();

};

#endif // POSTCODESDAO_H

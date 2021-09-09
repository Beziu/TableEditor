#ifndef POSTCODESDAO_H
#define POSTCODESDAO_H

#include "DAOLib.h"

class PostCodesDAO
{
public:
    static bool insertPostCode(const QString &code, const QString &city);


private:
    PostCodesDAO();

};

#endif // POSTCODESDAO_H

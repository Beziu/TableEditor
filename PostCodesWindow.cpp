#include "PostCodes.h"
#include "ui_PostCodes.h"

PostCodes::PostCodes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostCodes)
{
    ui->setupUi(this);
}

PostCodes::~PostCodes()
{
    delete ui;
}

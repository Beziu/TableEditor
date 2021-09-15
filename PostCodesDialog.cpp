#include "PostCodesDialog.h"
#include "ui_PostCodesDialog.h"

PostCodesDialog::PostCodesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PostCodesDialog)
{
    ui->setupUi(this);
}

PostCodesDialog::~PostCodesDialog()
{
    delete ui;
}

void PostCodesDialog::on_btnSave_clicked()
{

}


void PostCodesDialog::on_btnCancel_clicked()
{

}


void PostCodesDialog::on_textCity_textChanged(const QString &arg1)
{

}


void PostCodesDialog::on_textCode_textChanged(const QString &arg1)
{

}


#include "PostCodesWindow.h"
#include "ui_PostCodesWindow.h"

#include "PostCodesDAO.h"

PostCodes::PostCodes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostCodes)
{
    ui->setupUi(this);

    init();
}

PostCodes::~PostCodes()
{
    delete ui;
}

void PostCodes::init()
{

}

void PostCodes::showTable()
{
    QSqlTableModel *model = setTableViewModel();
}

QSqlTableModel *PostCodes::setTableViewModel()
{
    delete ui->tableView->model();

    QSqlTableModel *model = PostCodesDAO::readPostCodeInToTableModel();

    model->sort(model->record().indexOf("CODE"), Qt::SortOrder::AscendingOrder);





}

void PostCodes::deleteEntry(const QModelIndex index)
{

}


void PostCodes::on_actionNew_triggered()
{

}

void PostCodes::on_actionEdit_triggered()
{

}

void PostCodes::on_actionDelete_triggered()
{

}

void PostCodes::on_actionEXit_triggered()
{
    close();
}




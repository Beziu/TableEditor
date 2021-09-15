#include "PostCodesDialog.h"
#include "ui_PostCodesDialog.h"

#include "PostCode.h"
#include "PostCodesDAO.h"

PostCodesDialog::PostCodesDialog(qint64 key, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PostCodesDialog)
{
    ui->setupUi(this);

    dialogKey = key;

    init();

}

PostCodesDialog::~PostCodesDialog()
{
    delete ui;
}

void PostCodesDialog::init()
{
    if (dialogKey > 0)
        readEntry(dialogKey);

    //qDebug() << "Key: " << dialogKey;

}

void PostCodesDialog::readEntry(qint64 key)
{
    PostCode *pc = PostCodesDAO::readPostCode(key);

    if ( pc == nullptr)
        return;

    ui->textCode->setText(pc->getCode());
    ui->textCity->setText(pc->getCity());

    timeStamp = pc->getTimeStamp();

    ui->btnSave->setEnabled(false);

    isModified = false;

    delete pc;

}

bool PostCodesDialog::querySave()
{
    bool retValue = false;

    if (!isModified)
        return true;

    int msgValue = QMessageBox::warning(this, this->windowTitle(),
                                        tr("The data has been changed.\nDo you want to save your changes"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Cancel);

    if (msgValue == QMessageBox::Discard)
        retValue = true;
    else if (msgValue == QMessageBox::Cancel)
        retValue = false;
    else
        retValue = saveEntry();

    return retValue;

}

bool PostCodesDialog::saveEntry()
{
    bool retValue = false;

    if (!entryIsValid())
        return retValue;

    if (dialogKey > 0)
        retValue = updateEntry(dialogKey);
    else
        retValue = insertEntry();

    if (retValue)
        emit refreshData(dialogKey);

    isModified = !retValue;

    return retValue;

}

bool PostCodesDialog::updateEntry(qint64 key)
{
    PostCode *pc = PostCodesDAO::readPostCode(key);

    if (pc == nullptr)
        return false;

    QString currentTimeStamp = pc->getTimeStamp();

    delete pc;

    if (timeStamp != currentTimeStamp)
    {
        QMessageBox::warning(this, this->windowTitle(),
                             tr("This entry has since been changed by another user"));
        return true;
    }

    return PostCodesDAO::updatePostCode(key, ui->textCode->text(), ui->textCity->text());

}

bool PostCodesDialog::insertEntry()
{
    bool retValue = false;

    if (PostCodesDAO::insertPostCode(ui->textCode->text(), ui->textCity->text()))
    {
        dialogKey = PostCodesDAO::getLastIdentity();
        retValue = true;
    }

    return retValue;

}

bool PostCodesDialog::entryIsValid()
{
    bool retValue = true;

    if (ui->textCode->text().length() == 0)
    {
        QMessageBox::critical(this, this->windowTitle(), tr("Entry missing"));
        ui->textCode->setFocus();
        retValue = false;
    }
    else if (ui->textCity->text().length() == 0)
    {
        QMessageBox::critical(this, this->windowTitle(), tr("Entry missing"));
        ui->textCode->setFocus();
        retValue = false;
    }
    else if (PostCodesDAO::postCodeExists(ui->textCode->text(), ui->textCity->text()))
    {
        QMessageBox::critical(this, this->windowTitle(), tr("Post code and city no double entry"));
        ui->textCode->setFocus();
        retValue = false;
    }

    return retValue;

}

void PostCodesDialog::closeEvent(QCloseEvent *event)
{
    if (querySave())
        event->accept();
    else
        event->ignore();

}

void PostCodesDialog::reject()
{
    // return
    // lub
    close();

}

void PostCodesDialog::on_btnSave_clicked()
{
    if (isModified)
    {
        if (!saveEntry())
            return;
    }

    close();

}

void PostCodesDialog::on_btnCancel_clicked()
{
    close();
}

void PostCodesDialog::on_textCity_textChanged(const QString &)
{
    ui->btnSave->setEnabled(true);
    isModified = true;

}

void PostCodesDialog::on_textCode_textChanged(const QString &)
{
    ui->btnSave->setEnabled(true);
    isModified = true;

}

void PostCodesDialog::on_textCode_returnPressed()
{
    this->focusNextChild();
}

void PostCodesDialog::on_textCity_returnPressed()
{
    this->focusNextChild();
}


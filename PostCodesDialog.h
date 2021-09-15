#ifndef POSTCODESDIALOG_H
#define POSTCODESDIALOG_H

#include <QDialog>

namespace Ui {
class PostCodesDialog;
}

class PostCodesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PostCodesDialog(QWidget *parent = nullptr);
    ~PostCodesDialog();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_textCity_textChanged(const QString &arg1);

    void on_textCode_textChanged(const QString &arg1);

private:
    Ui::PostCodesDialog *ui;
};

#endif // POSTCODESDIALOG_H

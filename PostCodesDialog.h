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
    explicit PostCodesDialog(qint64 key, QWidget *parent = nullptr);
    ~PostCodesDialog();

signals:
    void refreshData(const qint64 key);

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_textCity_textChanged(const QString &arg1);

    void on_textCode_textChanged(const QString &arg1);

    void on_textCode_returnPressed();

    void on_textCity_returnPressed();

private:
    Ui::PostCodesDialog *ui;

    qint64 dialogKey;
    QString timeStamp;
    bool isModified;

    void init();
    void readEntry(qint64 key);
    bool querySave();
    bool saveEntry();
    bool updateEntry(qint64 key);
    bool insertEntry();
    bool entryIsValid();

    void closeEvent(QCloseEvent *event) override;

    // Klawisz ESC jest aktywny w tym oknie
    void reject() override;

};

#endif // POSTCODESDIALOG_H

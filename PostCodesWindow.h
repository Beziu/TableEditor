#ifndef POSTCODESWINDOW_H
#define POSTCODESWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVariant>
#include <QKeyEvent>

namespace Ui {
class PostCodes;
}

class PostCodes : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostCodes(QWidget *parent = nullptr);
    ~PostCodes();

private slots:
    void on_actionEXit_triggered();

    void on_actionNew_triggered();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void updateTableView(const qint64 key);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::PostCodes *ui;

    QLabel *statusLabel;
    QSqlTableModel *setTableViewModel();

    void init();
    void showTable();
    void deleteEntry(const QModelIndex index);
    void showPostCodesDialog(const qint64 key);
    void findItemInTableView(const QString &columnName, const QVariant &value);
    bool eventFilter(QObject *sender, QEvent *event) override;


};

#endif // POSTCODESWINDOW_H

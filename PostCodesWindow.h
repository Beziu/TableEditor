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

private:
    Ui::PostCodes *ui;

    QLabel *statusLabel;

    void init();
    void showTable();
    void deleteEntry(const QModelIndex index);

    QSqlTableModel *setTableViewModel();



};

#endif // POSTCODESWINDOW_H

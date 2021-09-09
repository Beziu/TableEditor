#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QTranslator>
#include <QLibraryInfo>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QFile>
#include <QDir>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    QTranslator *qtTranslator;
    QLabel *statusLabel;
    QProgressBar *progressBar;

    bool importIsRunning;
    bool stopImport;

    void init();
    void enableDatabase(bool);
    qint64 getFileSize(const QString &fileName);
    int getRecordCount(const QString &fileName);



    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};
#endif // MAINWINDOW_H

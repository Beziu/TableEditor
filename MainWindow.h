#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QTranslator>
#include <QCloseEvent>
#include <QKeyEvent>


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

    QTranslator *translator;
    QLabel *statuslabel;
    QProgressBar *progressBar;

    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void init();
    bool importIsRunning;
    bool stopImport;





    Q_PROPERTY(QTranslator * translator READ getTranslator CONSTANT)
};
#endif // MAINWINDOW_H

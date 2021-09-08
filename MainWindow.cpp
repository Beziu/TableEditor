#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::init()
{
    // Domyslna wartosc poszatkowa wskazujaca ze nie ma importu danych
    importIsRunning = false;

    // Utwórz nową etykietę, która będzie używana na pasku stanu dla wiadomości tekstowych.
    statusLabel = new QLabel(this);

    // Nieznaczne wcięcie tekstu w etykiecie
    statusLabel->setIndent(5);

    // Pozycja etykiety na lewo w pasku statusu
    // Drugi parametr stretch = 1 rozciąga etykietę na cały pasek stanu.
    statusBar()->addWidget(statusLabel, 1);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (importIsRunning)
        event->ignore();
    else
    {
        DAOLib::closeConnection();
        event->accept();
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        stopImport = true;

}


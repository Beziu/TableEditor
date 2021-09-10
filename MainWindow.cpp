#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DAOLib.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Deklaracja dla translatora
    qtTranslator = new QTranslator();
    if (qtTranslator->load("qt_en", QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(qtTranslator);

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

    // Deklaracja nowego paska postepu
    progressBar = new QProgressBar(this);

    // Deklaracja stylu w postaci ciagu znakow
    QString styleSheet = "QProgressBar";
    styleSheet += "{border: 2px solid grey;";
    styleSheet += "border-radius: 5px;";
    styleSheet += "text-align: center;}";
    styleSheet += "QProgressBar::chunk";
    styleSheet += "{background-color: lightgreen;";
    styleSheet += "width: 10px; margin: 1px;}";

    // Przypisanie stylu do paska postepu
    progressBar->setStyleSheet(styleSheet);

    // Ustawienie stalej szerokosci
    progressBar->setFixedWidth(200);

    // Deklaracja zeby nie bylo go widac
    progressBar->setVisible(false);

    // Dodanie paska postepu do paska stanu
    statusBar()->addPermanentWidget(progressBar);

    enableDatabase(openDatabase("127.0.0.1:3306", "postcodes"));

}

void MainWindow::enableDatabase(bool bEnable)
{
    ui->menuImport->setEnabled(bEnable);
    ui->actionPostCodes->setEnabled(bEnable);

    if (bEnable)
        statusLabel->setText("Databank: " + DAOLib::getDatabaseName());
    else
        statusLabel->setText("Databank: (none!)");
}

bool MainWindow::openDatabase(const QString &server, const QString &database)
{
    QString driver = "QMYSQL";
    QString user = "root";
    QString passwd = "";

    return DAOLib::connectToDatabase(driver, server, user, passwd, database);

}

qint64 MainWindow::getFileSize(const QString &fileName)
{
    qint64 retValue = 0;

    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        retValue = file.size();
        file.close();
    }

    return retValue;

}

int MainWindow::getRecordCount(const QString &fileName)
{
    int retValue = 0;

    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            in.readLine();
            retValue++;
        }

        file.close();
    }

    return retValue;

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


void MainWindow::on_actionPostCodes_triggered()
{

}


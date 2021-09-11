#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DAOLib.h"
#include "PostCodesDAO.h"

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
        statusLabel->setText(tr("Databank: ") + DAOLib::getDatabaseName());
    else
        statusLabel->setText(tr("Databank: (none!)"));

}

bool MainWindow::openDatabase(const QString &server, const QString &database)
{
    QString driver = "QMYSQL";
    QString user = "root";
    QString passwd = "";

    return DAOLib::connectToDatabase(driver, server, user, passwd, database);

}

void MainWindow::importPostCodes()
{
    QString currentPath = QDir::homePath();
    QString defaultFilter = tr("Text file (*.txt)");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Text file opean"), currentPath,
                                                    tr("All files (*.*);;") + defaultFilter,
                                                    &defaultFilter);
    if (fileName.isEmpty())
        return;

    importPostCodesIntoDatabase(fileName);

}

void MainWindow::importPostCodesIntoDatabase(const QString &fileName)
{
    QString line;
    int recordCounter = 0;
    int insertCounter = 0;
    QString statusText;
    QString city;
    qint64 fileSize;
    int progressValue = 0;
    int msgValue;

    importIsRunning = true;
    stopImport = false;

    if (PostCodesDAO::getRowCount() > 0)
    {
        msgValue = QMessageBox::question(this, tr("Post Codes import"),
                                         tr("Would you like imports the post Ccdesin to database"),
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

        if (msgValue == QMessageBox::Cancel)
            return;

        if (msgValue == QMessageBox::Yes)
            PostCodesDAO::deleteTable();
    }

    statusText = statusLabel->text();
    statusLabel->setText(tr("Importing Post Codes. Press ESC to cancel..."));

    fileSize = getFileSize(fileName);

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(true);

    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            if (stopImport)
                msgValue = QMessageBox::question(this, tr("Importing Post Codes"),
                                                 tr("Do you really want to cancel"),
                                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

            if (msgValue == QMessageBox::Yes)
                break;

            stopImport = false;

            line = in.readLine();
            recordCounter++;

            progressValue += line.length() + 2;

            if (recordCounter % 50 == 0)
            {
                progressBar->setValue(static_cast<int>((progressValue * 100) / fileSize));
                QApplication::processEvents();
            }

            QStringList splitList = line.split(";");

            if (splitList.size() >= 2)
            {
                city = splitList[1];
                for (int i = 2; i < splitList.size(); i++)
                    city += ";" + splitList[i];

                if (PostCodesDAO::postCodeExists(splitList[0], city))
                    continue;

                if (PostCodesDAO::insertPostCode(splitList[0], city))
                    insertCounter++;
            }
        }

        file.close();

    }

    progressBar->setVisible(false);
    statusLabel->setText(statusText);

    if (stopImport)
        QMessageBox::information(this, tr("Importing Post Codes"),
                                 tr("The import of the data records was canceled by the user"));
    else
        QMessageBox::information(this, tr("Importing Post Codes"),
                                 QString(tr("%L1 data records were read.\n%L2 data records were successfully imported"))
                                 .arg(recordCounter).arg(insertCounter));

    importIsRunning = false;

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


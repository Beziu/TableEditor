#include "PostCodesWindow.h"
#include "ui_PostCodesWindow.h"

#include "PostCodesDAO.h"
#include "PostCodesDialog.h"

PostCodes::PostCodes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostCodes)
{
    ui->setupUi(this);

    init();
}

PostCodes::~PostCodes()
{
    delete ui;
}

void PostCodes::init()
{
    // Tworzenie etykiety do wyświetlania tekstów statusu na pasku stanu
    statusLabel = new QLabel(this);

    // Wyrównaj tekst poziomo i pionowo na środku
    statusLabel->setAlignment(Qt::AlignCenter);

    // Pozycjonowany  etykieta po lewej stronie paska stanu.
    // Drugi parametr  stretch = 1  rozciąga etykietę na całym pasku stanu.
    statusBar()->addWidget(statusLabel, 1);

    // Implementacja EventFilter dla widoku tabeli, zeby klawiszami wprowadzac
    ui->tableView->installEventFilter(this);

    QPalette palette = ui->tableView->palette();
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::Highlight, QColor(0, 112, 255));

    ui->tableView->setPalette(palette);

    showTable();

}

void PostCodes::showTable()
{
    // Istnieją 3 źródła danych umożliwiające dostęp do danych w bazie danych dla QTableView:
    // 1. QSqlQueryModel           (tylko do odczytu)
    // 2. QSqlTableModel           (do odczytu i zapisu do tabeli bazy danych)
    // 3. QSqlRelationalTableModel (rozszerza QSqlTableModel i obsługuje
    //                              Klucz obcy jako relacja 1 do 1 między a
    //                              Pole w tabeli i klucz podstawowy

    // Użyj TableModel jako źródła danych dla TableView
    QSqlTableModel *model = setTableViewModel();

    // Ustaw nieco większy rozmiar czcionki nagłówków kolumn
    QFont font = ui->tableView->horizontalHeader()->font();
    font.setPixelSize(14);
    ui->tableView->horizontalHeader()->setFont(font);

    // Zmień kolor czcionki nagłówków kolumn
    ui->tableView->horizontalHeader()->setStyleSheet("color: blue;");

    // Zmień kolor tła nagłówków kolumn
    ui->tableView->setStyleSheet("QHederView::section {background-color: lightgtrey;}");

    // Wszystkie nagłówki kolumn z wyrównaniem do lewej
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    // Ustawia kolumy jako niewidoczne
    ui->tableView->hideColumn(model->record().indexOf("PRIMARYKEY"));
    ui->tableView->hideColumn(model->record().indexOf("TIMESTAMP"));

    // Ostatnia kolumna CITY zajmuje całą pozostałą szerokość TableView
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Wybierz pierwszą linię w TableView
    if (model->rowCount() > 0)
        ui->tableView->selectRow(0);

}

QSqlTableModel *PostCodes::setTableViewModel()
{
    // Ewentualne przy aktualizacji usuwanie starego modelu
    delete ui->tableView->model();

    QSqlTableModel *model = PostCodesDAO::readPostCodeInToTableModel();

    // Posortowac wedlug Post Code rosnaco
    model->sort(model->record().indexOf("CODE"), Qt::SortOrder::AscendingOrder);

    // Ustaw nagłówki kolumn tabeli
    model->setHeaderData(model->record().indexOf("CODE"), Qt::Horizontal, tr("Post Code"));
    model->setHeaderData(model->record().indexOf("CITY"), Qt::Horizontal, tr("City"));

    // Przypisz model danych
    ui->tableView->setModel(model);

    // Jeśli jednak wszystkie rekordy danych są wymagane od początku,
    // poniższa instrukcja może być użyta do określenia pełnej
    // liczby rekordów danych w modelu danych.
    while (model->canFetchMore())
        model->fetchMore();



    return model;

}

void PostCodes::deleteEntry(const QModelIndex index)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableView->model());

    qint64 key = model->record(index.row()).value("PRIMARYKEY").toLongLong();

    QString code = model->record(index.row()).value("CODE").toString();
    QString city = model->record(index.row()).value("CITY").toString();

    int msgValue = QMessageBox::question(this, this->windowTitle(),
                                         tr("Delete entry:\n") + code + " - " + city,
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (msgValue == QMessageBox::No)
        return;

    if (!PostCodesDAO::deletePostCode(key))
        return;

    statusLabel->setText(tr("Entries are being updated ..."));
    QApplication::processEvents();

    setTableViewModel();

    // ustawia zaznaczenie na jeden mniej od usunietego
    int row = (index.row() - 1 < 0) ? 0 : index.row() - 1;

    // jesli ostatni zostal to ustawia na nim, nie moze byc ujemny
    if (ui->tableView->model()->rowCount() > 0)
        ui->tableView->selectRow(row);

}

void PostCodes::showPostCodesDialog(const qint64 key)
{
    PostCodesDialog pcDialog(key, this);

    // sygnal dla nowego okna
    connect(&pcDialog, SIGNAL(refreshData(const qint64)), this, SLOT(updateTableView(const qint64)));

    pcDialog.exec();

}

void PostCodes::findItemInTableView(const QString &columnName, const QVariant &value)
{
    int row;
    bool found = false;

    // Konwersja danych na QSqlTableModel
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableView->model());

    // Dane wewnatrz
    QSqlQuery query = model->query();

    // Index wierszy
    int colIndex = model->record().indexOf(columnName);

    if (colIndex < 0)
        return;

        // Pozycja pytania query na poczatek
    query.first();

    row = query.at();

    do
    {
        if (query.value(colIndex).toString().contains(value.toString(), Qt::CaseInsensitive))
        {
            found = true;
            break;
        }
        row++;

    } while (query.next());

    if (found)
        ui->tableView->selectRow(row);
    else
        ui->tableView->selectRow(0);

}

bool PostCodes::eventFilter(QObject *sender, QEvent *event)
{
    if (sender == ui->tableView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Home)
            {
                ui->tableView->scrollToTop();
                ui->tableView->selectRow(0);
            }
            else if (keyEvent->key() == Qt::Key_End)
            {
                ui->tableView->scrollToBottom();
                ui->tableView->selectRow(ui->tableView->model()->rowCount() -1);
            }
            else if (keyEvent->key() == Qt::Key_Return)
            {
                //on_TableView_doubleClicked(ui->tableView->currentIndex());
            }
            else if (keyEvent->key() == Qt::Key_Delete)
            {
                deleteEntry(ui->tableView->currentIndex());
            }
        }
    }

    return QObject::eventFilter(sender, event);

}

void PostCodes::on_tableView_doubleClicked(const QModelIndex &index)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableView->model());

    // Nowe okno do edycji wpisu
    showPostCodesDialog(model->record(index.row()).value("PRIMARYKEY").toULongLong());

}

void PostCodes::on_actionNew_triggered()
{
    showPostCodesDialog(-1);
}

void PostCodes::on_actionEdit_triggered()
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableView->model());

    QModelIndex index = ui->tableView->currentIndex();

    showPostCodesDialog(model->record(index.row()).value("PRIMARYKEY").toLongLong());

}

void PostCodes::on_actionDelete_triggered()
{
    deleteEntry(ui->tableView->currentIndex());
}

void PostCodes::updateTableView(const qint64 key)
{
    statusLabel->setText(tr("Entries are being updated ..."));

    QApplication::processEvents();

    setTableViewModel();

    // Pozycja kursora na ostatnio dodanym wierszu
    findItemInTableView("PRIMARYKEY", QVariant(key));

}

void PostCodes::on_actionEXit_triggered()
{
    close();
}







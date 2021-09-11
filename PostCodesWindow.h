#ifndef POSTCODESWINDOW_H
#define POSTCODESWINDOW_H

#include <QMainWindow>

namespace Ui {
class PostCodes;
}

class PostCodes : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostCodes(QWidget *parent = nullptr);
    ~PostCodes();

private:
    Ui::PostCodes *ui;
};

#endif // POSTCODESWINDOW_H

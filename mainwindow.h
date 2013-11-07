#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void display();
    void openInput();
    void clickedSha();

};
QString Sha1 (const QString);
char * QStringToCharStr(QString);

#endif // MAINWINDOW_H

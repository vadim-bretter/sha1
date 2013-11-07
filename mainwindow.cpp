#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QRegExp>
#include <QMessageBox>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    display();
    connect(ui->openFBut, SIGNAL( clicked() ), SLOT( openInput() ));
    connect(ui->pushSha, SIGNAL( clicked() ), SLOT( clickedSha() ));

}

void MainWindow::display()
{
    ui->openFBut->setVisible( true );
    ui->openF->setVisible( true );


    ui->pushSha->setVisible( true );

    ui->textOut->setVisible( true );

}

void MainWindow::openInput()
{
    QString str = QFileDialog::getOpenFileName( this, QString::fromLocal8Bit( "Выбрать файл для хешировнаия" ) );
    if ( !str.isEmpty() ) {
        ui->openF->setText( str );
        ui->openF->setVisible( true );
        ui->pushSha->setVisible( true );

    }
}



void MainWindow::clickedSha()
{
    ui->textOut->setVisible( true );
    QString str;
    str = Sha1( ui->openF->text());

    ui->textOut->setText( str );

}




MainWindow::~MainWindow()
{
    delete ui;
}

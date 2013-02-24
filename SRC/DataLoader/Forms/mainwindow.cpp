#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SRC/DataLoader/Parsers/parsedefaultfile.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->commandLinkButton, SIGNAL(clicked()), SLOT(ParseDefaultFile()));
}

void MainWindow::ParseDefaultFile()
{
    //std::wstring str = L"123|ф";
   // ui->commandLinkButton->setText(
   //             QString(str.c_str()));
    //return;


    QString infileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

    DataLoader::ParseDefaultFile parser;
    parser.LoadFromFile(infileName.toStdString());
    //Load data from file



    QString outfileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("Files (*.*)"));
    //Save in json
    parser.SaveToJson(outfileName.toStdString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

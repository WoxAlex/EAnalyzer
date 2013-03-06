#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SRC/DataLoader/Parsers/parsedefaultfile.h"

#include <fstream>
#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->commandLinkButton, SIGNAL(clicked()), SLOT(ParseDefaultFile()));
    connect(this->ui->commandLinkButton_2, SIGNAL(clicked()), SLOT(LoadJsonFile()));
    connect(this->ui->commandLinkButton_3, SIGNAL(clicked()), SLOT(EditJson()));
    editForm = new EditJsonFile(this);

}

void MainWindow::ParseDefaultFile()
{
    QString infileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    if(infileName == "")
        return;
    DataLoader::ParseDefaultFile parser;
    parser.LoadFromFile(infileName.toStdString());
    //Load data from file
    ui->commandLinkButton->setText(QString::fromStdWString(parser.election.info.city));

    QString outfileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("Files (*.*)"));
    parser.SaveToJson(outfileName.toStdString());
}

void MainWindow::LoadJsonFile()
{
    QString infileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    if(infileName == "")
        return;

    EAnalyzer::Election election = EAnalyzer::Election::JsonParserFromFile(infileName.toStdString());
    ui->commandLinkButton_2->setText(QString::fromStdWString(election.info.city));
}

void MainWindow::EditJson()
{
    editForm->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete editForm;
}

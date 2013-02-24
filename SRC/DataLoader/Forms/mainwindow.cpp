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

    std::wifstream file;
    std::wstring str;
    file.open(infileName.toStdString().c_str());
    while(file.good())
    {
        std::wstring t;
        file >> t;
        str += t+ L" ";
    }
    //file >> str;
    file.close();

    EAnalyzer::Election election = EAnalyzer::Election::JsonParser(str);
    ui->commandLinkButton_2->setText(QString::fromStdWString(election.info.city));
}

MainWindow::~MainWindow()
{
    delete ui;
}

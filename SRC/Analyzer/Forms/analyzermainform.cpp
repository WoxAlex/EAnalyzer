#include "analyzermainform.h"
#include "ui_analyzermainform.h"

AnalyzerMainForm::AnalyzerMainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyzerMainForm)
{
    ui->setupUi(this);
}

AnalyzerMainForm::~AnalyzerMainForm()
{
    delete ui;
}

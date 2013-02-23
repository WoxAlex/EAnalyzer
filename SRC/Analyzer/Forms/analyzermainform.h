#ifndef ANALYZERMAINFORM_H
#define ANALYZERMAINFORM_H

#include <QMainWindow>

namespace Ui {
class AnalyzerMainForm;
}

class AnalyzerMainForm : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit AnalyzerMainForm(QWidget *parent = 0);
    ~AnalyzerMainForm();
    
private:
    Ui::AnalyzerMainForm *ui;
};

#endif // ANALYZERMAINFORM_H

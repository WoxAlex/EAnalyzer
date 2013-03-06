#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./SRC/DataLoader/Forms/editjsonfile.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);



    ~MainWindow();

public slots:
        void ParseDefaultFile();
        void LoadJsonFile();
        void EditJson();
private:
    Ui::MainWindow *ui;
    EditJsonFile* editForm;
};
//connect(m_myButton, SIGNAL(clicked()), SLOT(ParseDefaultFile()));

#endif // MAINWINDOW_H

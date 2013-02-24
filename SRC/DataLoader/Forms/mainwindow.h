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

public slots:
        void ParseDefaultFile();
        void LoadJsonFile();
private:
    Ui::MainWindow *ui;
};
//connect(m_myButton, SIGNAL(clicked()), SLOT(ParseDefaultFile()));

#endif // MAINWINDOW_H

#include <QApplication>
#include <iostream>
#include "./SRC/DataLoader/Forms/mainwindow.h"
#include "./SRC/DataLoader/Forms/editjsonfile.h"
#include <locale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainw;
    mainw.show();


    return a.exec();
}

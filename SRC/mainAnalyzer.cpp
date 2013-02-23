#include <QApplication>
#include "../SRC/Analyzer/Forms/analyzermainform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnalyzerMainForm w;
    w.show();

    return a.exec();
}


#include "plastikstyle.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PlastikStyle w;
    w.show();

    return app.exec();
}


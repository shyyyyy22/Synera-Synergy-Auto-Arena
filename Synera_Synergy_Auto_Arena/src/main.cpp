#include "GameWindow.h"
#include <QApplication>
#include<QDebug>
#include<QGraphicsScene>
#include<QGraphicsView>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Synera: Synergy Auto-Arena");
    app.setApplicationVersion("1.0");


    GameWindow window;
    window.setWindowTitle("Synera: Synergy Auto-Arena");
    window.resize(2400,1000);
    window.show();

    return app.exec();
}

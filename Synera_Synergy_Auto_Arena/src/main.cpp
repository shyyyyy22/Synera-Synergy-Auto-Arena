#include "GameWindow.h"
#include"Unit.h"
#include"Board.h"
#include <QApplication>
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Synera: Synergy Auto-Arena");
    app.setApplicationVersion("1.0");

    GameWindow window;
    window.setWindowTitle("Synera: Synergy Auto-Arena");
    window.resize(900, 700);
    window.show();

    return app.exec();
}

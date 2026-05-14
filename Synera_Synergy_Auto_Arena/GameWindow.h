#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>


QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

};
#endif // GAMEWINDOW_H

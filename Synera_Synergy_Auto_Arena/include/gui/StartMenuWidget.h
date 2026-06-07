#ifndef STARTMENUWIDGET_H
#define STARTMENUWIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>

class StartMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartMenuWidget(QWidget *parent = nullptr);

signals:
    void clickStartBtn();
    void clickLoadBtn();

private:
    QLabel* m_logoLabel;
    QPushButton* m_startBtn;
    QPushButton* m_loadBtn;
    QPushButton* m_exitBtn;
};

#endif // STARTMENUWIDGET_H

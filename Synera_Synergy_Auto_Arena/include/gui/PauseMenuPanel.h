#ifndef PAUSEMENUPANEL_H
#define PAUSEMENUPANEL_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>

class PauseMenuPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PauseMenuPanel(QWidget *parent = nullptr);

signals:
    void clickContinueBtn();
    void clickSaveBtn();

private:
    QLabel* m_titleLabel;
    QPushButton* m_continueBtn;
    QPushButton* m_saveBtn;
};

#endif // PAUSEMENUPANEL_H

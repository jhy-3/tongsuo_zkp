#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);

private:
    //home页面的欢迎语
    QLabel *welcome_label;
    QLabel *first_text_label;
    QLabel *second_text_label;
    QLabel *third_text_label;
};

#endif // HOME_H

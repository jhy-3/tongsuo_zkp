#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>

#include "home.h"
#include "range_proof.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //~MainWindow();
private:
    /* widget 小部件 */
    QWidget *widget;
    /* 水平布局 */
    QHBoxLayout *h_box_layout;
    /* 列表视图 */
    QListWidget *list_widget;
    /* 堆栈窗口部件 */
    QStackedWidget *stacked_widget;
    /* 首页界面 */
    Home *tongsuo_home;
    /* 功能1range_proof界面 */
    range_proof *tongsuo_range_proof;
};
#endif // MAINWINDOW_H

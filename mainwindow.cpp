#include "mainwindow.h"
#include "home.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主界面设置 */
    this->setGeometry(500, 300, 900, 500);

    this->setWindowTitle(QString("TongSuo"));


    /* widget 小部件实例化 */
    widget = new QWidget(this);

    /* 设置居中 */
    this->setCentralWidget(widget);


    /* 垂直布局实例化 */
    h_box_layout = new QHBoxLayout();

    /* 堆栈部件实例化 */
    stacked_widget = new QStackedWidget();

    /* 列表实例化 */
    list_widget = new QListWidget();

    /* 首页实例化 */
    tongsuo_home = new Home();
    tongsuo_range_proof = new range_proof ();
    /* 左侧功能导航 */
    QList <QString>list_widget_list;
    list_widget_list<< "首页" << "资产验证";

    for (int i = 0; i < 2; i++){
        /* listWidget 插入项 */
        list_widget->insertItem( i,list_widget_list[i]);
    }

    /* 子页面插入 */
    stacked_widget->addWidget(tongsuo_home);

    stacked_widget->addWidget(tongsuo_range_proof);

    /* 设置列表的最大宽度 */
    list_widget->setMaximumWidth(200);

    /* 添加到水平布局 */
    h_box_layout->addWidget(list_widget);
    h_box_layout->addWidget(stacked_widget);

    /* 将 widget 的布局设置成 hboxLayout */
    widget->setLayout(h_box_layout);

    /* 利用 listWidget 的信号函数 currentRowChanged()与槽函数 setCurrentIndex()进行信号与槽连接*/
    connect(list_widget, SIGNAL(currentRowChanged(int)),
            stacked_widget, SLOT(setCurrentIndex(int)));
}

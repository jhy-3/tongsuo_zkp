#include "home.h"

Home::Home(QWidget *parent) : QWidget(parent)
{
    // 创建标签和布局管理器
    welcome_label = new QLabel("欢迎使用Tongsuo零知识证明系统", this);
    first_text_label = new QLabel("为了保护个人隐私和数据安全，铜锁密码库率先支持了前沿密码算法，包括同态加密、零知识证明等技术，解决数据隐私保护中的关键问题，为隐私计算提供基础能力。", this);
    second_text_label = new QLabel("我们将实现一个零知识证明下的资产验证问题。", this);
    third_text_label = new QLabel("关于Tongsuo的更多消息请查看网址：<a href=\"https://github.com/Tongsuo-Project\">https://github.com/Tongsuo-Project</a>", this);
    third_text_label->setTextFormat(Qt::RichText);
    third_text_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 设置标签样式和对齐方式
    welcome_label->setStyleSheet("font-size: 20px; font-weight: bold;");
    welcome_label->setAlignment(Qt::AlignCenter);
    first_text_label->setAlignment(Qt::AlignCenter);
    second_text_label->setAlignment(Qt::AlignCenter);
    third_text_label->setAlignment(Qt::AlignCenter);

    // 设置自动换行
    first_text_label->setWordWrap(true);

    // 将标签添加到布局管理器中
    layout->addWidget(welcome_label);
    layout->addWidget(first_text_label);
    layout->addWidget(second_text_label);
    layout->addWidget(third_text_label);

    // 设置页面布局
    setLayout(layout);
}

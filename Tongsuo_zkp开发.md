# 搭建开发环境

选用ubuntu+qt5开发

安装c++开发环境：

```bash
sudo apt-get update
sudo apt-get install gcc g++ 
```



qt5安装：

下载qt安装包：

网址：http://download.qt.io/archive/qt/5.14/5.14.0/

选择**qt-opensource-linux-x64–5.12.0.run**下载到ubuntu本地

安装qt：

下载完成，进入下载目录，右键-终端。执行命令，给可执行权限

```bash
chmod a+x ./qt-opensource-linux-x64-5.14.0.run
```

然后终端运行，开始安装

```bash
./qt-opensource-linux-x64-5.14.0.run
```

一直下一步 or next，自己选择安装目录

在选择组件的时候，一定要选择：Desktop, Sources和developer and designer tools其余可选

- Desktop gcc 64-bit 这是Qt预编译好的64位的Qt库。
- Sources 给出你可视化开发的一些基本资源
- Developer and Designer Tools 这就是我们的主角QtCreator

接下来同意协议，安装完成，打开QtCreator

![img](https://gitee.com/jia-huaiyu/images/raw/master/202309251718403.png)

# 开发流程

打开Qtcreator，新建项目，命名为tongsuo_zkp，注意修改路径，在kits的选择中，推荐选择select all kits，这将为我们提供可视化平台。其余选择默认即可。

## home界面开发

我们想在mainwindow的基础上增加一个home界面。因此鼠标右键点击项目，选择add new

![img](https://gitee.com/jia-huaiyu/images/raw/master/202309251724665.png)

选择Qt设计师界面类

![image-20230925172534701](https://gitee.com/jia-huaiyu/images/raw/master/202309251725784.png)

命名为home，此时项目中多了三个文件：home.h,home.cpp,home.ui

home.h：主要用来设置一些用得到的组件，我需要添加欢迎语，因此我添加了三个label：

```c
	QLabel *welcome_label;
    QLabel *first_text_label;
    QLabel *second_text_label;
    QLabel *third_text_label;
```



home.cpp：界面处理，在这里我添加了我想说的欢迎语，并且让它们对齐

```cpp
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
```

这个文件要注意引用"#include "home.h""，不过一般qtcreator默认添加好了



home.ui：ui界面设置，我没有做任何改动

## range_proof界面开发

同上，增加三个新的文件：range_proof.h,range_proof.cpp,range_proof.ui

range_proof.h：我需要增加一个button响应函数

```h
private slots:
    void on_push_button_clicked();
```



range_proof.cpp：主要是两个函数的实现，on_push_button_clicked响应函数，range_proofs_test零知识证明函数

我们首先必须检查用户的输入是不是合法输入

```cpp
    //用户输入
    QString input = this->ui->line_edit->text();

    // 检查用户是否输入了数字
    if (input.isEmpty()) {
        // 如果没有输入，则提示用户输入一个数字
        this->ui->text_browser_output->setText("请输入一个数字!");
        return;
    }

    bool ok;
    qint64 verified_num = input.toLongLong(&ok);

    // 检查用户输入是否为有效数字
    if (!ok) {
        // 如果输入不是有效数字，则提示用户重新输入
        this->ui->text_browser_output->setText("输入无效，请输入一个有效的数字！");
        return;
    }

    int ret;
    if (verified_num < 0) {
        this->ui->text_browser_output->setText("请输入一个大于0的数");
    }
```

然后针对用户输入的数字进行资产验证

```cpp
		int64_t secrets[16];
        secrets[0] = verified_num;
        /* 这里指定的位数为16，即 range 范围为：[0, 65535] */
        ret = range_proofs_test(32, secrets, 1);
        int res = (ret == 1) ? 0 : 1;
        if (res ==0) {
            this->ui->text_browser_output->setText("资产验证不通过");
        }
        else {
            this->ui->text_browser_output->setText("资产验证通过");
        }
```

要主要在该文件开头引入：#include <openssl/bulletproofs.h>

range_proof.ui：

主要分为三部分：一个button按钮，用户点击这个按钮开始验证，edit框用户可以输入自己的资产数字，以及一个结果的输出框。

![img](https://gitee.com/jia-huaiyu/images/raw/master/202309251736037.png)

## 环境文件配置

此时我们的tongsuo_zkp还缺少必要的环境，主要是涉及到零知识证明密码库编译路径，打开项目的pro文件，添加：

```pro
LIBS += -L/opt/tongsuo/lib64 -lcrypto
INCLUDEPATH += /opt/tongsuo/include
QMAKE_LFLAGS += -Wl,-rpath=/opt/tongsuo/lib64
```

这样就可以正常运行。

# 测试

主界面：

![image-20230925174110944](https://gitee.com/jia-huaiyu/images/raw/master/202309251741039.png)

range_proof界面

![image-20230925174136739](https://gitee.com/jia-huaiyu/images/raw/master/202309251741787.png)

非法输入

![image-20230925174156180](https://gitee.com/jia-huaiyu/images/raw/master/202309251741223.png)

![image-20230925174210285](https://gitee.com/jia-huaiyu/images/raw/master/202309251742336.png)

![image-20230925174223960](https://gitee.com/jia-huaiyu/images/raw/master/202309251742004.png)

资产验证：

![image-20230925174255717](https://gitee.com/jia-huaiyu/images/raw/master/202309251742762.png)

![image-20230925174309558](https://gitee.com/jia-huaiyu/images/raw/master/202309251743604.png)





# zkp_range_proof for 2^k

zkp实现更加细分的零知识证明

参考官网源代码：https://github.com/Tongsuo-Project/Tongsuo/blob/master/test/zkp_gadget_test.c

![image.png](https://gitee.com/jia-huaiyu/images/raw/master/202309301613336.png)

从这个示意图中，我们知晓zkp系统的参数分为三部分，分别是公共参数，V方参数，P方参数

其中V需要将密文witness发送给P，P生成ctx，并且接受V方的proof

verify(proof_p, ctx_v)

```c
//public parameters
    int curve_id = NID_X9_62_prime256v1;
    ZKP_TRANSCRIPT *transcript = NULL;
    ZKP_RANGE_PUB_PARAM *pp = NULL;
    EC_KEY *key = NULL;

//setup pp
    if (!(transcript = ZKP_TRANSCRIPT_new(ZKP_TRANSCRIPT_METHOD_sha256(), "test")))
        goto err;

    if (!(key = EC_KEY_new_by_curve_name(curve_id)))
        goto err;

    if (!(EC_KEY_generate_key(key)))
        goto err;
    
    if (!(pp = ZKP_RANGE_PUB_PARAM_new(EC_KEY_get0_group(key), 32)))
        goto err;
```

```c
// P parameters
    BN_CTX *bn_ctx = NULL;
    BIGNUM *v;
    ZKP_RANGE_WITNESS *witness_p = NULL;
    ZKP_RANGE_CTX *ctx_p = NULL;
    ZKP_RANGE_PROOF *proof_p = NULL;

if (!(witness_p = ZKP_RANGE_WITNESS_new(pp, NULL, v)))
        goto err;

    if (!(ctx_p = ZKP_RANGE_CTX_new(transcript, pp, witness_p, key)))
        goto err;

    if (!(proof_p = ZKP_RANGE_PROOF_prove(ctx_p, left_bound_bits, right_bound_bits)))
        goto err;
```

```c
//V parameters
    ZKP_RANGE_WITNESS *witness_v = NULL;
    ZKP_RANGE_CTX *ctx_v = NULL;

witness_v = witness_p;
	if (!(ctx_v = ZKP_RANGE_CTX_new(transcript, pp, witness_v, key)))
        goto err;
	if (!(ZKP_RANGE_PROOF_verify(ctx_v, proof_p, left_bound_bits, right_bound_bits)))
        goto err;
```

通过这个，V方可以验证P方提交的数字是不是在一个有效范围内。

# 后续工作安排

目前，zkp_range_test只是个测试用例，不符合实际使用情况。在接下来的开发中，应该关注现实中，零知识证明系统，是如何输入的，又是如何将P方的加密数据传递给V方
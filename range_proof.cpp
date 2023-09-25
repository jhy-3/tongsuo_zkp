#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QTextBrowser>

#include "range_proof.h"
#include "ui_range_proof.h"

#include <openssl/bulletproofs.h>

range_proof::range_proof(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::range_proof)
{
    ui->setupUi(this);
}

range_proof::~range_proof()
{
    delete ui;
}

static int range_proofs_test(int bits, int64_t secrets[], int len)
{
    int ret = 0, i;
    BIGNUM *v = NULL;
    BP_TRANSCRIPT *transcript = NULL;
    BP_PUB_PARAM *pp = NULL;
    BP_WITNESS *witness = NULL;
    BP_RANGE_CTX *ctx = NULL;
    BP_RANGE_PROOF *proof = NULL;

    v = BN_new();

    /* 创建交互抄本对象，证明者和验证者需要使用相同的方法和标签，否则验证失败 */
    transcript = BP_TRANSCRIPT_new(BP_TRANSCRIPT_METHOD_sha256(), "test");

    /* 创建公共参数对象，这里最大的批量验证个数为8 */
    pp = BP_PUB_PARAM_new_by_curve_id(NID_secp256k1, bits, 8);

    if (v == NULL || transcript == NULL || pp == NULL)
        goto err;

    /* 创建该公共参数下的证据对象 */
    witness = BP_WITNESS_new(pp);
    if (witness == NULL)
        goto err;

    for (i = 0; i < len; i++) {
        if (!BN_lebin2bn((const unsigned char *)&secrets[i], sizeof(secrets[i]), v))
            goto err;

        /* 往证据对象中提交明文证据，由于是 range proof，不需要绑定名称，所以名称可以直接传 NULL */
        if (!BP_WITNESS_commit(witness, NULL, v))
            goto err;
    }

    /* 创建 range proof 上下文对象 */
    ctx = BP_RANGE_CTX_new(pp, witness, transcript);
    if (ctx == NULL)
        goto err;

    /* 创建证明对象 */
    proof = BP_RANGE_PROOF_new_prove(ctx);
    if (proof == NULL)
        goto err;

    /* 验证证明对象是否有效 */
    if (!BP_RANGE_PROOF_verify(ctx, proof))
        goto err;

    ret = 1;
err:
    BP_RANGE_PROOF_free(proof);
    BP_RANGE_CTX_free(ctx);
    BP_WITNESS_free(witness);
    BP_PUB_PARAM_free(pp);
    BP_TRANSCRIPT_free(transcript);
    BN_free(v);

    return ret;
}

void range_proof::on_push_button_clicked()
{
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
    else {
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

    }

}



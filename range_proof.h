#ifndef RANGE_PROOF_H
#define RANGE_PROOF_H

#include <QWidget>

namespace Ui {
class range_proof;
}

class range_proof : public QWidget
{
    Q_OBJECT

public:
    explicit range_proof(QWidget *parent = nullptr);
    ~range_proof();

private slots:
    void on_push_button_clicked();

private:
    Ui::range_proof *ui;
};

#endif // RANGE_PROOF_H

#include "counter.h"
#include "ui_counter.h"

Counter::Counter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Counter)
{
    ui->setupUi(this);
    connect(this->ui->btn_0,SIGNAL(clicked(bool)),this,SLOT(getBtn0()));
    connect(this->ui->btn_1,SIGNAL(clicked(bool)),this,SLOT(getBtn1()));
    connect(this->ui->btn_2,SIGNAL(clicked(bool)),this,SLOT(getBtn2()));
    this->mode = new model;
    this->tmp = "";//初始化
}

Counter::~Counter()
{
    delete ui;
}

void Counter::getBtn0()
{
   if(this->tmp != ""){
    this->tmp += this->ui->btn_0->text();
    this->ui->label_display->setText(this->tmp);
   }
}

void Counter::getBtn1()
{
    this->tmp += this->ui->btn_1->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::getBtn2()
{
    this->tmp += this->ui->btn_2->text();
    this->ui->label_display->setText(this->tmp);
}


void Counter::on_btn_3_clicked()
{
    this->tmp += this->ui->btn_3->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_4_clicked()
{
    this->tmp += this->ui->btn_4->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_5_clicked()
{
    this->tmp += this->ui->btn_5->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_6_clicked()
{
    this->tmp += this->ui->btn_6->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_7_clicked()
{
    this->tmp += this->ui->btn_7->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_8_clicked()
{
    this->tmp += this->ui->btn_8->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_9_clicked()
{
    this->tmp += this->ui->btn_9->text();
    this->ui->label_display->setText(this->tmp);
}

void Counter::on_btn_plus_clicked()
{
    int num = this->tmp.toInt();
    this->mode->setNum1(num);
    this->tmp = "";
    QString ex = this->ui->btn_plus->text();
    this->mode->setFlag(ex);
}

void Counter::on_btn_sub_2_clicked()
{
    int num = this->tmp.toInt();
    this->mode->setNum1(num);
    this->tmp = "";
    QString ex = this->ui->btn_sub_2->text();
    this->mode->setFlag(ex);
}

void Counter::on_btn_mul_clicked()
{
    int num = this->tmp.toInt();
    this->mode->setNum1(num);
    this->tmp = "";
    QString ex = this->ui->btn_mul->text();
    this->mode->setFlag(ex);
}

void Counter::on_btn_div_clicked()
{
    int num = this->tmp.toInt();
    this->mode->setNum1(num);
    this->tmp = "";
    QString ex = this->ui->btn_div->text();
    this->mode->setFlag(ex);
}

void Counter::on_btn_eq_clicked()
{
    int num = this->tmp.toInt();
    this->mode->setNum2(num);
    this->mode->doExpr();
    QString res = this->mode->doExpr();
    this->ui->label_display->setText(res);
    this->tmp = "";
}

void Counter::on_btn_del_clicked()
{
    this->tmp = "";
    this->ui->label_display->setText(0);
}

#include "addstu.h"
#include "ui_addstu.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QList>
#include <QAbstractButton>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QIODevice>


addStu::addStu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addStu)
{
    ui->setupUi(this);
    //connect(this->ui->btn_cancel,SIGNAL(clicked(bool),this,SLOT(close()));
}

addStu::~addStu()
{
    delete ui;
}

void addStu::on_btn_ok_clicked()
{
    QString name = this->ui->le_name->text();
    QString id = this->ui->le_id->text();
    QString sex = this->ui->sexGroup->checkedButton()->text();

    //遍历多选框，选中的保存在ins中
    QList<QAbstractButton *> ins_list = this->ui->insGroup->buttons();
    QString ins;
    int i = 0;
    for(i=0;i<ins_list.length();i++){
     QAbstractButton* che = ins_list.at(i);
     if(che->isChecked()){
         ins += che->text()+" ";
     }
    }
 //QMessageBox msgBox;
 //msgBox.setText("请确认信息");
 //msgBox.setInformativeText(name + '\n' + id);
 //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
 //msgBox.setDefaultButton(QMessageBox::Save);
 //msgBox.addButton("确定",QMessageBox::AcceptRole);
 //QPushButton * cbtn=msgBox.addButton("取消",QMessageBox::RejectRole);
 //msgBox.setDefaultButton(cbtn);
 //int ret = msgBox.exec();//返回用户的选择
 //QMessageBox::aboutQt(this,"hh");
 QString age = this->ui->cbb_age->currentText();
 QString dev = this->ui->cbb_yx->currentText();

 QString content = name + '\n' + id + '\n' + sex + '\n' + age + '\n' + dev + '\n' + ins;
 QString cnt = name + " " + id + " " + sex + " " + age + " " + dev + " " + ins + "\n";
 if(name.length() < 1 || id.length() < 10 || ins.length() < 1){
     QMessageBox::critical(this,"错误","信息填写不完整，请重新检查","确定");
 }
 else
 {
    int ret = QMessageBox::question(this,"请确认信息",content,"确定","取消");//接收返回按钮，顺序为0123
    if(ret == 0){
        clearUserInterface();
        writeToFile(cnt);
    }
 }
}

void addStu::clearUserInterface()
{
    this->ui->le_name->clear();
    this->ui->le_id->clear();
    this->ui->rbtn_male->setChecked(true);
    this->ui->cbb_age->setCurrentIndex(0);
    this->ui->cbb_yx->setCurrentIndex(0);
    QList<QAbstractButton *>ins_list = this->ui->insGroup->buttons();
    int i = 0;
    for(i=0;i<ins_list.length();i++){
        QAbstractButton * box = ins_list.at(i);
        box->setChecked(false);
    }
    this->ui->le_name->setFocus();
}

void addStu::writeToFile(QString cnt)
{
    QFile file("stu.txt");//没有指定存放位置，当前路径，程序在哪就在哪，在debug文件里面
    if(!file.open(QIODevice::Append|QIODevice::Text))
    {
        QMessageBox::critical(this,"错误","文件打开失败，信息没有保存！","确定");
        return;
    }

    QTextStream out(&file);
    out<<cnt;
    file.close();
}


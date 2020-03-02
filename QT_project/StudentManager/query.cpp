#include "query.h"
#include "ui_query.h"
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>


query::query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::query)
{
    ui->setupUi(this);
    if(readFromFile()==-1){
        QMessageBox::critical(this,"严重错误","文件打开失败，请重试");
        this->close();
    }
    this->model = new QStandardItemModel;
    //设置表头
    this->model->setHorizontalHeaderItem(0,new QStandardItem("姓名"));
    this->model->setHorizontalHeaderItem(1,new QStandardItem("学号"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("性别"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("年龄"));
    this->model->setHorizontalHeaderItem(4,new QStandardItem("院系"));
    this->model->setHorizontalHeaderItem(5,new QStandardItem("兴趣"));

    this->ui->tableView->setModel(model);
    this->ui->tableView->setColumnWidth(0,100);
    this->ui->tableView->setColumnWidth(1,150);
    this->ui->tableView->setColumnWidth(2,50);
    this->ui->tableView->setColumnWidth(3,50);
    this->ui->tableView->setColumnWidth(4,100);
    this->ui->tableView->setColumnWidth(5,150);
}

query::~query()
{
    delete ui;
}

int query::readFromFile()
{
    QFile file("stu.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return -1;
    }
    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();
        stu_lines.append(line);
    }
    file.close();

    int i=0;
    for(i=0;i<stu_lines.length();i++)
    {
        qDebug()<<stu_lines.at(i);
    }

    return 0;
}

void query::on_btn_serch_clicked()
{
    //清空，再重新设置表头
    this->model->clear();
    this->model->setHorizontalHeaderItem(0,new QStandardItem("姓名"));
    this->model->setHorizontalHeaderItem(1,new QStandardItem("学号"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("性别"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("年龄"));
    this->model->setHorizontalHeaderItem(4,new QStandardItem("院系"));
    this->model->setHorizontalHeaderItem(5,new QStandardItem("兴趣"));

    this->ui->tableView->setColumnWidth(0,100);
    this->ui->tableView->setColumnWidth(1,150);
    this->ui->tableView->setColumnWidth(2,50);
    this->ui->tableView->setColumnWidth(3,50);
    this->ui->tableView->setColumnWidth(4,100);
    this->ui->tableView->setColumnWidth(5,150);

   int index = this->ui->cbb_method->currentIndex();
   QString cnt = this->ui->le_cnt->text();
   doQuert(index,cnt);
}


// line = line.trimmed();去掉头尾空白
//   subs.at(0);相当于数组
void query::doQuert(int index, QString cnt)
{
   int i = 0;
   int row = 0;
   for(i=0;i<stu_lines.length();i++)
   {
       QString line = stu_lines.at(i);
       line = line.trimmed();
       QStringList subs = line.split(" ");
       switch(index){
       case 1:
           if(cnt == subs.at(0)){
               qDebug()<<line;
               display(row++,subs);
           }
           break;
       case 2:
           if(cnt == subs.at(1)){
               qDebug()<<line;
               display(row++,subs);
           }
           break;
       case 3:
           if(cnt == subs.at(4)){
               qDebug()<<line;
               display(row++,subs);
           }
           break;
       default:
           break;
       }
   }
}


void query::display(int row,QStringList subs)
{
    int i = 0;
    for(i=0;i<5;i++)
    {
        this->model->setItem(row,i,new QStandardItem(subs.at(i)));
    }
    QString ins;
    for(i=5;i<subs.length();i++)
    {
        ins += subs.at(i)+",";
    }
   this->model->setItem(row,5,new QStandardItem(ins));
}

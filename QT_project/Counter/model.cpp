#include "model.h"

model::model()
{

}

void model::setNum1(int num)
{
    this->num1 = num;
}

void model::setNum2(int num)
{
    this->num2 = num;
}

void model::setFlag(QString flag)
{
    this->flag = flag;
}

QString model::doExpr()
{
    int result = 0;
    if(this->flag == "+")
    {
      result = num1 + num2;
    }
    else if(this->flag == "-")
    {
      result = num1 - num2;
    }
    else if(this->flag == "*")
    {
      result = num1 * num2;
    }
    else if(this->flag == "/")
    {
      if(this->num2 == 0)
      {
        return "ERROR";
      }
      return QString::number(result = this->num1 / this->num2);
    }
    else
      return QString::number(this->num1);

    return QString::number(result);
}

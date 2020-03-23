# 调试中遇到的问题

![1584748395438](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584748395438.png)

# C++   继承

```C++
# include<iostream>
using namespace std;

class BasePage
{
public:
    void Header()
    {
        cout<<"公共头部"<<endl;
    }
    void footer()
    {
        cout<<"公共底部"<<endl;
    }
    void left()
    {
        cout<<"公共分类列表"<<endl;
    }
};

//继承
class Java : public BasePage
{
public:
    void content()
    {
        cout<<"Java学科内容"<<endl;
    }
};

class Python : public BasePage
{
public:
    void content()
    {
        cout<<"Python学科内容"<<endl;
    }
};

void example()
{
   Java j;
   j.content();
   Python p;
   p.content();
}

int main()
{
   example();
   return 0;
}
```

继承好处：减少重复代码

语法： class 子类 ： 继承方式 父类

子类 也成为派生类

父类 也称为基类

## 继承方式

1、公共继承

2、保护继承

3、私有继承

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584593832817.png" alt="1584593832817" style="zoom:67%;" />

父类的私有成员任何一种继承方式子类都不能访问

私有继承 父类的公共和保护成员变成了子类的私有成员

```C++
# include<iostream>
using namespace std;

class Base1
{
public:
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};

//公共继承
class son1 :public Base1
{
public:
    void func()
    {
        m_a = 10;//父类中的公共权限成员 到子类中依然是公共权限
        m_b = 10;//父类中的保护权限成员 到子类中依然是保护权限
    }
};

class Base2
{
public:
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};


//保护继承
class son2 :protected Base2
{
public:
       void func()
       {
           m_a = 100;//父类中公有成员，到子类变为保护权限
           m_b = 100;//父类中保护成员，子类依旧是保护成员
           //m_c  子类访问不到父类的私有成员
       }
};

class Base3
{
public:
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};

//私有继承
class son3 :private Base3
{
public:
    void func()
    {
        m_a = 100; //父类中公有，到子类变为私有
        m_b = 100; //父类中保护，到子类变为私有
        //m_c 父类私有，子类访问不了
    }

};

void example()
{
   son1 s1;
   s1.m_a = 100;
   //s1.m_b，类外不可以访问保护成员

   son2 s2;
   //s2.m_a s2.m_b保护成员，类外不可以访问

   son3 s3;
   //没有成员可以访问，因为都是私有成员
}

int main()
{
   example();
   return 0;
}
```

## 继承中的对象模型

问题：从父类继承过来的成员，哪些属于子类对象中

```C++
# include<iostream>
using namespace std;

class Base1
{
public:
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};

class son :public Base1
{
public:
    int m_d;
};

void example()
{
  cout<<"sizeof son = "<< sizeof(son) << endl;
}

int main()
{
   example();
   return 0;
}

```

结果：

![1584595331742](C:\Users\Administrator\Desktop\1584595331742.png)

父类中所有非静态成员属性都会被子类继承下去

**父类中私有成员属性是被编译器隐藏了，虽然访问不到，但是确实被继承了**

![1584595702352](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584595702352.png)

## 继承中构造和析构顺序

子类继承父类后，当创建子类对象，也会调用父类的构造函数

问题：父类和子类的构造和析构顺序是谁先谁后？

```C++
# include<iostream>
using namespace std;

class Base
{
public:
     Base()
     {
         cout<<"Base的构造函数"<<endl;
     }
     ~Base()
     {
         cout<<"Base的析构函数"<<endl;
     }
};

class son :public Base
{
public:
     son()
     {
         cout<<"son的构造函数"<<endl;
     }
     ~son()
     {
         cout<<"son的析构函数"<<endl;
     }
};

void example()
{
   son s1;
}

int main()
{
   example();
   return 0;
}
```

结果：

![1584596094866](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584596094866.png)

**结论：**

**继承中的构造和析构顺序如下：**

**先构造父类，再构造子类，析构的顺序与构造的顺序相反**

## 继承同名成员处理方式

问题：当子类与父类出现同名的成员，如何通过子类对象，访问到子类或父类中同名的数据？

访问子类同名成员  直接访问即可

访问父类同名成员  需要加作用域

同名成员属性处理方式：

```C++
# include<iostream>
using namespace std;

class Base
{
public:
     Base()
     {
         m_a = 100;
     }
     int m_a;
};

class son :public Base
{
public:
    son()
    {
        m_a = 200;
    }
    int m_a;
};

void example()
{
   son s1;
   cout<< "son 下  m_a = "<< s1.m_a <<endl;  //结果是200
   cout<< "Base 下  m_a = "<< s1.Base::m_a <<endl;//结果是100
}

int main()
{
   example();
   return 0;
}
```

同名成员函数处理方式：

```C++
# include<iostream>
using namespace std;

class Base
{
public:
     Base()
     {
         m_a = 100;
     }
     void func()
     {
        cout << "Base func()的调用" << endl;
     }
     int m_a;
};

class son :public Base
{
public:
    son()
    {
        m_a = 200;
    }

    void func()
    {
        cout << "son func()的调用" << endl;
    }
    int m_a;
};

void example()
{
   son s1;
   s1.func();//结果  son func()的调用
   s1.Base::func();//结果：Base func()的调用
}

int main()
{
   example();
   return 0;
}
```

跟成员属性一样

结论：如果子类中出现和父类同名的成员函数，子类的同名成员会隐藏掉父类中所有同名成员函数，要访问需要加作用域

## 继承同名静态成员处理方式

问题：继承中同名的静态成员在子类对象上如何进行访问？

子类出现和父类同名静态成员函数，也会隐藏父类中所有同名成员函数

如果想访问父类中被隐藏同名成员，需要加作用域

```C++
# include<iostream>
using namespace std;

class Base
{
public:
     static int m_a;//类内声明，类外初始化
     static void func()
     {
       cout << "Base 下成员函数" << endl;
     }
};

int Base::m_a = 1;//类外初始化


class son :public Base
{
public:

   static int m_a;
   static void func()
     {
       cout << "son 下成员函数" << endl;
     }
};
int son::m_a = 2;//类外初始化

void example()  //同名静态成员属性
{
   son s1;
   cout << "son 下 m_a = " << s1.m_a << endl;  //结果是2
   cout << "Base 下 m_a = " << s1.Base::m_a << endl;  //结果是1
   //也可以通过类名来访问
   cout << son::m_a << endl;
   //第一个::代表通过类名方式访问 第二个::代表访问父类作用域下
   cout << son::Base::m_a << endl;
}

void example1()  //同名静态成员函数
{
    son s1;
    //通过对象访问
    s1.func();//son
    s1.Base::func();

    //通过类名访问
    son::func();
    son::Base::func();
}

int main()
{
   example1();
   return 0;
}
```

总结：同名静态成员处理方式和非静态处理方式一样，只不过有两种访问的方式（通过对象和通过类名）

## 多继承语法

C++允许一个类继承多个类

语法：class 子类 ：继承方式 父类1 ， 继承方式 父类2........

多继承可能会引发父类中有同名成员出现，需要加作用域区分

C++实际开发中不建议用多继承

它占用内存也是包含全部父类的成员的

## 菱形继承

菱形继承（钻石继承)概念：

两个类继承同一个基类

又有某个类同时继承两个派生类

![1584613283170](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584613283170.png)

![1584613352112](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584613352112.png)



当菱形继承的时候，两个父类拥有相同数据，需要加以作用域区分

利用虚继承，解决菱形继承问题

```C++
# include<iostream>
using namespace std;

class Animal
{
public:
    int m_a;
};

//继承之前，加上关键字，virtual变为虚继承
//Animal类称为虚基类   继承的时候 继承的是一个指针
class Sheep :virtual public Animal{};
class Tuo :virtual public Animal{};

class SheepTuo :public Sheep, public Tuo{};

void example()
{
   SheepTuo s1;
   s1.Sheep::m_a = 18;
   s1.Tuo::m_a = 20;
   //当菱形继承的时候，两个父类拥有相同数据，需要加以作用域区分
   cout << "s1.Sheep::m_a = " << s1.Sheep::m_a << endl;
   cout << "s1.Tuo::m_a = " << s1.Tuo::m_a << endl;

   //这份数据只要一份就可以，菱形继承导致有两份，资源浪费
   //这时候就可以直接访问了
   cout << s1.m_a << endl; //不会报错
}

int main()
{
    example();
    return 0;
}
```

# C++多态

## 基本概念

![1584616226480](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584616226480.png)

父类可以直接引用子类

```C++
class Animal
{
public:
    virtual void speak()
    {
        cout << "动物在说话" << endl;//加上virtual之后就地址晚绑定
    }
}
class Cat :public Animal
{
public:
    //重写 函数返回值类型 函数名 参数列表 完全相同
    void speak()
    {
         cout << "猫在说话" << endl;
    }
  
}
//地址早绑定，在编译阶段确定函数地址
//如果想执行猫说话，那么这个函数地址就不能提前绑定，需要在运行阶段进行绑定，地址晚绑定
void doSpeak(Animal &animal)//这里Animal是父类，可以直接传一个子类cat进来
{
    animal.speak();//这里调用的是父类的成员函数，如果不加virtual关键字
}

void example()
{
    Cat cat;
    doSpeak(cat);
}
```

动态多态条件：

1、有继承关系

2、子类重写父类的虚函数

重写：函数返回值类型 函数名 参数列表 完全一致称为重写

动态多态使用：

父类的指针或者引用，执行子类对象

## 多态的原理剖析

空类的大小是1

![1584666035868](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584666035868.png)

当父类的指针或者引用指向子类对象时候，发生多态

Animal & animal = cat;

animal.speak();

## 多态案例--计算器类

普通写法：

```C++
#include <iostream>
#include <string>
using namespace std;

class Calculator
{
public:
    int getResult(string oper)
    {
        if(oper == "+")
        {
            return m_num1 + m_num2;
        }
        else if(oper == "-")
        {
            return m_num1 - m_num2;
        }
        else if(oper == "*")
        {
            return m_num1 * m_num2;
        }
    }
    int m_num1;
    int m_num2;

};

void example()
{
    Calculator c;
    c.m_num1 = 10;
    c.m_num2 = 1;
    cout << c.m_num1 << "+" << c.m_num2 << "=" << c.getResult("+") << endl;
}

int main()
{
    example();
    return 0;
}
```

如果想扩展新的功能，需要修改源代码

在真是开发中 提倡开闭原则

开闭原则：对扩展进行开发，对修改进行关闭

多态写法：

```C++
#include <iostream>
#include <string>
using namespace std;

class AbstractCalculator
{
public:
    virtual int getResult()
    {
        return 0;
    }
    int m_num1;
    int m_num2;
};

//加法计算器类
class AddCalculator :public AbstractCalculator
{
public:
    int getResult()
    {
        return m_num1 + m_num2;
    }
};

//减法计算器类
class SubCalculator :public AbstractCalculator
{
public:
    int getResult()
    {
        return m_num1 - m_num2;
    }
};

//乘法计算器类
class MulCalculator :public AbstractCalculator
{
public:
    int getResult()
    {
        return m_num1 * m_num2;
    }
};

void example1()
{
    //多态使用条件
    //父类指针或者引用指向子类对象

    AbstractCalculator * a = new AddCalculator;
    a->m_num1 = 9;
    a->m_num2 = 2;
    cout << a->m_num1 << "+" << a->m_num2 << "=" << a->getResult() << endl;
    delete a;
}

int main()
{
    example1();
    return 0;
}
```

实际开发尽量考虑多使用多态，结构清晰，方便维护

## 纯虚函数和抽象类

![1584670454965](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584670454965.png)

```C++
//它是抽象类，无法实例化对象，子类必须重写它的纯虚函数
class Base
{
public:
    virtual void func() = 0;
}

class son :public Base
{
    virtual void function()
    {
        cout << "func函数调用" << endl;//重写之后可以创建对象
    }
}

//son s  如果不重写父类的虚函数，就不能创建对象，它也是抽象类
```

## 多态案例2-制作饮品

有一个警告：||===warning: deleting object of abstract class type 'AbstractDrinking' which has non-virtual destructor will cause undefined behaviour 

这个是因为父类中没有虚析构函数，导致内存泄漏了

```C++
#include <iostream>
using namespace std;

class AbstractDrinking
{
public:
    //煮水
    virtual void Boil() = 0;
    //冲泡
    virtual void Brew() = 0;
    //倒入杯中
    virtual void PourInCup() = 0;
    //加入辅料
    virtual void PutSomething() = 0;

    //制作饮品
    void MakeDrink()
    {
       Boil();
       Brew();
       PourInCup();
       PutSomething();
    }
};

//咖啡
class Coffee :public AbstractDrinking
{
public:
    virtual void Boil()
    {
        cout << "煮农夫山泉" << endl;
    }
    virtual void Brew()
    {
        cout << "冲泡咖啡" << endl;

    }
    virtual void PourInCup()
    {
        cout << "倒入杯中" << endl;
    }
    virtual void PutSomething()
    {
        cout << "加入糖和牛奶" << endl;
        cout << "咖啡制作完毕" << endl;
    }
};

//茶
class Tea :public AbstractDrinking
{
public:
    virtual void Boil()
    {
        cout << "煮矿泉水" << endl;
    }
    virtual void Brew()
    {
        cout << "冲泡茶叶" << endl;

    }
    virtual void PourInCup()
    {
        cout << "倒入杯中" << endl;
    }
    virtual void PutSomething()
    {
        cout << "加入柠檬" << endl;
    }
};

void dowork(AbstractDrinking * a)
{
   a->MakeDrink();
   delete a;//释放
}

void example()
{
    //制作咖啡
    dowork(new Coffee);
    //泡茶
    dowork(new Tea);
}

int main()
{
    example();
    return 0;
}
```

## 虚析构和纯虚析构

![1584671973796](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584671973796.png)

有了纯虚析构之后，这个类也属于抽象类，无法实例化对象

在父类加入虚析构或者纯虚析构的目的是解决子类中析构函数无法调用的问题

```C++
#include <iostream>
#include <string>
using namespace std;

class Animal
{
public:
    Animal()
    {
       cout << "Animal的构造函数调用" << endl;
    }
    //利用虚析构可以解决，父类指针释放子类对象时不干净的问题
    //virtual ~Animal()
    // {
    //    cout << "Animal的析构函数调用" << endl;
    //}
    //如果是直接在类内纯虚析构，会报错，在类外实现就不会
    //即它需要声明，也需要实现，有了纯虚析构之后，这个类也属于抽象类，无法实例化对象
    virtual ~Animal() = 0;

    virtual void speak() = 0;
};

//纯虚析构
Animal::~Animal()
{
    cout << "Animal的纯虚析构" << endl;
}

class Cat :public Animal
{
public:
    Cat(string name)
    {
        cout << "Cat的构造函数调用" << endl;
        m_name = new string(name);
    }
    virtual void speak()
    {
        cout << *m_name << "小猫在说话" << endl;
    }

    ~Cat()
    {
        if(m_name != NULL)
        {
            cout << "Cat的析构函数调用" << endl;
            delete m_name;
            m_name = NULL;
        }
    }

    string * m_name;

};

void example()
{
    Animal * animal = new Cat("Tom");
    animal->speak();
    //父类指针在析构的时候，不会调用子类中析构函数，导致子类如果有堆区属性，出现内存泄漏
    delete animal;
}

int main()
{
    example();
    return 0;
}
```

**总结：**

![1584673571357](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584673571357.png)

## 多态案例3-电脑组装

![1584673808558](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584673808558.png)

![1584674229927](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584674229927.png)

# C++文件操作

![1584674296966](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584674296966.png)

## 文本文件

![1584674453561](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584674453561.png)

文件操作方式

![1584674666382](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584674666382.png)

**写文件**

```C++
#include <iostream>
#include <fstream>
using namespace std;

void example()
{
    ofstream ofs;
    //如果不指定路径，则默认工程路径
    ofs.open("123.txt",ios::out);

    ofs << "姓名：haha" << endl;
    ofs << "姓名：ha" << endl;
    ofs << "姓名：taha" << endl;

    ofs.close();
}

int main()
{
    example();
    return 0;
}
```

**读文件**

![1584675180402](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584675180402.png)

```C++
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void example()
{
    ofstream ofs;
    //如果不指定路径，则默认工程路径
    ofs.open("123.txt",ios::out);

    ofs << "姓名：haha" << endl;
    ofs << "姓名：ha" << endl;
    ofs << "姓名：taha" << endl;

    ofs.close();
}

void example1()
{
    ifstream ifs;
    ifs.open("123.txt",ios::in);
    if(!ifs.is_open())
    {
        cout << "文件打开失败" <<endl;
        return;
    }
    //读数据
    //第一种
   // char buf[1024] = {};
   // while(ifs >> buf)
   // {
   //     cout << buf << endl;
   // }

    //第二种
 //   char buf1[1024] = {0};
  //  while (ifs.getline(buf,sizeof(buf)))
  //  {
   //     cout << buf1 << endl;
  //  }

    //第三种
    string buf;
    while(getline(ifs,buf))
    {
        cout << buf << endl;
    }
    //第四种 不推荐这种 效率低
    char c;
    while((c = ifs.get()) != EOF)//EOF end of file
    {
        cout << c;
    }
}

int main()
{
    example1();
    return 0;
}
```

## 二进制文件

![1584697205552](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584697205552.png)

读和写：

```C++
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Person
{
public:
    char m_name[64];
    int age;
};

void example()
{
    //写文件
    //ofstream ofs("person.txt",ios::out|ios::binary);
    ofstream ofs;
    ofs.open("person.txt", ios::out|ios::binary); //或者可以直接在创建对象的时候传到构造函数里
    Person p = {"张三",18};
    ofs.write((const char *)&p,sizeof(Person));
    ofs.close();
}

void example1()
{
    ifstream ifs;

    ifs.open("person.txt",ios::in|ios::binary);

    if(!ifs.is_open())
    {
        cout << "文件打开失败" << endl;
        return;
    }

    Person p;
    ifs.read((char *)&p,sizeof(Person));
    cout << "姓名：" << p.m_name << "年龄：" << p.age << endl;
    ifs.close();
}

int main()
{
    example1();
    return 0;
}
```

# C++提高 （泛型编程和STL） 

# 模板

## 函数模板

![1584707884576](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584707884576.png)

```C++
#include <iostream>
using namespace std;

template <typename T>//声明一个模板，告诉编译器后面代码中紧跟着的T不要报错，T是一个通用数据类型
void nySwap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

void example()
{
    int a = 1;
    int b = 2;

    //用函数模板，两种方式
    //1、自动类型推导
    nySwap(a, b);
    //2、显式指定类型
    nySwap<int>(a,b);
    cout << "a = " << a << "b = " << b << endl;
}

int main()
{
  example();
  return 0;
}

```

总结：

## ![1584709258357](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584709258357.png)

注意事项：

![1584709329316](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584709329316.png)

例子：

```C++
#include <iostream>
using namespace std;

template<class T>//typename可以替换成class
void mySwap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

void example()
{
    int a = 1;
    int b = 2;
    char c = 'c';

    //mySwap(a, c)//错误的，推导不出一致的T类型

    mySwap(a, b);
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
}


template<class T>
void func()
{
    cout << "func的调用" << endl;
}

void example1()
{
    func<int>();//确定数据类型，不添加int会出错
}

int main()
{
    example1();
    return 0;
}
```

## 函数模板案例

![1584711405144](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584711405144.png)

```C++
#include <iostream>
using namespace std;

//交换函数模板
template<class T>
void mySwap(T &a, T &b)
{
    T Temp = a;
    a = b;
    b = Temp;
}

//实现通用对数组进行排序的函数  选择排序
template<class T>
void mySort(T arr[], int len)
{
     for(int i=0; i<len; i++)
     {
         int max = i;
         for(int j=i+1; j<len; j++)
         {
             if(arr[max] < arr[j])
             {
                 max = j;
             }
         }
         if(max != i)
         {
             mySwap(arr[max],arr[i]);
         }
     }
}

//打印的函数模板
template<class T>
void printArray(T arr[], int len)
{
    for(int i = 0; i < len; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void example()
{
   char charArr[] = "abdsvj";
   int num = sizeof(charArr) / sizeof(char);
   mySort(charArr, num);
   printArray(charArr,num);
}

void example1()
{
    int arrArr[] = {3,5,7,2,8,9};
    int num = sizeof(arrArr) / sizeof(int);
    mySort(arrArr,num);
    printArray(arrArr,num);
}

int main()
{
    example();
    example1();
    return 0;
}
```

## 普通函数与函数模板的区别

![1584748664019](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584748664019.png)

```C++
#include <iostream>
using namespace std;

int myAdd(int a, int b)
{
    return a + b;
}

template<class T>
T myAdd2(T a, T b)
{
    return a + b;
}

int main()
{
   int a = 1;
   int b = 2;
   char c = 'c';
   cout << myAdd(a, b) << endl;
   //cout << myAdd2(a, c)) << endl; //如果传进a和c则报错 自动类型推导不发生自动类型转换
   cout << myAdd2<int>(a, c) << endl;//指定类型不会报错 显式指定类型 发生隐式类型转换
    return 0;
}
```

## 普通函数与函数模板的调用规则

![1584749910907](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584749910907.png)

```C++
#include <iostream>
using namespace std;

//普通函数
void myPrint(int a,int b)
{
    cout << "普通函数调用" << endl;
}

//函数模板
template<class T>
void myPrint(T a, T b)
{
    cout << "函数模板调用" << endl;
}


//函数模板重载
template<class T>
void myPrint(T a, T b, T c)
{
    cout << "重载函数模板调用" << endl;
}

void example()
{
    int a = 1;
    int b = 2;
    //myPrint(a, b);//结果 普通函数调用
    //myPrint<>(a, b);//结果 函数模板调用  空模板参数列表，强制调用函数模板
    char c1 = 'z';
    char c2 = 's';
    myPrint(c1, c2);
}

int main()
{
    example();
    return 0;
}
```

**总结**

![1584750564538](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584750564538.png)

## 模板的局限性

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584750666001.png" alt="1584750666001" style="zoom: 67%;" />



**总结：**

利用具体化的模板，可以解决自定义类型的通用化

学习模板并不是为了写模板，而是在STL能够运用系统提供的模板

```C++
#include <iostream>
#include <string>
using namespace std;

class Person
{
public:
    Person(string name, int age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    string m_name;
    int m_age;
};

template<class T>
bool myCompare(T &a, T &b)
{
    if(a == b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//利用具体化person的版本实现代码，具体化优先调用
template<> bool myCompare(Person &a, Person &b)
{
    if(a.m_name == b.m_name && a.m_age == b.m_age)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void example()
{
    int a = 1;
    int b = 2;
    bool ret = myCompare(a, b);
    if(ret)
    {
        cout << "相等" << endl;
    }
    else
    {
       cout << "不相等" << endl;
    }
}

void example1()
{
    Person p1("Tom",10);
    Person p2("Tom",10);

    bool ret = myCompare(p1, p2);//用原来函数模板会报错 重载之后不会出错
    if(ret)
    {
        cout << "相等" << endl;
    }
    else
    {
       cout << "不相等" << endl;
    }
}

int main()
{
    example1();
    return 0;
}

```

## 类模板

![1584752065320](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584752065320.png)

```C++
#include <iostream>
#include <string>
using namespace std;

//类模板
template<class NameType, class AgeType>
class Person
{
public:
    Person(NameType name, AgeType age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    void showPerson()
    {
        cout << this->m_name << endl;
        cout << this->m_age << endl;
    }
    NameType m_name;
    AgeType m_age;
};

void example()
{
    Person<string,int> p1("孙悟空",500);
    p1.showPerson();
}

int main()
{
    example();
    return 0;
}
```

## 类模板与函数模板的区别

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584752623311.png" alt="1584752623311" style="zoom: 80%;" />



```C++
#include <iostream>
#include <string>
using namespace std;

//类模板
template<class NameType, class AgeType = int>//类模板在模板参数列表中可以有默认参数
class Person
{
public:
    Person(NameType name, AgeType age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    void showPerson()
    {
        cout << this->m_name << endl;
        cout << this->m_age << endl;
    }
    NameType m_name;
    AgeType m_age;
};

void example()
{
    //Person p("ahsja",78); 错误，类模板没有自动类型推导
    Person<string,int> p1("孙悟空",500);
    p1.showPerson();
}

void example1()
{
    Person<string> p("猪八戒", 890);
    p.showPerson();
}

int main()
{
    example1();
    return 0;
}
```

## 类模板中成员函数的创建时机

![1584753323554](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584753323554.png)

```C++
#include <iostream>
#include <string>
using namespace std;

class Person
{
public:
    void showPerson()
    {
      cout << "Person show" << endl;
    }
};

class Person1
{
public:
    void showPerson1()
    {
      cout << "Person1 show" << endl;
    }
};

template<class T>
class myclass
{
public:
    T obj;

    void func1()
    {
        obj.showPerson();//这样用还不会报错，因为不知道obj是什么数据类型
    }
    void func2()
    {
        obj.showPerson1();
    }
};

void example()
{
  myclass<Person> m;
  m.func1();
  //m.func2();//func2中调用的是Person1中的成员函数，所以这里会报错，声明的时候只声明了Person
}

int main()
{
    example();
    return 0;
}
```

## 类模板对象做函数参数

![1584754078389](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584754078389.png)

使用比较广泛是第一种：指定传入类型

```C++
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

template<class T1, class T2>
class Person
{
public:
    Person(T1 name, T2 age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    void showPerson()
    {
        cout << this->m_name << endl;
        cout << this->m_age << endl;
    }

    T1 m_name;
    T2 m_age;
};

//指定传入类型 最常用
void printPerson(Person<string ,int> &p)
{
   p.showPerson();
}

void example()
{
   Person<string ,int> p("sa",89);
   printPerson(p);
}

//参数模板化
template<class T1, class T2>
void printPerson1(Person<T1 , T2> &p)
{
   p.showPerson();
   cout << "T1的类型" << typeid(T1).name() << endl;
   cout << "T2的类型" << typeid(T2).name() << endl;
}

void example1()
{
   Person<string ,int> p1("sfef",989);
   printPerson1(p1);
}


//整个类模板化
template<class T>
void printPerson3(T &p)
{
    p.showPerson();
     cout << "T的类型" << typeid(T).name() << endl;
}

void example3()
{
    Person<string ,int> p("sewsda",89);
    printPerson3(p);
}

int main()
{
    example3();
    return 0;
}
```

## 类模板与继承

![1584755454279](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584755454279.png)

```C++
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

template<class T>
class Base
{
    T a;
};

//class son :public Base//错误，必须要知道父类中T的类型才能  继承给子类
class son :public Base<int>//指定参数类型
{

};

//如果想灵活指定父类中T类型，子类也需要变为类模板
template<class T1,class T2>
class son2 :public Base<T2>
{
public:
    son2()
    {
        cout << "T1 type : " << typeid(T1).name() << endl;
        cout << "T2 type : " << typeid(T2).name() << endl;
    }
    T1 obj;
};

void example()
{
   son2<int,char> s2;
}

int main()
{
    example();

    return 0;
}

```

## 类模板成员函数类外实现

```C++
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

//类模板成员函数类外实现
template<class T1, class T2>
class Person
{
public:
    Person(T1 name, T2 age);
//    {
//       this->m_name = name;
//       this->m_age = age;
//    }
    void showPerson();
//    {
//        cout << this->m_name << endl;
//        cout << this->m_age << endl;
//    }
    T1 m_name;
    T2 m_age;
};

//构造函数类外实现
template<class T1,class T2>
Person<T1,T2>::Person(T1 name, T2 age)
{
    this->m_name = name;
    this->m_age = age;
}

//成员函数的类外实现
template<class T1,class T2>
void Person<T1,T2>::showPerson()
{

    cout << this->m_name << endl;
    cout << this->m_age << endl;

}

void example()
{
    Person<string ,int> p("hha",29);
    p.showPerson();
}

int main()
{
    example();

    return 0;
}
```

**总结：**类模板中成员函数类外实现时，需要加上模板参数列表

## 类模板分文件编写

```C++
//main.cpp
#include <iostream>
#include <string>
#include <typeinfo>
//第一种解决方式，直接包含源文件
//#include "Person.cpp" //注意这里
//解析，为什么要包含.cpp文件，因为类模板成员函数在调用的时候才会被创建，如果
//只包含.h头文件，它只找到类模板成员函数的声明，并没有找到实现

//第二种解决方式，将.h和.cpp中的内容写到一起，将后缀名改为.hpp文件
#include "Person.hpp"
using namespace std;


void example()
{
    Person<string,int> p("hha",29);
    p.showPerson();
}

int main()
{
    example();
    return 0;
}

//Person.hpp

#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED
#include <string>
using namespace std;
template<class T1, class T2>
class Person
{
public:
    Person(T1 name, T2 age);
    void showPerson();

    T1 m_name;
    T2 m_age;
};

//构造函数类外实现
template<class T1,class T2>
Person<T1,T2>::Person(T1 name, T2 age)
{
    this->m_name = name;
    this->m_age = age;
}

//成员函数的类外实现
template<class T1,class T2>
void Person<T1,T2>::showPerson()
{
    cout << this->m_name << endl;
    cout << this->m_age << endl;
}

#endif // PERSON_H_INCLUDED

```

## 类模板与友元

全局函数类内实现，直接在类内声明友元即可

全局函数类外实现，需要提前让编译器知道全局函数的存在

```C++
#include <iostream>
#include <string>
using namespace std;

template<class T1, class T2>
class Person;//声明类


//类外实现 把函数放在类前
template<class T1, class T2>
void printPerson2(Person<T1, T2>  p)
{
   cout << "类外实现" << endl;
   cout << p.m_name << p.m_age << endl;
}


template<class T1, class T2>
class Person
{
    //全局函数类内实现
    friend void printPerson(Person<T1, T2>  p)
    {
        cout << p.m_name << p.m_age << endl;
    }
    //全局函数类外实现
    //如果全局函数是类外实现，需要让编译器提前知道这个函数的存在
    friend void printPerson2<>(Person<T1, T2>  p);//加空参数列表

public:
    Person(T1 name, T2 age)
    {
       this->m_name = name;
       this->m_age = age;
    }
private:
    T1 m_name;
    T2 m_age;
};

void example1()
{
    Person<string ,int> p1("哈哈哈",23);
    printPerson2(p1);
}

//全局函数在类内实现
void example()
{
    Person<string ,int> p("asa",23);
    printPerson(p);

}

int main()
{
    example1();
    return 0;
}

```

建议类内实现，类外实现较为麻烦

## 类模板案例

![1584836184804](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584836184804.png)



```C++
//main.cpp
#include <iostream>
#include <string>
#include "MyArray.hpp"
using namespace std;

void printIntArray(MyArray<int>& a)
{
    for(int i=0;i<a.getSize();i++)
    {
        cout << a[i] << endl;
    }
}

void example()
{
    MyArray <int>arr1(5);

    for(int i = 0;i < 5;i++)
    {
        arr1.Push_Back(i);
    }
    printIntArray(arr1);
    cout << "arr1 's capacity:" << arr1.getCapacity() << endl;
    cout << "arr1 's size:" << arr1.getSize() << endl;

    MyArray <int>arr2(arr1);//拷贝构造
    printIntArray(arr2);
    cout << "arr2尾删后：" << endl;
    arr2.Pop_Back();
    cout << "arr2 's capacity:" << arr2.getCapacity() << endl;
    cout << "arr2 's size:" << arr2.getSize() << endl;
//   MyArray <int>arr3(100);
//    arr3 = arr1;
}

//测试自定义数据类型
class Person
{
public:
    Person() {};//这里如果不添加会报错
    Person(string name,int age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    string m_name;
    int m_age;
};

void printPersonArray(MyArray<Person>& b)
{
    for(int i=0;i<b.getSize();i++)
    {
        cout << "name:" << b[i].m_name << "   age:" << b[i].m_age << endl;
    }
}

void example1()
{
    MyArray<Person> arr(10);

    Person p1("haha1",23);
    Person p2("haha2",23);
    Person p3("haha3",23);
    Person p4("haha4",24);
    Person p5("haha5",22);

    //将数据插入到数组中
    arr.Push_Back(p1);
    arr.Push_Back(p2);
    arr.Push_Back(p3);
    arr.Push_Back(p4);
    arr.Push_Back(p5);

    //打印数组
    printPersonArray(arr);
    cout << "arr 's capacity:" << arr.getCapacity() << endl;
    cout << "arr 's size:" << arr.getSize() << endl;
}
int main()
{
    example1();
    return 0;
}


//MyArray.hpp
#ifndef MYARRAY_HPP_INCLUDED
#define MYARRAY_HPP_INCLUDED
#include <iostream>
using namespace std;
#include <string>

template<class T>
class MyArray
{
public:
    MyArray(int capacity)
    {
        //cout << "MyArray有参构造调用" << endl;
        this->m_capacity = capacity;
        this->m_size = 0;
        this->pAddress = new T[this->m_capacity];
    }
    MyArray(const MyArray& arr)
    {
        //cout << "MyArray拷贝构造调用" << endl;
        this->m_capacity = arr.m_capacity;
        this->m_size = arr.m_size;
        this->pAddress = new T[arr.m_capacity];//深拷贝防止堆区内存重复释放

        for(int i = 0;i < this->m_size;i++)//把原来数组中有的内容也拷贝过来
        {
            this->pAddress[i] = arr.pAddress[i];
        }
    }
    //尾插法
    void Push_Back(const T &val)
    {
        //判断容量是否合适
        if(this->m_capacity == this->m_size)
        {
            return;
        }
        this->pAddress[this->m_size] = val;//在数组末尾插入数据
        this->m_size++;//更新数组大小
    }

    //尾删
    void Pop_Back()
    {
        //让用户访问不到最后一个元素，即为尾删，逻辑删除
        if(this->m_size == 0)
        {
            return ;
        }
        this->m_size--;
    }
    //可以通过下标方式访问数组中的元素
    T& operator[](int index)
    {
        return this->pAddress[index];
    }
    //返回数组的容量
    int getCapacity()
    {
        return this->m_capacity;
    }
    //返回数组大小
    int getSize()
    {
        return this->m_size;
    }
    ~MyArray()
    {
        //cout << "MyArray析构调用" << endl;
        if(this->pAddress != NULL)
        {
           delete[] this->pAddress;
           this->pAddress = NULL;
        }
    }
    //operator= 防止浅拷贝问题
    MyArray& operator=(const MyArray& arr)
    {
        //cout << "MyArray  operator=构造调用" << endl;
        //先判断原来堆区是否有数据，如果有先释放
        if(this->pAddress != NULL)
        {
            delete[] this->pAddress;
            this->pAddress = NULL;
            this->m_capacity = 0;
            this->m_size = 0;
        }

        //深拷贝
        this->m_capacity = arr.m_capacity;
        this->m_size = arr.m_size;
        this->pAddress = new T[arr.m_capacity];
        for(int i = 0;i < this->m_size;i++)//把原来数组中有的内容也拷贝过来
        {
            this->pAddress[i] = arr.pAddress[i];
        }
        return *this;
    }

private:
    T * pAddress;//指针指向堆区开辟的数组
    int m_capacity;
    int m_size;
};

#endif // MYARRAY_HPP_INCLUDED

```

# STL

## STL基本概念

![1584838264976](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584838264976.png)

![1584838479363](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584838479363.png)

![1584838705921](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584838705921.png)

![1584838957012](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584838957012.png)

![1584839002934](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584839002934.png)

## 容器算法迭代器初识

![1584839151581](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584839151581.png)

for_each函数

![1584840289857](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584840289857.png)

```C++
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>//标准算法头文件
using namespace std;


void myPrint(int val)
{
    cout << val << endl;
}

//Vector容器存放内置数据类型
void example()
{
    //创建了一个vector容器
    vector<int> v;
    //向容器中插入数据
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);

    //通过迭代器访问容器中的数据
//    vector<int>::iterator itBegin = v.begin();//起始迭代器 指向容器中第一个元素
//    vector<int >::iterator itEnd = v.end();//结束迭代器 指向容器中最后一个元素的下一个位置
//
//    //第一种遍历方式
//    while(itBegin != itEnd)
//    {
//        cout << *itBegin << endl;
//        itBegin++;
//    }

//   第二种遍历方式
     for(vector<int>::iterator it = v.begin(); it != v.end();it++)
     {
         cout << *it << endl;
     }

     //第三种遍历方式 利用STL中提供的遍历算法
     for_each(v.begin(),v.end(),myPrint);
}


int main()
{
    example();
    return 0;
}
```

vector存放自定义数据类型

```C++
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>//标准算法头文件
using namespace std;

class Person
{
public:
    Person(string name, int age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    string m_name;
    int m_age;
};


void example()
{
   vector<Person> v;
   Person p1("a",37);
   Person p2("c",35);
   Person p3("v",31);
   Person p4("b",32);
   Person p5("h",34);

   //向容器中添加数据
   v.push_back(p1);
   v.push_back(p2);
   v.push_back(p3);
   v.push_back(p4);
   v.push_back(p5);

   //开始遍历容器中的数据
   for(vector<Person>::iterator c = v.begin(); c != v.end(); c++)
   {
       cout << "name:" << (*c).m_name << "age :" << (*c).m_age << endl;
   }
}

void example1()
{
      vector<Person *> v;
   Person p1("a",37);
   Person p2("c",35);
   Person p3("v",31);
   Person p4("b",32);
   Person p5("h",34);

   //向容器中添加数据
   v.push_back(&p1);
   v.push_back(&p2);
   v.push_back(&p3);
   v.push_back(&p4);
   v.push_back(&p5);

   //开始遍历容器中的数据
   for(vector<Person *>::iterator c = v.begin(); c != v.end(); c++)
   {
       cout << "name:" << (*c)->m_name << "age :" << (*c)->m_age << endl;
   }
}

int main()
{
    example1();
    return 0;
}
```

## Vector容器中嵌套容器

```C++
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>//标准算法头文件
using namespace std;

//容器嵌套容器
void example()
{
    vector<vector<int> > v;

    //创建小容器
    vector<int> v1;
    vector<int> v2;
    vector<int> v3;
    vector<int> v4;

    //向小容器中添加数据
    for(int i=0;i<4;i++)
    {
        v1.push_back(i+1);
        v2.push_back(i+2);
        v3.push_back(i+3);
        v4.push_back(i+4);
    }

    //将小容器插入到大容器中
    v.push_back(v1);
    v.push_back(v2);
    v.push_back(v3);
    v.push_back(v4);

    //大容器遍历数据
    for(vector<vector<int> >::iterator it = v.begin();it != v.end();it++)
    {
        //(*it) --- 容器 vector<int>
        for(vector<int>::iterator vit = (*it).begin();vit != (*it).end();vit++)
        {
            cout << *vit << " ";
        }
        cout << endl;
    }
}

int main()
{
    example();
    return 0;
}
```

## string容器

**string基本概念**

![1584851290594](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584851290594.png)

![1584851572155](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584851572155.png)

```C++
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>//标准算法头文件
using namespace std;


void example()
{
    string s1;

    const char * str = "haha";
    string s2(str);

    cout << "s2 = " << s2 << endl;

    string s3(s2);
    cout << "s3 = " << s3 << endl;

    string s4(10,'d');//使用n个字符c初始化
    cout << "s4 = " << s4 << endl;
}

int main()
{
    example();
    return 0;
}
```

**string赋值操作**

![1584864473609](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584864473609.png)

```C++
#include <iostream>
#include <string>
using namespace std;


void example()
{
   string str;
   str.assign("hahah");
   cout << str << endl;

   string str1;
   str1.assign("nuighcduahah",5);
   cout << str1 << endl;

   string str2;
   str2.assign(str1);
   cout << str2 << endl;

   string str3;
   str3.assign(10,'e');
   cout << str3 << endl;
}

int main()
{
    example();
    return 0;
}
```

**string字符串拼接**

![1584882063109](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584882063109.png)




















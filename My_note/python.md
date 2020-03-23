# 2020-3-18

split()--分割，返回一个列表
	list1 = mystr.split(and)
	print(list1)
分割之后分割子串会丢失

-------------------------------------
	join() 合并列表的字符串为一个大字符串
语法：字符或子串.join(多字符串组成的序列)
	new_str = ',,,'.join(mylist)
	

-----------------------
capitalize() ：将字符串第一个字符转换成大写

----------------------
title()将每个单词的首字母变成大写

-------------------------
upper()全部字母变成大写

---------------------------
lower()将全部字母变成小写

-----------------------------
lstrip():删除字符串左侧空白字符

----------------------------
rstrip()删除字符串右侧空白字符

--------------------------------------------
strip()删除字符串两侧空白字符

------------------------------------
字符串对齐
左对齐
	str.ljust(10)
	str.ljust(10,'.')
右对齐
	str.rjust()
中间对齐

str.center()

布尔型
startswitch():检查字符串是否是以指定子串开头，是则返回True，否则返回False。如果设置开始和结束位置下标，则在指定范围内检查
语法：
	字符串序列.startswitch(子串，开始位置下标，结束位置下标)//后面两个参数可以省略

endswitch()



--------------------------------------------
isspace():如果字符串中只包含空白，则返回True，否则返回False

isalpha():字母，全是字母返回真

isdigit():数字，全是数字返回真

isalnum():数字或字母组合

---------------------------------------------

```python
# 列表
# name_list = ['Tom', 'Lily', 'Rose']
# print(name_list)
# print(name_list[0])

# 列表查找
# len():访问列表长度，即列表数据的个数

# print(name_list.index('as'))  # 要查找的字符串不存在则报错
# print(len(name_list))  #输出结果为3

# print('Tom' in name_list)  # 结果为True
# print('ha' in name_list)   # 结果为False
# print('Tom' not in name_list)  # 结果为False
# print('Tons' not in name_list)  # 结果为True检查数据是否重复

# example
"""

name = input('请输入你的账号名字：')

if name in name_list:
    print(f'您输入的名字是{name}，此用户名已经存在')
else:
    print(f'您输入的名字是{name}，可以注册')

"""

# 增加 append():列表结尾追加数据 列表是可变数据类型
# name_list = ['Tom', 'Lily', 'Rose']
# name_list.append('hh')
# nprint(name_list)

# extend():列表结尾追加数据，如果数据是一个序列，则将这个序列的数据逐一添加到列表
# name_list.extend('hohi')
# print(name_list)  # 输出结果 ['Tom', 'Lily', 'Rose', 'h', 'o', 'h', 'i']

# insert():指定位置新增数据
# name_list.insert(1, 'aaa')
# print(name_list)  # 输出结果 ['Tom', 'aaa', 'Lily', 'Rose']

# name_list = ['Tom', 'Lily', 'Rose']

# del  删除
# 用法
# del name_list[0]
# print(name_list)  # 输出结果：['Lily', 'Rose']
# 又或者
# del name_list  # 整个删除

# pop() 删除指定下标的数据，如果不指定下标，默认删除最后一个数据
# 无论是按照下标还是删除最后一个，pop函数都会返回这个被删除的数据
# del_name = name_list.pop()或者del_name = name_list.pop(1)
# print(del_name)  # 输出结果 Rose
# print(name_list)  # 输出结果 ['Tom', 'Lily']


# remove
# name_list = ['Tom', 'Lily', 'Rose']
# name_list.remove('Tom')
# print(name_list)  # 输出结果['Lily', 'Rose']

# clear
# name_list.clear()
# print(name_list)  # 输出结果 []

# reverse() 逆置
# num_list = [1, 2, 3, 4, 5, 6, 7, 8]
# num_list.reverse()
# print(num_list)  # 输出结果：[8, 7, 6, 5, 4, 3, 2, 1]

# sort() 排序：升序 和降序  默认升序排列
# 语法： 列表序列.sort( key=None, reverse=False)
# num_list.sort(reverse=True)  # 这样就降序
# print(num_list)  # 输出结果 [1, 2, 3, 4, 5, 6, 7, 8]


```



# 2020-3-19

##### 字符串复制

```python
name_list = ['Tom', 'Lily', 'Rose']
name_list2 = name_list.copy()
```

##### 列表的遍历

```python
name_list = ['Tom', 'Lily', 'Rose']
i = 0
while i < len(name_list):
    print(name_list[i])
    i += 1
```

结果

![1584608087194](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584608087194.png)

用for

```python
name_list = ['Tom', 'Lily', 'Rose']
i = 0
for i in name_list:
    print(i)

```

结果

![1584608297918](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584608297918.png)

##### 列表嵌套

```python
name_list = [['小明', '小红', '小兰'], ['Tom', 'Lily', 'Rose'], ['张三', '李四', '王五']]
# 找李四 先找到所在列表就OK
print(name_list[2])
# 再定位李四
print(name_list[2][1])

```

##### 一个简单案例

```python
import random  # 导入生成随机数所需模块
# 需求：8位老师，3个办公室，将8位老师随机分配到3个办公室
teachers = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H']
offices = [[], [], []]

for name in teachers:
    num = random.randint(0, 2)
    offices[num].append(name)
# print(offices)
i = 1
for office in offices:
    print(f'办公室{i}的人数是{len(office)},老师分别是：')
    for name in office:
        print(name)
    i += 1

```

结果：

![1584609600448](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584609600448.png)

##### 总结

![1584609635778](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584609635778.png)

##### 元祖

一个元祖可以存储多个数据，元祖内的数据是不能修改的

元祖特点:定义元祖使用小括号，且逗号隔开各个数据，数据可以是不同的数据类型

注意：如果定义的元祖只有一个数据，那么这个数据后面要添加逗号，否则数据类型为唯一的这个数据的数据类型

例子：

![1584610997370](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1584610997370.png)


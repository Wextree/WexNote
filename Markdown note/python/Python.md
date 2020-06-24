# 语法

## if

if 要判断的条件:

​	条件成立时，要做的事情（前面用tab或者是4个空格）**建议空格，但是IDE会自动缩进**

​	......

else :

​	条件不成立时，要做的事情

##逻辑运算

**and or not**

**elif**  其实就是我们C中的 else if

（shift + tab/back 来统一缩进）

**随机数的使用：**

import random

random.randint（a,b）   返回a，b之间的整数（包含ab，且a<=b）



### 循环

**while**

while 条件（判断 计数器 是否达到 目标次数）:

​	条件满足时，做的事情

​	处理条件（计数器加一）



## 赋值运算符

=			+=			-=			*=			/=			//=

%=			**=



print自动换行，若不想自动换行，可以：

print("*",end = "")



## 函 数

def 函数的名称（）：

​	“”“在这里是函数的标准注释”“”

​	块

**（应该先定义后使用）**

### 常用函数

![1562158653165](C:\Users\58354\AppData\Local\Temp\1562158653165.png)

![1562158698930](C:\Users\58354\AppData\Local\Temp\1562158698930.png)

![1562162462730](C:\Users\58354\AppData\Local\Temp\1562162462730.png)

![1562162987811](C:\Users\58354\AppData\Local\Temp\1562162987811.png)

![1562163457576](C:\Users\58354\AppData\Local\Temp\1562163457576.png)









## 列表

namee_list = ["xiaoming","xiaowang","dabao"]    **索引从0开始**（用中括号标索引）

列表.sort() 升序排序

列表.sort(reverse = True) 降序排序

列表.reverse() 反转/逆序

### 列表操作

.append		.count	.insert	.reverse		.clear	.extend		.pop

.sort		.copy	.index	.remove

### 列表循环

for name in name_list:

​	内部操作

​	print(name)



## 元组

info_tuple = ("zhangsan",18,1.75)	不能修改

**如果元组只有一个元素时候要在后面加上逗号**

只有两个方法：.count			.index

同样可以用for循环来遍历列表、元组、字典和字符串

#### 元组和列表的相互转化

list(元组)			tuple(列表)

fromkeys()：可以设置多个键的值是一样的

zip()：可以把两个序列对应打包

>可以使用dict(zip())生成对应的字典

```python
dict = {"wangdachui":2000, "mayun":100000, "fuyun":5600}

dict.keys()
# ['wangdachui', 'mayun', 'fuyun']
dict.values()
# [2000, 100000, 5600]

# items()把每对键值对封装成元组返回
for k,v in dict.items():
    print(k,v)
    
# 更新字典
olddict.update(newdict)

# 直接用键值查找，若是没有对应的键会报错，可以使用get()
print(dict.get('AAA'))

# 字典和json的相互转化
json_str = json.dumps(dict)
dict = json.loads(json_str)
```





## 字典

xiaoming = {"name":"xiaoming",

​			"age":18,

​			"gender":True,

​			"height":1.75}

**键不可以相同，要唯一**

**不在意字典的顺序，但是在意所属的对应关系**



## 字符串

**在python中可以用双引号也可以用单引号定义**

可以使用索引来获取一个字符串中指定位置的字符，从0开始

string.isspace() 				检测空白字符（\t\r\n空格等）(返回True)

**有对应字符串的 查找和替换 还有 文本对齐方法 还有 去除空吧字符 的函数，可查**

###切片



### TODO注释

**# TODO 加上注释**

（提示需要做的事情）



### 引用

运用字典，列表这些可变变量时，使用对应的方法不会改变它的位置，但是如果使用赋值语句就会改变位置。

只能用不可变的数据来作为字典的key

hash(o)函数只能接收一个不可变的数据类型



## 确省参数

应该要放在参数列表最后

### 多值参数

用元组（*args）或者是字典（**kwargs)接受

## 类方法

在上面加上

@classmethod

不用第一个参数self,改为cls



## 静态方法

@staticmethod

不需要传递第一个参数

# 捕获异常

try:

​	尝试执行的代码

except（错误类型，可省略）:

​	若出现错误的处理

else：

​	没有异常时可以执行的代码

finally:

​	有没有异常都会执行的效果

## 主动抛出异常

raise（）

## 隐藏函数

__ name__ 用来测试却不会影响到导入

## 制作压缩包步骤

1.创建setup.py文件

2.python setup.py build

3.python setup.py sdist

# 文件操作

open		打开文件，返回文件操作对象（文件名区分大小写）

read		将文件内容读写到内存

write		将指定内容写入文件

close		关闭文件

```python
with open(filename, mode) as f:
    f.write()
```



## 正则表达式

**re库默认贪婪匹配，也就是会匹配字符最长的一个字符串，如果要最小匹配，可以在操作符后面加上？**

- . ：匹配除换行符以外的任意字符
- '*'：重复前面的子表达式0次或多次
- +：重复前面的子表达式1次或多次
- ？：重复前面的子表达式0次或1次
- ^ ：匹配字符串的开始
- $ ：匹配字符串的结束
- {n}：重复n次
- {n,}：重复n次或更多次
- {n,m}：重复n到m次
- \b：匹配单词的开始或结尾即单词边界，"\B"匹配非单词边界
- \d：匹配数字，"\D"匹配任意非数字非空白符
- \s：匹配任意空白符，"\S"匹配任意非空白符
- \w：匹配任意字母，数字或下划线的标识符字符，"\W"匹配任意非标识符字符
- [a-z]：匹配指定范围内的任意字符
- [^a-z]：匹配任意不再指定范围内的任意字符

> regex101.com是一个匹配正则字符串的网站



### Re库主要功能函数

- re.search(pattern, string, flag=0)：在一个字符串中搜索匹配正则表达式的第一个位置， 返回match对象
  - pattern： 正则匹配表达式
  - string：待匹配字符串
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.match(pattern, string, flag=0)：从一个字符串的开始位置起匹配正则表达式， 返回match对象
  - pattern： 正则匹配表达式
  - string：待匹配字符串
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.findall(pattern, string, flag=0)：搜索字符串，以列表类型返回全部能匹配的子串
  - pattern： 正则匹配表达式
  - string：待匹配字符串
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.split(pattern, string, maxsplit, flag=0)：将一个字符串按照正则表达式匹配结果进行分割，返回列表类型
  - pattern： 正则匹配表达式
  - string：待匹配字符串
  - maxsplit：最大分割数
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.finditer(pattern, string, flag=0)：搜索字符串， 返回一个匹配结果的迭代类型， 每个迭代元素是match对象
  - pattern： 正则匹配表达式
  - string：待匹配字符串
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.sub(pattern, repl, string, count, flag=0)：在一个字符串中替换所有匹配正则表达式的子串，返回替换后的字符串
  - pattern： 正则匹配表达式
  - repl：替换匹配的字符串
  - string：待匹配字符串
  - count：匹配的最大替换次数
  - flag：正则表达式使用时的控制标志（可查，主要有三个）
- re.compile(pattern, flags=0)：将正则表达式转化成正则表达式对象，然后就可以使用re库的六种与上面等价的搜索方法



### match对象

主要属性：

- .string：待匹配的文本
- .re：正则表达式
- .pos：搜索文本的开始位置
- .endpos：搜索文本的结束位置

主要方法：

- .group(0)：获得匹配后的字符串
- .start：匹配字符串在原始字符串的开始位置
- .end：匹配字符串在原始字符串的结束位置
- .span：返回（.start, .end）



## 函数式编程

```python 
# map()  reduce()  filter()

lst = [3, 2, 5, 8, 1]
list(map(lambda x:x*2, lst))
# 以里面的表达式计算列表每一个值并生成一个新的列表
# [6, 4, 10, 16, 2]

lst = [1, 2, 3, 4, 5, 6]
list(filter(lambda x:x%2==0, lst))
# 选出符合表达式的值
# [2, 4, 6]

from functools import reduce
lst = [1, 2, 3, 4, 5]
reduce(lambda x,y:x+y, lst)
# 递归计算列表的值
# 15
```



## 深拷贝和浅拷贝

浅拷贝：x = y[:]  或者 x = y.copy

深拷贝：x = copy.deepcopy(y)

# 100天学习（随记）

## 模块

### turtle

```python
# turtle是Python内置的一个非常有趣的模块，特别适合对计算机程序设计进行初体验的小伙伴
# 更多可以查阅https://blog.csdn.net/zengxiantao1994/article/details/76588580
import turtle

# 设置笔的粗细和颜色
turtle.pensize(4)
turtle.pencolor('red')

# forward前进一段距离
# right以当前方向向右转动一定角度
turtle.forward(100)
turtle.right(90)
turtle.forward(100)
turtle.right(90)
turtle.forward(100)
turtle.right(90)
turtle.forward(100)

# q启动事件循环，必须是最后一个函数
turtle.mainloop()

```



## 语法

- `int()`：将一个数值或字符串转换成整数，可以指定进制。
- `float()`：将一个字符串转换成浮点数。
- `str()`：将指定的对象转换成字符串形式，可以指定编码。
- `chr()`：将整数转换成该编码对应的字符串（一个字符）。
- `ord()`：将字符串（一个字符）转换成对应的编码（整数）。
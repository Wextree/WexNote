

# 数据库（mysql）



## 命令行基本命令

1.登录数据库服务器

mysql -uroot -p110325

2.查询数据库中所有的数据库

show databases;

3.选取其中一个数据库

use 数据库名

select * from admin (Admin_ID=1)

 4.退出数据库

exit

5.创建数据库

create database test

 6.创建一个数据表

CREATE TABLE pet(

​	 name VARCHAR(20),

​	owner VARCHAR(20),

​	SPECIES VARCHAR(20),

​	sex CHAR(1),

​	birth DATE,

​	death DATE);

7.查看具体数据表的结构

describe pet;

8.查看表中记录

select * from pet;

9.插入

INSERT INTO

VALUES(.........,....,.....)

10. 删除

    delete from pet where name='......';

11. 修改

    update pet set name='.....' where owner='zhouxingchi';

    

##主要约束

- 主键约束（primary key）：唯一确定表中的一条记录，规定不能为空也不能重复。

  联合主键primary key(a,b)： 只要是两个不要同时重复就好

- 自增约束（auto_increment）：和主键一起使用 时可以自动自增，不需要传入数据。

- 建表时忘记添加约束：alter table name add primary key(key);（drop 为删除，modify 为修改）

- 唯一约束（unique）：所规定的属性不能重复，也支持联合约束

- 非空约束（not null）：修饰的字段不能为空

- 默认约束（default）：就是不传东西时默认值（缺省）

- 外键约束（foreign key）：设涉及两个表，父表和子表。父表没有的字表不能有，被子表引用的父表不能删除



##  数据库设计范式

- **第一范式（1NF）**：所有字段不可以再拆分
- **第二范式（2NF）**:在满足第一范式的前提下，除主键以外的每一列都要完全依赖于主键，如果出现不完全依赖，只能发生在联合主键
- **第三范式（3NF）**：在满足第二范式的前提下，除开主键列的其他列之间不能有传递依赖。



## 数据库查询（某些注意点）

```mysql
消除重复：
select distinct depart from Teacher;

查询区间：
select * from score where decree between 20 and 80;
or
select * from score where decree>20 and decree<80;

可以不同列先后升序降序：
select * from student order by cno desc,sno asc;

```

## 连接查询



### 内连接

- 内连接：inner join 或者 join
- 有点像自然连接



###外连接

- 左连接 ：left join 或者 left outer，右边没有的话会补null
- 右连接： right join 或者 right outer，左边补
- 完全外连接： full join 或者 full outer，两边都补（mysql不支持，但是可以左连接和右连接进行联合获得全连接）



## 事务

**autocommit默认设置开启**，不能使用rollback，不可以使用回滚。可以设置为0，手动commit进行不可撤销确认。

```mysql
select * @@autocommit;

set autocommit = 1 or 0;
commit;

begin;
or
start transaction;
```



### 四大特征（ACID）

- A 原子性：事务是最小的单位，不可以再分割。
- C 一致性：事务要求同一事务中的sql语句必须保证同时成功或者同时失败。
- I 隔离性：不同事务之间具有隔离性
- D 持久性：事务一旦结束（commit,rollback），就不可以返回







### 提示符

| 提示符 | 含义                                              |
| ------ | ------------------------------------------------- |
| mysql> | 准备好接受新命令。                                |
| ->     | 等待多行命令的下一行。                            |
| '>     | 等待下一行，等待以单引号(“'”)开始的字符串的结束。 |
| ">     | 等待下一行，等待以双引号(“"”)开始的字符串的结束   |
| `>     | 等待下一行，等待以反斜点(‘`’)开始的识别符的结束。 |
| /*>    | 等待下一行，等待以/*开始的注释的结束。            |



你不能只是输入\c，因为**mysql**作为它正在收集的字符串的一部分来解释它！相反，应输入关闭的引号字符(这样**mysql**知道你完成了字符串)，然后输入\c：

```sql
mysql >  SELECT * FROM my_table WHERE name = 'Smith AND age < 30;
     '>  '\c
mysql >
```



## 函数

- **bit_count**：用来计算二进制数中包含1的个数。
- **bit_or**：就是对两个二进制数进行或运算。

```sql
SELECT year,month,BIT_COUNT(BIT_OR(1<<day)) AS days FROM t1
GROUP BY year,month;

// (1<< 02) | (1 << 23) | (1<<23) = 100 | 1000...（23个0）| 1000...（23个0） = 1000...100
```

- **LAST_INSERT_ID()**：即为获取最后插入的ID值。



## 慢查询

### 字段解释

|             字段              |                          含义                           |
| :---------------------------: | :-----------------------------------------------------: |
|        long_query_time        |               慢查询时间阈值（单位：秒）                |
|        slow_query_log         | 这个参数设置为ON，可以捕获执行时间超过一定数值的SQL语句 |
|      slow_query_log_file      |                    记录日志的文件名                     |
| log_queries_not_using_indexes |   这个参数设置为ON，可以捕获到所有未使用索引的SQL语句   |



### 有关命令

- **set long_query_time=1：**设置慢查询时间阈值为1





### 检测MySQL中sql语句的效率的方法

####1. 通过查询日志

>Windows下开启MySQL慢查询
>
>MySQL在Windows系统中的配置文件一般是是my.ini找到[mysqld]下面加上
>
>log slow queries = E:/LOG/MySql/mysqlslowquery.log
>
>long_query_time = 2



####2.explain来了解SQL执行的状态

>explain显示了mysql如何使用索引来处理select语句以及连接表。可以帮助选择更好的索引和写出更优化的查询语句。
>
>使用方法：在select语句前加上explain



## EXPLAIN Output Columns

| 列名          | 说明                                                         |
| ------------- | ------------------------------------------------------------ |
| id            | 执行编号，标识select所属的行。如果在语句中没子查询或关联查询，只有唯一的select，每行都将显示1。否则，内层的select语句一般会顺序编号，对应于其在原始语句中的位置 |
| select_type   | 显示本行是简单或复杂select。如果查询有任何复杂的子查询，则最外层标记为PRIMARY（DERIVED、UNION、UNION RESUlT） |
| table         | 访问引用哪个表（引用某个查询，如“derived3”）                 |
| type          | 数据访问/读取操作类型（ALL、index、range、ref、eq_ref、const/system、NULL） |
| possible_keys | 揭示哪一些索引可能有利于高效的查找                           |
| key           | 显示mysql决定采用哪个索引来优化查询                          |
| key_len       | 显示mysql在索引里使用的字节数                                |
| ref           | 显示了之前的表在key列记录的索引中查找值所用的列或常量        |
| rows          | 为了找到所需的行而需要读取的行数，估算值，不精确。通过把所有rows列值相乘，可粗略估算整个查询会检查的行数 |
| Extra         | 额外信息，如using index、filesort等                          |

### id

id是用来顺序标识整个查询中SELELCT 语句的，在嵌套查询中id越大的语句越先执行。该值可能为NULL，如果这一行用来说明的是其他行的联合结果。

### select_type

表示查询的类型

| 类型               | 说明                                                         |
| ------------------ | ------------------------------------------------------------ |
| simple             | 简单子查询，不包含子查询和union                              |
| primary            | 包含union或者子查询，最外层的部分标记为primary               |
| subquery           | 一般子查询中的子查询被标记为subquery，也就是位于select列表中的查询 |
| derived            | 派生表——该临时表是从子查询派生出来的，位于from中的子查询     |
| union              | 位于union中第二个及其以后的子查询被标记为union，第一个就被标记为primary如果是union位于from中则标记为derived |
| union result       | 用来从匿名临时表里检索结果的select被标记为union result       |
| dependent union    | 顾名思义，首先需要满足UNION的条件，及UNION中第二个以及后面的SELECT语句，同时该语句依赖外部的查询 |
| dependent subquery | 和DEPENDENT UNION相对UNION一样                               |

### table

对应行正在访问哪一个表，表名或者别名

- 关联优化器会为查询选择关联顺序，左侧深度优先
- 当from中有子查询的时候，表名是derivedN的形式，N指向子查询，也就是explain结果中的下一列
- 当有union result的时候，表名是union 1,2等的形式，1,2表示参与union的query id

注意：MySQL对待这些表和普通表一样，但是这些“临时表”是没有任何索引的。

### type

type显示的是访问类型，是较为重要的一个指标，结果值从好到坏依次是：
 system > const > eq_ref > ref > fulltext > ref_or_null > index_merge > unique_subquery > index_subquery > range > index > ALL ，一般来说，得保证查询至少达到range级别，最好能达到ref。

| 类型   | 说明                                                         |
| ------ | ------------------------------------------------------------ |
| All    | 最坏的情况,全表扫描                                          |
| index  | 和全表扫描一样。只是扫描表的时候按照索引次序进行而不是行。主要优点就是避免了排序, 但是开销仍然非常大。如在Extra列看到Using index，说明正在使用覆盖索引，只扫描索引的数据，它比按索引次序全表扫描的开销要小很多 |
| range  | 范围扫描，一个有限制的索引扫描。key 列显示使用了哪个索引。当使用=、 <>、>、>=、<、<=、IS NULL、<=>、BETWEEN 或者 IN 操作符,用常量比较关键字列时,可以使用 range |
| ref    | 一种索引访问，它返回所有匹配某个单个值的行。此类索引访问只有当使用非唯一性索引或唯一性索引非唯一性前缀时才会发生。这个类型跟eq_ref不同的是，它用在关联操作只使用了索引的最左前缀，或者索引不是UNIQUE和PRIMARY KEY。ref可以用于使用=或<=>操作符的带索引的列。 |
| eq_ref | 最多只返回一条符合条件的记录。使用唯一性索引或主键查找时会发生 （高效） |
| const  | 当确定最多只会有一行匹配的时候，MySQL优化器会在查询前读取它而且只读取一次，因此非常快。当主键放入where子句时，mysql把这个查询转为一个常量（高效） |
| system | 这是const连接类型的一种特例，表仅有一行满足条件。            |
| Null   | 意味说mysql能在优化阶段分解查询语句，在执行阶段甚至用不到访问表或索引（高效） |

### possible_keys

显示查询使用了哪些索引，表示该索引可以进行高效地查找，但是列出来的索引对于后续优化过程可能是没有用的

### key

key列显示MySQL实际决定使用的键（索引）。如果没有选择索引，键是NULL。要想强制MySQL使用或忽视possible_keys列中的索引，在查询中使用FORCE INDEX、USE INDEX或者IGNORE INDEX。

### key_len

key_len列显示MySQL决定使用的键长度。如果键是NULL，则长度为NULL。使用的索引的长度。在不损失精确性的情况下，长度越短越好 。

### ref

ref列显示使用哪个列或常数与key一起从表中选择行。

### rows

rows列显示MySQL认为它执行查询时必须检查的行数。注意这是一个预估值。

### Extra

Extra是EXPLAIN输出中另外一个很重要的列，该列显示MySQL在查询过程中的一些详细信息，MySQL查询优化器执行查询的过程中对查询计划的重要补充信息。

| 类型                         | 说明                                                         |
| ---------------------------- | ------------------------------------------------------------ |
| Using filesort               | MySQL有两种方式可以生成有序的结果，通过排序操作或者使用索引，当Extra中出现了Using filesort 说明MySQL使用了后者，但注意虽然叫filesort但并不是说明就是用了文件来进行排序，只要可能排序都是在内存里完成的。大部分情况下利用索引排序更快，所以一般这时也要考虑优化查询了。使用文件完成排序操作，这是可能是ordery by，group by语句的结果，这可能是一个CPU密集型的过程，可以通过选择合适的索引来改进性能，用索引来为查询结果排序。 |
| Using temporary              | 用临时表保存中间结果，常用于GROUP BY 和 ORDER BY操作中，一般看到它说明查询需要优化了，就算避免不了临时表的使用也要尽量避免硬盘临时表的使用。 |
| Not exists                   | MYSQL优化了LEFT JOIN，一旦它找到了匹配LEFT JOIN标准的行， 就不再搜索了。 |
| Using index                  | 说明查询是覆盖了索引的，不需要读取数据文件，从索引树（索引文件）中即可获得信息。如果同时出现using where，表明索引被用来执行索引键值的查找，没有using where，表明索引用来读取数据而非执行查找动作。这是MySQL服务层完成的，但无需再回表查询记录。 |
| Using index condition        | 这是MySQL 5.6出来的新特性，叫做“索引条件推送”。简单说一点就是MySQL原来在索引上是不能执行如like这样的操作的，但是现在可以了，这样减少了不必要的IO操作，但是只能用在二级索引上。 |
| Using where                  | 使用了WHERE从句来限制哪些行将与下一张表匹配或者是返回给用户。**注意**：Extra列出现Using where表示MySQL服务器将存储引擎返回服务层以后再应用WHERE条件过滤。 |
| Using join buffer            | 使用了连接缓存：**Block Nested Loop**，连接算法是块嵌套循环连接;**Batched Key Access**，连接算法是批量索引连接 |
| impossible where             | where子句的值总是false，不能用来获取任何元组                 |
| select tables optimized away | 在没有GROUP BY子句的情况下，基于索引优化MIN/MAX操作，或者对于MyISAM存储引擎优化COUNT(*)操作，不必等到执行阶段再进行计算，查询执行计划生成的阶段即完成优化。 |
| distinct                     | 优化distinct操作，在找到第一匹配的元组后即停止找同样值的动作 |





# SQL练习

首先建表，主要有四个表：

- **学生表（Student）**
- **课程表（Course）**
- **教师表（Teacher）**
- **成绩表（SC）**

在分别介绍一下每个表的字段 ：

- **学生表（Student ）有四个字段 sid--学生id，sname--学生姓名，sage--学生年龄，ssex--学生性别**

- **课程表（Course）有三个字段，cid--课程id，cname--课程名，tid--教师id**

- **教师表（Teacher）有两个字段，tid--教师id，tname--教师姓名**

- **成绩表（SC）有三个字段，sid--学生id，cid--课程id，score--成绩**

```mysql
# 建表语句
# 学生表
create table Student(
	sid int primary key auto_increment,
    sname varchar(10),
    sage datetime,ssex nvarchar(10)
    );
insert into Student values('01' , '赵雷' , '1990-01-01' , '男');
insert into Student values('02' , '钱电' , '1990-12-21' , '男');
insert into Student values('03' , '孙风' , '1990-05-20' , '男');
insert into Student values('04' , '李云' , '1990-08-06' , '男');
insert into Student values('05' , '周梅' , '1991-12-01' , '女');
insert into Student values('06' , '吴兰' , '1992-03-01' , '女');
insert into Student values('07' , '郑竹' , '1989-07-01' , '女');
insert into Student values('08' , '王菊' , '1990-01-20' , '女');

# 教师表
create table Teacher(
	tid int primary key auto_increment,
    tname varchar(10)
    );
insert into Teacher values('01' , '张三');
insert into Teacher values('02' , '李四');
insert into Teacher values('03' , '王五');

# 课程表
create table Course(
	cid int primary key auto_increment,
    cname varchar(10),
    tid int,
    foreign key(tid) references teacher(tid)
    );
insert into Course values('01' , '语文' , '02');
insert into Course values('02' , '数学' , '01');
insert into Course values('03' , '英语' , '03');

# 选修成绩表
create table SC(
	sid int,
    cid int,
    score int,
    primary key(sid, cid),
    foreign key(sid) references student(sid),
    foreign key(cid) references course(cid)
    );
insert into SC values('01' , '01' , 80);
insert into SC values('01' , '02' , 90);
insert into SC values('01' , '03' , 99);
insert into SC values('02' , '01' , 70);
insert into SC values('02' , '02' , 60);
insert into SC values('02' , '03' , 80);
insert into SC values('03' , '01' , 80);
insert into SC values('03' , '02' , 80);
insert into SC values('03' , '03' , 80);
insert into SC values('04' , '01' , 50);
insert into SC values('04' , '02' , 30);
insert into SC values('04' , '03' , 20);
insert into SC values('05' , '01' , 76);
insert into SC values('05' , '02' , 87);
insert into SC values('06' , '01' , 31);
insert into SC values('06' , '03' , 34);
insert into SC values('07' , '02' , 89);
insert into SC values('07' , '03' , 98);
```



1.查询"01"课程比"02"课程成绩高的学生的信息及课程分数

```mysql
# 先查出1课程和2课程的成绩表，然后进行相应比较，最后使用右外连接和学生信息匹配起来
select * 
from student right join
(select t1.sid,class1,class2
 from (select sid, score as class1 from sc where cid = 1) as t1,
	  (select sid, score as class2 from sc where cid = 2) as t2
 where t1.sid = t2.sid and class1 > class2
) as cs
on cs.sid = student.sid; 

---------------------------------------------------
2	钱电	1990-12-21 00:00:00	男	2	70	60
4	李云	1990-08-06 00:00:00	男	4	50	30
```



2.1 查询存在" 01 "课程但可能不存在" 02 "课程的情况(不存在时显示为 null )

```mysql
# 选择两个分别选择课程1和课程2的学员表，然后把这两个表join起来
SELECT *
FROM(SELECT *
      FROM sc
      WHERE sc.cid = '01' ) t1 
LEFT JOIN 
 	(SELECT *
  	 FROM sc 
 	 WHERE sc.cid = '02') t2
ON t1.sid = t2.sid ;

-----------------------------------
1	1	80	1	2	90
2	1	70	2	2	60
3	1	80	3	2	80
4	1	50	4	2	30
5	1	76	5	2	87
6	1	31	null ...		
```

2.2 查询同时存在01和02课程的情况

```mysql
# 先查询1课程的信息，再查出2课程的信息，然后从这两张表中找到相同学号的信息匹配起来
select * 
from (select * from sc where cid = 1) as t1,
	 (select * from sc where cid = 2) as t2
where t1.sid = t2.sid;

------------------------------------------
1	1	80	1	2	90
2	1	70	2	2	60
3	1	80	3	2	80
4	1	50	4	2	30
5	1	76	5	2	87
```

2.3 查询选择了02课程但没有01课程的情况

```mysql
# 先查出选了2课程的所有信息，然后使用not in剔除掉所有的选了1课程的信息
select * 
from (select * from sc where cid = 2) as t
where t.sid not in (select sid from sc where cid = 1);

-----------------------------------------
7	2	89
```



3. 查询平均成绩大于等于 60 分的同学的学生编号和学生姓名和平均成绩

```mysql
# 先将学生表和成绩表按照学号连接起来，然后分组按要求求得
select sc.sid,sname,avg(score) as avg
from student, sc
where student.sid = sc.sid
group by sc.sid
having avg(score) >= 60;	

----------------------------------
1	赵雷	89.6667
2	钱电	70.0000
3	孙风	80.0000
5	周梅	81.5000
7	郑竹	93.5000
```



4.查询在 SC 表存在成绩的学生信息

```mysql
# 两张表通过id连接，然后求单独值
# 但是mysql无法通过这样，因为它是所有列都相同才算相同
# 详细请看https://blog.csdn.net/guocuifang655/article/details/3993612
SELECT DISTINCT student.*
FROM sc,student
WHERE student.sid = sc.sid ;

#修改为：
SELECT student.*
FROM sc,student
WHERE student.sid = sc.sid 
group by student.sid;
```



5.查询所有同学的学生编号、学生姓名、选课总数、所有课程的成绩总和

```mysql
SELECT s.sid,s.sname,COUNT(sc.cid),SUM(score)
FROM student AS s INNER JOIN sc 
ON s.sid = sc.sid
GROUP BY sc.sid;
```



6.查询「李」姓老师的数量

```mysql
SELECT count(*)
FROM teacher
WHERE tname like '李%';
```



7.查询学过「张三」老师授课的同学的信息

```mysql
# 多重连接
SELECT *
from student
where sid in (select sid 
			  from sc,teacher,course
              where teacher.tname = '张三' 
			    and teacher.tid = course.tid
                and sc.cid = course.cid);
                
------------------------------------------------------
1	赵雷	1990-01-01 00:00:00	男
2	钱电	1990-12-21 00:00:00	男
3	孙风	1990-05-20 00:00:00	男
4	李云	1990-08-06 00:00:00	男
5	周梅	1991-12-01 00:00:00	女
7	郑竹	1989-07-01 00:00:00	女
```



8.查询没有学全所有课程的同学的信息

```mysql
# 先找出科目的次数然后再一起比较
SELECT *
from student
where sid in (select sid 
			  from sc
              group by sid
              having count(cid) < (select count(*) from course)
              );
              
-----------------------------------------------------------
5	周梅	1991-12-01 00:00:00	女
6	吴兰	1992-03-01 00:00:00	女
7	郑竹	1989-07-01 00:00:00	女

# 其他方法
SELECT s.*
FROM student AS s
WHERE s.sid NOT IN (SELECT sc.sid
FROM sc 
GROUP BY sc.sid
HAVING COUNT(cid) >= 3);
			
```



9.查询至少有一门课与学号为" 01 "的同学所学相同的同学的信息

```mysql
# 首先要查询出01同学所学的课程，然后使用cid in 01同学的课程，并排除sid=01的同学
select student.*
from student,sc
where sc.sid = student.sid
  and sc.cid in (select cid from sc where sid = 1)
  and sc.sid != 1
group by sc.sid;

--------------------------------------------------------
2	钱电	1990-12-21 00:00:00	男
3	孙风	1990-05-20 00:00:00	男
4	李云	1990-08-06 00:00:00	男
5	周梅	1991-12-01 00:00:00	女
6	吴兰	1992-03-01 00:00:00	女
7	郑竹	1989-07-01 00:00:00	女
```



10.查询和" 01 "号的同学学习的课程完全相同的其他同学的信息

```mysql
  # 先通过sc表找出选修和1号同学同程的信息
  # 通过按学号分组判断是否该学号选修的科目数和1号同学一致来判断完全选修到1号同学的科目
  # 接下来用IN来找出对应的学生信息，最后剔除1号同学的信息即可
  SELECT * 
  FROM student 
  WHERE sid IN 
    (SELECT sid 
     FROM
      (SELECT * 
       FROM sc AS a 
       WHERE cid IN 
        (SELECT cid 
         FROM sc 
         WHERE sid = 01)
		) b 
    GROUP BY sid 
    HAVING COUNT(cid) = 
      (SELECT COUNT(cid) 
       FROM sc c 
       WHERE sid = 01)
	) 
    AND sid != 01 ;  
```



11.查询没学过"张三"老师讲授的任一门课程的学生姓名

```mysql
# 先要查询出张三老师的教授课程，然后利用not in 来找到学生的id
select sid,sname
from student
where sid not in 
	(select sid
	 from sc right join course on sc.cid = course.cid
     where tid = (select tid from teacher where tname='张三'));
     
---------------
6	吴兰
8	王菊
```



12.查询两门及其以上不及格课程的同学的学号，姓名及其平均成绩

```mysql
# 先将成绩表和学生表做一个连接，找出所有不及格的记录，按学号分组后找出所有不及格多于两门课的学生
select student.sid,sname,avg(score) as avg
from student,sc
where student.sid = sc.sid
  and score < 60
group by sc.sid
having count(score) >= 2;
```



13.检索" 01 "课程分数小于 60，按分数降序排列的学生信息

```mysql
select student.*,score
from sc,student
where cid = 1
  and score < 60
  and sc.sid = student.sid
order by score desc;

----------------------------
4	李云	1990-08-06 00:00:00	男	50
6	吴兰	1992-03-01 00:00:00	女	31
```



14.按平均成绩从高到低显示所有学生的所有课程的成绩以及平均成绩

```mysql
# 找出每个学生的平均成绩做连接
select sc.*,avg
from sc left join
	(select sid,avg(score) as avg from sc group by sid) as t
	on sc.sid = t.sid
order by avg desc;

---------------------------------------------
7	2	89	93.5000
7	3	98	93.5000
1	2	90	89.6667
1	3	99	89.6667
1	1	80	89.6667
5	1	76	81.5000
5	2	87	81.5000
3	2	80	80.0000
3	3	80	80.0000
3	1	80	80.0000
2	1	70	70.0000
2	2	60	70.0000
2	3	80	70.0000
4	1	50	33.3333
4	2	30	33.3333
4	3	20	33.3333
6	1	31	32.5000
6	3	34	32.5000
```



15.查询各科成绩最高分、最低分和平均分,以如下形式显示：

以如下形式显示：课程 ID，课程 name，最高分，最低分，平均分，及格率，中等率，优良率，优秀率

及格为**>=60**，中等为：**70-80**，优良为：**80-90**，优秀为：**>=90**

要求输出课程号和选修人数，查询结果按人数降序排列，若人数相同，按课程号**升序排列**

```mysql
SELECT 
  cid AS 课程ID,
  COUNT(sid) AS 课程人数,
  MAX(score) AS 最高分,
  MIN(score) AS 最低分,
  AVG(score) AS 平均分,
  SUM(及格) / COUNT(sid) AS 及格率,
  SUM(中等) / COUNT(sid) AS 中等率,
  SUM(优良) / COUNT(sid) AS 优良率,
  SUM(优秀) / COUNT(sid) AS 优秀率 
FROM
  (SELECT *,
    CASE
      WHEN score >= 60 
      THEN 1 
      ELSE 0 
    END AS 及格,
    CASE
      WHEN score >= 70 
      AND score < 80 
      THEN 1 
      ELSE 0 
    END AS 中等,
    CASE
      WHEN score >= 80 
      AND score < 90 
      THEN 1 
      ELSE 0 
    END AS 优良,
    CASE
      WHEN score >= 90 
      THEN 1 
      ELSE 0 
    END AS 优秀 
  FROM sc) a 
GROUP BY cid 
ORDER BY COUNT(sid) DESC,cid ;

----------------------------------------------------------------
1	6	80	31	64.5000	0.6667	0.3333	0.3333	0.0000
2	6	90	30	72.6667	0.8333	0.0000	0.5000	0.1667
3	6	99	20	68.5000	0.6667	0.0000	0.3333	0.3333
```



16.按各科成绩进行排序，并显示排名， Score 重复时保留名次空缺

```mysql
# 先将成绩进行排序，然后再统计比它多的成绩
SELECT a.*, 
	(select count(distinct score) 
     from sc as b 
     where a.cid = b.cid and b.score >= a.score) as 排名
FROM sc AS a 
ORDER BY a.cid, a.score desc;
```



17.按各科成绩进行行排序，并显示排名， Score 重复时合并名次

```mysql
SELECT a.*, 
	(select count(score)+1
     from sc as b 
     where a.cid = b.cid and b.score > a.score) as 排名
FROM sc AS a 
ORDER BY a.cid, a.score desc;
```



18.查询学生的总成绩，并进行排名

```mysql
# 引入一个用户变量
# 总分重复时保留名次空缺
SELECT a.*, @rank := @rank+1 as 排名
from 
	(select sid, cid, sum(score)
     from sc
     group by sid
     order by sum(score) desc) as a,
(select @rank := 0) b;


# 总分重复时不保留名次空缺
SELECT a.*,
  CASE
    WHEN @fscore = a.sumscore 
    THEN @rank 
    WHEN @fscore := a.sumscore 
    THEN @rank := @rank + 1 
  END AS 排名 
FROM
  (SELECT sc.sid, SUM(score) AS sumscore 
   FROM sc 
   GROUP BY sid 
   ORDER BY SUM(score) DESC) AS a,
  (SELECT 
    @rank := 0,
    @fscore := NULL) AS t ;
    

```



19.统计各科成绩各分数段人数：课程编号，课程名称，[100-85]，[85-70]，[70-60]，[60-0] 及所占百分比

```mysql
SELECT sc.cid AS 课程编号, cname AS 课程名称,
  SUM(
    CASE
      WHEN score >= 0 
      AND score <= 60 
      THEN 1 
      ELSE 0 
    END
  ) AS '[60-0]',
  SUM(
    CASE
      WHEN score >= 0 
      AND score <= 60 
      THEN 1 
      ELSE 0 
    END
  ) / COUNT(sid) AS '[60-0]百分比',
  SUM(
    CASE
      WHEN score >= 60 
      AND score <= 70 
      THEN 1 
      ELSE 0 
    END
  ) AS '[70-60]',
  SUM(
    CASE
      WHEN score >= 60 
      AND score <= 70 
      THEN 1 
      ELSE 0 
    END
  ) / COUNT(sid) AS '[70-60]百分比',
  SUM(
    CASE
      WHEN score >= 70 
      AND score <= 85 
      THEN 1 
      ELSE 0 
    END
  ) AS '[85-70]',
  SUM(
    CASE
      WHEN score >= 70 
      AND score <= 85 
      THEN 1 
      ELSE 0 
    END
  ) / COUNT(sid) AS '[85-70]百分比',
  SUM(
    CASE
      WHEN score >= 85 
      AND score <= 100 
      THEN 1 
      ELSE 0 
    END
  ) AS '[100-85]',
  SUM(
    CASE
      WHEN score >= 85 
      AND score <= 100 
      THEN 1 
      ELSE 0 
    END
  ) / COUNT(sid) AS '[100-85]百分比' 
FROM sc JOIN course ON sc.cid = course.cid 
GROUP BY sc.cid,cname ;
```



20.查询各科成绩前三名的记录

```mysql
SELECT a.*,COUNT(b.score) +1 AS ranking
FROM SC AS a LEFT JOIN SC AS b 
ON a.cid = b.cid AND a.score<b.score
GROUP BY a.cid,a.sid
HAVING ranking <= 3
ORDER BY a.cid,ranking;
```



21.查询每门课程被选修的学生数

```mysql
SELECT cid,COUNT(sid) AS num
FROM  sc 
GROUP BY cid;
```



22.查询出只选修两门课程的学生学号和姓名

```mysql
select s.sid,sname
from student as s,
	(select sid from sc
     group by sid
     having count(cid) = 2) as a
where s.sid = a.sid;

--------------------------------
5	周梅
6	吴兰
7	郑竹
```



23.查询男生、女生人数

```mysql
SELECT ssex,COUNT(ssex) as sum 
FROM student
GROUP BY ssex;

--------------------
男	4
女	4
```



24.查询名字中含有「风」字的学生信息

```mysql
SELECT *
FROM student
WHERE sname LIKE '%风%';
```



25.查询同名同姓学生名单，并统计同名人数

```mysql
SELECT sname,COUNT(sname) AS 同名人数
FROM student
GROUP BY sname
HAVING 同名人数>1;
```



26.查询 1990 年出生的学生名单

```mysql
SELECT *
FROM student
WHERE YEAR(sage) >= 1990 and year(sage) < 2000;

------------------------------------------------
SELECT *
FROM student
WHERE YEAR(sage) LIKE '1990%';
```



27.查询每门课程的平均成绩，结果按平均成绩降序排列，平均成绩相同时，按课程编


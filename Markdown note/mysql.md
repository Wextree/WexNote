

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






























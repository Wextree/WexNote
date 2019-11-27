

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



### 隔离性

1. read committed
2. 
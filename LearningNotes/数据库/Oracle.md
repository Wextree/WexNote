# Oracle

>以用户作为逻辑单位划分（mysql中是以数据库进行划分的）



## 基本操作

- **表空间的创建和删除**

```sql
--创建表空间设置路径，大小，自动扩展空间
create tablespace spacename
datafile 'c:\xxxxx.dbf'
size 100m
autoextend on
next 10m;

--删除表空间（只是断开连接，然后可以自行删除）
drop tablespace spacename

```

- **创建用户和授权**

```sql
--创建用户并设置密码和绑定表空间
create user xxxx
identified by xxxx
default tablespace xxxxx;

--给用户授权，常见角色：connect基本角色，resource开发者角色，dba超级管理员角色
grant dba to username;

```

- **常用数据类型**

|     数据类型     |                             描述                             |
| :--------------: | :----------------------------------------------------------: |
| varchar,varchar2 |  表示一个字符串，前者是定长，后者是可变长度，后者比较常用。  |
|      NUMBER      | NUMBER(n)表示一个整数，长度是n；NUMBER(m,n)表示一个小数，总长度是m，小数部分是n。 |
|       DATE       |                         表示一个日期                         |
|       CLOB       |                大对象，放的是文本数据，最大4G                |
|       BLOB       |               大对象，放的是二进制数据，最大4G               |

- **表的操作**

```sql
--创建一张表
create table user(
	pid number(20),
    name varchar2(10)
);

--给表增加一列
alter table user add gender number(1);

--修改列属性
alter table user modify gender char(1);
alter table user rename column gender to sex;
alter table user drop column sex;

--对表的增删改和mysql一样，除了查询，其他操作记得对事物进行提交
```

- **序列**

```sql
--序列并不是属于某一张表，但是可以逻辑上与表做绑定
--默认从1开始，依次递增，主要用来给主键赋值
--dual：只为了补全语法，并没有任何实际意义

create sequence s_user; --可以指定步长和开始点
select s_user.nextval from dual; -- 获取当前序列值用currval
insert into user(pid,name) values(s_user,"zhangsan");
```



## 查询

- **scott用户**

```sql
--默认密码是tiger，第一次使用要用超级管理员解锁
alter user scott account unlock;
alter user scott indentified by tiger; --此操作也可以用来修改密码

```

- **单行函数**

```sql
--大小写转化
select lower('yes') from daul;
select upper('yes') from daul;

--四舍五入和截取，后面的参数是保留的位数
select round(26.32,1) from daul;
select trunc(26.32,1) from daul;

--求余
select mod(26,3) from daul;

--日期函数
--sysdate表示当前时间，日期可以相加减，结果是天
--用months_between可以算出中间隔了多少月
--日期转换函数
select to_char(sysdate, 'yyyy-mm-dd hh:mi:ss') from daul;
select to_date('2020-01-13 5:40:30', 'yyyy-mm-dd hh:mi:ss') from daul;
--去零和24小时制
select to_char(sysdate, 'fm yyyy-mm-dd hh24:mi:ss') from daul;

--null值转换，若是null值则转换为第二个参数
select nvl(money, 0);
```

- **条件表达式**

```sql
--当如何就如何，when判断，else可以省略，省略其他值就为空值
select e.ename 
	case e.ename
		when '' then ''
			when '' then ''
				else ''
				end 
from emp e; 

--oracle专用写法，除了起别名，都用单引号
select e.ename 
	decode(e.ename,
		'', ''
			'', ''
				'') "xxx"
from emp e; 

```

- **多行函数：**count(1) , sum() , min() , max() , avg()

- **Oracle专用外连接**

```sql
--oracle可以使用mysql的连接语法，但是有专门的外连接语法
--加号在哪边，就是对面的外连接，以对面属性为准
select *
from xxx a, xxx b
where a.xx = b.xx(+);
```

- **分页查询**

```sql
--分页查询，利用rownum，它不属于任何一张表
select * from (
    select rownum rn, e.* from( --为了保证行号不乱序
    	select	* from emp order by sal desc
    )e where rownum < 11 --行号从1开始依次递增，不可以直接拿它大于一个整数
) where rn > 5; --加上别名之后就变成了一个普通的列了
```



## Oracle对象

### 视图

- 不建议修改，其实试图上的数据也还是表中的数据

- 生成只读视图加上with read only
- 主要用在隐藏敏感字段，或者分部还有总部数据的及时统一



### 索引

- 单列索引的触发条件是必须是索引列中的原始值，模糊 查询，单行函数都会影响触发。
- 复合索引第一列为优先索引列，查询必须包括第一列的原始值。
- 如果单列索引和复合索引都包括同一列，单只查询那一列的话，优先触发单列索引，or两边的条件必须都有对应的索引才会触发索引，即一边有一边没有的话是不触发的。



## Oracle编程

```sql
--定义变量
declare 
	i number(2) := 2;
	s varchar2(2) := '小明';
    ena emp.ename%type; --引用表中的类型
    emprow emp%rowtype; --记录型变量，记录的是一行记录	
begin
	dbms_output.put_line(i);
	dbms_output.put_line(s);
	select ename into ena from emp where eno = ×××; --赋值
	dbms_output.put_line(ena);
	select * into emprow from emp where eno = ×××; --赋值
	dbms_output.put_line(emprow.xxx//emprow.xxx);
end;
    
--判断语句
begin
	if xxx then ;
	elsif xxx then ;
	else ;
	end if;
end;

--循环
begin
	while xxx loop
		(循环体)
	end loop;
end;

begin
	loop
		exit when xxx;
		(循环体)
	end loop;
end;

begin
	for i in 1..10 loop
		(循环体)
	end loop;
end;

--游标
declare
	emprow emp%rowtype; --记录型变量，记录的是一行记录
	cursor c is select * from emp; --有条件的话可以在游标后面打个括号传参数，open时传参

begin
	open c
		loop
			fetch c into emprow;
			exit when c1%notfound;
			(循环体)	
		end loop;
	close c;
end;

--存储过程
create or replace procedure xx(in/out xx)
is/as
begin
	pl/sql
end;

--存储函数（有返回值）可以用来做自定义函数
create or replace function xx(in/out xx) return xxx --类型不可以加长度
is/as
xxx xxx --声明函数内部所需要的变量
begin
	pl/sql
	return;
end;

--触发器实现主键自增
create or replace trigger xx
before
insert
on user
for each row
declare

begin
	select s_user.nextval into :new.pid from daul;
end;
```












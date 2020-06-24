# Spring

## 注解

### @Target  @Retention  @Documented注解

> 参考文档：https://www.jianshu.com/p/8738ce6bd771

### @Target

用于描述注解的**使用范围**（即：被描述的注解可以用在什么地方）

取值(**ElementType**)有：

1. **CONSTRUCTOR: **用于描述构造器

2. **FIELD: **用于描述域

3. **LOCAL_VARIABLE: **用于描述局部变量

4. **METHOD: **用于描述方法

5. **PACKAGE: **用于描述包

6. **PARAMETER: **用于描述参数

7. **TYPE: **用于描述类、接口(包括注解类型) 或`enum`声明



### @Retention

Retention**(保留)**注解说明,这种类型的注解会被保留到那个阶段. 有三个值:

1. `RetentionPolicy.SOURCE` —— 这种类型的Annotations只在**源代码级别保留**,**编译时就会被忽略**

2. `RetentionPolicy.CLASS `—— 这种类型的Annotations**编译**时被保留,**在class文件中**存在,但**JVM将会忽略**

3. `RetentionPolicy.RUNTIME `— 这种类型的Annotations将**被JVM保留**,所以他们能在运行时被**JVM或其他使用反射机制的代码所读取和使用**.



### @Documented

Documented 注解表明这个注解应该被 `javadoc`工具记录. 默认情况下,`javadoc`是不包括注解的. 但如果声明注解时指定了 @Documented,则它会被` javadoc `之类的工具处理, 所以注解类型信息也会被包括在生成的文档中.



### @Inherited

这是一个稍微复杂的注解类型. 它指明被注解的类**会自动继承**. 更具体地说,如果定义注解时使用了 **@Inherited** 标记,然后用定义的注解来标注另一个父类, 父类又有一个子类(subclass),则**父类的所有属性将被继承到它的子类**中.



## 模块

![](..\..\Audition Points\imgs\640 (10).jpg)

- **Spring Core**  spring的核心功能： IOC容器, 解决对象创建及依赖关系

- **Spring Web**  Spring对web模块的支持。

- - 可以与struts整合,让struts的action创建交给spring
  - spring mvc模式

- Spring DAO  Spring 对jdbc操作的支持  【JdbcTemplate模板工具类】

- Spring ORM  spring对orm的支持：

- - 既可以与hibernate整合，【session】
  - 也可以使用spring的对hibernate操作的封装

- Spring AOP  切面编程

- SpringEE   spring 对javaEE其他模块的支持



## Core

> **Core模块是：IOC容器，解决对象创建和之间的依赖关系**。
>
> **Core模块主要是学习如何得到IOC容器，通过IOC容器来创建对象、解决对象之间的依赖关系、IOC细节。**

Spring核心的配置文件`applicationContext.xml`或者叫`bean.xml`

在Spring中总体来看可以通过**三种方式**来配置对象:

- **使用XML文件配置**
- **使用注解来配置**
- **使用JavaConfig来配置**





## Ioc

![](..\..\Audition Points\imgs\640 (26).webp)

![](..\..\Audition Points\imgs\640 (27).webp)



## AOP

Spring AOP主要做的事情就是：「把重复的代码抽取，在运行的时候往业务方法上**动态植入**“切面类代码”」








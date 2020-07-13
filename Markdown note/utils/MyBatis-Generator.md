# SpringBoot 集成 mybatis generator 自动生成代码插件

> 参考文档：https://blog.csdn.net/gnail_oug/article/details/80404870



## mybatis generator 介绍

**MyBatis Generator**，缩写MBG，是一个Mybatis和iBatis的代码生成器。它支持Mybatis的**所有版本**，支持iBatis 2.2.0以后的版本。通过MyBatis Generator可以根据数据库表生成相应的**实体、sql映射文件、Dao**等，能应付简单的CRUD（Create, Retrieve, Update, Delete），对于**连接查询或存储过程**等还是要手动编写sql和对象。



### 生成内容

MyBatis Generator将生成以下内容：

- 与表结构匹配的**Java POJO**。这可能包括：
  - 一个关于表主键的类(如果表有主键)
  - 一个关于表非主键字段的类（BLOB字段除外）
  - 一个包含表BLOB字段的类(如果表有BLOB字段)
  - 一个支持动态查询、修改和删除的类
- MyBatis或iBATIS兼容的SQL映射XML文件。MBG为配置中指定的每个表的简单CRUD函数生成SQL，生成的SQL语句包括：
  - 插入
  - 通过主键更新
  - 通过动态where子句更新
  - 通过主键删除
  - 通过动态where子句删除
  - 通过主键查询
  - 通过动态where子句查询
  - 通过动态where子句查询条数

> 注意：上面的内容并不一定全部生成，比如：如果表没有主键，则不会生成通过主键删除等sql。

- 适合使用上述对象的Java类，Java类的生成是可选的。MBG将为MyBatis 3.X生成以下类型的Java类：
  - 与MyBatis 3.x映射器基础结构配合使用的映射器接口
- MBG将为iBATIS 2.x生成以下类型的Java类：
  - 符合Spring框架的DAO
  - 只使用iBATIS SQL映射API的DAO。这些DAO可以通过两种方式生成：通过构造函数或setter注入来提供SqlMapClient。
  - 符合iBATIS DAO框架的DAO（这是iBATIS的一个可选部分，此框架已被弃用，所以建议改用Spring框架）



**在迭代开发过程中使用MyBatis Generator的注意事项：**

如果存在与新生成的XML文件具有相同名称的现有文件，MBG将**自动合并XML文件**，合并时，MBG**不会覆盖**你在XML文件中添加的任何内容。你可以重复生成而不用担心丢失XML文件中你添加的内容，MBG只替换通过MBG生成的任何XML元素。
MBG**不会合并Java文件**，它可以覆盖现有文件或使用不同的唯一名称保存新生成的文件(**文件名后加1、2、3等序号**)。如果重复生成可以选择自动覆盖替换或手动合并更改。

## 引入插件

```xml
<build>
    <plugins>
        <!--mybatis自动生成代码插件-->
        <plugin>
            <groupId>org.mybatis.generator</groupId>
            <artifactId>mybatis-generator-maven-plugin</artifactId>
            <version>1.3.6</version>
            <configuration>
                <!-- 是否覆盖，true表示会替换生成的JAVA文件，false则不覆盖 -->
                <overwrite>true</overwrite>
                <configurationFile>
                    src/main/resources/generatorConfig.xml
                </configurationFile>
            </configuration>
            <dependencies>
                <!--mysql驱动包-->
                <dependency>
                    <groupId>mysql</groupId>
                    <artifactId>mysql-connector-java</artifactId>
                    <version>8.0.19</version>
                </dependency>
            </dependencies>
        </plugin>
    </plugins>
```



## 使用简单的配置文件

相关配置有非常多，但是我们搭一个最为简单的配置文件，然后之后需要什么功能就去官网找对应的配置，然后添加到相关的位置即可。

**至少得配置要包括如何连接数据库（数据库的四个基本配置），还有导出的实体类的对应包名位置，还有对应的项目路径。**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
        PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
<generatorConfiguration>
    <context id="MySql">
        <jdbcConnection
                driverClass="com.mysql.cj.jdbc.Driver"
                connectionURL="jdbc:mysql://localhost:3306/test?useSSL=false&amp;serverTimezone=UTC"
                userId="root"
                password="110325">
            <!--MySQL 不支持 schema 或者 catalog 所以需要添加这个-->
            <!--参考 : http://www.mybatis.org/generator/usage/mysql.html-->
            <property name="nullCatalogMeansCurrent" value="true"/>
        </jdbcConnection>
        <javaModelGenerator targetPackage="com.wex.mybatisgeneratordemo.domain" targetProject="src/main/java"></javaModelGenerator>
        <table tableName="user"></table>
    </context>
</generatorConfiguration>
```

**双击插件生成对应的实体类：**

![](..\imgs\Snipaste_2020-07-13_22-21-05.png)

![](..\imgs\Snipaste_2020-07-13_22-22-03.png)



![](..\imgs\Snipaste_2020-07-13_22-22-44.png)



## 常规配置

上面我们了解怎么用MyBatis Generator的maven插件来生成代码，上面只是生成了POJO，我们可能还想生成映射文件，dao接口文件等，先把`generatorConfig.xml`配置文件复制备份为`generatorConfig-min.xml`

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
        PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
<generatorConfiguration>

    <!-- defaultModelType="flat" 设置复合主键时不单独为主键创建实体 -->
    <context id="MySql" defaultModelType="flat">
        <!-- 生成的POJO实现java.io.Serializable接口 -->
        <plugin type="org.mybatis.generator.plugins.SerializablePlugin" />

        <!--注释-->
        <commentGenerator>
            <!-- 将数据库中表的字段描述信息添加到注释 -->
            <property name="addRemarkComments" value="true"/>
            <!-- 注释里不添加日期 -->
            <property name="suppressDate" value="true"/>
        </commentGenerator>
        <!-- 数据库连接 -->
        <jdbcConnection
                driverClass="com.mysql.cj.jdbc.Driver"
                connectionURL="jdbc:mysql://localhost:3306/test?useSSL=false&amp;serverTimezone=UTC"
                userId="root"
                password="110325">
            <!--MySQL 不支持 schema 或者 catalog 所以需要添加这个-->
            <!--参考 : http://www.mybatis.org/generator/usage/mysql.html-->
            <property name="nullCatalogMeansCurrent" value="true"/>
        </jdbcConnection>

        <!-- 生成POJO对象，并将类放到com.songguoliang.springboot.entity包下 -->
        <javaModelGenerator targetPackage="com.wex.mybatisgeneratordemo.domain" targetProject="src/main/java"></javaModelGenerator>
        <!-- 生成mapper xml文件，并放到resources下的mapper文件夹下 -->
        <sqlMapGenerator targetPackage="mapper"  targetProject="src/main/resources"></sqlMapGenerator>


        <!-- 生成mapper xml对应dao接口，放到com.wex.mybatisgeneratordemo.dao包下-->
        <javaClientGenerator targetPackage="com.wex.mybatisgeneratordemo.dao" targetProject="src/main/java" type="XMLMAPPER"></javaClientGenerator>

        <!-- table标签可以有多个，至少一个，tableName指定表名，可以使用_和%通配符 -->
        <table tableName="user">
            <!-- 是否只生成POJO对象 -->
            <property name="modelOnly" value="false"/>
            <!-- 数据库中表名有时我们都会带个前缀，而实体又不想带前缀，这个配置可以把实体的前缀去掉 -->
<!--            <domainObjectRenamingRule searchString="^Tbl" replaceString=""/>-->

        </table>
    </context>
</generatorConfiguration>
```

**pom文件对应配置文件路径需要修改**

![](..\imgs\Snipaste_2020-07-13_22-34-06.png)



**继续双击插件生成对应的文件：**

![](..\imgs\Snipaste_2020-07-13_22-35-18.png)



**更多配置请参考官方文档：**http://mybatis.org/generator/
















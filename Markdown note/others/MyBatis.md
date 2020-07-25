# MyBatis



## 相关问题

### #{}和${}的区别

- \#{}解析传递进来的参数数据
- ${}对传递进来的参数**原样**拼接在SQL中
- **#{}是预编译处理，${}是字符串替换**。
- 使用#{}可以有效的防止SQL注入，提高系统安全性。



### 当实体类中的属性名和表中的字段名不一样 

- 在SQL语句中使用别名，与实体类中的命名一致
- 使用resultMap来使之一一对应



### 如何获得刚刚插入下去的主键

- 通过**LAST_INSERT_ID()**获取刚插入记录的自增主键值，在insert语句执行后，执行**select LAST_INSERT_ID()**就可以获取自增主键。

```xml
<insert id="insertUser" parameterType="cn.itcast.mybatis.po.User">
        <selectKey keyProperty="id" order="AFTER" resultType="int">
            select LAST_INSERT_ID()
        </selectKey>
        INSERT INTO USER(username,birthday,sex,address) 
        VALUES(#{username},#{birthday},#{sex},#{address})
</insert>
```



### 在mapper中如何传递对应参数

- 使用**#{0}和#{1}**充当占位符
- 使用**@Param**注解形式指明对应关系
- 使用**Map集合**作为参数传递


### 动态SQL

> MyBatis动态sql可以让我们在XML映射文件内，**以标签的形式编写动态sql，完成逻辑判断和动态拼接sql的功能**
>
> **原理：**使用OGNL从SQL参数中计算表达式的值，根据表达式的值动态的拼接SQL，完成对应功能

- **foreach：**循环遍历
  - item：集合中元素迭代时的别名，
  - index：集合中元素迭代时的索引
  - open：常用语where语句中，表示以什么开始，比如以'('开始
  - separator：表示在每次进行迭代时的分隔符，
  - close：常用语where语句中，表示以什么结束，
  - collection：
    - 如果传入的是单参数且参数类型是一个**List**的时候，**collection属性值为list** .
    - 如果传入的是单参数且参数类型是一个**array数组**的时候，**collection的属性值为array .**
    - 如果传入的**参数是多个**的时候，我们就需要把它们**封装成一个Map**了，当然单参数也可以封装成map，实际上如果你在传入参数的时候，**在MyBatis里面也是会把它封装成一个Map的**，map的key就是参数名，所以这个时候collection属性值就是传入的List或array对象在自己封装的map里面的key.

```xml
<foreach collection="userids" item="userid" index="index" open="(" separator="," close=")">
	#{userid}
</foreach>
```

-  **concat：**模糊查询

```xml
<where>
    <if test="name!=null">
    	name like concat('%',concat(#{name},'%'))
    </if>
</where>
```

- **choose：**按顺序判断其内部**when**标签中的**test**条件出否成立，如果有一个成立，则 choose 结束。当 choose 中所有 **when** 的条件**都不满足**时，则执行 **otherwise** 中的sql。

```xml
<where>  
    <choose>  
        <when test="username !=null ">  
            u.username LIKE CONCAT(CONCAT('%', #{username, jdbcType=VARCHAR}),'%')  
        </when >  
        <when test="sex != null and sex != '' ">  
            AND u.sex = #{sex, jdbcType=INTEGER}  
        </when >  
        <when test="birthday != null ">  
            AND u.birthday = #{birthday, jdbcType=DATE}  
        </when >  
        <otherwise>  
        </otherwise>  
    </choose>  
</where>    

```

- **selectKey：**获取生成主键
- **if，where，set**
- **trim：**可以代替**where**和**set**

```xml
<trim prefix="WHERE" prefixOverrides="AND|OR"><trim>
<trim prefix="SET" suffixOverrides=","><trim> 
```



### 半自动ORM映射

- Hibernate属于全自动ORM映射工具，使用Hibernate查询关联对象或者关联集合对象时，可以**根据对象关系模型直接获取**，所以它是全自动的。
- 而Mybatis在查询关联对象或关联集合对象时，需要**手动编写sql**来完成，所以，称之为半自动ORM映射工具。



### 延迟加载

MyBatis仅支持association关联对象和collection关联集合对象的延迟加载，**association指的就是一对一**，**collection指的就是一对多查询**。在MyBatis配置文件中，**可以配置是否启用延迟加载`lazyLoadingEnabled=true|false。`**

- **原理：**

它的原理是，**使用CGLIB创建目标对象的代理对象**，当调用目标方法时，**进入拦截器方法**，比如调用a.getB().getName()，拦截器invoke()方法发现a.getB()是null值，那么就会单独发送事先保存好的查询关联B对象的sql，把B查询上来，然后调用a.setB(b)，于是a的对象b属性就有值了，接着完成a.getB().getName()方法的调用。这就是延迟加载的基本原理。



### Executor执行器

- **SimpleExecutor：**每执行一次update或select，就开启一个Statement对象，**用完立刻关闭Statement对象**。

- **ReuseExecutor：**执行update或select，以sql作为key查找Statement对象，存在就使用，不存在就创建，用完后，不关闭Statement对象，而是放置于Map

  内，供下一次使用。简言之，**就是重复使用Statement对象**。

- **BatchExecutor：**执行update（没有select，JDBC批处理不支持select），将所有sql都添加到批处理中（addBatch()），等待统一执行（executeBatch()），**它缓存了多个Statement对象，每个Statement对象都是addBatch()完毕后，等待逐一执行executeBatch()批处理。与JDBC批处理相同**。


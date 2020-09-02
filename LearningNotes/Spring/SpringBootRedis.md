# Spring Boot 集成 Redis

> 参考文档：https://juejin.im/post/5da2728251882528e8363676，https://www.jianshu.com/p/0fa4c100e9a9



## Redis

Redis 是目前使用的非常广泛的免费开源内存数据库，是一个高性能的 key-value 数据库。

Redis 与其他 key-value 缓存（如 Memcached ）相比有以下三个特点:

> 1.Redis 支持数据的持久化，它可以将内存中的数据保存在磁盘中，重启的时候可以再次加载进行使用。 2.Redis 不仅仅支持简单的 key-value 类型的数据，同时还提供 list，set，zset，hash 等数据结构的存储。 3.Redis 支持数据的备份，即 master-slave 模式的数据备份。

Redis 优势如下：

> 1.**性能极高**。Redis 能读的速度是 110000 次/s，写的速度是 81000 次/s。 2.**丰富的数据类型**。Redis 支持二进制案例的 Strings，Lists，Sets 及 Ordered Sets 数据类型操作。 3.**原子性**。Redis 所有的操作都是原子性的，意思是要么成功执行要么失败完全不执行。单个操作是原子性的，多个操作也是，通过 MULTI 和 EXEC 指令抱起来。 4.**丰富的特性**。Redis 还支持 publish/subscribe，通知，key 过期等特性。



## SpringBoot 集成 Redis

### 导入依赖

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-data-redis</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
        <exclusions>
            <exclusion>
                <groupId>org.junit.vintage</groupId>
                <artifactId>junit-vintage-engine</artifactId>
            </exclusion>
        </exclusions>
    </dependency>
</dependencies>
```

查看 jar 包时发现，Spring Data Redis 下 org.springframework.data.redis.connection 包路径下面**默认有两个包 jedis 和 lettuce**，这说明 Spring Boot 已经默认包装适配了这两个 Redis 客户端。

> 在 springboot 1.5.x版本的默认的Redis客户端是 Jedis实现的，springboot 2.x版本中默认客户端是用 lettuce实现的。



### Lettuce 与 Jedis 比较

[Lettuce](https://lettuce.io/core/release/reference/) 和 [Jedis](http://tool.oschina.net/uploads/apidocs/redis/clients/jedis/Jedis.html) 的都是连接 Redis Server的客户端。

> **Jedis** 在实现上是直连 redis server，多线程环境下非线程安全，除非使用连接池，为每个 redis实例增加物理连接。

------

> **Lettuce** 是 一种可伸缩，线程安全，完全非阻塞的Redis客户端，多个线程可以共享一个RedisConnection,它利用Netty NIO 框架来高效地管理多个连接，从而提供了异步和同步数据访问方式，用于构建非阻塞的反应性应用程序。



### Lettuce 集成 Redis 服务

由于 Spring Boot 2.X 默认集成了 Lettuce ，所以无需导入依赖。

#### application.yml配置文件

```yaml
# 基础配置
spring:
  redis:
    # Redis数据库索引（默认为0）
    database: 0
    # Redis服务器地址
    host: 127.0.0.1
    # Redis服务器连接端口
    port: 6379
    # 链接超时时间 单位 ms（毫秒）
    timeout: 3000
    # 线程池配置
    lettuce:
      pool:
        # 连接池最大连接数（使用负值表示没有限制） 默认 8
        max-active: 8
        # 连接池中的最大空闲连接 默认 8
        max-idle: 8
        # 连接池最大阻塞等待时间（使用负值表示没有限制） 默认 -1
        max-wait: -1
        # 连接池中的最小空闲连接 默认 0
        min-idle: 0
```



#### 自定义 RedisTemplate

默认情况下的模板只能支持 `RedisTemplate<String,String>`，只能存入字符串，很多时候，我们需要自定义 RedisTemplate ，设置序列化器，这样我们可以很方便的操作实例对象。如下所示：

```java
@Configuration
public class LettuceRedisConfig {

    @Bean
    public RedisTemplate<String, Serializable> redisTemplate(LettuceConnectionFactory connectionFactory){
        RedisTemplate<String, Serializable> redisTemplate = new RedisTemplate<>();
        redisTemplate.setKeySerializer(new StringRedisSerializer());
        redisTemplate.setValueSerializer(new GenericJackson2JsonRedisSerializer());
        redisTemplate.setConnectionFactory(connectionFactory);
        return redisTemplate;
    }

    @Bean(name = "connectionFactory")
    public LettuceConnectionFactory lettuceConnectionFactory(){
        return new LettuceConnectionFactory();
    }
}
```



#### 序列化实体类

```java
package com.wex.redis_demo.domain;

import lombok.Data;

import java.io.Serializable;

@Data
public class UserEntity implements Serializable {
    private static final long serialVersionUID = 5237730257103305078L;

    private Long id;
    private String userName;
    private String userSex;

    public UserEntity() {
    }

    public UserEntity(Long id, String userName, String userSex) {
        this.id = id;
        this.userName = userName;
        this.userSex = userSex;
    }
}
```



#### 单元测试

```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserEntityTest {

    @Autowired
    private RedisTemplate<String, String> stringRedisTemplate;

    @Autowired
    private RedisTemplate<String, Serializable> serializableRedisTemplate;

    @Test
    public void testString(){
        stringRedisTemplate.opsForValue().set("strKey", "wex");
        System.out.println(stringRedisTemplate.opsForValue().get("strKey"));
    }

    @Test
    public void testSerializable(){
        UserEntity user = new UserEntity(1l, "wex", "男");
        serializableRedisTemplate.opsForValue().set("user", user);
        UserEntity user2 = (UserEntity)serializableRedisTemplate.opsForValue().get("user");
        System.out.println(user2);
    }

}
```



### Jedis 集成 Redis 服务

**pom.xml**

依赖需要先**排除lettuce包**，再添加Jedis客户端

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
    <exclusions>
        <!-- 排除lettuce包 -->
        <exclusion>
            <groupId>io.lettuce</groupId>
            <artifactId>lettuce-core</artifactId>
        </exclusion>
    </exclusions>
</dependency>

<!-- 添加jedis客户端 -->
<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
</dependency>
```

配置文件需要将lettuce换成jedis

```yaml
# 基础配置
spring:
  redis:
    # Redis数据库索引（默认为0）
    database: 0
    # Redis服务器地址
    host: 127.0.0.1
    # Redis服务器连接端口
    port: 6379
    # 链接超时时间 单位 ms（毫秒）
    timeout: 3000
    # 线程池配置
    jedis:
      pool:
        # 连接池最大连接数（使用负值表示没有限制） 默认 8
        max-active: 8
        # 连接池中的最大空闲连接 默认 8
        max-idle: 8
        # 连接池最大阻塞等待时间（使用负值表示没有限制） 默认 -1
        max-wait: -1
        # 连接池中的最小空闲连接 默认 0
        min-idle: 0
```



#### JedisRedisConfig

```java
@Configuration
public class JedisRedisConfig {
    @Bean
    public RedisTemplate<String, Serializable> redisTemplate(JedisConnectionFactory connectionFactory){
        RedisTemplate<String, Serializable> redisTemplate = new RedisTemplate<>();
        redisTemplate.setKeySerializer(new StringRedisSerializer());
        redisTemplate.setValueSerializer(new GenericJackson2JsonRedisSerializer());
        redisTemplate.setConnectionFactory(connectionFactory);
        return redisTemplate;
    }

    @Bean(name = "connectionFactory")
    public JedisConnectionFactory lettuceConnectionFactory(){
        return new JedisConnectionFactory();
    }
}

```



**单元测试也是一样的，直接执行就可以获得答案了**



## RedisTemplate使用方法归纳

### 介绍

RedisTemplate是Spring Data Redis提供给用户的**最高级的抽象客户端**，用户可直接通过RedisTemplate进行多种操作，那么，我们先来看看RedisTemplate封装了哪些操作。下面这列表是RedisTemplate的继承关系和所有方法（已过滤重载方法，共有81个方法）



#### 继承关系

```dart
//RedisAccessor是RedisTemplate定义普通属性的基类，不直接使用
//RedisOperations是指定RedisTemplate实现的Redis connection操作的集合接口
//BeanClassLoaderAware是给其实现类是设置类加载器的接口
RedisTemplate<K, V> extends RedisAccessor implements RedisOperations<K, V>, BeanClassLoaderAware
```



#### 方法

```go
//配置默认序列化与反序列化工具类
2.afterPropertiesSet
//根据参数执行相关operation操作，例如，事务
3.execute
//执行pipelining流水线相关操作
4.executePipelined
//执行指定connection连接的相关操作
5.executeWithStickyConnection
//执行session内的execute方法
6.executeSession
//创建RedisConnection代理类
7.createRedisConnectionProxy
//connection连接的预处理
8.preProcessConnection
//结果的后处理，默认什么都不做
9.postProcessResult
//是否向RedisCallback暴露本地连接
10.isExposeConnection
//设置是否向RedisCallback暴露本地连接
11.setExposeConnection
//12到26都是设置和获取相关序列化工具类
12.isEnableDefaultSerializer
13.setEnableDefaultSerializer
14.getDefaultSerializer
15.setDefaultSerializer
16.setKeySerializer
17.getKeySerializer
18.setValueSerializer
19.getValueSerializer
20.getHashKeySerializer
21.setHashKeySerializer
22.getHashValueSerializer
23.setHashValueSerializer
24.getStringSerializer
25.setStringSerializer
26.setScriptExecutor
//27到34为私有方法，不对外提供使用
27.rawKey
28.rawString
29.rawValue
30.rawKeys
31.deserializeKey
32.deserializeMixedResults
33.deserializeSet
34.convertTupleValues
//执行事务
35.exec
36.execRaw
//删除操作
37.delete
//接触链接
38.unlink
//查看是否含有指定key
39.hasKey
40.countExistingKeys
//设置过期时间
41.expire
42.expireAt
//转换成字节流并向channel发送message
43.convertAndSend
//获取过期时间
44.getExpire
//根据传入的正则表达式返回所有的key
46.keys
//取消指定key的过期时间
47.persist
//移动指定的key和index到数据库中
48.move
//从键空间随机获取一个key
49.randomKey
//将指定key改成目标key
50.rename
//key不存在时，将指定key改成目标key
51.renameIfAbsent
//设置存储在指定key的类型
52.type
//检索存储在key的值的序列化版本
53.dump
//执行Redis的restore的命令
54.restore
//标记事务阻塞的开始
55.multi
//丢弃所有在multi之后发出的命令
56.discard
//观察指定key在事务处理开始即multi之后的修改情况
57.watch
//刷新先前观察的所有key
58.unwatch
//为key元素排序
59.sort
//关闭客户端连接
60.killClient
//请求连接客户端的相关信息和统计数据
61.getClientList
//更改复制配置到新的master
62.slaveOf
//将本机更改为master
63.slaveOfNoOne
//64到79都是获取相对应的操作
64.opsForCluster
65.opsForGeo
66.boundGeoOps
67.boundHashOps
68.opsForHash
69.opsForHyperLogLog
70.opsForList
71.boundListOps
72.boundSetOps
73.opsForSet
74.opsForStream
75.boundStreamOps
76.boundValueOps
77.opsForValue
78.boundZSetOps
79.opsForZSet
//设置是否支持事务
80.setEnableTransactionSupport
//设置bean的类加载器
81.setBeanClassLoader
```



#### 功能介绍

spring-data-redis的提供了如下功能：

1. 连接池自动管理，提供了一个高度封装的“RedisTemplate”类
2. 进行了归类封装,将同一类型操作封装为operation接口

> ValueOperations：简单K-V操作
>  SetOperations：set类型数据操作
>  ZSetOperations：zset类型数据操作
>  HashOperations：针对map类型的数据操作
>  ListOperations：针对list类型的数据操作

3. 提供了对key的“bound”(绑定)便捷化操作API，可以通过bound封装指定的key，然后进行一系列的操作而无须“显式”的再次指定Key，即BoundKeyOperations：

> BoundValueOperations
>  BoundSetOperations
>  BoundListOperations
>  BoundSetOperations
>  BoundHashOperations

4. 将事务操作封装，有容器控制。

5. 针对数据的“序列化/反序列化”，提供了多种可选择策略(RedisSerializer)

> 1.JdkSerializationRedisSerializer：POJO对象的存取场景，使用JDK本身序列化机制，将pojo类通过ObjectInputStream/ObjectOutputStream进行序列化操作，最终redis-server中将存储字节序列。是目前最常用的序列化策略。
>
>  2.StringRedisSerializer：Key或者value为字符串的场景，根据指定的charset对数据的字节序列编码成string，是“newString(bytes,charset)”和“string.getBytes(charset)”的直接封装。是最轻量级和高效的策略。 
>
>  3.JacksonJsonRedisSerializer：jackson-json工具提供了javabean与json之间的转换能力，可以将pojo实例序列化成json格式存储在redis中，也可以将json格式的数据转换成pojo实例。因为jackson工具在序列化和反序列化时，需要明确指定Class类型，因此此策略封装起来稍微复杂。  
>
>  4.OxmSerializer：提供了将javabean与xml之间的转换能力，目前可用的三方支持包括jaxb，apache-xmlbeans；redis存储的数据将是xml工具。不过使用此策略，编程将会有些难度，而且效率最低；不建议使用。【需要spring-oxm模块的支持】

如果你的数据需要被第三方工具解析，那么数据应该使用StringRedisSerializer而不是JdkSerializationRedisSerializer。



### 依赖导入

```xml
redis的引用包：
<dependency>
    <groupId>org.springframework.data</groupId>
    <artifactId>spring-data-redis</artifactId>
    <version>1.8.6.RELEASE</version>
</dependency>
```



### 具体使用

**(1) String类型：**

- 插入操作



```cpp
UserInfo markIfFirstSync = new UserInfo();
userRedisTemplate.opsForValue().set(CACHE_KEY, markIfFirstSync);
```

- 向redis中某个string类型的key下面插入一个对象。



```dart
批量插入操作
public Map<String, OrgRelationInfo> mappingRelationRefresh = new HashMap<>();
redisTemplate.opsForValue().multiSet(mappingRelationRefresh);
```

- 获取对象操作



```css
  userRedisTemplate.opsForValue().get(CACHE_NAME + CACHE_KEY_EXIST_PREFIX);
```

- 从redis中获取某个key下面的某个对象，如果不存在就返回null。



```dart
 //批量获取对象操作
  List<String> sourceIdList = new ArrayList<>();
  List<OrgRelationInfo> orgMappingRelationList = redisTemplate.opsForValue().multiGet(sourceIdList);
//从redis中获取多个key下面的多个对象，返回一个List列表，但是即使对应的key下面没有值，这个**value也会返回**，不过是
//null，因此要判断是否List都为空，不能够用isEmpty直接判断，而应该一个一个的判断是否为空，才能判断整体为空。
```

**(2) List类型**

- 批量插入



```dart
//向redis的某个key下面的list列表里面插入一个list列表，不会去重。
List<OrgRelationInfo> remainOrgNodes = new ArrayList<>();
redisTemplate.opsForList().leftPushAll(CACHE_KEY, remainOrgNodes);
```

- 批量取出



```objectivec
//从redis中取出某一个key下面的list列表， 0表示从列表的第0个元素开始取，-1表示直取到倒数第一个元素，也就是整个列表的所有元素都取出来。
List<OrgRelationInfo> lastRemainOrgNodeList = redisTemplate.opsForList().range(CACHE_NAME + CACHE_REMAIN_KEY_PREFIX, 0, -1);
```

**(3) Hash类型**

- 批量插入



```csharp
// 向redis中某个key下面插入key，hash的Map。
Map<Long, UserRelationInfo> value = new HashMap<>();
userHashRedisTemplate.opsForHash().putAll(KEY, value );
```

-单个删除



```cpp
//从redis中某个key下面删除掉某个hashkey所在的value。
userHashRedisTemplate.opsForHash().delete(key, sourceOrgId);
```

- 单个获取



```csharp
//从redis中某个key下面得到这个key对应的hashkey的value值。前一个key只能是String类型，hashKey可以声明为自己需要的类型。
userHashRedisTemplate.opsForHash().get(Key， hashKey);
```

- 批量获取



```dart
//从redis中得到某个key下面的所有的hashkey和hashvalue值。
Map<Object, Object> userOrgMap = userHashRedisTemplate.opsForHash().entries(getUserNodeCacheKey(syncUserNode.getSourceId()));
```

**(4) Set类型**

- 单个插入



```csharp
//向redis的某个key下面的set列表里面插入一个元素，回去重，且无序。
userRoleSetRedisTemplate.opsForSet().add(KEY, cloudtOrgRoleInfo);
```

- 批量取出



```bash
//从redis的某个key下面得到set集合的所有元素，返回的也是一个Set集合。
cloudtOrgRoleSet = userRoleSetRedisTemplate.opsForSet().members(KEY);
```

- 单个删除



```csharp
//从redis的某个key下面的set集合中删除掉一个元素。
userRoleSetRedisTemplate.opsForSet().remove( KEY, subDeleteOrgRoleUserArray[i]);
```




















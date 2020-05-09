# Redis

> 快的原因：在内存操作，C语言编写，单机



## 常用命令

### 字符串

- **keys [通配符]**：遍历对应的key（一般不使用，速度慢）O(n)
- **set key value**：插入数据  O(1)
- **setns key value**：当key不存在，插入数据  O(1)
- **set key value xx**：当可以存在，插入数据  O(1)
- **mset key value key value ...**：批量插入数据  O(n) 省去网络时间，效率高
- **get key**：获得对应键的值  O(1)
- **mget key key ...**：批量获得对应键的值  O(n) 省去网络时间，效率高
- **dbsize**：key的数量（实时更新）  O(1)
- **exists key**：判断一个key是否存在  O(1)
- **del key**：删除对应键值  O(1)
- **expire key seconds**：在seconds秒之后过期  O(1)
- **ttl key**：查看key的过期时间  O(1)
- **persist key**：去除key的过期时间  O(1)
- **type key**：返回键的类型  O(1)
- **incr/decr key**：key对应的值自增/自减，如果没有值就等于1
- **incrby/decrby key k**：自增自减k，如果没有值就等于k
- **getset key newvalue**：设置一个新值并且返回旧值
- **append key value**：将新值追加到旧值
- **strlen key**：返回字符串的长度（注意中文）



### 哈希

> map里面又是map

- **hget key field**：获取对应键的对应属性的值
- **hset key field value**：设置对应键的对应属性的值
- **hdel key field **：删除对应键的对应属性的值
- **hexists key field**：判断对应键有没有对应属性
- **hlen key**：返回key中field数量
- **mhset/mhget**：同字符串
- **hincr/hdecr**：同字符串
- **hgetall key**：获取key里面所有的field和value（效率低）
- **hgetvals key**：获取所有key中的value
- **hkeys key**：获取所有key中的field



### 列表

- **rpush key value1 value2 ...**：从列表右端插入值
- **rpop key**：右侧弹出
- **brpop key timeout**：右侧弹出，阻塞版
- **linsert key before|after value newvalue**：在value之前或之后插入newvalue
- **lrem key count value**：删除key中最多n个值为value的项。
  - 当count = 0：删除所有
  - 当count < 0，从右往左删count绝对值个项
  - 当count > 0，从左往右删count个项
- **lrange key start end**：获取从start到end的元素
- **lindex key index**：获取对应索引值的元素
- **llen key**：获得列表长度
- **lset key index value**：设置某个下标的新值



### 集合

- **sadd key element**：向集合里添加元素，如果有此元素，添加失败
- **srem key element**：删除集合里的元素
- **scard key**：集合里面的元素数量
- **sismember key element**：元素是否存在
- **srandmember key count**：从集合里随机抽取count个元素，不破坏集合
- **spop key**：随机弹出
- **smember key**：取出集合里面的全部元素
- **sdiff|sinter|sunion key1 key2**：差集，交集，并集
- **sdiff|sinter|sunion + store destkey** ：并保存在destkey中



### 有序集合

- **zadd key score element**：添加，score可以重复，element不可以
- **zrem key element**：删除集合里的元素
- **zscore key element**：获取对应的分值
- **zcard key**：计算元素总数量
- **zrank key element**：获取元素的排名
- **zrange key start end [withscore]**：返回指定索引的升序元素【是否打印分数】
- **zrangebyscore key min max [withscore]**：返回指定分数范围的元素
- **zcount key min max**：指定分数范围的个数
- **zremrangebyrank/score key start end**：根据指定排名或者分数范围删除



## 慢查询

###配置

- **slow-max-len**：先进先出队列的长度
- **slowlog-log-lower-than**：慢查询的时间阈值（单位：微妙）

### 命令

- **slowlog get[n]**：获取n条慢查询队列信息
- **slowlog len**：获取慢查询队列长度
- **slowlog reset**：重置慢查询队列



## 流水线

```java
Jedis jedis = JedisUtil.getJedis();
        Pipeline pipeline = jedis.pipelined();
        for (int i = 0; i < 100; i++) {
            pipeline.hset("hset"+i, "field"+i, "value"+i);
        }
        pipeline.syncAndReturnAll();
```





## 发布订阅

### 命令

- **subscribe [channel]**：订阅频道
- **publish [channel] message**：发布消息



## 位图

### 命令

- **getbit key offset**：获取指定位的值
- **setbit key offset value**：设定指定位的值
- **bitcount key [start end]**：获取指定区间1的个数
- **bitop  op destkey key key ...**：把不同key做交集等，然后保存在destkey
- **bitpos key targetbit [start end]**：返回指定范围内第一个位等于targetbit的位



## hyperloglog

### 命令

- **pfadd key element element2...**：添加元素
- **pfcount key**：统计元素
- **pfmerge destkey key1 key2**：将两个key的元素合并并且放进去destkey



## geo（redis3.2以上）

>可以计算经纬度，主要用于地理计算





## 持久化

### RDB

- **触发机制：**
  - save（同步）
  - bgsave（异步）（fork）
  - 自动（配置实现）



### AOF

- **写入策略：**
  - always
  - everysec
  - no
- **AOF重写:**
  - bgrewriteaof
  - 重写配置



## fork（同步）

- 与内存量息息相关，内存越大，耗时越大
- info：latest_fork_usec，查执行时间



## 主从复制

### 命令

- **slaveof socket：**复制主数据库（异步） 
- **slaveof no one：**取消复制，但不会删除自身已有的数据



##数据结构

Redis**使用对象**来表示数据库中的键和值。每次我们在Redis数据库中新创建一个键值对时，**至少会创建出两个对象**。一个是**键对象**，一个是**值对象**。

### SDS简单动态字符串

- Redis使用**sdshdr结构**来表示一个SDS值：

```c++
struct sdshdr{

    // 字节数组，用于保存字符串
    char buf[];

    // 记录buf数组中已使用的字节数量，也是字符串的长度
    int len;

    // 记录buf数组未使用的字节数量
    int free;
}
```

![](E:\git\WexNote\Audition Points\imgs\640.png)

- **优势：**

  1. sdshdr使用**len**来记录字符串长度，可以**直接获取**，无需遍历。
  2. 内部实现了**动态扩展机制**，如果修改了SDS之后，内存不足，会**先扩展空间**，再进行修改。
  3. 减少空间分配的次数，因为内部除了分配必要的空间，还会分配一定的**空闲空间**。
  4. 是二进制安全的，所有的API都会以处理**二进制的方式**去处理buf数组里面的数据。

  

### 链表

```c
// 使用listNode结构来表示每个节点：
typedef strcut listNode{

    //前置节点
    strcut listNode  *pre;

    //后置节点
    strcut listNode  *next;

    //节点的值
    void  *value;

}listNode

// 使用listNode是可以组成链表了，Redis中使用list结构来持有链表：
typedef struct list{

    //表头结点
    listNode  *head;

    //表尾节点
    listNode  *tail;

    //链表长度
    unsigned long len;

    //节点值复制函数
    void *(*dup) (viod *ptr);

    //节点值释放函数
    void  (*free) (viod *ptr);

    //节点值对比函数
    int (*match) (void *ptr,void *key);

}list
```

![](E:\git\WexNote\Audition Points\imgs\640 (1).webp)

- **特性：**
  - **无环双向**链表。
  - 保存**头尾指针**和**链表长度**，可以一次获取。
  - 使用 `void *` 记录值，所以可以**保存不同类型**的值。



### 哈希表

在Redis中，`key-value`的数据结构底层就是哈希表来实现的。

```c
// 哈希表
typedef struct dictht{

    //哈希表数组
    dictEntry **table;  

    //哈希表大小
    unsigned long size;    

    //哈希表大小掩码，用于计算索引值
    //总是等于size-1
    unsigned long sizemark;     

    //哈希表已有节点数量
    unsigned long used;

}dictht

// 哈希结点
typedef struct dictEntry {

    //键
    void *key;

    //值
    union {
        void *value;
        uint64_tu64;
        int64_ts64;
    }v;    

    //指向下个哈希节点，组成链表
    struct dictEntry *next;

}dictEntry;
```

![](E:\git\WexNote\Audition Points\imgs\640 (2).webp)

```c
// Redis为了更好的操作，对哈希表往上再封装了一层(参考上面的Redis实现链表)，使用dict结构来表示：
typedef struct dict {

    //类型特定函数
    dictType *type;

    //私有数据
    void *privdata;

    //哈希表
    dictht ht[2];

    //rehash索引
    //当rehash不进行时，值为-1
    int rehashidx;  

}dict;


//-----------------------------------

typedef struct dictType{

    //计算哈希值的函数
    unsigned int (*hashFunction)(const void * key);

    //复制键的函数
    void *(*keyDup)(void *private, const void *key);

    //复制值的函数
    void *(*valDup)(void *private, const void *obj);  

    //对比键的函数
    int (*keyCompare)(void *privdata , const void *key1, const void *key2)

    //销毁键的函数
    void (*keyDestructor)(void *private, void *key);

    //销毁值的函数
    void (*valDestructor)(void *private, void *obj);  

}dictType
```

![](E:\git\WexNote\Audition Points\imgs\640.jpg)

- 从代码实现和示例图上我们可以发现，**Redis中有两个哈希表**：
  - ht[0]：用于存放**真实**的 `key-vlaue` 数据
  - ht[1]：用于**扩容(rehash)**

- Redis中哈希算法和哈希冲突跟Java实现的差不多，它俩**差异**就是：
  - Redis哈希冲突时：是将新节点添加在链表的**表头**。
  - JDK1.8后，Java在哈希冲突时：是将新的节点添加到链表的**表尾**。

#### rehash过程

在对哈希表进行扩展或者收缩操作时，reash过程并不是一次性地完成的，而是**渐进式**地完成的。

- 在字典中维持一个**索引计数器变量**rehashidx，并将设置为**0**，表示rehash开始。
- 在rehash期间每次对字典进行增加、查询、删除和更新操作时，**除了执行指定命令外**；还会将ht[0]中rehashidx索引上的值**rehash到ht[1]**，操作完成后rehashidx+1。
- 字典操作不断执行，最终在某个时间点，所有的键值对完成rehash，这时**将rehashidx设置为-1，表示rehash完成**
- 在渐进式rehash过程中，字典会**同时使用两个哈希表ht[0]和ht[1]**，所有的更新、删除、查找操作也会在两个哈希表进行。例如要查找一个键的话，**服务器会优先查找ht[0]，如果不存在，再查找ht[1]**，诸如此类。此外当执行**新增操作**时，新的键值对**一律保存到ht[1]**，不再对ht[0]进行任何操作，以保证ht[0]的键值对数量**只减不增**，直至变为空表。



### 跳跃表

跳跃表(shiplist)是实现sortset(**有序**集合)的底层数据结构之一

```c
// zskiplistNode表示跳跃表的节点
typeof struct zskiplistNode {
        // 后退指针
        struct zskiplistNode *backward;
        // 分值
        double score;
        // 成员对象
        robj *obj;
        // 层
        struct zskiplistLevel {
                // 前进指针
                struct zskiplistNode *forward;
                // 跨度
                unsigned int span;
        } level[];
} zskiplistNode;

// zskiplist保存跳跃表的信息(表头，表尾节点，长度)
typeof struct zskiplist {
        // 表头节点，表尾节点
        struct skiplistNode *header,*tail;
        // 表中节点数量
        unsigned long length;
        // 表中最大层数
        int level;
} zskiplist;
```

![](E:\git\WexNote\Audition Points\imgs\640 (1).jpg)

### 整数集合

```c
typeof struct intset {
        // 编码方式
        unit32_t encoding;
        // 集合包含的元素数量
        unit32_t lenght;
        // 保存元素的数组
        int8_t contents[];
} intset;
```



### 压缩列表

压缩列表(ziplist)是list和hash的底层实现之一。如果list的每个都是小整数值，或者是比较短的字符串，压缩列表(ziplist)作为list的底层实现。

压缩列表(ziplist)是Redis为了节约内存而开发的，是由一系列的**特殊编码的连续内存块**组成的**顺序性**数据结构。

压缩列表结构图例如下：

![压缩列表的组成部分](https://mmbiz.qpic.cn/mmbiz_png/2BGWl1qPxib3601DAl7JNGKSFZFgcxM67dPV9J3ANXSZ3OZiaodMUlQYbL6L06KzFP3ia8niaxia2Ljul9PX7ya3mpg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)压缩列表的组成部分

下面我们看看节点的结构图：

![img](https://mmbiz.qpic.cn/mmbiz_png/2BGWl1qPxib3601DAl7JNGKSFZFgcxM67Fd7lkj3IWjkicM0SibERgVm2KMpu1gNEpYaAvUtd6ibOm0SicHJdBnPib8Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

> 压缩列表从表尾节点**倒序遍历**，首先指针通过zltail偏移量指向表尾节点，然后通过指向**节点记录的前一个节点的长度依次向前遍历访问整个压缩列表**。



### 数据结构的对象

![](E:\git\WexNote\Audition Points\imgs\640 (2).jpg)

#### 字符串对象

在上面的图我们知道string类型有三种**编码格式**：

- int：整数值，这个整数值可以使用long类型来表示

- - 如果是浮点数，那就用embstr或者raw编码。具体用哪个就看这个数的长度了

- embstr：字符串值，这个字符串值的长度小于32字节

- raw：字符串值，这个字符串值的长度大于32字节

embstr和raw的**区别**：

- raw分配内存和释放内存的次数是两次，embstr是一次
- embstr编码的数据保存在一块**连续**的内存里面

编码之间的**转换**：

- int类型如果存的**不再是一个整数值**，则会从int转成raw
- embstr是只读的，在修改的时候回从embstr转成raw



#### 列表对象

在上面的图我们知道list类型有两种**编码格式**：

- ziplist：字符串元素的长度都小于64个字节并且总数量少于512个
- linkedlist：字符串元素的长度大于64个字节或者总数量大于512个

```
redis > RPUSH numbers 1 "three" 5
(integer) 3 
```

![](E:\git\WexNote\Audition Points\imgs\640 (3).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (3).jpg)

编码之间的**转换：**

- 原本是ziplist编码的，如果保存的数据长度太大或者元素数量过多，会转换成linkedlist编码的。



#### 哈希对象

在上面的图我们知道hash类型有两种**编码格式**：

- ziplist：key和value的字符串长度都小于64字节并且键值对总数量小于512
- hashtable：key和value的字符串长度大于64字节或者键值对总数量大于512

![](E:\git\WexNote\Audition Points\imgs\640 (4).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (5).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (6).webp)

编码之间的**转换：**

- 原本是ziplist编码的，如果保存的数据长度太大或者元素数量过多，会转换成hashtable编码的。



#### 集合对象

在上面的图我们知道set类型有两种**编码格式**：

- intset：保存的元素全都是整数并且总数量小于512
- hashtable：保存的元素不是整数或者总数量大于512

intset编码的集合结构：

![](E:\git\WexNote\Audition Points\imgs\640 (7).webp)

hashtable编码的集合结构：

![](E:\git\WexNote\Audition Points\imgs\640 (4).jpg)

编码之间的**转换：**

- 原本是intset编码的，如果保存的数据不是整数值或者元素数量大于512，会转换成hashtable编码的。



####有序集合

在上面的图我们知道set类型有两种**编码格式**：

- ziplist：元素长度小于64并且总数量小于128
- skiplist：元素长度大于64或者总数量大于128

ziplist编码的有序集合结构：

![](E:\git\WexNote\Audition Points\imgs\640 (4).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (10).webp)



skiplist编码的有序集合结构：

![](E:\git\WexNote\Audition Points\imgs\640 (9).webp)



有序集合(sortset)对象**同时采用skiplist和哈希表来实现**：

- skiplist能够达到插入的时间复杂度为O(logn)，根据成员查分值的时间复杂度为O(1)

编码之间的**转换：**

- 原本是ziplist编码的，如果保存的数据长度大于64或者元素数量大于128，会转换成skiplist编码的。



## Redis数据库

Redis默认有16个数据库，数据库之间是**相互隔离**的，也就是说，再15号数据库里存的东西，在14号数据库是找不到的。（数据库编号**默认从0开始**）

### 原理

```c
// Redis服务器用redisServer结构体来表示，其中redisDb是一个数组，用来保存所有的数据库，dbnum代表数据库的数量(这个可以配置，默认是16)
struct redisServer{  

    //redisDb数组,表示服务器中所有的数据库
    redisDb *db;  

    //服务器中数据库的数量
    int dbnum;  

};

// Redis是C/S结构，Redis客户端通过redisClient结构体来表示
typedef struct redisClient{  

    //客户端当前所选数据库
    redisDb *db;  

}redisClient;

// Redis中对每个数据库用redisDb结构体来表示：
typedef struct redisDb { 
    int id;         // 数据库ID标识
    dict *dict;     // 键空间，存放着所有的键值对              
    dict *expires;  // 过期哈希表，保存着键的过期时间                          
    dict *watched_keys; // 被watch命令监控的key和相应client    
    long long avg_ttl;  // 数据库内所有键的平均TTL（生存时间）     
} redisDb;
```

![](E:\git\WexNote\Audition Points\imgs\640 (11).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (12).webp)

### 过期时间

Redis是**基于内存**，内存是比较昂贵的，容量肯定比不上硬盘的。就

因为我们的内存是**有限**的。所以我们**会干掉不常用的数据，保留常用的数据**。这就需要我们设置一下键的过期(生存)时间了。

- 设置键的**生存**时间可以通过`EXPIRE`或者`PEXPIRE`命令。
- 设置键的**过期**时间可以通过`EXPIREAT`或者`PEXPIREAT`命令。

其实`EXPIRE`、`PEXPIRE`、`EXPIREAT`这三个命令都是通过`PEXPIREAT`命令来实现的。

我们在redisDb结构体中还发现了`dict *expires;`属性，**存放所有键过期的时间**。

既然有设置过期(生存)时间的命令，那肯定也有**移除过期时间**，**查看剩余生存时间**的命令了：

- **PERSIST**(移除过期时间)
- **TTL(Time To Live)**返回剩余生存时间，以秒为单位
- **PTTL**以毫秒为单位返回键的剩余生存时间

#### 策略

- **定时删除**(对内存友好，对CPU不友好)

- - **到时间点**上就把所有过期的键删除了。

- **惰性删除**(对CPU极度友好，对内存极度不友好)

- - 每次从键空间取键的时候，判断一下该键是否过期了，如果过期了就删除。

- **定期删除**(折中)

- - **每隔**一段时间去删除过期键，**限制**删除的执行时长和频率。


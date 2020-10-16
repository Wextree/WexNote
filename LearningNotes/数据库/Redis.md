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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640.png)

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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6401.webp)

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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6402.webp)

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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640.jpg)

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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6401.jpg)



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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/ziplist.webp)

压缩列表的组成部分

下面我们看看节点的结构图：

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/ziplistnode.webp)



> 压缩列表从表尾节点**倒序遍历**，首先指针通过zltail偏移量指向表尾节点，然后通过指向**节点记录的前一个节点的长度依次向前遍历访问整个压缩列表**。



### 数据结构的对象

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6402.jpg)

#### 字符串对象

在上面的图我们知道string类型有三种**编码格式**：

- int：整数值，这个整数值可以使用long类型来表示

- 如果是浮点数，那就用embstr或者raw编码。具体用哪个就看这个数的长度了

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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6403.webp)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(3).jpg)

编码之间的**转换：**

- 原本是ziplist编码的，如果保存的数据长度太大或者元素数量过多，会转换成linkedlist编码的。



#### 哈希对象

在上面的图我们知道hash类型有两种**编码格式**：

- ziplist：key和value的字符串长度都小于64字节并且键值对总数量小于512
- hashtable：key和value的字符串长度大于64字节或者键值对总数量大于512

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(4).webp)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(5).webp)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(6).webp)

编码之间的**转换：**

- 原本是ziplist编码的，如果保存的数据长度太大或者元素数量过多，会转换成hashtable编码的。



#### 集合对象

在上面的图我们知道set类型有两种**编码格式**：

- intset：保存的元素全都是整数并且总数量小于512
- hashtable：保存的元素不是整数或者总数量大于512

intset编码的集合结构：

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/intset编码的集合结构.webp)

hashtable编码的集合结构：

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/hashtable编码的集合结构.jpg)

编码之间的**转换：**

- 原本是intset编码的，如果保存的数据不是整数值或者元素数量大于512，会转换成hashtable编码的。



####有序集合

在上面的图我们知道set类型有两种**编码格式**：

- ziplist：元素长度小于64并且总数量小于128
- skiplist：元素长度大于64或者总数量大于128

ziplist编码的有序集合结构：

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/6401111.webp)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(10).webp)



skiplist编码的有序集合结构：

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(9).webp)



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

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(11).webp)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(12).webp)

### 过期时间

Redis是**基于内存**，内存是比较昂贵的，容量肯定比不上硬盘的。就因为我们的内存是**有限**的。所以我们**会干掉不常用的数据，保留常用的数据**。这就需要我们设置一下键的过期(生存)时间了。

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
- 到时间点**上就把所有过期的键删除了**。
- **惰性删除**(对CPU极度友好，对内存极度不友好)
- 每次从键空间取键的时候，判断一下该键是否过期了，如果过期了就删除。
- **定期删除**(折中)
- **每隔**一段时间去删除过期键，**限制**删除的执行时长和频率。



## 事件

Redis服务器是一个**事件驱动程序**，主要处理以下两类事件：

- 文件事件：文件事件其实就是**对Socket操作的抽象**，Redis服务器与Redis客户端的通信会产生文件事件，服务器通过监听并处理这些事件来完成一系列的网络操作
- 时间事件：时间事件其实就是对**定时操作的抽象**，前面我们已经讲了RDB、AOF、定时删除键这些操作都可以由服务端去定时或者周期去完成，底层就是通过触发时间事件来实现的！



### 文件事件

Redis开发了自己的网络事件处理器，这个处理器被称为**文件事件处理器**。

有**四部分**：

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/64(18).webp)

文件事件处理器使用I/O多路复用程序来**同时监听多个Socket**。Redis中的I/O多路复用程序会将所有**产生事件的Socket放到一个队列**里边，然后通过这个队列以有序、同步、每次一个Socket的方式向文件事件分派器传送套接字。也就是说：当上一个Socket**处理完毕**后，I/O多路复用程序才会向文件事件分派器传送**下一个**Socket。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(8).jpg)

### 时间事件

持续运行的Redis服务器会**定期**对自身的资源和状态进行检查和调整，这些定期的操作由**serverCron**函数负责执行，它的主要工作包括：

- 更新服务器的统计信息(时间、内存占用、数据库占用)
- 清理数据库的过期键值对
- AOF、RDB持久化
- 如果是主从服务器，对从服务器进行定期同步
- 如果是集群模式，对进群进行定期同步和连接
- …

Redis服务器将时间事件放在一个**链表**中，当时间事件执行器运行时，会遍历整个链表。时间事件包括：

- **周期性事件**(Redis一般只执行serverCron时间事件，serverCron时间事件是周期性的)
- 定时事件



### 时间事件和文件事件

- 文件事件和时间事件之间是**合作**关系，服务器会轮流**处理**这两种事件，并且处理事件的过程中不会发生抢占。
- 时间事件的实际处理事件通常会比设定的到达时间**晚**一些



### 为什么单线程Redis那么快

- 纯内存操作
- 核心是基于**非阻塞**的**IO多路复用操作**
- 单线程**避免了**频繁的**上下文切换**工作



##客户端和服务器

服务器使用clients**链表**连接多个客户端状态，新添加的客户端状态会被放到**链表的末尾**

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(19).webp)

- 一个服务器可以与多个客户端建立网络连接，每个**客户端可以向服务器发送命令请求**，而服务器则接收并处理客户端发送的命令请求，并向客户端返回命令回复。
- Redis服务器使用**单线程单进程**的方式处理命令请求。在数据库中保存**客户端执行命令所产生的数据**，并通过**资源管理来维持**服务器自身的运转。



### 客户端

客户端章节中主要讲解了**Redis客户端的属性**(客户端状态、输入/输出缓冲区、命令参数、命令函数等等)

```c
typedef struct redisClient{

    //客户端状态的输入缓冲区用于保存客户端发送的命令请求,最大1GB,否则服务器将关闭这个客户端
    sds querybuf;  


    //负责记录argv数组的长度。
    int argc;   

    // 命令的参数
    robj **argv;  

    // 客户端要执行命令的实现函数
    struct redisCommand *cmd, *lastcmd;  


    //记录了客户端的角色(role),以及客户端所处的状态。 (REDIS_SLAVE | REDIS_MONITOR | REDIS_MULTI) 
    int flags;             

    //记录客户端是否通过了身份验证
    int authenticated;     

    //时间相关的属性
    time_t ctime;           /* Client creation time */       
    time_t lastinteraction; /* time of the last interaction, used for timeout */
    time_t obuf_soft_limit_reached_time;


    //固定大小的缓冲区用于保存那些长度比较小的回复
    /* Response buffer */
    int bufpos;
    char buf[REDIS_REPLY_CHUNK_BYTES];

    //可变大小的缓冲区用于保存那些长度比较大的回复
    list *reply; //可变大小缓冲区由reply 链表和一个或多个字符串对象组成
    //...
}
```

### 服务器

**服务器章节**中主要讲解了Redis服务器读取客户端发送过来的命令是如何解析，以及初始化的过程。

服务器从启动到能够处理客户端的命令请求需要执行以下的步骤：

- 初始化服务器状态
- 载入服务器配置
- 初始化服务器的数据结构
- 还原数据库状态
- 执行事件循环

```c
def main():

    init_server();

    while server_is_not_shutdown();
        aeProcessEvents()

    clean_server();
```

客户端发送命令到完成主要包括的步骤：

- 客户端将命令请求发送给服务器
- 服务器读取命令请求，分析出**命令参数**
- 命令执行器根据参数**查找命令的实现函数**，执行实现函数并得出命令回复
- 服务器将命令回复返回给客户端



## 主从架构

下面我们来看看Redis的主从架构特点：

- **主**服务器负责接收**写**请求
- **从**服务器负责接收**读**请求
- 从服务器的数据由主服务器**复制**过去。主从服务器的数据是**一致**的

主从架构的**好处**：

- 读写分离(主服务器负责写，从服务器负责读)
- 高可用(某一台从服务器挂了，其他从服务器还能继续接收请求，不影响服务)
- 处理更多的并发量(每台从服务器**都可以接收读请求**，读QPS就上去了)



### 复制

> 在Redis中，用户可以通过执行**SALVEOF命令**或者设置salveof选项，让一个服务器去复制(replicate)另一个服务器，我们称呼被复制的服务器为**主服务器(master)**，而对主服务器进行复制的服务器则被称为**从服务器(salve)**

####复制功能分为两个操作：

- 同步(sync)

- - 将从服务器的数据库状态**更新至**主服务器的数据库状态

- 命令传播(command propagate)

- - 主服务器的数据库状态**被修改**，导致主从服务器的数据库状态**不一致**，让主从服务器的数据库状态**重新回到一致状态**。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(20).webp)

从服务器对主服务器的**同步又可以分为两种情况**：

- 初次同步：从服务器**没有复制过任何**的主服务器，或者从服务器要复制的主服务器跟上次复制的主服务器**不一样**。
- 断线后同步：处于**命令传播阶段**的主从服务器因为**网络原因**中断了复制，从服务器通过**自动重连**重新连接主服务器，并继续复制主服务器

> 在Redis2.8以前，断线后复制这部分其实缺少的只是**部分的数据**，但是要让主从服务器**重新执行SYNC命令**，这样的做法是非常低效的。(因为执行SYNC命令是把**所有的数据**再次同步，而不是只同步丢失的数据)

####前置工作

首先我们来看一下**前置的工作**：

- 从服务器设置主服务器的IP和端口
- 建立与主服务器的Socket连接
- 发送PING命令(检测Socket读写是否正常与主服务器的通信状况)
- 身份验证(看有没有设置对应的验证配置)
- 从服务器给主服务器发送端口的信息，主服务器记录监听的端口

> Redis从2.8版本开始，使用PSYNC命令来**替代**SYNC命令执行复制时同步的操作。
>
> PSYNC命令具有**完整**重同步和**部分**重同步两种模式(其实就跟上面所说的初次复制和断线后复制差不多个意思)。

- **完整同步**
  - 从服务器向主服务器发送PSYNC命令
  - 收到PSYNC命令的主服务器执行BGSAVE命令，在后台**生成一个RDB文件**。并用一个**缓冲区**来记录从现在开始执行的所有**写命令**。
  - 当主服务器的BGSAVE命令执行完后，将生成的RDB文件发送给从服务器，**从服务器接收和载入RBD文件**。将自己的数据库状态更新至与主服务器执行BGSAVE命令时的状态。
  - 主服务器将所有缓冲区的**写命令发送给从服务器**，从服务器执行这些写命令，达到数据最终一致性。

- **部分重同步**
  - 主从服务器的**复制偏移量**
  - 主服务器的**复制积压缓冲区**
  - 服务器运行的ID(**run ID**)

**复制偏移量：**执行复制的双方都会**分别维护**一个复制偏移量（**对比主从复制的偏移量**）

- 主服务器每次传播N个字节，就将自己的复制偏移量加上N
- 从服务器每次收到主服务器的N个字节，就将自己的复制偏移量加上N

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(21).webp)

那断线重连以后，从服务器向主服务器发送PSYNC命令，报告现在的偏移量是36，那么主服务器该对从服务器执行完整重同步还是部分重同步呢？？这就交由**复制积压缓冲区**来决定。

当主服务器进行命令传播时，不仅仅会将写命令发送给所有的从服务器，还会将写命令**入队到复制积压缓冲区**里面(这个大小可以调的)。如果复制积压缓冲区**存在**丢失的偏移量的数据，那就执行部分重同步，否则执行完整重同步。

服务器运行的ID(**run ID**)实际上就是用来比对ID是否相同。如果不相同，则说明从服务器断线之前复制的主服务器和当前连接的主服务器是两台服务器，这就会进行完整重同步。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/64022.webp)

#### 命令传播

> 当完成了同步之后，主从服务器就会进入命令传播阶段。这时主服务器只要将自己的写命令发送给从服务器，而从服务器接收并执行主服务器发送过来的写命令，就可以保证主从服务器一直保持数据一致了！

在命令传播阶段，从服务器默认会以每秒一次的频率，向服务器发送命令`REPLCONF ACK <replication_offset>` 其中replication_offset是从服务器当前的复制偏移量

发送这个命令主要有三个作用：

- **检测主从服务器的网络状态**
- 辅助实现min-slaves选项
- 检测命令丢失



## 哨兵机制

> 如果主服务器挂了，我们可以将从服务器**升级**为主服务器，等到旧的主服务器(挂掉的那个)重连上来，会将它(挂掉的主服务器)变成从服务器。

哨兵(Sentinel)机制主要用于实现Redis的**高可用性**，主要的功能如下：

- Sentinel**不停地监控**Redis主从服务器是否正常工作
- 如果某个Redis实例有故障，那么哨兵负责**发送消息通知**管理员
- 如果主服务器挂掉了，会**自动**将从服务器提升为主服务器(包括配置都会修改)。
- Sentinel可以作为**配置中心**，能够提供当前主服务器的信息。

### Sentinel

- Sentinel本质上只是一个**运行在特殊模式下的Redis服务器**。
- Sentinel在初始化的时候并**不会载入AOF/RDB文件**，因为Sentinel根本就不用数据库
- 然后，在启动的时候会将普通Redis服务器的代码替换成**Sentinel专用代码**。(所以Sentinel虽然作为Redis服务器，但是它**不能执行S**ET、DBSIZE等等命令，因为命令表的代码被替换了)
- 初始化Sentinel的状态，并根据给定的配置文件**初始化**Sentinel监视的**主服务器列表**。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(23).webp)

- 最后，Sentinel会创建两个**连向主服务器的网络连接**：
  - 命令连接(发送和接收命令)
  - 订阅连接(订阅主服务器的`sentinel:hello`频道)

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(9).jpg)

- Sentinel通过**主服务器**发送**INFO命令**来获得主服务器属下**所有从服务器**的地址信息，并为这些从服务器创建相应的实例结构。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(24).webp)

- 当发现有**新的从服务器出现时**，除了创建对应的从服务器实例结构，Sentinel还会创建命令连接和订阅连接。

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/640(25).webp)

在Sentinel运行的过程中，通过命令连接会以每两秒一次的频率向**监视的主从服务器**的`_sentinel_:hello频道`发送命令(主要发送Sentinel本身的信息，监听主从服务器的信息)，并通过订阅连接接收`_sentinel_:hello频道`的信息。

- 这样一来一回，我们就可以**更新每个Sentinel实例结构的信息**。



### 判断下线

> 在多少毫秒内无效回复才认定主服务器是主观下线的，以及多少个Sentinel认为主服务器是下线的，才认定为客观下线。这都是**可以配置**的

- **主观下线**

  - Sentinel会以每秒一次的频率向与它创建命令连接的实例(包括主从服务器和其他的Sentinel)**发送PING命令**，通过PING命令返回的信息判断实例是否在线
  - 如果一个**主服务器**在down-after-milliseconds毫秒内连续向Sentinel发送**无效回复**，那么当前Sentinel就会**主观认为**该主服务器已经下线了。

- **客观下线**

  - 当Sentinel将一个主服务器判断为主观下线以后，为了确认该主服务器是否真的下线，它会向同样监视该主服务器的Sentinel**询问**，看它们是否也认为该主服务器是否下线。
  - 如果**足够多**的Sentinel认为该主服务器是下线的，那么就判定该主服务为客观下线，并对主服务器执行故障转移操作。



### 选举头Sentinel和故障转移

当一个主服务器认为为客观下线以后，监视这个下线的主服务器的各种Sentinel会进行协商，**选举出一个领头的Sentinel**，领头的Sentinel会对下线的主服务器执行故障转移操作。

> 选举领头Sentinel的规则也比较多，总的来说就是**先到先得**(哪个快，就选哪个)

- 在已下线主服务器**属下的从服务器中**，挑选一个转换为主服务器
- 让已下线主服务器属下的所有从服务器改为**复制新的主服务器**
- 已下线的主服务器**重新连接时**，让他成为新的主服务器的从服务器

挑选某一个从服务器作为主服务器也是有**策略**的，大概如下：

- 跟master断开连接的时长
- slave优先级
- 复制offset
- run id



## 面试问题








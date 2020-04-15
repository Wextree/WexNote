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





# 持久化

## RDB

- **触发机制：**
  - save（同步）
  - bgsave（异步）（fork）
  - 自动（配置实现）



## AOF

- **写入策略：**
  - always
  - everysec
  - no
- **AOF重写:**
  - bgrewriteaof
  - 重写配置



# fork（同步）

- 与内存量息息相关，内存越大，耗时越大
- info：latest_fork_usec，查执行时间



# 主从复制

## 命令

- **slaveof socket：**复制主数据库（异步） 
- **slaveof no one：**取消复制，但不会删除自身已有的数据


































# ZooKeeper

> 参考文档：https://github.com/ZhongFuCheng3y/3y
>
> 1. ZooKeeper也可以作为**注册中心**
> 2. ZooKeeper可以作为**分布式锁**的一种实现
> 3. Kafka使用ZooKeeper**管理自己的元数据配置**。



## 一、 ZooKeeper的定义

- ZooKeeper主要服务于**分布式系统**，可以用ZooKeeper来做：
  - 统一配置管理
  - 统一命名服务
  - 分布式锁
  - 集群管理
- 使用分布式系统就无法避免对节点的管理问题（需要实时感知节点的状态，对节点进行统一管理等），而由于这些问题处理起来可能相对麻烦和提高了系统的复杂性，而ZooKeeper可以通用解决这些问题。



## 二、ZooKeeper的特点

> ZooKeeper和Redis一样，也是C/S结构(分成客户端和服务端)

ZooKeeper的数据结构，跟**Unix文件系统**非常类似，可以看做是一颗**树**，每个节点叫做**ZNode**。每一个节点可以通过**路径**来标识，结构图如下：

<img src="..\imgs\ZooKeeper结构.png" style="zoom:200%;" />



ZooKeeper的节点我们称之为**Znode**，Znode分为**两种**类型：

- **短暂/临时(Ephemeral)**：当客户端和服务端断开连接后，所创建的Znode(节点)**会自动删除**
- **持久(Persistent)**：当客户端和服务端断开连接后，所创建的Znode(节点)**不会删除**



### 2.1 监听器

常见的**监听事件**：

- 监听Znode节点的数据变化
- 监听Znode节点的增减变化




























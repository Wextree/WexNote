# AQS

> 参考文档：https://segmentfault.com/a/1190000017372067

## Lock

### Lock API

Lock是一个接口，方法定义如下：

```java
void lock() // 如果锁可用就获得锁，如果锁不可用就阻塞直到锁释放
void lockInterruptibly() // 和 lock()方法相似, 但阻塞的线程可中断，抛出 java.lang.InterruptedException异常
boolean tryLock() // 非阻塞获取锁;尝试获取锁，如果成功返回true
boolean tryLock(long timeout, TimeUnit timeUnit) //带有超时时间的获取锁方法
void unlock() // 释放锁
```



### Lock的实现

实现Lock接口的类有很多，以下为几个常见的锁实现

- ReentrantLock：表示重入锁，它是唯一一个实现了Lock接口的类。重入锁指的是线程在获得锁之后，再次获取该锁不需要阻塞，而是直接关联一次计数器增加重入次数
- ReentrantReadWriteLock：重入读写锁，它实现了ReadWriteLock接口，在这个类中维护了两个锁，一个是ReadLock，一个是WriteLock，他们都分别实现了Lock接口。读写锁是一种适合读多写少的场景下解决线程安全问题的工具，基本原则是：`读和读不互斥、读和写互斥、写和写互斥`。也就是说涉及到影响数据变化的操作都会存在互斥。
- StampedLock： stampedLock是JDK8引入的新的锁机制，可以简单认为是读写锁的一个改进版本，读写锁虽然通过分离读和写的功能使得读和读之间可以完全并发，但是读和写是有冲突的，如果大量的读线程存在，可能会引起写线程的饥饿。stampedLock是一种乐观的读策略，使得乐观锁完全不会阻塞写线程



### ReentrantLock的简单使用

```java
public class Demo {
    private static int count=0;
    static Lock lock=new ReentrantLock();
    public static void inc(){
        lock.lock();
        try {
            Thread.sleep(1);
            count++;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally{
            lock.unlock();
        }
    }
```

这段代码主要做一件事，就是通过一个静态的`incr()`方法对共享变量`count`做连续递增，在没有加同步锁的情况下多线程访问这个方法一定会存在线程安全问题。所以用到了`ReentrantLock`来实现同步锁，并且在finally语句块中释放锁。

**多个线程通过lock竞争锁时，当竞争失败的锁是如何实现等待以及被唤醒的呢?**



## AQS

aqs全称为AbstractQueuedSynchronizer，它提供了一个FIFO队列，可以看成是一个用来实现同步锁以及其他涉及到同步功能的核心组件，常见的有:**ReentrantLock**、**CountDownLatch**等。

AQS是一个**抽象类**，主要是通过继承的方式来使用，它本身没有实现任何的同步接口，仅仅是定义了**同步状态的获取**以及**释放的方法**来提供自定义的同步组件。



### AQS的两种功能

从使用层面来说，AQS的功能分为两种：独占和共享

- 独占锁，每次只能有一个线程持有锁，比如前面给大家演示的ReentrantLock就是以独占方式实现的互斥锁
- 共享锁，允许多个线程同时获取锁，并发访问共享资源，比如ReentrantReadWriteLock

### ReentrantLock的类图

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/ReentrantLock%E7%9A%84%E7%B1%BB%E5%9B%BE.jpg)



### AQS的内部实现

AQS的实现依赖内部的同步队列,也就是FIFO的双向队列，如果当前线程竞争锁失败，那么AQS会把当前线程以及等待状态信息构造成一个Node加入到同步队列中，同时再阻塞该线程。当获取锁的线程释放锁以后，会从队列中唤醒一个阻塞的节点(线程)。

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/AQS%E9%98%9F%E5%88%97.png)

> AQS队列内部维护的是一个FIFO的双向链表，这种结构的特点是每个数据结构都有两个指针，分别指向直接的后继节点和直接前驱节点。所以双向链表可以从任意一个节点开始很方便的访问前驱和后继。每个Node其实是由线程封装，当线程争抢锁失败后会封装成Node加入到AQS队列中去

```java
static final class Node {
        static final Node SHARED = new Node();
        static final Node EXCLUSIVE = null;
        static final int CANCELLED =  1;
        static final int SIGNAL    = -1;
        static final int CONDITION = -2;
        static final int PROPAGATE = -3;
        volatile int waitStatus;
        volatile Node prev; //前驱节点
        volatile Node next; //后继节点
        volatile Thread thread;//当前线程
        Node nextWaiter; //存储在condition队列中的后继节点
        //是否为共享锁
        final boolean isShared() { 
            return nextWaiter == SHARED;
        }

        final Node predecessor() throws NullPointerException {
            Node p = prev;
            if (p == null)
                throw new NullPointerException();
            else
                return p;
        }

        Node() {    // Used to establish initial head or SHARED marker
        }
        //将线程构造成一个Node，添加到等待队列
        Node(Thread thread, Node mode) {     // Used by addWaiter
            this.nextWaiter = mode;
            this.thread = thread;
        }
        //这个方法会在Condition队列使用
        Node(Thread thread, int waitStatus) { // Used by Condition
            this.waitStatus = waitStatus;
            this.thread = thread;
        }
    }
```



### 释放锁以及添加线程对于队列的变化

#### 添加节点

当出现锁竞争以及释放锁的时候，AQS同步队列中的节点会发生变化，首先看一下添加节点的场景。

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/CAS%E8%AE%BE%E7%BD%AE%E5%B0%BE%E7%BB%93%E7%82%B9.png)

这里会涉及到两个变化

- 新的线程封装成Node节点追加到同步队列中，设置prev节点以及修改当前节点的前置节点的next节点指向自己
- 通过**CAS**将tail重新指向新的尾部节点

#### 释放锁移除节点

head节点表示获取锁成功的节点，当头结点在释放同步状态时，会唤醒后继节点，如果后继节点获得锁成功，会把自己设置为头结点，节点的变化过程如下

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/%E8%AE%BE%E7%BD%AE%E5%A4%B4%E7%BB%93%E7%82%B9.png)

这个过程也是涉及到两个变化

- 修改head节点指向下一个获得锁的节点
- 新的获得锁的节点，将prev的指针指向null

这里有一个小的变化，就是设置head节点不需要用CAS，原因是设置head节点是由获得锁的线程来完成的，而同步锁只能由一个线程获得，所以不需要CAS保证，只需要把head节点设置为原首节点的后继节点，并且断开原head节点的next引用即可



## AQS源码分析

> 仍然以ReentrantLock为模型。



### ReentrantLock的时序图

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/4225553403-5c12887528099_articlex.jpg)

从图上可以看出来，当锁获取失败时，会调用addWaiter()方法将当前线程封装成Node节点加入到AQS队列，基于这个思路，我们来分析AQS的源码实现

### 分析源码

#### ReentrantLock.lock()

```java
public void lock() {
    // abstract static class Sync extends AbstractQueuedSynchronizer
    sync.lock();
}
```

sync是一个静态内部类，它继承了AQS这个抽象类，前面说过AQS是一个同步工具，主要用来实现同步控制。我们在利用这个工具的时候，会继承它来实现同步控制功能。
通过进一步分析，发现Sync这个类有两个具体的实现，分别是`NofairSync(非公平锁)`,`FailSync(公平锁)`.

- 公平锁 表示所有线程严格按照FIFO来获取锁
- 非公平锁 表示可以存在抢占锁的功能，也就是说不管当前队列上是否存在其他线程等待，新线程都有机会抢占锁

> 接下来的分析仍然以`非公平锁`作为主要分析逻辑。



#### NonfairSync.lock

- 由于这里是非公平锁，所以调用lock方法时，先去通过cas去抢占锁
- 如果抢占锁成功，保存获得锁成功的当前线程
- 抢占锁失败，调用acquire来走锁竞争逻辑

```java
final void lock() {
    if (compareAndSetState(0, 1)) //通过cas操作来修改state状态，表示争抢锁的操作
      setExclusiveOwnerThread(Thread.currentThread());//设置当前获得锁状态的线程
    else
      acquire(1); //尝试去获取锁
}
```

**compareAndSetState的代码实现逻辑如下**:

```java
// 如果当前内存中的state的值和预期值expect相等，则替换为update。更新成功返回true，否则返回false.
return unsafe.compareAndSwapInt(this, stateOffset, expect, update);
```

> Unsafe类是在sun.misc包下，不属于Java标准。但是很多Java的基础类库，包括一些被广泛使用的高性能开发库都是基于Unsafe类开发的，比如Netty、Hadoop、Kafka等；Unsafe可认为是Java中留下的后门，提供了一些低层次操作，如直接内存访问、线程调度等

**state**：

- 当state=0时，表示无锁状态
- 当state>0时，表示已经有线程获得了锁，也就是state=1，但是因为ReentrantLock允许重入，所以同一个线程多次获得同步锁的时候，state会递增，比如重入5次，那么state=5。 而在释放锁的时候，同样需要释放5次直到state=0其他线程才有资格获得锁



#### acquire

```java
  public final void acquire(int arg) {
        if (!tryAcquire(arg) &&
            acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
            selfInterrupt();
    }
```

这个方法的主要逻辑是

- 通过tryAcquire尝试获取独占锁，如果成功返回true，失败返回false
- 如果tryAcquire失败，则会通过addWaiter方法将当前线程封装成Node添加到AQS队列尾部
- acquireQueued，将Node作为参数，通过自旋去尝试获取锁。






























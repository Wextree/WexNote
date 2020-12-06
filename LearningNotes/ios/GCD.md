# GCD

> 参考文档：https://www.jianshu.com/p/2d57c72016c6



## 1. 简介

### 1.1 百度百科

**Grand Central Dispatch（GCD）** 是 Apple 开发的一个多核编程的较新的解决方法。它主要用于优化应用程序以支持多核处理器以及其他对称多处理系统。它是一个在线程池模式的基础上执行的并发任务。在 Mac OS X 10.6 雪豹中首次推出，也可在 iOS 4 及以上版本使用。



### 1.2 优势

- GCD 可用于多核的并行运算；

- GCD 会自动利用更多的 CPU 内核（比如双核、四核）；

- GCD 会自动管理线程的生命周期（创建线程、调度任务、销毁线程）；

- 程序员只需要告诉 GCD 想要执行什么任务，不需要编写任何线程管理代码。



## 2. GCD任务和队列

### 2.1 任务

**任务**就是执行操作的意思，换句话说就是你在线程中执行的那段代码。**在 GCD 中是放在 block 中的**。

执行任务有**两种方式**：

- **同步执行**：等待队列的任务执行结束，不具备开启新线程的能力。
- **异步执行**：不用等待队列的任务执行结束，具备开启新线程的能力。

> 注意：**异步执行（async）**虽然具有开启新线程的能力，但是并不一定开启新线程。这跟任务所指定的队列类型有关。



### 2.2 队列

这里的队列指执行任务的等待队列，即用来存放任务的队列。队列是一种特殊的线性表，采用 FIFO（先进先出）的原则，即新任务总是被插入到队列的末尾，而读取任务的时候总是从队列的头部开始读取。每读取一个任务，则从队列中释放一个任务。

在 GCD 中有两种队列：

- **串行队列**：每次只有一个任务被执行，一个接着一个执行。（只开启一个线程）
- **并发队列**：可以让多个任务并发执行。（可以开启多个线程）

> 两者都符合 FIFO（先进先出）的原则。

> 注意：**并发队列** 的并发功能只有在异步（dispatch_async）方法下才有效。



## 3. GCD的使用

### 3.1 使用步骤

1. 创建一个队列（串行队列或并发队列）；
2. 将任务追加到任务的等待队列中，然后系统就会根据任务类型执行任务（同步执行或异步执行）。



### 3.2 队列的创建和获取方法

#### 队列的创建

可以使用 `dispatch_queue_create` 方法来创建队列。该方法需要传入两个参数：

- 第一个参数表示队列的唯一标识符，用于 DEBUG，可为空。队列的名称推荐使用应用程序 ID 这种逆序全程域名。

- 第二个参数用来识别是串行队列还是并发队列。`DISPATCH_QUEUE_SERIAL` 表示串行队列，`DISPATCH_QUEUE_CONCURRENT` 表示并发队列。

```objc
// 串行队列的创建方法
dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_SERIAL);
// 并发队列的创建方法
dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_CONCURRENT);
```



#### 队列的获取

对于**串行队列**，GCD 默认提供了：**主队列（Main Dispatch Queue）**。

- 所有放在主队列中的任务，都会放到主线程中执行。
- 可使用 `dispatch_get_main_queue()` 方法获得主队列。

> 注意：**主队列其实并不特殊。** 主队列的实质上就是一个普通的串行队列，只是因为默认情况下，当前代码是放在主队列中的，然后主队列中的代码，有都会放到主线程中去执行，所以才造成了主队列特殊的现象。

```objc
// 主队列的获取方法
dispatch_queue_t queue = dispatch_get_main_queue();
```



对于**并发队列**，GCD 默认提供了 **全局并发队列（Global Dispatch Queue）**。

- 可以使用 `dispatch_get_global_queue` 方法来获取全局并发队列。需要传入两个参数。第一个参数表示队列优先级，一般用 `DISPATCH_QUEUE_PRIORITY_DEFAULT`。第二个参数暂时没用，用 `0` 即可。

```objc
// 全局并发队列的获取方法
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
```



### 3.3 任务的创建

**GCD** 提供了同步执行任务的创建方法 `dispatch_sync` 和异步执行任务创建方法 `dispatch_async`。

```objc
// 同步执行任务创建方法
dispatch_sync(queue, ^{
    // 这里放同步执行任务代码
});
// 异步执行任务创建方法
dispatch_async(queue, ^{
    // 这里放异步执行任务代码
});
```



虽然使用 GCD 只需两步，但是既然我们有两种队列（串行队列 / 并发队列），两种任务执行方式（同步执行 / 异步执行），那么我们就有了四种不同的组合方式。这**四种不同的组合方式**是：

1. 同步执行 + 并发队列
2. 异步执行 + 并发队列
3. 同步执行 + 串行队列
4. 异步执行 + 串行队列

实际上，刚才还说了两种默认队列：全局并发队列、主队列。**全局并发队列可以作为普通并发队列来使用。**但是当前代码默认放在主队列中，所以主队列很有必要专门来研究一下，所以我们就又多了两种组合方式。这样就有六种不同的组合方式了。

5. 同步执行 + 主队列
6. 异步执行 + 主队列



### 3.4 主线程中任务和队列不同组合

当前线程为 **主线程** 的环境下，**不同队列+不同任务** 简单组合使用的不同区别。暂时不考虑 **队列中嵌套队列** 的这种复杂情况。

|     区别      |           并发队列           |             串行队列              |            主队列            |
| :-----------: | :--------------------------: | :-------------------------------: | :--------------------------: |
| 同步（sync）  | 没有开启新线程，串行执行任务 |   没有开启新线程，串行执行任务    |        死锁卡住不执行        |
| 异步（async） |  有开启新线程，并发执行任务  | 有开启新线程（1条），串行执行任务 | 没有开启新线程，串行执行任务 |

>注意：从上边可看出： **『主线程』** 中调用 **『主队列』+『同步执行』** 会导致死锁问题。
 这是因为 **主队列中追加的同步任务** 和 **主线程本身的任务** 两者之间相互等待，阻塞了 **『主队列』**，最终造成了主队列所在的线程（主线程）死锁问题。
 而如果我们在 **『其他线程』** 调用 **『主队列』+『同步执行』**，则不会阻塞 **『主队列』**，自然也不会造成死锁问题。最终的结果是：**不会开启新线程，串行执行任务**。





### 3.5 队列嵌套

实际在使用『串行队列』的时候，也可能出现阻塞『串行队列』所在线程的情况发生，从而造成死锁问题。这种情况多见于同一个串行队列的嵌套使用。

```objc
// 在异步执行+串行队列的任务中，又嵌套了当前的串行队列，然后进行同步执行。
dispatch_queue_t queue = dispatch_queue_create("test.queue", DISPATCH_QUEUE_SERIAL);
dispatch_async(queue, ^{    // 异步执行 + 串行队列
    dispatch_sync(queue, ^{  // 同步执行 + 当前串行队列
        // 追加任务 1
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"1---%@",[NSThread currentThread]);      // 打印当前线程
    });
});
```

**不同队列+不同任务** 组合，以及 **队列中嵌套队列** 使用的区别：

|     区别      | 『异步执行+并发队列』嵌套『同一个并发队列』 | 『同步执行+并发队列』嵌套『同一个并发队列』 | 『异步执行+串行队列』嵌套『同一个串行队列』 | 『同步执行+串行队列』嵌套『同一个串行队列』 |
| :-----------: | :-----------------------------------------: | :-----------------------------------------: | :-----------------------------------------: | :-----------------------------------------: |
| 同步（sync）  |       没有开启新的线程，串行执行任务        |        没有开启新线程，串行执行任务         |               死锁卡住不执行                |               死锁卡住不执行                |
| 异步（async） |         有开启新线程，并发执行任务          |         有开启新线程，并发执行任务          |     有开启新线程（1 条），串行执行任务      |     有开启新线程（1 条），串行执行任务      |



### 3.6 形象理解

假设现在有 5 个人要穿过一道门禁，这道门禁总共有 10 个入口，管理员可以决定同一时间打开几个入口，可以决定同一时间让一个人单独通过还是多个人一起通过。不过默认情况下，管理员只开启一个入口，且一个通道一次只能通过一个人。

- 这个故事里，人好比是 **任务**，管理员好比是 **系统**，入口则代表 **线程**。
  - 5 个人表示有 5 个任务，10 个入口代表 10 条线程。
  - **串行队列** 好比是 5 个人排成一支长队。
  - **并发队列** 好比是 5 个人排成多支队伍，比如 2 队，或者 3 队。
  - **同步任务** 好比是管理员只开启了一个入口（当前线程）。
  - **异步任务** 好比是管理员同时开启了多个入口（当前线程 + 新开的线程）。
- **『异步执行 + 并发队列』** 可以理解为：现在管理员开启了多个入口（比如 3 个入口），5 个人排成了多支队伍（比如 3 支队伍），这样这 5 个人就可以 3 个人同时一起穿过门禁了。
- **『同步执行 + 并发队列』** 可以理解为：现在管理员只开启了 1 个入口，5  个人排成了多支队伍。虽然这 5 个人排成了多支队伍，但是只开了 1 个入口啊，这 5 个人虽然都想快点过去，但是 1 个入口一次只能过 1 个人，所以大家就只好一个接一个走过去了，表现的结果就是：顺次通过入口。
- 换成 GCD 里的语言就是说：
  - 『异步执行 + 并发队列』就是：系统开启了多个线程（主线程+其他子线程），任务可以多个同时运行。
  - 『同步执行 + 并发队列』就是：系统只默认开启了一个主线程，没有开启子线程，虽然任务处于并发队列中，但也只能一个接一个执行了。



## 4. 六种组合方式详解

### 4.1 同步执行 + 并发队列

- 在当前线程中执行任务，不会开启新线程，执行完一个任务，再执行下一个任务。

```objc
/**
 * 同步执行 + 并发队列
 * 特点：在当前线程中执行任务，不会开启新线程，执行完一个任务，再执行下一个任务。
 */
- (void)syncConcurrent {
    NSLog(@"currentThread---%@",[NSThread currentThread]);  // 打印当前线程
    NSLog(@"syncConcurrent---begin");
    
    dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_CONCURRENT);
    
    dispatch_sync(queue, ^{
        // 追加任务 1
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"1---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    dispatch_sync(queue, ^{
        // 追加任务 2
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"2---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    dispatch_sync(queue, ^{
        // 追加任务 3
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"3---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    NSLog(@"syncConcurrent---end");
}
```

>输出结果：
 2019-08-08 14:32:53.542816+0800 YSC-GCD-demo[16332:4171500] currentThread---<NSThread: 0x600002326940>{number = 1, name = main}
 2019-08-08 14:32:53.542964+0800 YSC-GCD-demo[16332:4171500] syncConcurrent---begin
 2019-08-08 14:32:55.544329+0800 YSC-GCD-demo[16332:4171500] 1---<NSThread: 0x600002326940>{number = 1, name = main}
 2019-08-08 14:32:57.545779+0800 YSC-GCD-demo[16332:4171500] 2---<NSThread: 0x600002326940>{number = 1, name = main}
 2019-08-08 14:32:59.547154+0800 YSC-GCD-demo[16332:4171500] 3---<NSThread: 0x600002326940>{number = 1, name = main}
 2019-08-08 14:32:59.547365+0800 YSC-GCD-demo[16332:4171500] syncConcurrent---end

从 `同步执行 + 并发队列` 中可看到：

- 所有任务都是在当前线程（主线程）中执行的，没有开启新的线程（`同步执行`不具备开启新线程的能力）。
- 所有任务都在打印的 `syncConcurrent---begin` 和 `syncConcurrent---end` 之间执行的（`同步任务` 需要等待队列的任务执行结束）。
- 任务按顺序执行的。按顺序执行的原因：虽然 `并发队列` 可以开启多个线程，并且同时执行多个任务。但是因为本身不能创建新线程，只有当前线程这一个线程（`同步任务` 不具备开启新线程的能力），所以也就不存在并发。而且当前线程只有等待当前队列中正在执行的任务执行完毕之后，才能继续接着执行下面的操作（`同步任务` 需要等待队列的任务执行结束）。所以任务只能一个接一个按顺序执行，不能同时被执行。



### 4.2 异步执行 + 并发队列

- 可以开启多个线程，任务交替（同时）执行。

```objc
/**
 * 异步执行 + 并发队列
 * 特点：可以开启多个线程，任务交替（同时）执行。
 */
- (void)asyncConcurrent {
    NSLog(@"currentThread---%@",[NSThread currentThread]);  // 打印当前线程
    NSLog(@"asyncConcurrent---begin");
    
    dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_CONCURRENT);
    
    dispatch_async(queue, ^{
        // 追加任务 1
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"1---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    dispatch_async(queue, ^{
        // 追加任务 2
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"2---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    dispatch_async(queue, ^{
        // 追加任务 3
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"3---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    NSLog(@"asyncConcurrent---end");
}
```

> 输出结果：
>  2019-08-08 14:36:37.747966+0800 YSC-GCD-demo[17232:4187114] currentThread---<NSThread: 0x60000206d380>{number = 1, name = main}
>  2019-08-08 14:36:37.748150+0800 YSC-GCD-demo[17232:4187114] asyncConcurrent---begin
>  2019-08-08 14:36:37.748279+0800 YSC-GCD-demo[17232:4187114] asyncConcurrent---end
>  2019-08-08 14:36:39.752523+0800 YSC-GCD-demo[17232:4187204] 2---<NSThread: 0x600002010980>{number = 3, name = (null)}
>  2019-08-08 14:36:39.752527+0800 YSC-GCD-demo[17232:4187202] 3---<NSThread: 0x600002018480>{number = 5, name = (null)}
>  2019-08-08 14:36:39.752527+0800 YSC-GCD-demo[17232:4187203] 1---<NSThread: 0x600002023400>{number = 4, name = (null)}

在 `异步执行 + 并发队列` 中可以看出：

- 除了当前线程（主线程），系统又开启了 3 个线程，并且任务是交替/同时执行的。（`异步执行` 具备开启新线程的能力。且 `并发队列` 可开启多个线程，同时执行多个任务）。
- 所有任务是在打印的 `syncConcurrent---begin` 和 `syncConcurrent---end` 之后才执行的。说明当前线程没有等待，而是直接开启了新线程，在新线程中执行任务（`异步执行` 不做等待，可以继续执行任务）。



### 4.3 同步执行 + 串行队列

- 不会开启新线程，在当前线程执行任务。任务是串行的，执行完一个任务，再执行下一个任务。

```objc
/**
 * 同步执行 + 串行队列
 * 特点：不会开启新线程，在当前线程执行任务。任务是串行的，执行完一个任务，再执行下一个任务。
 */
- (void)syncSerial {
    NSLog(@"currentThread---%@",[NSThread currentThread]);  // 打印当前线程
    NSLog(@"syncSerial---begin");
    
    dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_SERIAL);
    
    dispatch_sync(queue, ^{
        // 追加任务 1
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"1---%@",[NSThread currentThread]);      // 打印当前线程
    });
    dispatch_sync(queue, ^{
        // 追加任务 2
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"2---%@",[NSThread currentThread]);      // 打印当前线程
    });
    dispatch_sync(queue, ^{
        // 追加任务 3
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"3---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    NSLog(@"syncSerial---end");
}
```

>输出结果为：
 2019-08-08 14:39:31.366815+0800 YSC-GCD-demo[17285:4197645] currentThread---<NSThread: 0x600001b5e940>{number = 1, name = main}
 2019-08-08 14:39:31.366952+0800 YSC-GCD-demo[17285:4197645] syncSerial---begin
 2019-08-08 14:39:33.368256+0800 YSC-GCD-demo[17285:4197645] 1---<NSThread: 0x600001b5e940>{number = 1, name = main}
 2019-08-08 14:39:35.369661+0800 YSC-GCD-demo[17285:4197645] 2---<NSThread: 0x600001b5e940>{number = 1, name = main}
 2019-08-08 14:39:37.370991+0800 YSC-GCD-demo[17285:4197645] 3---<NSThread: 0x600001b5e940>{number = 1, name = main}
 2019-08-08 14:39:37.371192+0800 YSC-GCD-demo[17285:4197645] syncSerial---end

在 `同步执行 + 串行队列` 可以看到：

- 所有任务都是在当前线程（主线程）中执行的，并没有开启新的线程（`同步执行` 不具备开启新线程的能力）。
- 所有任务都在打印的 `syncConcurrent---begin` 和 `syncConcurrent---end` 之间执行（`同步任务` 需要等待队列的任务执行结束）。
- 任务是按顺序执行的（`串行队列` 每次只有一个任务被执行，任务一个接一个按顺序执行）。



### 4.4 异步执行 + 串行队列

- 会开启新线程，但是因为任务是串行的，执行完一个任务，再执行下一个任务

```objc
/**
 * 异步执行 + 串行队列
 * 特点：会开启新线程，但是因为任务是串行的，执行完一个任务，再执行下一个任务。
 */
- (void)asyncSerial {
    NSLog(@"currentThread---%@",[NSThread currentThread]);  // 打印当前线程
    NSLog(@"asyncSerial---begin");
    
    dispatch_queue_t queue = dispatch_queue_create("net.bujige.testQueue", DISPATCH_QUEUE_SERIAL);
    
    dispatch_async(queue, ^{
        // 追加任务 1
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"1---%@",[NSThread currentThread]);      // 打印当前线程
    });
    dispatch_async(queue, ^{
        // 追加任务 2
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"2---%@",[NSThread currentThread]);      // 打印当前线程
    });
    dispatch_async(queue, ^{
        // 追加任务 3
        [NSThread sleepForTimeInterval:2];              // 模拟耗时操作
        NSLog(@"3---%@",[NSThread currentThread]);      // 打印当前线程
    });
    
    NSLog(@"asyncSerial---end");
}
```

>输出结果为：
 2019-08-08 14:40:53.944502+0800 YSC-GCD-demo[17313:4203018] currentThread---<NSThread: 0x6000015da940>{number = 1, name = main}
 2019-08-08 14:40:53.944615+0800 YSC-GCD-demo[17313:4203018] asyncSerial---begin
 2019-08-08 14:40:53.944710+0800 YSC-GCD-demo[17313:4203018] asyncSerial---end
 2019-08-08 14:40:55.947709+0800 YSC-GCD-demo[17313:4203079] 1---<NSThread: 0x6000015a0840>{number = 3, name = (null)}
 2019-08-08 14:40:57.952453+0800 YSC-GCD-demo[17313:4203079] 2---<NSThread: 0x6000015a0840>{number = 3, name = (null)}
 2019-08-08 14:40:59.952943+0800 YSC-GCD-demo[17313:4203079] 3---<NSThread: 0x6000015a0840>{number = 3, name = (null)}

在 `异步执行 + 串行队列` 可以看到：

- 开启了一条新线程（`异步执行` 具备开启新线程的能力，`串行队列` 只开启一个线程）。
- 所有任务是在打印的 `syncConcurrent---begin` 和 `syncConcurrent---end` 之后才开始执行的（`异步执行` 不会做任何等待，可以继续执行任务）。
- 任务是按顺序执行的（`串行队列` 每次只有一个任务被执行，任务一个接一个按顺序执行）。








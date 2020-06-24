# Quartz

## 初识

Quartz 是一个完全由 Java 编写的开源作业调度框架，为在 Java 应用程序中进行作业调度提供了简单却强大的机制。

Quartz 可以与[ J2EE](https://www.w3cschool.cn/java_interview_question/java_interview_question-wvr326ra.html) 与 J2SE 应用程序相结合也可以单独使用。

Quartz 允许程序开发人员根据时间的间隔来调度作业。

Quartz 实现了作业和触发器的多对多的关系，还能把多个作业与不同的触发器关联。



### 核心概念

1. Job 

   表示一个工作，要执行的具体内容。此接口中只有一个方法，如下：

   ```
   void execute(JobExecutionContext context) 
   ```

2. **JobDetail** 表示一个具体的可执行的调度程序，Job 是这个可执行程调度程序所要执行的内容，另外 JobDetail 还包含了这个任务调度的方案和策略。

3. **Trigger** 代表一个调度参数的配置，什么时候去调。

4. **Scheduler** 代表一个调度容器，一个调度容器中可以注册多个 JobDetail 和 Trigger。当 Trigger 与 JobDetail 组合，就可以被 Scheduler 容器调度了。



### 运行环境

- Quartz 可以运行嵌入在另一个独立式应用程序。
- Quartz 可以在应用程序服务器(或 [servlet 容器](https://www.w3cschool.cn/spring_mvc_documentation_linesh_translation/spring_mvc_documentation_linesh_translation-it8y27sq.html))内被实例化，并且参与 XA 事务。
- Quartz 可以作为一个独立的程序运行(其自己的 Java 虚拟机内)，可以通过 RMI 使用。
- Quartz 可以被实例化，作为独立的项目集群(负载平衡和故障转移功能)，用于作业的执行。




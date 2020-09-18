# 注解开发

## 一、准备

**设置配置类，将其他类注册在容器中：**

```java
@Configuration
// 扫描对应的包
@ComponentScan(value = "com.wex")
public class MainConfiguration {

    @Bean
    // 默认方法名
    public Person person(){
        return new Person(1, "wex", new Date());
    }
}
```



**利用分层方法多建几个类，放进去IoC容器中：**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/Snipaste_2020-07-17_20-58-16.png)



**写一个测试类查看当前的注册的类：**

```java
public class IoCTest {

    @Test
    public void test01(){
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(MainConfiguration.class);
        String[] definitionNames = applicationContext.getBeanDefinitionNames();
        for (String name: definitionNames) {
            System.out.println(name);
        }
    }
}
```

```log
org.springframework.context.annotation.internalConfigurationAnnotationProcessor
org.springframework.context.annotation.internalAutowiredAnnotationProcessor
org.springframework.context.annotation.internalCommonAnnotationProcessor
org.springframework.context.event.internalEventListenerProcessor
org.springframework.context.event.internalEventListenerFactory
mainConfiguration
bookController
bookDao
bookService
person
```



## 二、ComponentScan

### 1. 排除扫描的类

```java
@Configuration
// 扫描对应的包
@ComponentScan(value = "com.wex", excludeFilters = {
        // 以注解方式排除，排除Controller注解的类
        @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = {Controller.class})
})
public class MainConfiguration {

    @Bean
    // 默认方法名
    public Person person(){
        return new Person(1, "wex", new Date());
    }
}
```

```verilog
org.springframework.context.annotation.internalConfigurationAnnotationProcessor
org.springframework.context.annotation.internalAutowiredAnnotationProcessor
org.springframework.context.annotation.internalCommonAnnotationProcessor
org.springframework.context.event.internalEventListenerProcessor
org.springframework.context.event.internalEventListenerFactory
mainConfiguration
bookDao
bookService
person
```



### 2. 只包含某些类

> 但是要取消默认扫描，不然没有效果，因为默认规则是全部扫描

```java
@Configuration
// 扫描对应的包
@ComponentScan(value = "com.wex", includeFilters = {
        // 以注解方式排除，排除Controller注解的类
        @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = {Controller.class})
}, useDefaultFilters = false)
public class MainConfiguration {

    @Bean
    // 默认方法名
    public Person person(){
        return new Person(1, "wex", new Date());
    }
}
```

```verilog
org.springframework.context.annotation.internalConfigurationAnnotationProcessor
org.springframework.context.annotation.internalAutowiredAnnotationProcessor
org.springframework.context.annotation.internalCommonAnnotationProcessor
org.springframework.context.event.internalEventListenerProcessor
org.springframework.context.event.internalEventListenerFactory
mainConfiguration
bookController
person
```



### 3. 多个规则

```java
@ComponentScans(value = {
    @ComponentScan
    @ComponentScan
    @ComponentScan
    ...
})

// 或者直接设置多个@ComponentScan，高版本jdk可以使用
@ComponentScan
@ComponentScan
@ComponentScan
...
```



## 三、FilterType

```java
// 以注解方式排除，排除Controller注解的类
@ComponentScan.Filter(type = FilterType.ANNOTATION, classes = {Controller.class}),
// 直接指定类型
@ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE, classes = {BookService.class}),
// ASPECTJ表达式和正则表达式
@ComponentScan.Filter(type = FilterType.ASPECTJ),
@ComponentScan.Filter(type = FilterType.REGEX)
```



### 自定义FilterType

```java
// 实现对应接口
public class MyTypeFilter implements TypeFilter {
    @Override
    /**
     * metadataReader: 扫描到的当前类的信息
     * metadataReaderFactory: 可以探索其他类的信息
     * return: 是否匹配
     */
    public boolean match(MetadataReader metadataReader, MetadataReaderFactory metadataReaderFactory) throws IOException {
        // 获取当前类的注解的信息
        AnnotationMetadata annotationMetadata = metadataReader.getAnnotationMetadata();
        // 获取当前正在扫描的类的类信息
        ClassMetadata classMetadata = metadataReader.getClassMetadata();
        // 获取当前类的资源信息
        Resource resource = metadataReader.getResource();

        String name = classMetadata.getClassName();
        System.out.println("------>" + name);
        // 如果类名中包含er，那么通过
        if (name.contains("er"))
            return true;
        return false;
    }
}
```

**配置类：**

```java
@Configuration
// 扫描对应的包
@ComponentScan(value = "com.wex", includeFilters = {
        @ComponentScan.Filter(type = FilterType.CUSTOM, classes = {MyTypeFilter.class})
}, useDefaultFilters = false)
public class MainConfiguration {
    ...
}
```

```verilog
------>com.wex.IoCTest
------>com.wex.controller.BookController
------>com.wex.dao.BookDao
------>com.wex.domain.Person
------>com.wex.filter.MyTypeFilter
------>com.wex.service.BookService
org.springframework.context.annotation.internalConfigurationAnnotationProcessor
org.springframework.context.annotation.internalAutowiredAnnotationProcessor
org.springframework.context.annotation.internalCommonAnnotationProcessor
org.springframework.context.event.internalEventListenerProcessor
org.springframework.context.event.internalEventListenerFactory
mainConfiguration
bookController
person
myTypeFilter
bookService
```



## 四、Conditional

> 可以按照条件进行注册bean

首先我们注册两个新的bean，然后加上Conditional注解来判断是否是windows或者linux系统：

```java
@Configuration
public class MainConfiguration1 {


    @Bean
    public Person person(){
        return new Person(1, "wex", new Date());
    }

    @Conditional({WindowsCondition.class})
    @Bean("wu")
    public Person person01(){
        return new Person(2, "wu", new Date());
    }

    @Conditional({LinuxCondition.class})
    @Bean("huang")
    public Person person02(){
        return new Person(3, "huang", new Date());
    }
}
```

```java
public class WindowsCondition implements Condition {
    @Override
    /**
     * conditionContext: 判断条件能使用的上下文
     * annotatedTypeMetadata: 注释信息
     */
    public boolean matches(ConditionContext conditionContext, AnnotatedTypeMetadata annotatedTypeMetadata) {
        // 获取到ioc使用的Bean工厂
//        ConfigurableListableBeanFactory beanFactory = conditionContext.getBeanFactory();
        // 获取类加载器
//        ClassLoader classLoader = conditionContext.getClassLoader();
        // 获取到bean定义的注册类
//        BeanDefinitionRegistry registry = conditionContext.getRegistry();

        // 获取当前环境信息
        Environment environment = conditionContext.getEnvironment();
        String properties = environment.getProperty("os.name");
        if (properties.contains("Windows"))
            return true;

        return false;
    }
}

public class LinuxCondition implements Condition {
    @Override
    /**
     * conditionContext: 判断条件能使用的上下文
     * annotatedTypeMetadata: 注释信息
     */
    public boolean matches(ConditionContext conditionContext, AnnotatedTypeMetadata annotatedTypeMetadata) {
        Environment environment = conditionContext.getEnvironment();
        String properties = environment.getProperty("os.name");
        if (properties.contains("linux"))
            return true;

        return false;
    }
}
```

**输出所有注册的bean：（只有“wu”，没有“huang”）**

```verilog
mainConfiguration1
person
wu
```



## 五、注册组件

### 1. Import

**快速注册一个组件，名字默认是全限定类名：**

```java
@Configuration
@Import(Color.class)
public class MainConfiguration1 {}
```



### 2. ImportSelector

> 实现该接口可以配合@Import注解实现快速导入多个组件

```java
// 自定义返回要注册的组件
public class MyImportSelector implements ImportSelector {
    // 返回值就是要导入容器中的组件的全限定类名
    // annotationMetadata: 注解的所有信息
    @Override
    public String[] selectImports(AnnotationMetadata annotationMetadata) {
        return new String[] {"com.wex.domain.Green", "com.wex.domain.Red"};
    }
}
```

```java
@Configuration
@Import({Color.class, MyImportSelector.class})
public class MainConfiguration1 {}
```



**还有一个类似的方式：**

```java
public class MyImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {

    /**
     *
     * @param importingClassMetadata: 当前类的注解信息
     * @param registry: BeanDefinition注册类
     *                把所有需要添加到容器里的Bean；调用
     *                BeanDefinitionRegistry.registryBeanDefinition手工注册进来
     */
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        boolean hasDefinition = registry.containsBeanDefinition("com.wex.domain.Red");
        System.out.println(hasDefinition);
        if (hasDefinition){
            RootBeanDefinition beanDefinition = new RootBeanDefinition(Red.class);
            registry.registerBeanDefinition("red", beanDefinition);
        }
    }
}
```



### 3. FactoryBean 

继承FactoryBean实现工厂类添加Bean

```java
package com.wex.domain;

import org.springframework.beans.factory.FactoryBean;

public class ColorBeanFactory implements FactoryBean<Color> {

    @Override
    // 返回创建的对象
    public Color getObject() throws Exception {
        return new Color();
    }

    @Override
    // 返回创建对象类型
    public Class<?> getObjectType() {
        return Color.class;
    }

    @Override
    // 是否是单例
    public boolean isSingleton() {
        return true;
    }
}
```

```java
package com.wex.config;

import com.wex.domain.ColorBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MainConfiguration2 {

    @Bean
    public ColorBeanFactory colorBeanFactory(){
        return new ColorBeanFactory();
    }

}
```



```java
@Test
public void test03(){
    AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(MainConfiguration2.class);
    String[] definitionNames = applicationContext.getBeanDefinitionNames();
    for (String name: definitionNames) {
        System.out.println(name);
    }

    // 查看对应类型
    System.out.println(applicationContext.getBean("colorBeanFactory").getClass());
}
```

类名字是 **colorBeanFactory**， 但是它的类型是 **com.wex.domain.Color**而不是我们加入的工厂类。

如果想要获取工厂类对象，应该在名字前面加上 **'&'**

```java
System.out.println(applicationContext.getBean("&colorBeanFactory").getClass());
```



## 六、生命周期

### 第一种

指定初始化和销毁方法

```java
package com.wex.domain;

public class Car {

    public Car() {
    }

    private void init(){
        System.out.println("Car...init...");
    }
    
    private void destroy(){
        System.out.println("Car...destroy...");
    }
}
```



```java
@Bean(initMethod = "init", destroyMethod = "destroy")
public Car car(){
    return new Car();
}
```



### 第二种

**实现接口来定义初始化逻辑和销毁逻辑：**

```java
public class Cat implements InitializingBean, DisposableBean {

    public Cat() {
    }

    @Override
    public void destroy() throws Exception {
        System.out.println("Cat...Destroy...");
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("Cat...init...");
    }
}
```

```java
@Bean
public Cat cat(){
    return new Cat();
}
```



### 第三种

使用**JSR250**对应的注解

```java
public class Dog {
    public Dog() {
        System.out.println("...Construct...");
    }
    
    @PostConstruct // 在bean创建完成并且属性赋值完成之后
    public void init(){
        System.out.println("...@PostConstruct...");
    }
    
    @PreDestroy  // 在容器摧毁bean之前通知做清理工作
    public void destroy(){
        System.out.println("...@PreDestroy...");
    }
}
```

```java
@Bean
public Dog dog(){
    return new Dog();
}
```



### 第四种

 **bean的后置处理器（生效于所有bean）**

```java
@Component
public class MyBeanPostProcessor implements BeanPostProcessor {

    @Override
    // 在所有初始化处理之前
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("postProcessBeforeInitialization..." + beanName + "==>" + bean);
        // 返回要使用的bean对象
        return bean;
    }

    @Override
    // 在所有初始化处理之后
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        System.out.println("postProcessAfterInitialization..." + beanName + "==>" + bean);
        return bean;
    }
}
```



## 七、BeanPostProcessor

## 原理

当我们的容器在`getBean()`获取对象时，没有该对象，则调用`doCreateBean()`进行对象的创建。

```java
// doCreateBean()
try {
    // 为该实例化的对象属性赋值
    this.populateBean(beanName, mbd, instanceWrapper);
    // 调用初始化方法
    exposedObject = this.initializeBean(beanName, exposedObject, mbd);
}
```



**开始调用初始化方法：**

```java
// initializeBean(beanName, exposedObject, mbd);
if (mbd == null || !mbd.isSynthetic()) {
    // 执行我们的BeanPostProcessor的处理器
    wrappedBean = this.applyBeanPostProcessorsBeforeInitialization(bean, beanName);
}

try {
    // 执行我们的初始化方法，像我们上面说的一样
    this.invokeInitMethods(beanName, wrappedBean, mbd);
} catch (Throwable var6) {
    throw new BeanCreationException(mbd != null ? mbd.getResourceDescription() : null, beanName, "Invocation of init method failed", var6);
}

if (mbd == null || !mbd.isSynthetic()) {
    // 执行我们的BeanPostProcessor的处理器
    wrappedBean = this.applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);
}
```

```java
// applyBeanPostProcessorsBeforeInitialization(bean, beanName)
public Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName) throws BeansException {
    Object result = existingBean;

    Object current;
    // 找到所有的BeanPostProcessors，然后执行postProcessBeforeInitialization(result, beanName)方法
    for(Iterator var4 = this.getBeanPostProcessors().iterator(); var4.hasNext(); result = current) {
        BeanPostProcessor processor = (BeanPostProcessor)var4.next();
        current = processor.postProcessBeforeInitialization(result, beanName);
        // 一旦为null，直接跳出循环
        if (current == null) {
            return result;
        }
    }

    return result;
}
```





## 八、属性赋值

### @Value

1. 直接在后面写上对应属性的值。
2. 使用`”#{}“`加上Spel表达式进行赋值。

```java
public class Person {
    @Value("#{20-1}")
    private Integer id;
    @Value("zhangsan")
    private String name;   
}
```

3. 结合**@PropertySource**从配置文件中获取。

```java
@PropertySource(value = {"classpath:person.properties"})
```

```java
@Value("${name}")
private String name;
```

```properties
person.name = zhangsan
```



## 九、自动装配

### 1. @Autowired

```java
@Autowired
private BookDao bookDao;
```

1. **默认先按照类型**导入。
2. 如果有多个相同类型的，那么按照**属性名**作为ID去查找。
3. `@Qualifier("bookDao")`使用该注解可以**强制**进行区分选择。
4. 默认情况下，若没有找到，就会报错。可以设置`@Autowired(required = false)`不报错。
5. **@Primary**可以让Spring默认首选的bean。



#### 注解标注位置

```java
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD, ElementType.PARAMETER, ElementType.FIELD, ElementType.ANNOTATION_TYPE})
```

可以在**构造器、参数、方法、属性**上进行标注。

- **加在setter方法上**：单组件装配上属性的时候，会从Ioc容器中找到对应的组件加入。
- **加在有参构造器（或者加在构造器的参数上）**：也会从容器中自动配置。
- **@Bean**标注的方法创建对象的时候，方法参数的值从容器中获取。



### 2. @Resource（JSR250规则）和@Inject（JSR330规则）

> 这些都是java规范的注解（不是Spring框架）

- **@Resource**：
  - 这也可以实现自动装配，但是这个自动装配默认是按照名称进行配置的，可以使用`name`属性进行控制。
  - 但是没有**@Primary**和**required = false**的支持。

- **@Inject**：
  - 需要导入`javax.inject`的依赖才可以使用。
  - 与**@Autowired**差不多。
  - 支持**@Primary**，但是没有**required = false**的支持。



### 3. @Autowired注入底层组件及原理

自定义组件实现xxxAware；在创建对象的时候，会调用规定的方法注入。

**Aware**是一个接口，实现这个接口，可以通过回调方法来使用Spring容器底层组件的功能。

```java
/**
 * A marker superinterface indicating that a bean is eligible to be notified by the
 * Spring container of a particular framework object through a callback-style method.
 * The actual method signature is determined by individual subinterfaces but should
 * typically consist of just one void-returning method that accepts a single argument.
 *
 */
public interface Aware {

}
```



#### 常用的Aware实现类

> 使用对应的processor去执行的

```java
// ApplicationContextAware, BeanNameAware, EmbeddedValueResolverAware
public class Red implements ApplicationContextAware, BeanNameAware, EmbeddedValueResolverAware {
    // 用来对容器进行一个保存
    private ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        System.out.println("IoC容器：" + applicationContext);
        this.applicationContext = applicationContext;
    }


    // 获取当前组件注册在容器中的名字
    @Override
    public void setBeanName(String name) {
        System.out.println(name);
    }


    // 获取容器的字符串解析器
    @Override
    public void setEmbeddedValueResolver(StringValueResolver resolver) {
        resolver.resolveStringValue("你好 ${os.name}");
    }
}
```



#### 底层原理

自定义组件实现xxxAware，在创建对象的时候，会调用规定的方法注入相关的组件，把Spring底层的一些组件注入到自定义的Bean中。

不同的Aware都是有对应的Processor进行处理的，其实这些都是后置处理器。比如**ApplicationContextAware**就会有对应的**ApplicationContextAwareProcessor**进行处理。

 在**注入IoC容器**那里打上断点，进行调试，查看左侧运行的栈：

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200905170202.png)

进入**ApplicationContextAwareProcessor**的**postProcessBeforeInitialization**方法：

```java
// 传入我们自定义的类和对应的名称
public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
   // 如果该类没有继承对应的Aware接口，不用处理直接返回
   if (!(bean instanceof EnvironmentAware || bean instanceof EmbeddedValueResolverAware ||
         bean instanceof ResourceLoaderAware || bean instanceof ApplicationEventPublisherAware ||
         bean instanceof MessageSourceAware || bean instanceof ApplicationContextAware)){
      return bean;
   }

   AccessControlContext acc = null;

   // 获取这个类的安全权限
   if (System.getSecurityManager() != null) {
      acc = this.applicationContext.getBeanFactory().getAccessControlContext();
   }

   if (acc != null) {
      // 安全性检测
      AccessController.doPrivileged((PrivilegedAction<Object>) () -> {
         invokeAwareInterfaces(bean);
         return null;
      }, acc);
   }
   else {
      // 调用接口
      invokeAwareInterfaces(bean);
   }

   return bean;
}
```

```java
// 查看该类实现了哪些Aware接口，使用对应的回调方法进行属性输入
private void invokeAwareInterfaces(Object bean) {
   if (bean instanceof EnvironmentAware) {
      ((EnvironmentAware) bean).setEnvironment(this.applicationContext.getEnvironment());
   }
   if (bean instanceof EmbeddedValueResolverAware) {
      ((EmbeddedValueResolverAware) bean).setEmbeddedValueResolver(this.embeddedValueResolver);
   }
   if (bean instanceof ResourceLoaderAware) {
      ((ResourceLoaderAware) bean).setResourceLoader(this.applicationContext);
   }
   if (bean instanceof ApplicationEventPublisherAware) {
      ((ApplicationEventPublisherAware) bean).setApplicationEventPublisher(this.applicationContext);
   }
   if (bean instanceof MessageSourceAware) {
      ((MessageSourceAware) bean).setMessageSource(this.applicationContext);
   }
   if (bean instanceof ApplicationContextAware) {
      ((ApplicationContextAware) bean).setApplicationContext(this.applicationContext);
   }
}
```



### profile环境激活

`@Profile`注解标注组件，只有对应的环境被激活才能被输入。（默认是default环境）

> 可以直接写在配置类上面，那么整个类的组件都要在特定激活环境才可以使用，如果没有注解，那么默认什么环境都可以使用

```java
@Profile("default")
@Bean
```

**切换环境的方法：**

- **命令行**：在虚拟机参数位置加载 `-Dspring.profiles.active=test`

- **通过Ioc容器设置环境**：

  ```java
  // 设置激活环境
  applicationContext.getEnvironment().setActiveProfiles("test", "dev");
  // 注册新的配置类
  applicationContext.register(MainConfiguration.class);
  // 重新刷新容器
  applicationContext.refresh();
  ```

  

## 十、 AOP

### 实现一个简单的AOP功能

1. 导入对应的依赖

   ```xml
   <dependency>
       <groupId>org.springframework</groupId>
       <artifactId>spring-aspects</artifactId>
       <version>5.2.7.RELEASE</version>
   </dependency>
   ```

2. 定义一个类方法，待会为它加上AOP功能

   ```java
   public class HelloAOP {
       
       public void sayHello(){
           System.out.println("Hello AOP!");
       }
   }
   ```

3. 定义一个切面日志类

   ```java
   public class AOPLog {
   
       // 切入点
       @Pointcut("execution(public int com.wex.aop.HelloAOP.*(..))")
       public void pointcut(){}
   
       // 在方法执行前
       @Before("pointcut()")
       public void logStart(){
           System.out.println("logStart...");
       }
   
       // 在方法执行后
       @After("pointcut()")
       public void logEnd(){
           System.out.println("logEnd...");
       }
   
       // 在返回值之后
       @AfterReturning("pointcut()")
       public void logReturn(){
           System.out.println("logReturn...");
       }
   
       // 执行异常之后
       @AfterThrowing("pointcut()")
       public void logException(){
           System.out.println("logException...");
       }
   }
   ```

   个人理解，这几个位置就对应**try-catch-finally**框架的四个位置

   ```java
   try{
       logStart
       method..
       logEnd
   }catch(...){
       logException
   }finally{
       return 0;
       logReturn
   }
   ```

4. 将两个类加入IoC容器，配置切面类标识，并且开启扫描切面注解

   ```java
   @EnableAspectJAutoProxy
   @Configuration
   public class MainConfigOfAOP {
       
       @Bean
       public HelloAOP helloAOP(){
           return new HelloAOP();
       }
       
       @Bean
       public AOPLog aopLog(){
           return new AOPLog();
       }
   }
   ```

   ```java
   @Aspect
   public class AOPLog {}
   ```

5. 进行一个测试

   ```java
   public class AOPTest {
   
       @Test
       public void test1(){
           AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(MainConfigOfAOP.class);
           HelloAOP helloAOP = applicationContext.getBean(HelloAOP.class);
   
           helloAOP.sayHello();
       }
   }
   ```

6. 结果

   ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200905190244.png)



### 获取方法执行内容

利用JoinPoint进行获取：

```java
// 在方法执行前
// JoinPoint一定要在参数列表的第一位
@Before("pointcut()")
public void logStart(JoinPoint joinPoint){
    // 获取方法签名
    Signature signature = joinPoint.getSignature();
    System.out.println("signature: " + signature + " name: " + signature.getName());
    // 获取方法参数
    Object[] args = joinPoint.getArgs();
    System.out.println("argsLen: " + args.length + " arg: " + args[0]);
    // 切入类型
    String kind = joinPoint.getKind();
    System.out.println("kind: " + kind);
    // 切面地址
    SourceLocation sourceLocation = joinPoint.getSourceLocation();
    System.out.println("sourceLocation: " + sourceLocation);
    // 目标方法类
    Object aThis = joinPoint.getThis();
    System.out.println("Object: " + aThis);
    // 和kind一样
    JoinPoint.StaticPart staticPart = joinPoint.getStaticPart();
    System.out.println("staticPart: " + staticPart);
    // 目标方法
    Object target = joinPoint.getTarget();
    System.out.println("target: " + target);
    // joinPoint的简短描述
    String string = joinPoint.toShortString();
    System.out.println("joinPoint: " + string);

    System.out.println("logStart...");
}
```



**获取返回值和异常：**

```java
@AfterReturning(value = "pointcut()", returning = "result")
public void logReturn(Object result){
```

```java
@AfterThrowing(value = "pointcut()", throwing = "exception")
public void logException(Exception exception){
```



### @EnableAspectJAutoProxy注解

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import(AspectJAutoProxyRegistrar.class)
public @interface EnableAspectJAutoProxy {
    
// 之前说过，实现这个接口可以向容器去注册一些组件   
// 其实就是注册AspectJAnnotationAutoProxyCreator
class AspectJAutoProxyRegistrar implements ImportBeanDefinitionRegistrar {
```

在**registerBeanDefinitions**方法上打上断点：

```java
@Override
public void registerBeanDefinitions(
    AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {

	// 向容器中注册AspectJAnnotationAutoProxyCreator，如果有必要的话
    // 其实就是一个注解切面的创建器
    AopConfigUtils.registerAspectJAnnotationAutoProxyCreatorIfNecessary(registry);

    AnnotationAttributes enableAspectJAutoProxy =
        // 获取属性
        AnnotationConfigUtils.attributesFor(importingClassMetadata, EnableAspectJAutoProxy.class);
    if (enableAspectJAutoProxy != null) {
        // 进行属性的判断
        if (enableAspectJAutoProxy.getBoolean("proxyTargetClass")) {
            AopConfigUtils.forceAutoProxyCreatorToUseClassProxying(registry);
        }
        if (enableAspectJAutoProxy.getBoolean("exposeProxy")) {
            AopConfigUtils.forceAutoProxyCreatorToExposeProxy(registry);
        }
    }
}
```

```java
@Nullable
private static BeanDefinition registerOrEscalateApcAsRequired(
    Class<?> cls, BeanDefinitionRegistry registry, @Nullable Object source) {

    Assert.notNull(registry, "BeanDefinitionRegistry must not be null");

    // 如果没有这个类再注册
    // AUTO_PROXY_CREATOR_BEAN_NAM=org.springframework.aop.config.internalAutoProxyCreator"
    if (registry.containsBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME)) {
        BeanDefinition apcDefinition = registry.getBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME);
        if (!cls.getName().equals(apcDefinition.getBeanClassName())) {
            int currentPriority = findPriorityForClass(apcDefinition.getBeanClassName());
            int requiredPriority = findPriorityForClass(cls);
            if (currentPriority < requiredPriority) {
                apcDefinition.setBeanClassName(cls.getName());
            }
        }
        return null;
    }
	
    // 一开始肯定没有，就到这一步，进行一个Bean的定义
    RootBeanDefinition beanDefinition = new RootBeanDefinition(cls);
    beanDefinition.setSource(source);
    beanDefinition.getPropertyValues().add("order", Ordered.HIGHEST_PRECEDENCE);
    beanDefinition.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);
    registry.registerBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME, beanDefinition);
    return beanDefinition;
}
```



### 创建和注册AspectJAnnotationAutoProxyCreator

**继承关系：**

**实现了后置处理器和aware接口**

```
AspectJAnnotationAutoProxyCreator
	->AspectJAwareAdvisorAutoProxyCreator
		->AbstractAdvisorAutoProxyCreator
		 	->AbstractAutoProxyCreator
		 	  implements SmartInstantiationAwareBeanPostProcessor, BeanFactoryAware  -- 后置处理器
```

为了了解后置处理器和aware接口实现的内容，我们应该找到对应的后置处理器和aware接口相关的方法。

从**AbstractAutoProxyCreator**开始往上找，我们可以发现只有在**AbstractAutoProxyCreator**有后置处理器相关的方法，后面的子类并没有重写，所以我们现在**postProcessBeforeInstantiation**和**postProcessAfterInitialization**两个方法打上断点。然后我们发现**AbstractAdvisorAutoProxyCreator**类上重写了aware接口对应方法**setBeanFactory**，也打上断点。里面有一个**initBeanFactory**方法，在**AnnotationAwareAspectJAutoProxyCreator**中重写了，所以我们在这个类的**initBeanFactory**上打断点。

最后在我们的配置类上也打上断点。

**debug**：（从左侧运行栈开始查看）

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200907143502.png)

1. 首先传入配置类，创建ioc容器

   ```java
   @Test
   public void test1(){
       AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(MainConfigOfAOP.class);
       HelloAOP helloAOP = applicationContext.getBean(HelloAOP.class);
   
       helloAOP.sayHello("wex");
   }
   ```

2. 注册配置类，调用refresh刷新容器

   ```java
   public AnnotationConfigApplicationContext(Class<?>... componentClasses) {
      this();
      register(componentClasses);
      refresh();
   }
   ```

3. 在refresh()方法中，调用**registerBeanPostProcessors(beanFactory);**注册后置处理器来拦截bean的创建。

   ```java
   // Register bean processors that intercept bean creation.
   registerBeanPostProcessors(beanFactory);
   ```

   **进入里面的回调方法：**

   ```java
   public static void registerBeanPostProcessors(
       ConfigurableListableBeanFactory beanFactory, AbstractApplicationContext applicationContext) {
   
   	// 获取对应的BeanPostProcessor的名字
       String[] postProcessorNames = beanFactory.getBeanNamesForType(BeanPostProcessor.class, true, false);
   
       // 计数
       int beanProcessorTargetCount = beanFactory.getBeanPostProcessorCount() + 1 + postProcessorNames.length;
       // 加入一些其他的BeanPostProcessor
       beanFactory.addBeanPostProcessor(new BeanPostProcessorChecker(beanFactory, beanProcessorTargetCount));
   
       // Separate between BeanPostProcessors that implement PriorityOrdered,
       // Ordered, and the rest.
       // 区分三种类型，实现了PriorityOrdered，Ordered和原生的
       List<BeanPostProcessor> priorityOrderedPostProcessors = new ArrayList<>();
       List<BeanPostProcessor> internalPostProcessors = new ArrayList<>();
       List<String> orderedPostProcessorNames = new ArrayList<>();
       List<String> nonOrderedPostProcessorNames = new ArrayList<>();
       for (String ppName : postProcessorNames) {
           if (beanFactory.isTypeMatch(ppName, PriorityOrdered.class)) {
               BeanPostProcessor pp = beanFactory.getBean(ppName, BeanPostProcessor.class);
               priorityOrderedPostProcessors.add(pp);
               if (pp instanceof MergedBeanDefinitionPostProcessor) {
                   internalPostProcessors.add(pp);
               }
           }
           else if (beanFactory.isTypeMatch(ppName, Ordered.class)) {
               orderedPostProcessorNames.add(ppName);
           }
           else {
               nonOrderedPostProcessorNames.add(ppName);
           }
       }
   
       // First, register the BeanPostProcessors that implement PriorityOrdered.
       sortPostProcessors(priorityOrderedPostProcessors, beanFactory);
       registerBeanPostProcessors(beanFactory, priorityOrderedPostProcessors);
   
       // Next, register the BeanPostProcessors that implement Ordered.
       // 我们所在的这一层，因为我们的对象实现了Ordered接口
       List<BeanPostProcessor> orderedPostProcessors = new ArrayList<>(orderedPostProcessorNames.size());
       for (String ppName : orderedPostProcessorNames) {
           BeanPostProcessor pp = beanFactory.getBean(ppName, BeanPostProcessor.class);
           orderedPostProcessors.add(pp);
           if (pp instanceof MergedBeanDefinitionPostProcessor) {
               internalPostProcessors.add(pp);
           }
       }
       sortPostProcessors(orderedPostProcessors, beanFactory);
       registerBeanPostProcessors(beanFactory, orderedPostProcessors);
   
       // Now, register all regular BeanPostProcessors.
       List<BeanPostProcessor> nonOrderedPostProcessors = new ArrayList<>(nonOrderedPostProcessorNames.size());
       for (String ppName : nonOrderedPostProcessorNames) {
           BeanPostProcessor pp = beanFactory.getBean(ppName, BeanPostProcessor.class);
           nonOrderedPostProcessors.add(pp);
           if (pp instanceof MergedBeanDefinitionPostProcessor) {
               internalPostProcessors.add(pp);
           }
       }
       registerBeanPostProcessors(beanFactory, nonOrderedPostProcessors);
   
       // Finally, re-register all internal BeanPostProcessors.
       sortPostProcessors(internalPostProcessors, beanFactory);
       registerBeanPostProcessors(beanFactory, internalPostProcessors);
   
       // Re-register post-processor for detecting inner beans as ApplicationListeners,
       // moving it to the end of the processor chain (for picking up proxies etc).
       beanFactory.addBeanPostProcessor(new ApplicationListenerDetector(applicationContext));
   }
   ```

   1. 获取所有的后置处理器

      ```java
      String[] postProcessorNames = beanFactory.getBeanNamesForType(BeanPostProcessor.class, true, false);
      ```

      ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200907144356.png)

   2. 进行计数和区分，PriorityOrdered，Ordered和原生的

   3. 优先注册实现了PriorityOrdered接口的

      ```java
      // First, register the BeanPostProcessors that implement PriorityOrdered.
      sortPostProcessors(priorityOrderedPostProcessors, beanFactory);
      registerBeanPostProcessors(beanFactory, priorityOrderedPostProcessors);
      ```

   4. 其次再注册实现了Ordered接口的，最后才实现没有优先级的

   5. 注册BeanPostProcessor，其实就是创建对象，然后保存在方法中

      1. 创建bean的实例

         ```java
         Object beanInstance = doCreateBean(beanName, mbdToUse, args);
         ```

      2. 给bean的各种属性赋值

         ```java
         populateBean(beanName, mbd, instanceWrapper);
         ```

      3. 初始化bean

         ```java
         exposedObject = initializeBean(beanName, exposedObject, mbd);
         ```

         1. `invokeAwareMethods(beanName, bean);`调用aware的那些回调接口

            ```java
            if (bean instanceof BeanFactoryAware) {
               ((BeanFactoryAware) bean).setBeanFactory(AbstractAutowireCapableBeanFactory.this);
            }
            ```

         2. `wrappedBean = applyBeanPostProcessorsBeforeInitialization(wrappedBean, beanName);`调用后置处理器的初始化前方法。

         3. `invokeInitMethods(beanName, wrappedBean, mbd);`执行初始化方法

         4. 然后`wrappedBean = applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);`调用后置处理器的初始化后方法。

      4. 继续debug，我们发现调用的是**AbstractAdvisorAutoProxyCreator**的setBeanFactory方法

      5. 调用**AnnotationAwareAspectJAutoProxyCreator**的initBeanFactory方法

         ```java
         protected void initBeanFactory(ConfigurableListableBeanFactory beanFactory) {
             super.initBeanFactory(beanFactory);
             if (this.aspectJAdvisorFactory == null) {
                 // 创建了反射的通知工厂
                 this.aspectJAdvisorFactory = new ReflectiveAspectJAdvisorFactory(beanFactory);
             }
             this.aspectJAdvisorsBuilder =
                 // 创建工厂的构建器
                 new BeanFactoryAspectJAdvisorsBuilderAdapter(beanFactory, this.aspectJAdvisorFactory);
         }
         ```

      6. 至此，创建成功。

   6. 将BeanPostProcessors注册到BeanFactory中。



### AspectJAnnotationAutoProxyCreator的使用

**继续debug**：

1. 创建剩下的单实例bean（在之前的refresh方法里面的）

   ```java
   // Instantiate all remaining (non-lazy-init) singletons.
   finishBeanFactoryInitialization(beanFactory);
   ```

   1. 获取容器中的beanNames，然后依次创建

      ```java
      List<String> beanNames = new ArrayList<>(this.beanDefinitionNames);
      ```

      ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200907152932.png)

      ```java
      for (String beanName : beanNames) {
          RootBeanDefinition bd = getMergedLocalBeanDefinition(beanName);
          if (!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) {
              // 是否是工厂对象
              if (isFactoryBean(beanName)) {
                  Object bean = getBean(FACTORY_BEAN_PREFIX + beanName);
                  if (bean instanceof FactoryBean) {
                      final FactoryBean<?> factory = (FactoryBean<?>) bean;
                      boolean isEagerInit;
                      if (System.getSecurityManager() != null && factory instanceof SmartFactoryBean) {
                          isEagerInit = AccessController.doPrivileged((PrivilegedAction<Boolean>)
                                                                      ((SmartFactoryBean<?>) factory)::isEagerInit,
                                                                      getAccessControlContext());
                      }
                      else {
                          isEagerInit = (factory instanceof SmartFactoryBean &&
                                         ((SmartFactoryBean<?>) factory).isEagerInit());
                      }
                      if (isEagerInit) {
                          getBean(beanName);
                      }
                  }
              }
              else {
                  // 不是就创建对象
                  getBean(beanName);
              }
      ```

   2. getBean -> doGetBean -> getSingleton

      会先从缓存中查看是否有该对象了，如果有，那么就封装下继续使用，如果没有再进行创建。（保证单实例）

   3. createBean创建对象

      1. 进行解析，如果后置处理器能够返回一个代理对象，那么就使用

         ```java
         // Give BeanPostProcessors a chance to return a proxy instead of the target bean instance.
         Object bean = resolveBeforeInstantiation(beanName, mbdToUse);
         ```

         ```java
         protected Object resolveBeforeInstantiation(String beanName, RootBeanDefinition mbd) {
             Object bean = null;
         	// 取出所有的后置处理器
             if (!Boolean.FALSE.equals(mbd.beforeInstantiationResolved)) {
                 // Make sure bean class is actually resolved at this point.
                 // 如果是InstantiationAwareBeanPostProcessor类型，那么久利用它进行创建
                 // 因为里面有对应的postProcessBeforeInstantiation方法
                 if (!mbd.isSynthetic() && hasInstantiationAwareBeanPostProcessors()) {
                     Class<?> targetType = determineTargetType(beanName, mbd);
                     if (targetType != null) {
                         bean = applyBeanPostProcessorsBeforeInstantiation(targetType, beanName);
                         if (bean != null) {
                             bean = applyBeanPostProcessorsAfterInitialization(bean, beanName);
                         }
                     }
                 }
                 mbd.beforeInstantiationResolved = (bean != null);
             }
             return bean;
         }
         ```

      2. 如果不行，调用**doCreateBean**创建。

         和之前一样：创建实例 -> 属性赋值 -> 调用后置处理器初始前方法 -> 自定义初始化方法 -> 调用后置处理器初始后方法

   > 着就是AOP的原理，在每一个创建Bean的时候会进行一个拦截，调用**postProcessBeforeInstantiation**生成代理对象

   2. 放行到我们的目标类的创建上

      ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200907160011.png)

      ```java
      @Override
      public Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) {
      
          // 先获取名字
          Object cacheKey = getCacheKey(beanClass, beanName);
      
          if (!StringUtils.hasLength(beanName) || !this.targetSourcedBeans.contains(beanName)) {
              // 判断当前的bean是否在advisedBeans中
              // advisedBeans这里面就是需要增强的bean
              // 一开始啥也没有，所以肯定返回false
              if (this.advisedBeans.containsKey(cacheKey)) {
                  return null;
              }
              
              // 判断当前bean是不是基础的bean
              // Advice、Pointcut、Advisor、AopInfrastructureBean或者是切面
              // 或者需要跳过，获取增强器，判断是不是AspectJPointcutAdvisor
              if (isInfrastructureClass(beanClass) || shouldSkip(beanClass, beanName)) {
                  this.advisedBeans.put(cacheKey, Boolean.FALSE);
                  return null;
              }
          }
      
          // Create proxy here if we have a custom TargetSource.
          // Suppresses unnecessary default instantiation of the target bean:
          // The TargetSource will handle target instances in a custom fashion.
          TargetSource targetSource = getCustomTargetSource(beanClass, beanName);
          if (targetSource != null) {
              if (StringUtils.hasLength(beanName)) {
                  this.targetSourcedBeans.add(beanName);
              }
              Object[] specificInterceptors = getAdvicesAndAdvisorsForBean(beanClass, beanName, targetSource);
              Object proxy = createProxy(beanClass, beanName, specificInterceptors, targetSource);
              this.proxyTypes.put(cacheKey, proxy.getClass());
              return proxy;
          }
      
          return null;
      }
      ```

   3. 调用**BeanPostProcessorsAfterInitialization**

      ```java
      public Object postProcessAfterInitialization(@Nullable Object bean, String beanName) {
          if (bean != null) {
              Object cacheKey = getCacheKey(bean.getClass(), beanName);
              if (this.earlyProxyReferences.remove(cacheKey) != bean) {
      			// 如果需要就包装
                  return wrapIfNecessary(bean, beanName, cacheKey);
              }
          }
          return bean;
      }
      ```

      ```java
      protected Object wrapIfNecessary(Object bean, String beanName, Object cacheKey) {
          ......
      
              // Create proxy if we have advice.
              // 获取当前bean可用的增强器，判断切点是不是能符合我们的方法（还会进行排序）
              Object[] specificInterceptors = getAdvicesAndAdvisorsForBean(bean.getClass(), beanName, null);
          if (specificInterceptors != DO_NOT_PROXY) {
              // 保存当前bean在advisedBeans中
              this.advisedBeans.put(cacheKey, Boolean.TRUE);
              // 创建代理对象
              // 1. 获取所有的增强器
              // 2. 创建代理工厂并且将增强器保存到proxyFactory
              // 3. 两种方式创建代理对象，由Spring自动决定（一般看有无实现接口）
              Object proxy = createProxy(
                  bean.getClass(), beanName, specificInterceptors, new SingletonTargetSource(bean));
              this.proxyTypes.put(cacheKey, proxy.getClass());
              return proxy;
          }
      
          this.advisedBeans.put(cacheKey, Boolean.FALSE);
          return bean;
      }
      ```

   4. 给容器返回由动态代理创建的对象。



### 目标方法执行

**目标方法执行打上断点：**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200918144042.png)



可以看到Spring AOP已经为我们生成了**代理对象**，和一系列的封装好的**增强器**。

![image-20200918144250509](C:\Users\58354\AppData\Roaming\Typora\typora-user-images\image-20200918144250509.png)



进入**CgLib的拦截方法**：

```java
@Override
@Nullable
public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
    Object oldProxy = null;
    boolean setProxyContext = false;
    Object target = null;
    TargetSource targetSource = this.advised.getTargetSource();
    try {
        if (this.advised.exposeProxy) {
            // Make invocation available if necessary.
            oldProxy = AopContext.setCurrentProxy(proxy);
            setProxyContext = true;
        }
        // Get as late as possible to minimize the time we "own" the target, in case it comes from a pool...
		// 获取目标对象
        target = targetSource.getTarget();
        Class<?> targetClass = (target != null ? target.getClass() : null);
        // 根据ProxyFactory对象获取将要执行的目标方法的拦截器链
        List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
        Object retVal;
        if (chain.isEmpty() && Modifier.isPublic(method.getModifiers())) {
            // 如果没有拦截器链，直接执行目标方法
            Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
            retVal = methodProxy.invoke(target, argsToUse);
        }
        else {
            // 如果有拦截器链，创建一个method invocation，并且调用proceed()方法
            retVal = new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();
        }
        retVal = processReturnType(proxy, target, method, retVal);
        return retVal;
    }
    finally {
        if (target != null && !targetSource.isStatic()) {
            targetSource.releaseTarget(target);
        }
        if (setProxyContext) {
            // Restore old proxy.
            AopContext.setCurrentProxy(oldProxy);
        }
    }
}
```



#### 获取拦截器链

```java
public List<Object> getInterceptorsAndDynamicInterceptionAdvice(Method method, @Nullable Class<?> targetClass) {
    // 先获取缓存，方便存放
    MethodCacheKey cacheKey = new MethodCacheKey(method);
    List<Object> cached = this.methodCache.get(cacheKey);
    if (cached == null) {
        // 通过advisorChainFactory工厂获取拦截器链
        cached = this.advisorChainFactory.getInterceptorsAndDynamicInterceptionAdvice(
            this, method, targetClass);
        this.methodCache.put(cacheKey, cached);
    }
    return cached;
}
```

```java
@Override
public List<Object> getInterceptorsAndDynamicInterceptionAdvice(
    Advised config, Method method, @Nullable Class<?> targetClass) {
    AdvisorAdapterRegistry registry = GlobalAdvisorAdapterRegistry.getInstance();
    Advisor[] advisors = config.getAdvisors();
    // 1. 申请一个拦截器的
    List<Object> interceptorList = new ArrayList<>(advisors.length);
    Class<?> actualClass = (targetClass != null ? targetClass : method.getDeclaringClass());
    Boolean hasIntroductions = null;

    // 2. 遍历所有的通知，然后按照类型进行处理
    for (Advisor advisor : advisors) {
        if (advisor instanceof PointcutAdvisor) {
            
            PointcutAdvisor pointcutAdvisor = (PointcutAdvisor) advisor;
            if (config.isPreFiltered() || pointcutAdvisor.getPointcut().getClassFilter().matches(actualClass)) {
                MethodMatcher mm = pointcutAdvisor.getPointcut().getMethodMatcher();
                boolean match;
                if (mm instanceof IntroductionAwareMethodMatcher) {
                    if (hasIntroductions == null) {
                        hasIntroductions = hasMatchingIntroductions(advisors, actualClass);
                    }
                    match = ((IntroductionAwareMethodMatcher) mm).matches(method, actualClass, hasIntroductions);
                }
                else {
                    match = mm.matches(method, actualClass);
                }
                if (match) {
                    // 3. 将通知封装成MethodInterceptor的数组
                    // 如果直接是MethodInterceptor，加进来，如果不是，会有指定的的适配器进行转换
                    MethodInterceptor[] interceptors = registry.getInterceptors(advisor);
                    if (mm.isRuntime()) {
                      
                        for (MethodInterceptor interceptor : interceptors) {
                            interceptorList.add(new InterceptorAndDynamicMethodMatcher(interceptor, mm));
                        }
                    }
                    else {
                        interceptorList.addAll(Arrays.asList(interceptors));
                    }
                }
            }
        }
        else if (advisor instanceof IntroductionAdvisor) {
            IntroductionAdvisor ia = (IntroductionAdvisor) advisor;
            if (config.isPreFiltered() || ia.getClassFilter().matches(actualClass)) {
                Interceptor[] interceptors = registry.getInterceptors(advisor);
                interceptorList.addAll(Arrays.asList(interceptors));
            }
        }
        else {
            Interceptor[] interceptors = registry.getInterceptors(advisor);
            // 4. 最后添加进我们的List中返回
            interceptorList.addAll(Arrays.asList(interceptors));
        }
    }

    return interceptorList;
}
```



#### 执行拦截器链

```java
public Object proceed() throws Throwable {
    // currentInterceptorIndex记录当前拦截器的索引，一开始默认是-1
    // 如果它等于interceptorsAndDynamicMethodMatchers.size() - 1
    // 也就是当前拦截器的数量减1，那么就是没有拦截器了
    if (this.currentInterceptorIndex == this.interceptorsAndDynamicMethodMatchers.size() - 1) {
        return invokeJoinpoint();
    }

    // 从拦截器数组中获取对应索引的拦截器，并把Index索引加一
    Object interceptorOrInterceptionAdvice =
        this.interceptorsAndDynamicMethodMatchers.get(++this.currentInterceptorIndex);
    if (interceptorOrInterceptionAdvice instanceof InterceptorAndDynamicMethodMatcher) {
        
        InterceptorAndDynamicMethodMatcher dm =
            (InterceptorAndDynamicMethodMatcher) interceptorOrInterceptionAdvice;
        Class<?> targetClass = (this.targetClass != null ? this.targetClass : this.method.getDeclaringClass());
        if (dm.methodMatcher.matches(this.method, targetClass, this.arguments)) {
            return dm.interceptor.invoke(this);
        }
        else {
           
            return proceed();
        }
    }
    else {
        // 如果是对应的拦截器，那么把对象闯进去执行方法
        return ((MethodInterceptor) interceptorOrInterceptionAdvice).invoke(this);
    }
}
```

```java
@Override
public Object invoke(MethodInvocation mi) throws Throwable {
    // ThreadLocal<MethodInvocation> invocation线程存储
    MethodInvocation oldInvocation = invocation.get();
    invocation.set(mi);
    try {
        // 其实就是执行这个对象的proceed方法
        return mi.proceed();
    }
    finally {
        invocation.set(oldInvocation);
    }
}
```

- 首先获得的是一个已经排序好的拦截器链（这个排序是在之前的通知获取中排序好的）
- 首先执行越后的通知拦截方法，一层一层堆栈下去，然后先执行前置通知
- 执行结束之后调用invoke()方法，执行目标的方法，然后往回退，执行后置方法
- 后置方法执行之后，如果有异常，就会直接抛出异常，这时回退之后的异常通知会收到异常然后进行执行
- 最后不管如何会执行最后的通知方法

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/20200918152158.png)



### 总结

1. @EnableAspectJAutoProxy 开启AOP功能
2. @EnableAspectJAutoProxy 会给容器中注册一个组件 AnnotationAwareAspectJAutoProxyCreator
3. AnnotationAwareAspectJAutoProxyCreator是一个后置处理器；
4. 容器的创建流程：
   1. registerBeanPostProcessors（）注册后置处理器；创建AnnotationAwareAspectJAutoProxyCreator对象
   2. finishBeanFactoryInitialization（）初始化剩下的单实例bean
   3. 创建业务逻辑组件和切面组件
   4. AnnotationAwareAspectJAutoProxyCreator拦截组件的创建过程
   5. 组件创建完之后，判断组件是否需要增强
   6. 切面的通知方法，包装成增强器（Advisor）;给业务逻辑组件创建一个代理对象（cglib）；

5. 执行目标方法：
   1. 代理对象执行目标方法
   2. CglibAopProxy.intercept()；
      1. 得到目标方法的拦截器链（增强器包装成拦截器MethodInterceptor）
      2. 利用拦截器的链式机制，依次进入每一个拦截器进行执行；
      3. 效果：
         - 正常执行：前置通知-》目标方法-》后置通知-》返回通知
         - 出现异常：前置通知-》目标方法-》后置通知-》异常通知



## 十一、 声明式事务

### 测试准备

```java
/**
 * 声明式事务
 */
@Configuration
@ComponentScan("com.wex.tx")
public class TxConfig {
    @Bean
    public DataSource dataSource() throws PropertyVetoException {
        ComboPooledDataSource dataSource = new ComboPooledDataSource();
        dataSource.setUser("root");
        dataSource.setPassword("110325");
        dataSource.setDriverClass("com.mysql.cj.jdbc.Driver");
        dataSource.setJdbcUrl("jdbc:mysql://localhost:3306/test?useSSL=true&serverTimezone=GMT%2B8");
        return dataSource;
    }

    @Bean
    public JdbcTemplate jdbcTemplate(DataSource dataSource){
        return new JdbcTemplate(dataSource);
    }
}
```

```java
@Repository
public class StudentDao {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    public void insert(){
        String sql = "insert into student values (?, ?, ?)";
        jdbcTemplate.update(sql, "wex", "311700", 1);
    }

}
```

```java
@Service
public class StudentService {
    @Autowired
    private StudentDao studentDao;

    public void insert(){
        studentDao.insert();
        System.out.println("insert finish...");
    }
}
```

```java
@Test
public void test1(){
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(TxConfig.class);
    StudentService studentService = context.getBean(StudentService.class);

    studentService.insert();
    context.close();
}
```



### 开启事务

```java
// 开始事务
@Transactional
public void insert(){
    studentDao.insert();
    System.out.println("insert finish...");
}
```

```java
@Configuration
// 开启事务支持
@EnableTransactionManagement
@ComponentScan("com.wex.tx")
public class TxConfig {
	...
}
```

```java
// 注册事务管理器
@Bean
public PlatformTransactionManager platformTransactionManager(DataSource dataSource){
    return new DataSourceTransactionManager(dataSource);
}
```



### @EnableTransactionManagement原理

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
// 利用selector来进行组件的导入
@Import(TransactionManagementConfigurationSelector.class)
public @interface EnableTransactionManagement {}
```

```java
// AdviceMode mode() default AdviceMode.PROXY;
// 这是注解定义的属性，默认是PROXY
@Override
protected String[] selectImports(AdviceMode adviceMode) {
   switch (adviceMode) {
      // 判断adviceMode，如果是PROXY
      // 导入AutoProxyRegistrar和ProxyTransactionManagementConfiguration两种组件
      case PROXY:
         return new String[] {AutoProxyRegistrar.class.getName(),
               ProxyTransactionManagementConfiguration.class.getName()};
      // 如果是ASPECTJ，会导入determineTransactionAspectClass
      case ASPECTJ:
         return new String[] {determineTransactionAspectClass()};
      default:
         return null;
   }
}
```



- **AutoProxyRegistrar**：
  - 给容器中注册一个 InfrastructureAdvisorAutoProxyCreator 组件；
   - 利用后置处理器机制在对象创建以后，包装对象，返回一个代理对象（增强器），代理对象执行方法利用拦截器链进行调用；
- **ProxyTransactionManagementConfiguration**
  - 给容器中注册事务增强器，要用事务注解的信息（事务属性），**TransactionAttributeSource**事务注解解析器。
  - 注册事务拦截器**transactionInterceptor**，保存了事务的属性信息和事务管理器。它是一个MethodInterceptor。






















































































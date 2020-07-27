# 注解开发

## 准备

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



## ComponentScan

### 排除扫描的类

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



### 只包含某些类

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



### 多个规则

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



## FilterType

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

**配置雷：**

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



## Conditional

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



## Import

**快速注册一个组件，名字默认是全限定类名：**

```java
@Configuration
@Import(Color.class)
public class MainConfiguration1 {}
```



## ImportSelector

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



## FactoryBean 

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



## 生命周期

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



## BeanPostProcessor

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




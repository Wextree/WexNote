# Spring

## 注解

### @Target  @Retention  @Documented注解

> 参考文档：https://www.jianshu.com/p/8738ce6bd771

### @Target

用于描述注解的**使用范围**（即：被描述的注解可以用在什么地方）

取值(**ElementType**)有：

1. **CONSTRUCTOR: **用于描述构造器

2. **FIELD: **用于描述域

3. **LOCAL_VARIABLE: **用于描述局部变量

4. **METHOD: **用于描述方法

5. **PACKAGE: **用于描述包

6. **PARAMETER: **用于描述参数

7. **TYPE: **用于描述类、接口(包括注解类型) 或`enum`声明



### @Retention

Retention**(保留)**注解说明,这种类型的注解会被保留到那个阶段. 有三个值:

1. `RetentionPolicy.SOURCE` —— 这种类型的Annotations只在**源代码级别保留**,**编译时就会被忽略**

2. `RetentionPolicy.CLASS `—— 这种类型的Annotations**编译**时被保留,**在class文件中**存在,但**JVM将会忽略**

3. `RetentionPolicy.RUNTIME `— 这种类型的Annotations将**被JVM保留**,所以他们能在运行时被**JVM或其他使用反射机制的代码所读取和使用**.



### @Documented

Documented 注解表明这个注解应该被 `javadoc`工具记录. 默认情况下,`javadoc`是不包括注解的. 但如果声明注解时指定了 @Documented,则它会被` javadoc `之类的工具处理, 所以注解类型信息也会被包括在生成的文档中.



### @Inherited

这是一个稍微复杂的注解类型. 它指明被注解的类**会自动继承**. 更具体地说,如果定义注解时使用了 **@Inherited** 标记,然后用定义的注解来标注另一个父类, 父类又有一个子类(subclass),则**父类的所有属性将被继承到它的子类**中.



## 模块

![](E:\git\WexNote\Audition Points\imgs\640 (10).jpg)

- Spring Core  spring的核心功能： IOC容器, 解决对象创建及依赖关系

- Spring Web  Spring对web模块的支持。

- - 可以与struts整合,让struts的action创建交给spring
  - spring mvc模式

- Spring DAO  Spring 对jdbc操作的支持  【JdbcTemplate模板工具类】

- Spring ORM  spring对orm的支持：

- - 既可以与hibernate整合，【session】
  - 也可以使用spring的对hibernate操作的封装

- Spring AOP  切面编程

- SpringEE   spring 对javaEE其他模块的支持



## Core

> **Core模块是：IOC容器，解决对象创建和之间的依赖关系**。
>
> **Core模块主要是学习如何得到IOC容器，通过IOC容器来创建对象、解决对象之间的依赖关系、IOC细节。**

Spring核心的配置文件`applicationContext.xml`或者叫`bean.xml`

在Spring中总体来看可以通过**三种方式**来配置对象:

- **使用XML文件配置**
- **使用注解来配置**
- **使用JavaConfig来配置**





## Ioc

![](E:\git\WexNote\Audition Points\imgs\640 (26).webp)

![](E:\git\WexNote\Audition Points\imgs\640 (27).webp)



## AOP

Spring AOP主要做的事情就是：「把重复的代码抽取，在运行的时候往业务方法上**动态植入**“切面类代码”」









# SpringBoot

## 自动配置原理

### SpringBoot配置文件

**SpringBoot**有各种**starter依赖**，需要什么就勾选就好了。如果需要什么自定义的配置，那么只需要直接在**配置文件中设置**就行了。

查看官网的配置样例：[官网配置](https://docs.spring.io/spring-boot/docs/2.1.3.RELEASE/reference/htmlsingle/#boot-features-external-config-yaml)

![](E:\git\WexNote\Markdown note\imgs\1588751089.png)



###配置原理

- **通过以下示例**

![](E:\git\WexNote\Markdown note\imgs\Snipaste_2020-05-06_15-47-46.png)



- **重点关注SpringBootApplication注解**

![](E:\git\WexNote\Markdown note\imgs\Snipaste_2020-05-06_15-50-54.png)

- **Target**  、**Retention **、**Documented** 和 **Inherited** 注解，是**`java.lang.annotation`**包下的原生注解。（具体的内容见上面解释）

- **@SpringBootConfiguration**注解：

  ![](E:\git\WexNote\Markdown note\imgs\Snipaste_2020-05-06_16-16-58.png)

  - 除了前面几个原生的注解之外，其实最重要的就只有一个**@Configuration注解**，而这个注解在Spring中，是标注一个类，被标注的**类充当配置类**并且会被放入容器中。

- **@EnableAutoConfiguration**注解（核心）：

  ![](E:\git\WexNote\Markdown note\imgs\Snipaste_2020-05-06_16-25-48.png)

  - **@AutoConfigurationPackage**注解
  - **AutoConfigurationImportSelector**（核心）：通过这个类来导入组件，以下是它的**主要方法**：

```java
@Override
public String[] selectImports(AnnotationMetadata annotationMetadata) {
    if (!isEnabled(annotationMetadata)) {
        return NO_IMPORTS;
    }
    // 调用类加载器获取对应路径下的配置信息
    AutoConfigurationMetadata autoConfigurationMetadata 
        = AutoConfigurationMetadataLoader.loadMetadata(this.beanClassLoader);
    // 开始获取自动配置项
    AutoConfigurationEntry autoConfigurationEntry = getAutoConfigurationEntry(autoConfigurationMetadata,annotationMetadata);
    return StringUtils.toStringArray(autoConfigurationEntry.getConfigurations());
}
```

```java
// AutoConfigurationMetadataLoader.loadMetadata(this.beanClassLoader);
// 资源的路径
protected static final String PATH = "META-INF/spring-autoconfigure-metadata.properties";
// 传入类加载器加载配置元信息
static AutoConfigurationMetadata loadMetadata(ClassLoader classLoader) {
    return loadMetadata(classLoader, PATH);
}

static AutoConfigurationMetadata loadMetadata(ClassLoader classLoader, String path) {
    try {
        // 通过类加载器加载资源的路径
        Enumeration<URL> urls = (classLoader != null) ? classLoader.getResources(path)
            : ClassLoader.getSystemResources(path);
        Properties properties = new Properties();
        while (urls.hasMoreElements()) {
            // 获取配置类
            properties.putAll(PropertiesLoaderUtils.loadProperties(new UrlResource(urls.nextElement())));
        }
        return loadMetadata(properties);
    }
    catch (IOException ex) {
        // 抛出IO异常
        throw new IllegalArgumentException("Unable to load @ConditionalOnClass location [" + path + "]", ex);
    }
}

static AutoConfigurationMetadata loadMetadata(Properties properties) {
    // 调用内部静态类
    return new PropertiesAutoConfigurationMetadata(properties);
}

```

```java
// getAutoConfigurationEntry(autoConfigurationMetadata,annotationMetadata);
// 开始自动获取配置
protected AutoConfigurationEntry getAutoConfigurationEntry(AutoConfigurationMetadata autoConfigurationMetadata,AnnotationMetadata annotationMetadata) {
		if (!isEnabled(annotationMetadata)) {
			return EMPTY_ENTRY;
		}
    	// 获取属性名称
		AnnotationAttributes attributes = getAttributes(annotationMetadata);
    	//  获取一个自动配置 List ，这个 List 就包含了所有自动配置的类名
		List<String> configurations 
            = getCandidateConfigurations(annotationMetadata, attributes);
		configurations = removeDuplicates(configurations);
		Set<String> exclusions = getExclusions(annotationMetadata, attributes);
		checkExcludedClasses(configurations, exclusions);
		configurations.removeAll(exclusions);
		configurations = filter(configurations, autoConfigurationMetadata);
		fireAutoConfigurationImportEvents(configurations, exclusions);
		return new AutoConfigurationEntry(configurations, exclusions);
}


protected List<String> getCandidateConfigurations(AnnotationMetadata metadata, AnnotationAttributes attributes) {
    // 通过 getSpringFactoriesLoaderFactoryClass 获取默认的 EnableAutoConfiguration.class 类名，传入 loadFactoryNames 方法
    List<String> configurations = 	SpringFactoriesLoader.loadFactoryNames(getSpringFactoriesLoaderFactoryClass(),           getBeanClassLoader());
    Assert.notEmpty(configurations, "No auto configuration classes found in META-INF/spring.factories. If you "
                    + "are using a custom packaging, make sure that file is correct.");
    return configurations;
}

// 获取默认的 EnableAutoConfiguration.class 类名
protected Class<?> getSpringFactoriesLoaderFactoryClass() {
		return EnableAutoConfiguration.class;
	}

public static List<String> loadFactoryNames(Class<?> factoryType, @Nullable ClassLoader classLoader) {
    	// 这个名字就是上面的类名,EnableAutoConfiguration.class
		String factoryTypeName = factoryType.getName();
		return loadSpringFactories(classLoader).getOrDefault(factoryTypeName, Collections.emptyList());
	}


private static Map<String, List<String>> loadSpringFactories(@Nullable ClassLoader classLoader) {
		MultiValueMap<String, String> result = cache.get(classLoader);
		if (result != null) {
			return result;
		}
	// FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories"
    // 获取该路径下的文件url
		try {
			Enumeration<URL> urls = (classLoader != null ?
					classLoader.getResources(FACTORIES_RESOURCE_LOCATION) :
					ClassLoader.getSystemResources(FACTORIES_RESOURCE_LOCATION));
			result = new LinkedMultiValueMap<>();
			while (urls.hasMoreElements()) {
               	// 遍历然后将文件的内容封装成properties
				URL url = urls.nextElement();
				UrlResource resource = new UrlResource(url);
				Properties properties = PropertiesLoaderUtils.loadProperties(resource);
				for (Map.Entry<?, ?> entry : properties.entrySet()) {
					String factoryTypeName = ((String) entry.getKey()).trim();
					for (String factoryImplementationName : StringUtils.commaDelimitedListToStringArray((String) entry.getValue())) {
						result.add(factoryTypeName, factoryImplementationName.trim());
					}
				}
			}
			cache.put(classLoader, result);
			return result;
		}
		catch (IOException ex) {
			throw new IllegalArgumentException("Unable to load factories from location [" +
					FACTORIES_RESOURCE_LOCATION + "]", ex);
		}
	}
```

```java
// spring-boot-autoconfigure-2.2.6.RELEASE.jar包下
// 就是加载这些类
# Auto Configure
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
org.springframework.boot.autoconfigure.admin.SpringApplicationAdminJmxAutoConfiguration,\
org.springframework.boot.autoconfigure.aop.AopAutoConfiguration,\
org.springframework.boot.autoconfigure.amqp.RabbitAutoConfiguration,\
org.springframework.boot.autoconfigure.batch.BatchAutoConfiguration,\
org.springframework.boot.autoconfigure.cache.CacheAutoConfiguration,\
org.springframework.boot.autoconfigure.cassandra.CassandraAutoConfiguration,\
org.springframework.boot.autoconfigure.cloud.CloudServiceConnectorsAutoConfiguration,\
org.springframework.boot.autoconfigure.context.ConfigurationPropertiesAutoConfiguration,\
org.springframework.boot.autoconfigure.context.MessageSourceAutoConfiguration,\
org.springframework.boot.autoconfigure.context.PropertyPlaceholderAutoConfiguration,\
org.springframework.boot.autoconfigure.couchbase.CouchbaseAutoConfiguration,\
org.springframework.boot.autoconfigure.dao.PersistenceExceptionTranslationAutoConfiguration,\
.........
```

1. 首先注意到 selectImports 方法，其实从方法名就能看出，这个方法用于给容器中导入组件，然后跳到 getAutoConfigurationEntry 方法就是用于获取自动配置项的。
2. 再来进入 getCandidateConfigurations 方法就是 获取一个自动配置 List ，这个 List 就包含了所有的自动配置的类名 。
3. 再进入 SpringFactoriesLoader 类的 loadFactoryNames 方法，跳转到 loadSpringFactories 方法发现 ClassLoader 类加载器指定了一个 FACTORIES_RESOURCE_LOCATION 常量。
4. 然后利用PropertiesLoaderUtils 把 ClassLoader 扫描到的这些文件的内容包装成 properties 对象，从 properties 中获取到 EnableAutoConfiguration.class 类（类名）对应的值，然后把他们添加在容器中。
5. 每个自动配置类都要有一定条件才可以生效，可以打开配置文件中的`#debug`功能。会输出很多信息，告诉你那些类自动启用，需要哪些类才能生效。



## SpringBoot 扩展 SpringMVC

```java
// spring 5.0 对应 springboot 2.1.8 之后
@Configuration
public class MvcConfig implements WebMvcConfigurer {
    // 需要什么功能就查看对应的方法，保留了springboot原始的自动配置
    
    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        // 浏览器访问对应路径，直接去success页面
        registry.addViewController("/hello").setViewName("success");
    }
}
```

原理：

```java
// WebMvcAutoConfiguration是SpringMVC的自动配置类
// 自动配置类里有该静态内部类，实现上述的接口，实现扩展配置
public static class WebMvcAutoConfigurationAdapter implements WebMvcConfigurer {}
    
// 该内部类做配置时会引入
@Import(EnableWebMvcConfiguration.class) 

// 这个类实现了自动配置
@Configuration(proxyBeanMethods = false)
public static class EnableWebMvcConfiguration extends DelegatingWebMvcConfiguration implements ResourceLoaderAware {}

// 查看父类
public class DelegatingWebMvcConfiguration extends WebMvcConfigurationSupport {

	private final WebMvcConfigurerComposite configurers = new WebMvcConfigurerComposite();

	// 自动装配，就是说要获取容器中所有的扩展配置类
    // 然后统一调用他们的方法
	@Autowired(required = false)
	public void setConfigurers(List<WebMvcConfigurer> configurers) {
		if (!CollectionUtils.isEmpty(configurers)) {
			this.configurers.addWebMvcConfigurers(configurers);
		}
	}
}

```

###@EnableWebMvc
可以全面接管MVC，即开启这个注解，会导致不自动装配，而是启用我们自己定义的。

```java
@Import(DelegatingWebMvcConfiguration.class)
public @interface EnableWebMvc {
}

@Configuration(proxyBeanMethods = false)
public class DelegatingWebMvcConfiguration extends WebMvcConfigurationSupport {
}

// 我们查看自动配置类
@Configuration(proxyBeanMethods = false)
@ConditionalOnWebApplication(type = Type.SERVLET)
@ConditionalOnClass({ Servlet.class, DispatcherServlet.class, WebMvcConfigurer.class })
// 这一句标识着当没有该组件的时候，自动配置类才生效
@ConditionalOnMissingBean(WebMvcConfigurationSupport.class)
@AutoConfigureOrder(Ordered.HIGHEST_PRECEDENCE + 10)
@AutoConfigureAfter({ DispatcherServletAutoConfiguration.class, TaskExecutionAutoConfiguration.class,
		ValidationAutoConfiguration.class })
public class WebMvcAutoConfiguration {
}
```











# SpringCloud

**大版本对应：**

| Spring Boot   | Spring Cloud             |
| :------------ | :----------------------- |
| 1.2.x         | Angel版本                |
| 1.3.x         | Brixton版本              |
| 1.4.x stripes | Camden版本               |
| 1.5.x         | Dalston版本、Edgware版本 |
| 2.0.x         | Finchley版本             |

**在实际开发过程中，我们需要更详细的版本对应：**

| spring-boot-starter-parent | spring-cloud-dependencies |        |                         |              |        |
| :------------------------- | :------------------------ | :----- | :---------------------- | :----------- | :----- |
| 版本号                     | 发布日期                  |        | 版本号                  | 发布日期     |        |
| 1.5.2.RELEASE              | 2017年3月                 | 稳定版 | Dalston.RC1             | 2017年未知月 |        |
| 1.5.9.RELEASE              | 2017年11月                | 稳定版 | Edgware.RELEASE         | 2017年11月   | 稳定版 |
| 1.5.16.RELEASE             |                           |        | Edgware.SR5             |              |        |
| 1.5.20.RELEASE             |                           |        | Edgware.SR5             |              |        |
| 2.0.2.RELEASE              | 2018年5月                 |        | Finchley.BUILD-SNAPSHOT | 2018年未知月 |        |
| 2.0.6.RELEASE              |                           |        | Finchley.SR2            |              |        |
| 2.1.4.RELEASE              |                           |        | Greenwich.SR1           |              |        |
| 待更新...                  |                           |        |                         |              |        |

## Eureka

> SpringCloud微服务注册中心
>
> 本文代码示例基于Spring Boot 2.3.0.RELEASE和Spring Cloud Hoxton.SR4版本；



### 简介

 Eureka是Netflix开源的服务发现组件，本身是一个基于REST的服务，包含Server和Client两部分，Spring Cloud将它集成在子项目Spring Cloud Netflix中。在微服务系统中，我们需要单独创建一个Eureka Server作为注册中心，其他的微服务就相当于客户端，注册到我们的注册中心中。



### 服务发现

原来我们服务间的调用，一般都是通过在消费者端配置生产者的ip和端口，来进行调用，但是，在微服务的环境中，服务数量可能会很多，而且，服务之间可能调用关系也会很多，如果还是配置ip和端口来互相调用的话，这个配置的工作就很庞杂，同时，如果某个微服务的ip发生了变化，那么带来的维护工作，也会很复杂。
   因此，微服务中引入了服务发现组件，也就是**注册中心**，一般使用Eureka,也有其他的组件,Consul,zookeeper等。实际使用中，将微服务都注册到注册中心，注册中已经包含了微服务的ip等信息，这样，微服务之间互相调用时，就可以现在注册中心获取对应微服务的ip信息。这样就不用自己维护很多的配置文件了



### 使用

#### pom文件

```xml
<parent>
        <groupId>org.springframework.boot</groupId>
    	<!-- 使用最新版springboot -->
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.3.0.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
</parent>

<properties>
    <java.version>1.8</java.version>
    <!-- 使用最新版springcloud -->
    <spring-cloud.version>Hoxton.SR4</spring-cloud.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter</artifactId>
    </dependency>
    <!-- 引用eureka注册中心服务器 -->
    <!-- 由于服务器有web模块，不需要再度引用 -->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
    </dependency>
	<!-- 为了引入@Data 和 @Slf4j -->
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <optional>true</optional>
    </dependency>
</dependencies>

<!-- 引入springcloud对组件版本的管理 -->
<dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework.cloud</groupId>
                <artifactId>spring-cloud-dependencies</artifactId>
                <version>${spring-cloud.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
</dependencyManagement>
```



#### application.yml

```yml
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/
    # 表示是否把该服务器注册到eureka上面，默认true
    register-with-eureka: false

  server:
    # 表示不检测频率
    enable-self-preservation: false

server:
  port: 8761

spring:
  application:
    name: eureka

```



#### 启动文件

```java
// 由于我们使用组件一般都会打开注册中心，所以建议打包成jar包，然后用命令行打开
// 我是打包后使用批处理放在桌面，每次都可以直接打开
@SpringBootApplication
// 一定要有这个标识
@EnableEurekaServer
public class EurakalearnApplication {

    public static void main(String[] args) {
        SpringApplication.run(EurakalearnApplication.class, args);
    }

}
```



#### 服务器互相注册实现高可用

在服务器项目中把server-port配置注释掉，因为我们打算用tomcat来配置端口，实现代码重用：

```yml
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/
    # 表示是否把该服务器注册到eureka上面，默认true
    register-with-eureka: false

  server:
    # 表示不检测频率
    enable-self-preservation: false

# server:
#  port: 8761

spring:
  application:
    name: eureka

```

![](E:\git\WexNote\Markdown note\imgs\1590289619(1).jpg)

进去复制两个启动器，通过修改配置以启动不同的端口：

![](E:\git\WexNote\Markdown note\imgs\1590289662(1).jpg)

启动时，服务器要注册的地址要是对方的服务器地址。如果是三个，那么就多加一个，文件就会被修改为：

```yml
eureka:
  client:
    service-url:
    	# 多个用逗号分割
      defaultZone: http://localhost:8761/eureka/,http://localhost:8762/eureka/
    register-with-eureka: false

spring:
  application:
    name: server  
```



#### 客户端

```xml
<!-- client下需要导入web模块，不然会自动停止 -->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```

```yml
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/,http://localhost:8762/eureka/
#  instance:
#    hostname: clientName

spring:
  application:
    name: client
```



#### 获取其他服务信息的方法

```java
@RestController
@Slf4j
public class DemoController {

    @GetMapping("/msg")
    public String returnMessage(){
        return "This is a message";
    }

    @GetMapping("/getMsg")
    public String getMessage(){
        RestTemplate restTemplate = new RestTemplate();
        // 第一种方式，但是url被写死了，而且也没有实现高可用
        String response = restTemplate.getForObject("http://localhost:8080/msg", String.class);

        log.info("response: {}", response);
        return response;
    }
}

----------------------------------------------------------------------------------------------

@RestController
@Slf4j
public class DemoController {
    @Autowired
    private LoadBalancerClient loadBalancerClient;

    @GetMapping("/msg")
    public String returnMessage(){
        return "This is a message";
    }

    @GetMapping("/getMsg")
    public String getMessage(){
        RestTemplate restTemplate = new RestTemplate();
        // 第二种方式，载入对应的服务名
        ServiceInstance serviceInstance = loadBalancerClient.choose("CLIENT");
        // url使用拼接完成
        String url = String.format("http://%s:%s",serviceInstance.getHost(), serviceInstance.getPort()) + "/msg";
        String response = restTemplate.getForObject(url, String.class);

        log.info("response: {}", response);
        return response;
    }
}

------------------------------------------------------------------------------------------------------
// 需要先配置
@Configuration
public class ClientConfig {

    @Bean
    // 使用该注解之后，注入RestTemplate可以直接通过服务名访问
    @LoadBalanced
    public RestTemplate restTemplate(){
        return new RestTemplate();
    }
}
    
    
@RestController
@Slf4j
public class DemoController {
    @Autowired
    private RestTemplate restTemplate;
    
    @GetMapping("/msg")
    public String returnMessage(){
        return "This is a message";
    }

    @GetMapping("/getMsg")
    public String getMessage(){
        // 第三种方式
        String response = restTemplate.getForObject("http://CLIENT/msg", String.class);

        log.info("response: {}", response);
        return response;
    }
}
```



### 负载均衡

使用**客户端发现机制**，客户端会向服务器拉取已注册的服务信息，然后选择所要的服务和负载均衡策略直接命中服务器。

由于上述第三种方法和第二种方法原理上是一致的，但是注解方式难以去理解，所以我们采用第二方式为例，以源码来追踪其实现的负载均衡机制。

```java
// 我们进入重点的choose()方法
ServiceInstance serviceInstance = loadBalancerClient.choose("CLIENT");

// 我们发现这是一个接口
public interface ServiceInstanceChooser {

	/**
	 * Chooses a ServiceInstance from the LoadBalancer for the specified service.
	 * @param serviceId The service ID to look up the LoadBalancer.
	 * @return A ServiceInstance that matches the serviceId.
	 */
	ServiceInstance choose(String serviceId);

}

// 我们找到对应的实现类RibbonLoadBalancerClient
// 打开图表我们可以查看继承关系：RibbonLoadBalancerClient -> LoadBalancerClient -> ServiceInstanceChooser
public class RibbonLoadBalancerClient implements LoadBalancerClient {
    // 第一个参数为服务的名字，第二个为所要实例化的对象
    public ServiceInstance choose(String serviceId, Object hint) {
        // 查找注册的服务列表，从中查找到服务
		Server server = getServer(getLoadBalancer(serviceId), hint);
        // 如果找不到对应服务就返回null
		if (server == null) {
			return null;
		}
		return new RibbonServer(serviceId, server, isSecure(server, serviceId),
				serverIntrospector(serviceId).getMetadata(server));
	}
    
    protected ILoadBalancer getLoadBalancer(String serviceId) {
    return this.clientFactory.getLoadBalancer(serviceId);
	}
    
    // 通过名字获取对应的负载均衡器
    public ILoadBalancer getLoadBalancer(String name) {
        // ILoadBalancer 来源于 ribbon-loadbalancer包下，eureka跟负载有关依靠ribbon实现
        return getInstance(name, ILoadBalancer.class);
    }
    
    protected Server getServer(ILoadBalancer loadBalancer, Object hint) {
		if (loadBalancer == null) {
			return null;
		}
		// null值转换为default
		return loadBalancer.chooseServer(hint != null ? hint : "default");
	}
}


```

```java
// ILoadBalancer接口
public interface ILoadBalancer {
	
	public void addServers(List<Server> newServers);
	
	public Server chooseServer(Object key);
	
	public void markServerDown(Server server);

	@Deprecated // 已被复制
	public List<Server> getServerList(boolean availableOnly);

    public List<Server> getReachableServers();

    /**
     * 使用它获取服务列表
     * @return All known servers, both reachable and unreachable.
     */
	public List<Server> getAllServers();
}

// 找到对应的实现类BaseLoadBalancer
public class BaseLoadBalancer extends AbstractLoadBalancer implements
        PrimeConnections.PrimeConnectionListener, IClientConfigAware {
    
    protected IRule rule = DEFAULT_RULE;
    
    @Override
    public List<Server> getAllServers() {
        // 将这个list作为不能修改的返回回去
        return Collections.unmodifiableList(allServerList);
    }
    
    // 选择策略
    public Server chooseServer(Object key) {
        if (counter == null) {
            counter = createCounter();
        }
        counter.increment();
        // 默认规则DEFAULT_RULE
        // private final static IRule DEFAULT_RULE = new RoundRobinRule();
        // 由名字可以知道默认是一个轮询，可以在yml配置那里改变
        if (rule == null) {
            return null;
        } else {
            try {
                return rule.choose(key);
            } catch (Exception e) {
                logger.warn("LoadBalancer [{}]:  Error choosing server for key {}", name, key, e);
                return null;
            }
        }
    }
}
```



### 服务间通信的其他方式：feign

> - 生命性REST客户端（伪RPC）
> - 采用了基于接口的注解

**引入依赖：**

```xml
<!--旧版本，具体多少之前不知道，都试试吧-->
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-feign</artifactId>
</dependency>

<!--新版本，也是我这个的版本-->
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```

**启动类加上标识注释：**

```java
@SpringBootApplication
@EnableDiscoveryClient
@EnableFeignClients
public class EurekaClientApplication {

    public static void main(String[] args) {
        SpringApplication.run(EurekaClientApplication.class, args);
    }

}
```

**设置FeignClient接口：**

```java
@FeignClient(name = "client")
public interface MsgClient {
    // 对应的服务的路径
    @GetMapping("/msg")
    public String getMessage();
}

```

**controller类直接调用连接：**

```java
@Autowired
private MsgClient msgClient;

@GetMapping("/getMsg2")
public String getMessage2(){
    // 封装成一个类调用，但是实际上也是利用http请求
    String response = msgClient.getMessage();
    log.info("response: {}", response);
    return response;
}
```



## Config

Spring Cloud Config项目是一个解决分布式系统的**配置管理方案**。它包含了**Client**和**Server**两个部分，server提供配置文件的存储、以接口的形式将配置文件的内容提供出去，client通过接口获取数据、并依据此数据初始化自己的应用。

### 导入相应依赖

```xml
<!-- pom.xml -->
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Hoxton.SR4</spring-cloud.version>
</properties>

<dependencies>
    <!-- config服务器包 -->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-config-server</artifactId>
    </dependency>
    
    <!-- 也要注册到eureka之上 -->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
    </dependency>
</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

```yaml
# application.yml
spring:
  application:
  	# 注册上去的服务名
    name: config
  # 配置网上拉取配置文件的地址，用户名和密码，可以设置加载到本地的路径
  cloud:
    config:
      server:
        git:
          uri: https://gitee.com/wextree/config-repo
          username: Wextree
          password: hawxe20151116.
          basedir: E:\softprograme\java\code\SpringCloud\config_demo\configdir

eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/
```

```java
// ConfigDemoApplication.class
@SpringBootApplication
@EnableDiscoveryClient
// 主类要加上标识
@EnableConfigServer
public class ConfigDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(ConfigDemoApplication.class, args);
    }
}
```

### 打开服务器获取配置文件

在码云上新建一个仓库

![](E:\git\WexNote\Markdown note\imgs\fe27447f2391e3a100cfcb1d3df6fa5.png)

打开浏览器输入对应URI，获取对应的配置文件

格式：{label}/{name}-{profile}.xxxx

- label：分支（可以省略）
- name：文件名
- profile：激活环境（必须要，如果没有设置那么也要随便加上一个）
- xxxx：对应后缀名，代表格式

![](E:\git\WexNote\Markdown note\imgs\1590580635(1).jpg)

**进行访问**

![](E:\git\WexNote\Markdown note\imgs\1590580725(1).jpg)

换个格式：（会自动帮我们转换的）

![](E:\git\WexNote\Markdown note\imgs\1590580771(1).jpg)

![](E:\git\WexNote\Markdown note\imgs\1590580850(1).jpg)



### 客户端配置

```xml
<!-- pom.xml -->
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Hoxton.SR4</spring-cloud.version>
</properties>

<dependencies>
    <!-- client下需要导入web模块，不然会自动停止 -->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
    </dependency>

    <!-- 客户端所需要的包 -->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-config-client</artifactId>
    </dependency>

    <!-- 非必要包 -->
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
    </dependency>

    <!-- 不是必要的，只是直接使用了之前的eureka客户端那个项目 -->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-openfeign</artifactId>
    </dependency>
</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

```yaml
# bootstrap.yml
# 这里相对于之前有比较大的改变，主要有两点
# 第一文件名改为bootstrap而不是application（区别见后详细介绍）
# 第二，我们把以前的一部分配置都删除了，为了展示从config服务中获取配置
spring:
  application:
    name: client
  cloud:
    config:
      discovery:
        enabled: true
        # 这是注册到eureka上面的id
        service-id: CONFIG
      # 激活环境
      profile: dev

server:
  port: 8081
```

```java
// EurekaClientApplication.java
// 不需要多增加标识
@SpringBootApplication
@EnableDiscoveryClient
@EnableFeignClients
public class EurekaClientApplication {

    public static void main(String[] args) {
        SpringApplication.run(EurekaClientApplication.class, args);
    }

}

```

运行之后就可以正常启动啦

![](E:\git\WexNote\Markdown note\imgs\1590581448(1).jpg)



### application和bootstrap的区别

- **加载顺序：**若application.yml 和bootstrap.yml 在同一目录下：bootstrap.yml 先加载 application.yml后加载
  
- bootstrap.yml 用于应用程序**上下文的引导阶段**。bootstrap.yml 由父Spring ApplicationContext加载。
  
- **配置区别：**
  - bootstrap.yml 用来程序引导时执行，应用于更加早期配置信息读取。可以理解成**系统级别**的一些参数配置，这些参数一般是不会变动的。一旦bootStrap.yml 被加载，则**内容不会被覆盖**。
  - application.yml 可以用来定义**应用级别**的， 应用程序特有配置信息，可以用来配置后续各个模块中需使用的公共参数等。

- **属性覆盖：**

  启动上下文时，**Spring Cloud** 会创建一个 Bootstrap Context，作为 Spring 应用的 Application Context 的父上下文。

  初始化的时候，Bootstrap Context 负责从外部源加载配置属性并解析配置。这两个上下文共享一个从外部获取的 Environment。**Bootstrap 属性有高优先级，默认情况下，它们不会被本地配置覆盖**。

  也就是说如果加载的 application.yml 的内容标签与 bootstrap 的标签一致，application 也不会覆盖 bootstrap，而 application.yml 里面的内容可以动态替换。

  

### springcloud Bus 配置刷新

#### 相关配置

```xml
<!--结合rabbitMQ-->
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-bus-amqp</artifactId>
</dependency>

<!-- 之后为了开放monitor接口给webhooks使用 -->
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-config-monitor</artifactId>
</dependency>
```

打开rabbitMQ（具体下载和安装使用在官网都有说明），和server服务器，可以看到已有队列：

![](E:\git\WexNote\Markdown note\imgs\1590673738(1).jpg)



 #### 相应准备

我们写一个类和controller来接收配置中的信息，方便我们感受更改网上配置，刷新直接可以返回新的信息。

```yaml
# 网上仓库的配置文件，记得把上面码云的重新拷贝到github，因为码云暂时没有办法很好适配spring
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/,http://localhost:8762/eureka/

spring:
  application:
    name: client
        
env: dev

# 新增部分，对应下面的类
girl:
  name: wextree
  age: 30
```

```java
// 写在客户端
@Data
@Component
// 获取配置中的信息
@ConfigurationProperties("girl")
// 启动自动刷新
@RefreshScope
public class GirlConfig {
    private String name;
    private Integer age;
}

// 只是为了可以调用url返回信息
@RestController
@RequestMapping("/girl")
public class GirlController {
    @Autowired
    private GirlConfig girlConfig;

    @GetMapping("/print")
    public String print(){
        return "name: " + girlConfig.getName() + "  age: " + girlConfig.getAge();
    }
}

```



#### 使用natapp进行内网穿透  

因为我们用主机当做服务器，github的webhook发送过来的消息无法直接传给127.0.0.1。所以我们需要暂时将我们的内网公开访问，设定一个暂时的域名。

官网下载：（需要下载客户端）https://natapp.cn/

上面有使用教程等，只需要购买一个免费的域名，利用它提供的密钥登陆即可。

接下来说的webhook里面的那串很乱的域名就是我利用这个申请的。



#### 配置webhook

利用github上的webhook在仓库更新时会自动发送消息，然后将config服务器开放对应的monitor接口，实时收取信息并且进行更新：

打开对应的仓库，进入设置：

![](E:\git\WexNote\Markdown note\imgs\6929927dd246a1dede9d622874f4ed7.png)

这时候我们就可以打开服务器和客户端进行访问啦

![1590674566857](C:\Users\58354\AppData\Roaming\Typora\typora-user-images\1590674566857.png)

![1590674580418](C:\Users\58354\AppData\Roaming\Typora\typora-user-images\1590674580418.png)

但是，我们更改仓库中的配置文件，刷新客户端程序，根本没有任何改变。

这就是我遇到的一个大坑，这是新版本的一个新规定。



#### ServiceID

接着上述的问题，当我们修改仓库中的配置信息，查看输出日志发现配置器服务端其实是收到了webhook的更新消息，并且刷新了配置的信息，但是却显示了204（即无内容）。

- **设置客户端打印日志级别进行代码排查：**

  ```yaml
  logging:
    level:
      org.springframework.cloud.bus: debug
  ```

  ```verilog
  In match: config:0:*, config:0:303d037c4a07208637a35314ef53d92f
  匹配不上，默认default
  ```

应用有一个**ServiceID**，默认的值是app:index:id的组装。

```
 app ：如果vcap.application.name存在，使用vcap.application.name，否则使用spring.application.name（默认值为application）
 index ：配置值的情况下优先使用vcap.application.instance_index，否则依次使用spring.application.index、local.server.port、server.port（默认值0）
 id: 如果vcap.application.instance_id存在，使用vcap.application.instance_id，否则给一个随机值
```

官方文档：

![](E:\git\WexNote\Markdown note\imgs\20190603110245704.png)

对应官方文档，以及我们的配置文件，我们可以依据serviceID的匹配规则来设置对应的参数去匹配webhooks。

如果发现app:index:id中的index不一致， 举例yml配置：

```yaml
vcap:
  application:
    instance_index: ${spring.cloud.config.profile}
```

或者直接修改bus.id的配置，如下：

```yaml
spring:
  application:
    name: client
  cloud:
    config:
      discovery:
        service-id: CONFIG
        enabled: true
      profile: dev 
    bus:
      id: {spring.application.name}:{spring.cloud.config.profile}:${random.value}
```



## Zuul

Zuul是**Netflix**开源的微服务网关，可以和Eureka、Ribbon、Hystrix等组件配合使用，Spring Cloud对Zuul进行了整合与增强，Zuul默认使用的HTTP客户端是Apache HTTPClient，也可以使用**RestClient**或**okhttp3.OkHttpClient**。  Zuul的主要功能是**路由转发和过滤器**。路由功能是微服务的一部分，比如/demo/test转发到到demo服务。zuul默认和Ribbon结合实现了负载均衡的功能

### 工作原理

zuul的核心是一系列的filters, 其作用类比Servlet框架的Filter，或者AOP。zuul把请求路由到用户处理逻辑的过程中，这些filter参与一些过滤处理，比如Authentication，Load Shedding等
![](E:\git\WexNote\Markdown note\imgs\17866147-0cd4b2b96f649f4f.webp)

- **功能：**
  - **身份验证和安全性 - 确定每个资源的身份验证要求并拒绝不满足这些要求的请求**
  - **洞察和监控 - 在边缘跟踪有意义的数据和统计数据，以便为我们提供准确的生产视图**
  - **动态路由 - 根据需要动态地将请求路由到不同的后端群集**
  - **压力测试 - 逐渐增加群集的流量以衡量性能。**
  - **Load Shedding - 为每种类型的请求分配容量并删除超过限制的请求**
  - **静态响应处理 - 直接在边缘构建一些响应，而不是将它们转发到内部集群**

- **生命周期：**

![](E:\git\WexNote\Markdown note\imgs\17866147-7cebc241af721a00.webp)



### 组件

- **zuul-core--zuul核心库，包含编译和执行过滤器的核心功能**。

- **zuul-simple-webapp--zuul Web应用程序示例，展示了如何使用zuul-core构建应用程序**。

- **zuul-netflix--lib包，将其他NetflixOSS组件添加到Zuul中，例如使用功能区进去路由请求处理**。

- **zuul-netflix-webapp--webapp，它将zuul-core和zuul-netflix封装成一个简易的webapp工程包**。



### 实现

**新建一个项目，导入依赖：**

```xml
<!-- pom.xml -->
<properties>
        <java.version>1.8</java.version>
        <spring-cloud.version>Hoxton.SR4</spring-cloud.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-config</artifactId>
        </dependency>
        
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
        </dependency>
        
        <!-- 新增核心组件 -->
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-netflix-zuul</artifactId>
        </dependency>
        
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-actuator</artifactId>
        </dependency>
    </dependencies>
```



**结合config组件开启配置，静态配置写在里面，动态配置写在外面：**

```yaml
# bootstrap.yml
spring:
  application:
    name: api-gateway
  cloud:
    config:
      discovery:
        enabled: true
        # 这是注册到eureka上面的id
        service-id: CONFIG
      profile: dev
    bus:
      id: ${spring.application.name}:${spring.cloud.config.profile}:${random.value}

server:
  port: 8082
  
# config  api-gateway-dev.yml
# 自定义配置规则
zuul:
  routes:
#    client: /myClient/**
#    跟着定义的规则的名字
    myClient:
      path: /myClient/**
      serviceId: client

# 访问/actuator/routes可以查看所有的路由规则
# ========添加配置=======
management:
  endpoints:
    web:
      exposure:
        include: "*" #注意这里*要加引号，暴露全部，也可以只暴露相应endpoint
  endpoint:
    routes:
      enabled: true # 默认为true，可以省略
      
env: dev
```

**主配置类：**

```java
// 主配置类
@SpringBootApplication
// 新增类
@EnableZuulProxy
public class ApiGatewayApplication {

    public static void main(String[] args) {
        SpringApplication.run(ApiGatewayApplication.class, args);
    }

    // 为了实现动态更新
    @ConfigurationProperties("zuul")
    @RefreshScope
    public ZuulProperties zuulProperties(){
        return new ZuulProperties();
    }
}
```

**同时启动客户端，配置组件服务端，还有这个zuul组件的服务器三个：**

可以通过zuul服务器访问客户端（和直接访问客户端一致）：

![](E:\git\WexNote\Markdown note\imgs\1590754408(1).jpg)

![](E:\git\WexNote\Markdown note\imgs\1590754462(1).jpg)



### 无法传递cookie问题

由于这个zuul组件路由对于cookie等一些敏感信息会进行过滤，这个可以从zuul路由器的配置类看出。

```java
@ConfigurationProperties("zuul")
public class ZuulProperties {
    private Set<String> sensitiveHeaders = new LinkedHashSet<>(
			Arrays.asList("Cookie", "Set-Cookie", "Authorization"));
    
    public void setSensitiveHeaders(Set<String> sensitiveHeaders) {
		this.sensitiveHeaders = sensitiveHeaders;
	}
}
```

**所以我们可以在配置那里设置敏感头部位空：**

```yml
zuul:
  routes:
#    client: /myClient/**
#    跟着定义的规则的名字
    myClient:
#      默认不添加cookie，需要设置为空
      sensitiveHeaders:
```



### 实现权限过滤

由于我们每个服务可能都要进行权限的过滤，但是我们要是一个个加上权限实在是太麻烦了，所以不如加在zuul微服务网关上。

**实现要求：**当url带有token时，可以正常访问，如果没有，无法访问并且401 。

在网关服务器**增加一个filte**r实现功能：

```java
package com.wex.apigateway.filter;

import com.netflix.zuul.ZuulFilter;
import com.netflix.zuul.context.RequestContext;
import com.netflix.zuul.exception.ZuulException;
import org.apache.http.HttpStatus;
import org.springframework.util.StringUtils;

import javax.servlet.http.HttpServletRequest;

import static org.springframework.cloud.netflix.zuul.filters.support.FilterConstants.PRE_DECORATION_FILTER_ORDER;
import static org.springframework.cloud.netflix.zuul.filters.support.FilterConstants.PRE_TYPE;

// 创建一个Zuul过滤器
// 所有参数及意义可以打开org.springframework.cloud.netflix.zuul.filters.support.FilterConstants查看
@Component
public class TokenFilter extends ZuulFilter {

    @Override
    public String filterType() {
        // public static final String PRE_TYPE = "pre";
        return PRE_TYPE;
    }

    @Override
    public int filterOrder() {
        // 在其之前-1
        // public static final int PRE_DECORATION_FILTER_ORDER = 5;
        return PRE_DECORATION_FILTER_ORDER-1;
    }

    @Override
    public boolean shouldFilter() {
        return true;
    }

    @Override
    public Object run() throws ZuulException {
        // 获取当前上下文
        RequestContext requestContext = RequestContext.getCurrentContext();

        // 获取参数
        HttpServletRequest request = requestContext.getRequest();
        String token = request.getParameter("token");
        if (StringUtils.isEmpty(token)){
            requestContext.setSendZuulResponse(false);
            requestContext.setResponseStatusCode(HttpStatus.SC_UNAUTHORIZED); // 401
        }

        return null;
    }
}

```

直接打开访问：

![](E:\git\WexNote\Markdown note\imgs\1590757485577.png)

带着token：

![](E:\git\WexNote\Markdown note\imgs\1590757528(1).jpg)



 ### 实现post请求返回

还是需要建立一个filter

```java
@Component
public class AddResponseHeaderFilter extends ZuulFilter {
    @Override
    public String filterType() {
        return POST_TYPE;
    }

    @Override
    public int filterOrder() {
        return SEND_RESPONSE_FILTER_ORDER-1;
    }

    @Override
    public boolean shouldFilter() {
        return true;
    }

    @Override
    public Object run() throws ZuulException {
        RequestContext requestContext = RequestContext.getCurrentContext();
        HttpServletResponse response = requestContext.getResponse();
        response.setHeader("uuid", UUID.randomUUID().toString());
        return null;
    }
}
```

![](E:\git\WexNote\Markdown note\imgs\1590758227(1).jpg)



### 令牌桶算法实现限流

设定一个桶，按一定速率放入令牌（token），拿到令牌就可以进行，拿不到就没办法。

我们利用其他组件已经写好的算法：

```java
package com.wex.apigateway.filter;

import com.google.common.util.concurrent.RateLimiter;
import com.netflix.zuul.ZuulFilter;
import com.netflix.zuul.exception.ZuulException;
import com.wex.apigateway.exception.RateLimiterException;
import org.springframework.stereotype.Component;

import static org.springframework.cloud.netflix.zuul.filters.support.FilterConstants.PRE_TYPE;
import static org.springframework.cloud.netflix.zuul.filters.support.FilterConstants.SERVLET_DETECTION_FILTER_ORDER;

@Component
public class RateLimiterFilter extends ZuulFilter {
    // 每秒多少个组件
    private static final RateLimiter RATE_LIMITER = RateLimiter.create(100);

    @Override
    public String filterType() {
        return PRE_TYPE;
    }

    @Override
    public int filterOrder() {
        // 一定要在最开始，所以我们设定在当前最高优先级还要高
        return SERVLET_DETECTION_FILTER_ORDER-1;
    }

    @Override
    public boolean shouldFilter() {
        return true;
    }

    @Override
    public Object run() throws ZuulException {
        // 如果没有拿到令牌
        if (!RATE_LIMITER.tryAcquire()){
            throw new RateLimiterException();
        }
        return null;
    }
}

public class RateLimiterException extends RuntimeException{
}
```



### 跨域

当我们对某个接口或者方法和类进行跨域声明时，我们可以加上注解

`@CrossOrigin(allowCredentials = "true")`

表示该方法允许跨域访问，并且可以传输cookie

但是当我们应用有很多方法和类时，这个不适用，所以我们可以利用网关进行统一管理

我们在zuul里面新建一个config/CorsConfig跨域配置：

```java
package com.wex.apigateway.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;
import org.springframework.web.filter.CorsFilter;

import java.util.Arrays;

@Configuration
public class CorsConfig {
    @Bean
    public CorsFilter corsFilter(){
        final UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
        final CorsConfiguration configuration = new CorsConfiguration();
        // 允许cookie传输
        configuration.setAllowCredentials(true);
        // 允许的原始域名
        configuration.setAllowedOrigins(Arrays.asList("*"));
        // 允许的头
        configuration.setAllowedHeaders(Arrays.asList("*"));
        // 设置跨域时间，一定时间内对相同的域不再检查
        configuration.setMaxAge(300l);

        // 设置跨域路径和配置
        source.registerCorsConfiguration("/**", configuration);

        return new CorsFilter(source);
    }
}

```



## Hystrix

### 雪崩效应

在微服务架构中通常会有多个服务层调用，基础服务的故障可能会导致级联故障，进而造成整个系统不可用的情况，这种现象被称为服务雪崩效应。服务雪崩效应是一种因“服务提供者”的不可用导致“服务消费者”的不可用,并将不可用逐渐放大的过程。



### 摘要

Spring Cloud Hystrix 是Spring Cloud Netflix 子项目的核心组件之一，具有服务容错及线程隔离等一系列服务保护功能，本文将对其用法进行详细介绍。



### Hystrix 简介

在微服务架构中，服务与服务之间通过远程调用的方式进行通信，一旦某个被调用的服务发生了故障，其依赖服务也会发生故障，此时就会发生故障的蔓延，最终导致系统瘫痪。Hystrix实现了断路器模式，当某个服务发生故障时，通过断路器的监控，给调用方返回一个错误响应，而不是长时间的等待，这样就不会使得调用方由于长时间得不到响应而占用线程，从而防止故障的蔓延。Hystrix具备服务降级、服务熔断、线程隔离、请求缓存、请求合并及服务监控等强大功能。




### 降级

当有一些服务挂掉了或者是连接不上，可以实现降级，然后引导对应的页面或者方法上。

- **简单实现一个客户端，实现对应的接口测试调用：**

  ```java
  @RestController
  @RequestMapping("/user")
  public class UserController {
  
      @GetMapping("/msg")
      public User userMsg(){
          return new User("zhangsan", 23, true);
      }
  }
  ```

  

- **另一个客户端访问该接口：**

  ```xml
  <!-- 新增依赖 -->
  <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-starter-netflix-hystrix</artifactId>
  </dependency>
  ```

  ```java
  // 主配置类
  //@SpringBootApplication
  //@EnableDiscoveryClient
  //@EnableCircuitBreaker
  @SpringCloudApplication  // 该注解就包含了上述三个注解
  @EnableFeignClients
  public class EurekaClientApplication {
  
      public static void main(String[] args) {
          SpringApplication.run(EurekaClientApplication.class, args);
      }
  
  }
  ```

  

  ```java
  @RestController
  @RequestMapping("/hystrix")
  @Slf4j
  // 默认的降级策略
  //@DefaultProperties(defaultFallback = "yourFallbackName")
  public class HystrixController {
  
      @Autowired
      private LoadBalancerClient loadBalancerClient;
  
      @GetMapping("/demo")
      @HystrixCommand(fallbackMethod = "fallback", commandProperties = {
          	// 这是设置超时时间，要去HystrixProperties里面找需要的名字和值，一般比较长
              @HystrixProperty(name = "execution.isolation.thread.timeoutInMilliseconds", value = "3000")
      }) // 表示找不到 url 之后启动的方法名
      public String getUser(){
          ServiceInstance serviceInstance = loadBalancerClient.choose("DEMO");
          String url = String.format("http://%s:%s", serviceInstance.getHost(), serviceInstance.getPort()) + "/user/msg";
          log.info("URL: {}",url);
          User user = new RestTemplate().getForObject(url, User.class);
          log.info("USER: {}",user.toString());
  //        我们可以通过自己在内部抛出异常触发降级
  //        throw new RuntimeException("test");
          return user.toString();
      }
  
      // 对应的降级策略，该方法对应的返回值要与调用它的方法的返回值一致
      public String fallback(){
          return "is to busy!";
      }
  }
  ```

  

![](E:\git\WexNote\Markdown note\imgs\1590979089(1).jpg)



### 熔断

熔断器的原理很简单，如同电力过载保护器。它可以实现快速失败，如果它在一段时间内侦测到许多类似的错误，会强迫其以后的多个调用快速失败，不再访问远程服务器，从而防止应用程序不断地尝试执行可能会失败的操作，使得应用程序继续执行而不用等待修正错误，或者浪费CPU时间去等到长时间的超时产生。熔断器也可以使应用程序能够诊断错误是否已经修正，如果已经修正，应用程序会再次尝试调用操作。

熔断器开关相互转换的逻辑如下图：

![](E:\git\WexNote\Markdown note\imgs\hystrix-2.png)



断路器很好理解, 当Hystrix Command请求后端服务**失败数量超过一定比例**(默认50%), 断路器会切换到开路状态(Open). 这时所有请求会直接失败而不会发送到后端服务. 断路器保持在**开路状态一段时间后(**默认5秒), 自动切换到**半开路状态**(HALF-OPEN). 这时会判断下一次请求的返回情况, 如果请求成功, 断路器切回闭路状态(CLOSED), 否则重新切换到开路状态(OPEN). Hystrix的断路器就像我们家庭电路中的保险丝, 一旦后端服务不可用, 断路器会直接切断请求链, 避免发送大量无效请求影响系统吞吐量, 并且断路器有自我检测并恢复的能力.

- **熔断器相关配置：**

  ```java
  @HystrixCommand(commandProperties = {
              // 熔断开关
              @HystrixProperty(name = "circuitBreaker.enabled", value = "true"),
              // 请求数量达到后开启熔断机制
              @HystrixProperty(name = "circuitBreaker.requestVolumeThreshold", value = "10"),
              // 熔断打开后等待n秒进入半关闭状态
              @HystrixProperty(name = "circuitBreaker.sleepWindowInMilliseconds", value = "10000"),
              // 当错误率达到60%开启熔断
              @HystrixProperty(name = "circuitBreaker.errorThresholdPercentage", value = "60")
      })
  ```



- **也可以把配置放在yml配置文件中（以超时时间为例）：**

```yaml
# 将hystrix的配置移动到这里
hystrix:
  command:
    default:
      execution:
        isolation:
          thread:
            timeoutInMilliseconds: 2000
#    可以针对特定方法
    getUser:
      execution:
        isolation:
          thread:
            timeoutInMilliseconds: 1000
```



### Feign + Hystrix

开启**hystrix**配置：

```yaml
# bootstrap.yml
# feign集成了hystrix，但是需要手动打开
feign:
  hystrix:
    enabled: true
```

定义相关的feign对应的hystrix的工厂类对象（实现了对应的接口）：

```java
@Component
public class ErrorHelloImpl implements FallbackFactory<HelloClient> {

    @Override
    public HelloClient create(Throwable throwable) {
        return new HelloClient() {
            @Override
            public User getUser() {
                return new User("error", 100, false);
            }
};
   
```

写一个测试的对应**feign接口**：

```java
@FeignClient(name = "demo", fallbackFactory = ErrorHelloImpl.class)
public interface HelloClient {
    @GetMapping("/user/msg")
    public User getUser();
}
```

写一个**controller**进行访问，访问的对象是上面之前定义的**demo服务**里面的**/user/msg**功能：

```java
@RestController
public class UserController {

    @Autowired
    private HelloClient helloClient;

    @GetMapping("/user")
    public User user(){
        User user = helloClient.getUser();
        return user;
    }

}

```

查看服务，如果关闭对应服务会自动调用fallback中我们设置的对应方法：

![](E:\git\WexNote\Markdown note\imgs\1591241545(1).jpg)



### Hystrix Dashboard

导入对应的**依赖**：

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-hystrix</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-hystrix-dashboard</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```

**配置**：

```yaml
# feign集成了hystrix，但是需要手动打开
feign:
  hystrix:
    enabled: true
    
# 暴露接口
management:
  endpoints:
    web:
      exposure:
        include: ["health","info","hystrix.stream"]
```

**启动类**：

```java
@SpringCloudApplication  // 已经包含了@EnableCircuitBreaker
@EnableFeignClients
@EnableHystrixDashboard
public class EurekaClientApplication {

    public static void main(String[] args) {
        SpringApplication.run(EurekaClientApplication.class, args);
    }

    // springboot 2.0 以上需要主动去设置
    @Bean
    public ServletRegistrationBean getServlet() {
        HystrixMetricsStreamServlet streamServlet = new HystrixMetricsStreamServlet();
        ServletRegistrationBean registrationBean = new ServletRegistrationBean(streamServlet);
        registrationBean.setLoadOnStartup(1);
        // 路径默认是/actuator/hystrix.stream
        registrationBean.addUrlMappings("/hystrix.stream");
        registrationBean.setName("HystrixMetricsStreamServlet");
        return registrationBean;
    }

}
```

访问`http://localhost:8081/hystrix`可以看到界面：

![](E:\git\WexNote\Markdown note\imgs\1591261949(1).jpg)





访问 `http://localhost:8081/hystrix.stream`可以看到：

![](E:\git\WexNote\Markdown note\imgs\1591261876(1).jpg)



####Unable to connect to Command Metric Stream.问题及其解决

```java
// HystrixAutoConfiguration类

@Configuration(proxyBeanMethods = false)
@ConditionalOnWebApplication(type = SERVLET)
@ConditionalOnBean(HystrixCommandAspect.class) // only install the stream if enabled
@ConditionalOnClass({ HystrixMetricsStreamServlet.class })
@EnableConfigurationProperties(HystrixProperties.class)
protected static class HystrixServletAutoConfiguration {

    @Bean
    @ConditionalOnAvailableEndpoint
    public HystrixStreamEndpoint hystrixStreamEndpoint(HystrixProperties properties) {
        return new HystrixStreamEndpoint(properties.getConfig());
    }

    @Bean
    public HasFeatures hystrixStreamFeature() {
        return HasFeatures.namedFeature("Hystrix Stream Servlet",
                                        // 重点
                                        HystrixMetricsStreamServlet.class);
    }

}

/**
 * Streams Hystrix metrics in text/event-stream format.
 * <p>
 * Install by:
 * <p>
 * 1) Including hystrix-metrics-event-stream-*.jar in your classpath.
 * <p>
 * 2) Adding the following to web.xml:
 * <pre>{@code
 * <servlet>
 *  <description></description>
 *  <display-name>HystrixMetricsStreamServlet</display-name>
 *  <servlet-name>HystrixMetricsStreamServlet</servlet-name>
 *  <servlet-class>com.netflix.hystrix.contrib.metrics.eventstream.HystrixMetricsStreamServlet</servlet-class>
 * </servlet>
 * <servlet-mapping>
 *  <servlet-name>HystrixMetricsStreamServlet</servlet-name>
 *  <url-pattern>/hystrix.stream</url-pattern>
 * </servlet-mapping>
 * } </pre>
 */
// 需要配置这个类去设置映射路径
public class HystrixMetricsStreamServlet extends HystrixSampleSseServlet {}
```

```java
// 在配置类或启动类里面配置相关信息
@Bean
public ServletRegistrationBean getServlet() {
    HystrixMetricsStreamServlet streamServlet = new HystrixMetricsStreamServlet();
    ServletRegistrationBean registrationBean = new ServletRegistrationBean(streamServlet);
    registrationBean.setLoadOnStartup(1);
    // 路径默认是/actuator/hystrix.stream
    registrationBean.addUrlMappings("/hystrix.stream");
    registrationBean.setName("HystrixMetricsStreamServlet");
    return registrationBean;
```

还有一个**大坑**：

根据页面提示，我们应该输入：

![](E:\git\WexNote\Markdown note\imgs\1591262325(1).jpg)

但是其实不应该用**https！！！！！**

应该用你本身访问时使用的协议：**http！！！！！**



## Sleuth

Spring Cloud Sleuth为服务之间调用提供**链路追踪**。通过Sleuth可以很清楚的了解到一个服务请求经过了哪些服务，每个服务处理花费了多长。从而让我们可以很方便的理清各微服务间的调用关系。此外Sleuth可以帮助我们：

- 耗时分析: 通过Sleuth可以很方便的了解到每个采样请求的耗时，从而分析出哪些服务调用比较耗时;
- 可视化错误: 对于程序未捕捉的异常，可以通过集成Zipkin服务界面上看到;
- 链路优化: 对于调用比较频繁的服务，可以针对这些服务实施一些优化措施。



**导入依赖：**

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-sleuth</artifactId>
</dependency>
```



直接开启调用服务就会有日志打印了：

![](E:\git\WexNote\Markdown note\imgs\1591274403(1).jpg)

```verilog
# 服务名  链路的唯一标识（traceId）  链路的基本单元（spanId）  不收集服务和展示 
INFO [client,385e19698aff5c98,6f8e9fa1f2e8dec1,false] 
```



###Zipkin

Zipkin 是一个**开放源代码分布式的跟踪系统**，由Twitter公司开源，它致力于收集服务的定时数据，以解决微服务架构中的延迟问题，包括数据的收集、存储、查找和展现。

每个服务向zipkin报告计时数据，zipkin会根据调用关系通过Zipkin UI生成依赖关系图，显示了多少跟踪请求通过每个服务，该系统让开发者可通过一个 Web 前端轻松的收集和分析数据，例如用户每次请求服务的处理时间等，可方便的监测系统中存在的瓶颈。

Zipkin提供了可插拔数据存储方式：In-Memory、MySql、Cassandra以及Elasticsearch。接下来的测试为方便直接采用In-Memory方式进行存储，生产推荐Elasticsearch。

**通过docker运行zipkin：**

```shell
docker run -d -p 9411:9411 openzipkin/zipkin:2.12.3  -- 该版本比较稳定

docker ps
CONTAINER ID        IMAGE                      COMMAND                  CREATED             STATUS       
addd32dd05da        openzipkin/zipkin:2.12.3   "/bin/bash -c 'test …"   15 seconds ago      Up 12 s
PORTS 									NAMES
9410/tcp, 0.0.0.0:9411->9411/tcp		  heuristic_moore
```

> 由于我使用的是toolbox，实际上是建立在一个虚拟机上，所以我们要先获取我们的**虚拟机IP**，然后通过IP访问，不能直接通过localhost进行访问，因为根本不是在这个本地机子上运行的服务器。

![](E:\git\WexNote\Markdown note\imgs\1591278499(1).jpg)










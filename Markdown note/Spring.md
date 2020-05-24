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








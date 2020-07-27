# SpringBoot

## 自动配置原理

### SpringBoot配置文件

**SpringBoot**有各种**starter依赖**，需要什么就勾选就好了。如果需要什么自定义的配置，那么只需要直接在**配置文件中设置**就行了。

查看官网的配置样例：[官网配置](https://docs.spring.io/spring-boot/docs/2.1.3.RELEASE/reference/htmlsingle/#boot-features-external-config-yaml)

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/1588751089.png)



### 配置原理

- **通过以下示例**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/Snipaste_2020-05-06_15-47-46.png)



- **重点关注SpringBootApplication注解**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/Snipaste_2020-05-06_15-50-54.png)

- **Target**  、**Retention **、**Documented** 和 **Inherited** 注解，是**`java.lang.annotation`**包下的原生注解。（具体的内容见上面解释）

- **@SpringBootConfiguration**注解：

  ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/Snipaste_2020-05-06_16-16-58.png)

  - 除了前面几个原生的注解之外，其实最重要的就只有一个**@Configuration注解**，而这个注解在Spring中，是标注一个类，被标注的**类充当配置类**并且会被放入容器中。

- **@EnableAutoConfiguration**注解（核心）：

  ![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/Snipaste_2020-05-06_16-25-48.png)

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



## SpringBoot自动配置流程

> SpringBoot 2.3.0.RELEASE

**打上断点：**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/1591709222(1).png)

```java
// step into
public static ConfigurableApplicationContext run(Class<?> primarySource, String... args) {
		return run(new Class<?>[] { primarySource }, args);
}

// 先创建Application，后执行run
public static ConfigurableApplicationContext run(Class<?>[] primarySources, String[] args) {
		return new SpringApplication(primarySources).run(args);
}
```

```java
// 初始化SpringApplication
public SpringApplication(ResourceLoader resourceLoader, Class<?>... primarySources) {
		this.resourceLoader = resourceLoader;
    	// 判空
		Assert.notNull(primarySources, "PrimarySources must not be null");
    	// 把我们的主配置类给存起来
		this.primarySources = new LinkedHashSet<>(Arrays.asList(primarySources));
    	// 判断是不是web应用，通过判断该应用是不是有对应的类，像javax.servlet.Servlet
		this.webApplicationType = WebApplicationType.deduceFromClasspath();
    	// 获取META-INF/spring.factories路径下的ApplicationContextInitializer配置，先保存起来
		setInitializers((Collection) getSpringFactoriesInstances(ApplicationContextInitializer.class));
    	// 获取META-INF/spring.factories路径下的ApplicationListener配置，先保存起来
		setListeners((Collection) getSpringFactoriesInstances(ApplicationListener.class));
    	// 找main方法的类
		this.mainApplicationClass = deduceMainApplicationClass();
}

// 获取META-INF/spring.factories路径下的配置，先保存起来
private <T> Collection<T> getSpringFactoriesInstances(Class<T> type, Class<?>[] parameterTypes, Object... args) {
    ClassLoader classLoader = getClassLoader();
    // Use names and ensure unique to protect against duplicates
    Set<String> names = new LinkedHashSet<>(SpringFactoriesLoader.loadFactoryNames(type, classLoader));
    List<T> instances = createSpringFactoriesInstances(type, parameterTypes, classLoader, args, names);
    AnnotationAwareOrderComparator.sort(instances);
    return instances;
}

// 和下面方法一起获取对应的配置
public static List<String> loadFactoryNames(Class<?> factoryType, @Nullable ClassLoader classLoader) {
    String factoryTypeName = factoryType.getName();
    return (List)loadSpringFactories(classLoader).getOrDefault(factoryTypeName, Collections.emptyList());
}

private static Map<String, List<String>> loadSpringFactories(@Nullable ClassLoader classLoader) {
    MultiValueMap<String, String> result = (MultiValueMap)cache.get(classLoader);
    if (result != null) {
        return result;
    } else {
        try {
            // 找到该路径
            Enumeration<URL> urls = classLoader != null ? classLoader.getResources("META-INF/spring.factories") : ClassLoader.getSystemResources("META-INF/spring.factories");
            LinkedMultiValueMap result = new LinkedMultiValueMap();

            while(urls.hasMoreElements()) {
                URL url = (URL)urls.nextElement();
                UrlResource resource = new UrlResource(url);
                Properties properties = PropertiesLoaderUtils.loadProperties(resource);
                Iterator var6 = properties.entrySet().iterator();

                while(var6.hasNext()) {
                    Entry<?, ?> entry = (Entry)var6.next();
                    String factoryTypeName = ((String)entry.getKey()).trim();
                    String[] var9 = StringUtils.commaDelimitedListToStringArray((String)entry.getValue());
                    int var10 = var9.length;

                    for(int var11 = 0; var11 < var10; ++var11) {
                        String factoryImplementationName = var9[var11];
                        result.add(factoryTypeName, factoryImplementationName.trim());
                    }
                }
            }

            cache.put(classLoader, result);
            return result;
        } catch (IOException var13) {
            throw new IllegalArgumentException("Unable to load factories from location [META-INF/spring.factories]", var13);
        }
    }
}
```

```factories
# META-INF/spring.factories
# Initializers
org.springframework.context.ApplicationContextInitializer=\
org.springframework.boot.autoconfigure.SharedMetadataReaderFactoryContextInitializer,\
org.springframework.boot.autoconfigure.logging.ConditionEvaluationReportLoggingListener
```

```java
// 开始运行
public ConfigurableApplicationContext run(String... args) {
    // 停止监听
    StopWatch stopWatch = new StopWatch();
    stopWatch.start();
    ConfigurableApplicationContext context = null;
    Collection<SpringBootExceptionReporter> exceptionReporters = new ArrayList<>();
    configureHeadlessProperty();
    // 从类路径META-INF/spring.factories下获取SpringApplicationRunListeners，并且打开
    SpringApplicationRunListeners listeners = getRunListeners(args);
    // 调用所有的listener里面的回调方法starting
    listeners.starting();
    try {
        // 封装命令行参数
        ApplicationArguments applicationArguments = new DefaultApplicationArguments(args);
        // 准备环境
        ConfigurableEnvironment environment = prepareEnvironment(listeners, applicationArguments);
        configureIgnoreBeanInfo(environment);
        // 打印Spring图标
        Banner printedBanner = printBanner(environment);
        // 创建Ioc容器
        context = createApplicationContext();
        exceptionReporters = getSpringFactoriesInstances(SpringBootExceptionReporter.class,
                                                         new Class[] { ConfigurableApplicationContext.class }, context);
        // 通过刚才我们获取的环境和初始化器，监听器等，为Ioc配置上下文
        prepareContext(context, environment, listeners, applicationArguments, printedBanner);
        // 刷新Ioc容器，也就是我们Ioc容器的初始化，包括我们自己定义的bean，config之类的
        // 如果是web应用，还会创建嵌入式的tomcat应用
        refreshContext(context);
        // 从Ioc容器中获取所有的ApplicationRunner，CommandLineRunner（有优先级关系），调用它们的回调方法
        afterRefresh(context, applicationArguments);
        stopWatch.stop();
        // 日志处理
        if (this.logStartupInfo) {
            new StartupInfoLogger(this.mainApplicationClass).logStarted(getApplicationLog(), stopWatch);
        }
        // 最后调用所有SpringApplicationRunListener的回调方法started	
        listeners.started(context);
        callRunners(context, applicationArguments);
    }
    catch (Throwable ex) {
        handleRunFailure(context, ex, exceptionReporters, listeners);
        throw new IllegalStateException(ex);
    }

    try {
        listeners.running(context);
    }
    catch (Throwable ex) {
        handleRunFailure(context, ex, exceptionReporters, null);
        throw new IllegalStateException(ex);
    }
    // 结束返回Ioc容器
    return context;
}

// 现在看到getSpringFactoriesInstances，就知道是从类路径下获取
private SpringApplicationRunListeners getRunListeners(String[] args) {
    Class<?>[] types = new Class<?>[] { SpringApplication.class, String[].class };
    return new SpringApplicationRunListeners(logger,
                                             getSpringFactoriesInstances(SpringApplicationRunListener.class, types, this, args));
}

// 准备环境对应的方法
private ConfigurableEnvironment prepareEnvironment(SpringApplicationRunListeners listeners,
                                                   ApplicationArguments applicationArguments) {
    // Create and configure the environment
    // 获取或者创建环境并且进行配置
    ConfigurableEnvironment environment = getOrCreateEnvironment();
    configureEnvironment(environment, applicationArguments.getSourceArgs());
    ConfigurationPropertySources.attach(environment);
    // 调用所有SpringApplicationRunListeners的回调方法environmentPrepared()
    listeners.environmentPrepared(environment);
    bindToSpringApplication(environment);
    // 判断如果是web环境，还需要帮我们转换成web环境
    if (!this.isCustomEnvironment) {
        environment = new EnvironmentConverter(getClassLoader()).convertEnvironmentIfNecessary(environment,
                                                                                               deduceEnvironmentClass());
    }
    ConfigurationPropertySources.attach(environment);
    return environment;
}

// 创建Ioc容器
protected ConfigurableApplicationContext createApplicationContext() {
    Class<?> contextClass = this.applicationContextClass;
    if (contextClass == null) {
        try {
            // 判断是什么类型的应用，创建对应的容器
            switch (this.webApplicationType) {
                case SERVLET:
                    contextClass = Class.forName(DEFAULT_SERVLET_WEB_CONTEXT_CLASS);
                    break;
                case REACTIVE:
                    contextClass = Class.forName(DEFAULT_REACTIVE_WEB_CONTEXT_CLASS);
                    break;
                default:
                    contextClass = Class.forName(DEFAULT_CONTEXT_CLASS);
            }
        }
        catch (ClassNotFoundException ex) {
            throw new IllegalStateException(
                "Unable create a default ApplicationContext, please specify an ApplicationContextClass", ex);
        }
    }
    // 利用BeanUtils通过反射创建出Ioc容器
    return (ConfigurableApplicationContext) BeanUtils.instantiateClass(contextClass);
}

// 准备上下文
private void prepareContext(ConfigurableApplicationContext context, ConfigurableEnvironment environment,
                            SpringApplicationRunListeners listeners, ApplicationArguments applicationArguments, Banner printedBanner) {
    // 设置保存环境
    context.setEnvironment(environment);
    // 注册几个小组件
    postProcessApplicationContext(context);
    // 调用初始化时获取的初始化器的initialize方法
    applyInitializers(context);
    // 调用SpringApplicationRunListeners的回调方法contextPrepared
    listeners.contextPrepared(context);
    // 准备日志
    if (this.logStartupInfo) {
        logStartupInfo(context.getParent() == null);
        logStartupProfileInfo(context);
    }
    // Add boot specific singleton beans
    // 添加启动类的一些单例对象
    ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
    beanFactory.registerSingleton("springApplicationArguments", applicationArguments);
    if (printedBanner != null) {
        beanFactory.registerSingleton("springBootBanner", printedBanner);
    }
    if (beanFactory instanceof DefaultListableBeanFactory) {
        ((DefaultListableBeanFactory) beanFactory)
        .setAllowBeanDefinitionOverriding(this.allowBeanDefinitionOverriding);
    }
    if (this.lazyInitialization) {
        context.addBeanFactoryPostProcessor(new LazyInitializationBeanFactoryPostProcessor());
    }
    // Load the sources
    // 获取主类
    Set<Object> sources = getAllSources();
    Assert.notEmpty(sources, "Sources must not be empty");
    load(context, sources.toArray(new Object[0]));
    // 最后一步，所有的listener回调contextLoaded，表示已经启动完成了
    listeners.contextLoaded(context);
}

// afterRefresh调用的方法
private void callRunners(ApplicationContext context, ApplicationArguments args) {
    List<Object> runners = new ArrayList<>();
    runners.addAll(context.getBeansOfType(ApplicationRunner.class).values());
    runners.addAll(context.getBeansOfType(CommandLineRunner.class).values());
    AnnotationAwareOrderComparator.sort(runners);
    for (Object runner : new LinkedHashSet<>(runners)) {
        if (runner instanceof ApplicationRunner) {
            callRunner((ApplicationRunner) runner, args);
        }
        if (runner instanceof CommandLineRunner) {
            callRunner((CommandLineRunner) runner, args);
        }
    }
}
```

**经过listener starting之后：(有十个监听器启动)**

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/1591712167(1).png)



**流程概括：**

1. 首先自动配置流程的启动分为两个部分，一个是初始化一个SpringApplication对象，然后另一个是运行它。
2. 初始化对象主要是判断是否是Web项目，还有获取类路径**META-INF/spring.factories**的初始化器还有监听器，主类等。
3. 之后是run()正式启动工作。
4. 先获取类路径上的监听器，然后调用starting()方法进行统一的运行（循环调用其回调方法）。
5. 之后封装好命令行参数，调用prepareEnvironment()配置对应的环境。其实就是调用所有监听器中的回调方法environmentPrepared()，如果是web环境也要另外配置。
6. 之后是创建对应的IoC容器，判断是什么类型的容器，然后利用BeanUtils通过反射进行创建。
7. 然后为容器准备上下文配置。调用刚才初始化时候配置的初始化器的initialize方法，和监听器的contextPrepared方法，之后添加一些小的启动组件和获取对应的主类，然后调用监听器的contextLoaded方法完成创建。
8. 创建完IoC容器之后会开始创建我们的自定义类和配置类等等。
9. 从Ioc容器中获取所有的ApplicationRunner，CommandLineRunner（有优先级关系），调用它们的回调方法
10. 最后调用我们的监听器的回调方法started完成。





## SpringBoot对静态资源的映射

我们对SpringMVC的自动配置都在这个`WebMvcAutoConfiguration`类中。

```java
public class WebMvcAutoConfiguration {
    // 自动配置中非常重要的类
    public static class WebMvcAutoConfigurationAdapter implements WebMvcConfigurer {
        // 添加资源映射
        @Override
        public void addResourceHandlers(ResourceHandlerRegistry registry) {
            if (!this.resourceProperties.isAddMappings()) {
                logger.debug("Default resource handling disabled");
                return;
            }
            // resourceProperties可以设置和资源有关的参数
            Duration cachePeriod = this.resourceProperties.getCache().getPeriod();
            CacheControl cacheControl = 		this.resourceProperties.getCache().getCachecontrol().toHttpCacheControl();
            if (!registry.hasMappingForPattern("/webjars/**")) {
                customizeResourceHandlerRegistration(registry.addResourceHandler("/webjars/**")
                                                     .addResourceLocations("classpath:/META-INF/resources/webjars/")
                                                     .setCachePeriod(getSeconds(cachePeriod)).setCacheControl(cacheControl));
            }
            // private String staticPathPattern = "/**";
            String staticPathPattern = this.mvcProperties.getStaticPathPattern();
            if (!registry.hasMappingForPattern(staticPathPattern)) {
                // private String[] staticLocations = CLASSPATH_RESOURCE_LOCATIONS;
                customizeResourceHandlerRegistration(registry.addResourceHandler(staticPathPattern)
                                                     .addResourceLocations(getResourceLocations(this.resourceProperties.getStaticLocations()))
                                                     .setCachePeriod(getSeconds(cachePeriod)).setCacheControl(cacheControl));
            }
        }
    }
	
    // 配置欢迎页映射
    @Bean
    public WelcomePageHandlerMapping welcomePageHandlerMapping(ApplicationContext applicationContext,
                                                               FormattingConversionService mvcConversionService, ResourceUrlProvider mvcResourceUrlProvider) {
        WelcomePageHandlerMapping welcomePageHandlerMapping = new WelcomePageHandlerMapping(
            new TemplateAvailabilityProviders(applicationContext), applicationContext, getWelcomePage(),
            this.mvcProperties.getStaticPathPattern());
        welcomePageHandlerMapping.setInterceptors(getInterceptors(mvcConversionService, mvcResourceUrlProvider));
        welcomePageHandlerMapping.setCorsConfigurations(getCorsConfigurations());
        return welcomePageHandlerMapping;
    }
    
    // 从配置文件中获取静态欢迎页
    private Optional<Resource> getWelcomePage() {
        String[] locations = getResourceLocations(this.resourceProperties.getStaticLocations());
        return Arrays.stream(locations).map(this::getIndexHtml).filter(this::isReadable).findFirst();
    }
}
```

- 所有的**webjars**请求都在**classpath:/META-INF/resources/webjars/**找资源。

  **webjars：**以jar包的方式引入静态文件。

- `private String staticPathPattern = "/**";`访问当前项目的任何资源。

  ```java
  @ConfigurationProperties(prefix = "spring.resources", ignoreUnknownFields = false)
  public class ResourceProperties {
      private static final String[] CLASSPATH_RESOURCE_LOCATIONS = { "classpath:/META-INF/resources/",
  			"classpath:/resources/", "classpath:/static/", "classpath:/public/" };
  
  	/**
  	 * 静态资源路径，如果没有设置，则去上面的默认配置中找
  	 * Locations of static resources. Defaults to classpath:[/META-INF/resources/,
  	 * /resources/, /static/, /public/].
  	 */
  	private String[] staticLocations = CLASSPATH_RESOURCE_LOCATIONS;
  }
  ```

  - **classpath:/META-INF/resources/**
  - **classpath:/resources/**
  - **classpath:/static/**
  - **classpath:/public/**
  - 可以在配置文件中配置新的静态文件夹的路径

- 获取欢迎页：

  ```java
  // 从静态文件夹中找所有的index.html页面，而且是被/**映射
  private Resource getIndexHtml(String location) {
      return this.resourceLoader.getResource(location + "index.html");
  }
  ```



## 模板引擎Thymeleaf

**导入依赖：**

```xml
<properties>
	<!-- 官方文档提供的方式去覆盖默认的2版本 -->
    <thymeleaf.version>3.0.10.RELEASE</thymeleaf.version>
    <!-- 模板的支持程序，thymeleaf3版本以上需要2以上的支持程序 -->
    <thymeleaf-layout-dialect.version>2.1.1</thymeleaf-layout-dialect.version>
</properties>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-thymeleaf</artifactId>
</dependency>
```

### 使用 & 语法

```java
// ThymeleafProperties
private static final Charset DEFAULT_ENCODING = StandardCharsets.UTF_8;

public static final String DEFAULT_PREFIX = "classpath:/templates/";

public static final String DEFAULT_SUFFIX = ".html";
```

- 只要我们把**html页面**放在**classpath:/templates/**路径下，themyleaf就会制动渲染。

- **导入thymeleaf的名称空间，为了有提示：**

  ```html
  <html lang="en" xmlns:th="http://www.thymeleaf.org">
  ```

- **语法：**

  ```html
  <!-- 将div里面的文本设置为，会覆盖本来的内容 -->
  <div th:text="${hello}"></div>
  ```

  ![](https://gitee.com/wextree/Wex_imgs/raw/master/img/2018-02-04_123955.png)

- **表达式：**

  ```properties
  Simple expressions:（表达式语法）
      Variable Expressions: ${...}：获取变量值；OGNL；
      		1）、获取对象的属性、调用方法
      		2）、使用内置的基本对象：
      			#ctx : the context object.
      			#vars: the context variables.
                  #locale : the context locale.
                  #request : (only in Web Contexts) the HttpServletRequest object.
                  #response : (only in Web Contexts) the HttpServletResponse object.
                  #session : (only in Web Contexts) the HttpSession object.
                  #servletContext : (only in Web Contexts) the ServletContext object.
                  
                  ${session.foo}
              3）、内置的一些工具对象：
  #execInfo : information about the template being processed.
  #messages : methods for obtaining externalized messages inside variables expressions, in the same way as they would be obtained using #{…} syntax.
  #uris : methods for escaping parts of URLs/URIs
  #conversions : methods for executing the configured conversion service (if any).
  #dates : methods for java.util.Date objects: formatting, component extraction, etc.
  #calendars : analogous to #dates , but for java.util.Calendar objects.
  #numbers : methods for formatting numeric objects.
  #strings : methods for String objects: contains, startsWith, prepending/appending, etc.
  #objects : methods for objects in general.
  #bools : methods for boolean evaluation.
  #arrays : methods for arrays.
  #lists : methods for lists.
  #sets : methods for sets.
  #maps : methods for maps.
  #aggregates : methods for creating aggregates on arrays or collections.
  #ids : methods for dealing with id attributes that might be repeated (for example, as a result of an iteration).
  
      Selection Variable Expressions: *{...}：选择表达式：和${}在功能上是一样；
      	补充：配合 th:object="${session.user}：
     <div th:object="${session.user}">
      <p>Name: <span th:text="*{firstName}">Sebastian</span>.</p>
      <p>Surname: <span th:text="*{lastName}">Pepper</span>.</p>
      <p>Nationality: <span th:text="*{nationality}">Saturn</span>.</p>
      </div>
      
      Message Expressions: #{...}：获取国际化内容
      Link URL Expressions: @{...}：定义URL；
      		@{/order/process(execId=${execId},execType='FAST')}
      Fragment Expressions: ~{...}：片段引用表达式
      		<div th:insert="~{commons :: main}">...</div>
      		
  Literals（字面量）
        Text literals: 'one text' , 'Another one!' ,…
        Number literals: 0 , 34 , 3.0 , 12.3 ,…
        Boolean literals: true , false
        Null literal: null
        Literal tokens: one , sometext , main ,…
  Text operations:（文本操作）
      String concatenation: +
      Literal substitutions: |The name is ${name}|
  Arithmetic operations:（数学运算）
      Binary operators: + , - , * , / , %
      Minus sign (unary operator): -
  Boolean operations:（布尔运算）
      Binary operators: and , or
      Boolean negation (unary operator): ! , not
  Comparisons and equality:（比较运算）
      Comparators: > , < , >= , <= ( gt , lt , ge , le )
      Equality operators: == , != ( eq , ne )
  Conditional operators:条件运算（三元运算符）
      If-then: (if) ? (then)
      If-then-else: (if) ? (then) : (else)
      Default: (value) ?: (defaultvalue)
  Special tokens:
      No-Operation: _ 
  ```

  



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



## SpringBoot创造Restful风格接口

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/4807654-e2d12ab79ceed5c2.webp)

### 特点

1. URL描述资源
2. 使用HTTP方法描述行为。使用HTTP状态码来表示不同的结果
3. 使用json交互数据
4. RESTful只是一种风格，并不是强制的标准



###实现

下面我们尝试使用Spring MVC来实现一组对User对象操作的RESTful API，配合注释详细说明在Spring MVC中如何映射HTTP请求、如何传参、如何编写单元测试。

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/12115116-079c163faa365326.webp)



**实体定义：**

```java
@Data
public class User {
    private Long id;
    private String name;
    private Integer age;
}
```



**实现对User对象的操作接口：**

```java
@RestController
@RequestMapping(value="/users")
public class UserController {
    // 创建线程安全的Map，模拟数据库存放信息
    static Map<Long, User> users = Collections.synchronizedMap(new HashMap<Long, User>());

    @GetMapping("/")
    public List<User> getUserList() {
        // 还可以通过@RequestParam从页面中传递参数来进行查询条件或者翻页信息的传递
        List<User> r = new ArrayList<User>(users.values());
        return r;
    }
    
    @PostMapping("/")
    public String postUser(@ModelAttribute User user) {
        // 用来创建User
        // 除了@ModelAttribute绑定参数之外，还可以通过@RequestParam从页面中传递参数
        users.put(user.getId(), user);
        return "success";
    }
    
    @GetMapping("/{id}")
    public User getUser(@PathVariable Long id) {
        // 处理"/users/{id}"的GET请求，用来获取url中id值的User信息
        // url中的id可通过@PathVariable绑定到函数的参数中
        return users.get(id);
    }

    //@RequestMapping(value="/{id}", method=RequestMethod.PUT)
    @PutMapping("/{id}")
    public String putUser(@PathVariable Long id, @ModelAttribute User user) {
        // 处理"/users/{id}"的PUT请求，用来更新User信息
        User u = users.get(id);
        u.setName(user.getName());
        u.setAge(user.getAge());
        users.put(id, u);
        return "success";
    }

    @DeleteMapping("/{id}")
    public String deleteUser(@PathVariable Long id) {
        // 处理"/users/{id}"的DELETE请求，用来删除User
        users.remove(id);
        return "success";
    }

}
```



### Controller单元测试方法

> 参考文档：http://tengj.top/2017/12/28/springboot12/#Controller

有时候需要对Controller层（API）做测试，这时候就得用到MockMvc了，你可以**不必启动**工程就能测试这些接口。

MockMvc实现了**对Http请求的模拟**，能够直接使用网络的形式，转换到Controller的调用，这样可以使得测试速度快、**不依赖网络环境**，而且提供了一套**验证的工具**，这样可以使得请求的验证统一而且很方便。

```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mvc;

    // 使用MockMvc的时候需要先用MockMvcBuilders使用构建MockMvc对象
//    @Before
//    public void setupMockMvc(){
//        System.out.println("初始化...");
//        mvc = MockMvcBuilders.webAppContextSetup(wac).build(); //初始化MockMvc对象
//        System.out.println("初始化...");
//    }

    @Test
    public void testUserController() throws Exception {
        mvc = MockMvcBuilders.webAppContextSetup(wac).build(); //初始化MockMvc对象
        // 1、get查一下user列表，应该为空
        mvc.perform(MockMvcRequestBuilders.get("/users/"))
                // 方法看请求的状态响应码是否为200如果不是则抛异常，测试不通过
                .andExpect(MockMvcResultMatchers.status().isOk())
                // 对比结果
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("[]")));

        // 2、post提交一个user
        mvc.perform(MockMvcRequestBuilders.post("/users/").param("id", "1")
                .param("name", "测试大师")
                .param("age", "20"))
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("success")));

        // 3、get获取user列表，应该有刚才插入的数据
        mvc.perform(MockMvcRequestBuilders.get("/users/"))
                // 方法看请求的状态响应码是否为200如果不是则抛异常，测试不通过
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("[{\"id\":1,\"name\":\"wexx\",\"age\":20}]")));


        // 4、put修改id为1的user
        mvc.perform(MockMvcRequestBuilders.put("/users/1")
                .param("name", "测试终极大师")
                .param("age", "30"))
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("success")));

        // 5、get一个id为1的user
        mvc.perform(MockMvcRequestBuilders.get("/users/1"))
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("{\"id\":1,\"name\":\"wex\",\"age\":30}")));

        // 6、del删除id为1的user
        mvc.perform(MockMvcRequestBuilders.delete("/users/1"))
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("success")));

        // 7、get查一下user列表，应该为空
        mvc.perform(MockMvcRequestBuilders.get("/users/"))
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andExpect(MockMvcResultMatchers.content().string(Matchers.equalTo("[]")));

    }

}

```




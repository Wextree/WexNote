# Spring

## @Target  @Retention  @Documented注解

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




























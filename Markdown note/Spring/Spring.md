# Spring

## 好处

- **非侵入式：** 支持基于POJO的编程模式，不强制性要求实现Spring框架中的接口或继承Spring框架中的类。
- **IoC容器：**  容器帮助应用程序管理对象以及对象之间的依赖关系，对象之间的依赖关系如果发生变化只需要修改配置文件而不是修改代码，因为代码的修改可能意味着项目的重新构建和完整的回归测试。再也不用自己编写工厂、单例，实现不再重复造轮子。
- **AOP：** 将所有的横切关注功能封装到切面中，通过配置的方式将横切关注功能动态添加到目标代码上，进一步实现了业务逻辑；和系统服务之间的分离。可以省去很多自己写代理类的工作。
- **MVC：** 挺过框架，为web表示提供了更好的解决办法。
- **事务管理：** 以宽广的胸怀接纳多种持久层技术，并且为其提供了声明式的事务管理，在不需要任何一行代码的情况下就能完成事务管理。
- **其他：** 为企业级开发提供了一站式选择，你可以在需要的时候使用它的部分和全部，更重要的是，甚至可以在感觉不到Spring存在的情况下，在你的项目中使用Spring提供的各种优秀的功能。



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

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/640 (10).jpg)

- **Spring Core**  spring的核心功能： IOC容器, 解决对象创建及依赖关系

- **Spring Web**  Spring对web模块的支持。

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

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/640 (26).webp)

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/640 (27).webp)

**IoC（Inversion of Control）控制反转，DI（Dependency Injection）依赖注入。**

**控制反转**是吧传统上由程序代码直接操控的对象的调用权**交给容器**，通过容器来实现对象组件的装配合管理。所谓的“控制反转”就是对组件对象控制权的转移，从程序代码本身转移到了外部容器，由容器**来创建对象并管理对象之间的依赖关系**。

**依赖注入**的基本原则是应用组件不应该负责查找资源或者其他依赖的协作对象。配置对象的工作应该由容器负责，查找资源的逻辑应该从应用组件的代码中抽取出来，交给容器来完成。DI是对IoC更为准确的描述，即组件之间的依赖关系由容器在运行期决定，形象的来说，即由容器动态的将某种依赖关系注入到组件之中。

一个类A需要调用到接口B中的方法，那么就需要为类A和接口B建立关联或者依赖关系，最原始的方法就是在类A中创建一个接口B的实现类C的实例，但这种方法需要开发人员自行去维护二者之间的依赖关系，也就是说当依赖关系发生变动时，需要修改代码并重新构建整个系统。如果通过一个容器来管理这些对象和对象的依赖关系，则只需要在类A中定义好用于关联B的方法（构造器或setter方法），将类A和接口B的实现类C放入容器中，通过对容器的配置来实现二者之间的关联。

依赖注入可以通过setter方法注入，构造器注入和接口注入三种方式来实现。



## AOP

Spring AOP主要做的事情就是：「把重复的代码抽取，在运行的时候往业务方法上**动态植入**“切面类代码”」

主要应用在：**事务管理、日志、缓存**。

AOP分动态代理和静态代理，其中静态代理的代表是**AspectJ**，而动态代理则以**Spring AOP**为代表。

通常使用**AspectJ**的编译时增强实现AOP，在编译时生成AOP代理类，因此也称为编译时增强。

**Spring AOP**的动态代理主要有两种方式：

- JDK动态代理：通过反射来接收被代理的类，并且要求被代理的类必须实现一个接口，其核心是InvocationHandler接口和Proxy类。
- CgLib：这是一个代码生成的类库，可以在运行时动态的生成某个类的子类，它是通过继承的方式来做动态代理，所以无法为final标注的类进行动态代理，其核心是拦截器和回调。





## 数据绑定

> 参考文档：https://blog.csdn.net/f641385712/article/details/96450469

### 前言

数据绑定 这个概念在任何一个成型的框架中都是特别重要的（尤其是web框架），它能让框架更多的自动化，更好容错性以及更高的编码效率。它提供的能力是：把字符串形式的参数转换成服务端真正需要的类型的转换（当然可能还包含校验）。



### DataBinder

此类所在的包是`org.springframework.validation`，所以可想而知，它不仅仅完成数据的绑定，还会和数据校验有关~

校验动作不发生在`DataBinder`本类

`DataBinder`实际上依赖的是`PropertyAccessor`。



#### Demo

**看一下基本的使用：**

```java
import lombok.Data;

@Data
public class Person {
    private String name;
    private Integer age;
}
```

```java
public class testDataBinder {

    @Test
    public void demo1() throws BindException {
        Person person = new Person();
        DataBinder binder = new DataBinder(person, "person");
        MutablePropertyValues pvs = new MutablePropertyValues();
        pvs.add("name", "wex");
        pvs.add("age", 18);

        binder.bind(pvs);
        Map<?, ?> close = binder.close();

        System.out.println(person);
        System.out.println(close);
    }
}
```

```verilog
Person(name=wex, age=18)
{person=Person(name=wex, age=18), org.springframework.validation.BindingResult.person=org.springframework.validation.BeanPropertyBindingResult: 0 errors}
```

其实直接使用顶层的`DataBinder`也是一般不会的，而是使用它的子类。比如web包下大名鼎鼎的`WebDataBinder`



#### 源码分析

**源码实在是太长而且比较复杂，因为涉及的功能比较多：**

有兴趣看源码请往参考文档地址走：https://blog.csdn.net/f641385712/article/details/96450469

从源源码的分析中，大概能总结到DataBinder它提供了如下**能力**：

把属性值PropertyValues绑定到target上（bind()方法，依赖于PropertyAccessor实现~）
提供校验的能力：提供了public方法validate()对各个属性使用Validator执行校验~
提供了注册属性编辑器（PropertyEditor）和对类型进行转换的能力（TypeConverter）

还需要**注意**的是：

1. `initBeanPropertyAccess`和`initDirectFieldAccess`两个初始化`PropertyAccessor`方法是互斥的
   1. `initBeanPropertyAccess()`创建的是`BeanPropertyBindingResult`，内部依赖`BeanWrapper`
   2. `initDirectFieldAccess`创建的是`DirectFieldBindingResult`，内部依赖`DirectFieldAccessor`
2. 这两个方法内部没有显示的调用，但是Spring内部默认使用的是`initBeanPropertyAccess()`，具体可以参照`getInternalBindingResult()`方法



#### 总结

本文介绍了Spring用于数据绑定的最直接类DataBinder，它位于spring-context这个工程的org.springframework.validation包内，所以需要再次明确的是：它是Spring提供的能力而非web提供的~

虽然我们DataBinder是Spring提供，但其实把它发扬光大是发生在Web环境，也就是大名鼎鼎的WebDataBinder，毕竟我们知道一般只有进行web交互的时候，才会涉及到字符串 -> Java类型/对象的转换，这就是下个章节讲述的重中之重~




### WebDataBinder

**作用：**从`web request`里（**注意：这里指的web请求，并不一定就是ServletRequest请求哟**）把web请求的`parameters`绑定到`JavaBean`上

`Controller`方法的参数类型可以是基本类型，也可以是封装后的普通Java类型。**若这个普通Java类型没有声明任何注解，则意味着它的`每一个属性`都需要到Request中去查找对应的请求参数。**



#### 比父类增强：

单从WebDataBinder来说，它对父类进行了增强，提供的增强能力如下：

支持对属性名以_打头的默认值处理（自动挡，能够自动处理所有的Bool、Collection、Map等）
支持对属性名以!打头的默认值处理（手动档，需要手动给某个属性赋默认值，自己控制的灵活性很高）
提供方法，支持把MultipartFile绑定到JavaBean的属性上~

**演示：**

```java
@Data
public class Person {
    private String name;
    private Integer age;

    // 基本数据类型
    public Boolean flag;
    public int index;
    public List<String> list;
    public Map<String, String> map;
}
```

**演示使用`!`手动精确控制字段的默认值：**

```java
@Test
public void testWebDataBinder(){
    Person person = new Person();
    WebDataBinder binder = new WebDataBinder(person, "person");
    MutablePropertyValues pvs = new MutablePropertyValues();
    // 使用!来模拟各个字段手动指定默认值
    //pvs.add("name", "fsx");
    pvs.add("!name", "不知火舞");
    pvs.add("age", 18);
    pvs.add("!age", 10); // 上面有确切的值了，默认值不会再生效

    binder.bind(pvs);
    System.out.println(person);
}
```

```verilog
Person(name=不知火舞, age=18, flag=null, index=0, list=null, map=null)
```



### ServletRequestDataBinder

**WebDataBinder**的一个重要子类**ServletRequestDataBinder**

> 到此类为止就已经把web请求限定为了Servlet Request，和Servlet规范强绑定了。

```java
public class ServletRequestDataBinder extends WebDataBinder {
	... // 沿用父类构造
	// 注意这个可不是父类的方法，是本类增强的~~~~意思就是kv都从request里来~~当然内部还是适配成了一个MutablePropertyValues
	public void bind(ServletRequest request) {
		// 内部最核心方法是它：WebUtils.getParametersStartingWith()  把request参数转换成一个Map
		// request.getParameterNames()
		MutablePropertyValues mpvs = new ServletRequestParameterPropertyValues(request);
		MultipartRequest multipartRequest = WebUtils.getNativeRequest(request, MultipartRequest.class);
	
		// 调用父类的bindMultipart方法，把MultipartFile都放进MutablePropertyValues里去~~~
		if (multipartRequest != null) {
			bindMultipart(multipartRequest.getMultiFileMap(), mpvs);
		}
		// 这个方法是本类流出来的一个扩展点~~~子类可以复写此方法自己往里继续添加
		// 比如ExtendedServletRequestDataBinder它就复写了这个方法，进行了增强（下面会说）  支持到了uriTemplateVariables的绑定
		addBindValues(mpvs, request);
		doBind(mpvs);
	}

	// 这个方法和父类的close方法类似，很少直接调用
	public void closeNoCatch() throws ServletRequestBindingException {
		if (getBindingResult().hasErrors()) {
			throw new ServletRequestBindingException("Errors binding onto object '" + getBindingResult().getObjectName() + "'", new BindException(getBindingResult()));
		}
	}
}

```

**演示：**

```java
@Test
public void testServletRequestDataBinder(){
    Person person = new Person();
    ServletRequestDataBinder binder = new ServletRequestDataBinder(person, "person");

    // 构造参数，此处就不用MutablePropertyValues，以HttpServletRequest的实现类MockHttpServletRequest为例吧
    MockHttpServletRequest request = new MockHttpServletRequest();
    // 模拟请求参数
    request.addParameter("name", "fsx");
    request.addParameter("age", "18");

    // flag不仅仅可以用true/false  用0和1也是可以的？
    request.addParameter("flag", "1");

    // 设置多值的
    request.addParameter("list", "4", "2", "3", "1");
    // 给map赋值(Json串)
    // request.addParameter("map", "{'key1':'value1','key2':'value2'}"); // 这样可不行
    request.addParameter("map['key1']", "value1");
    request.addParameter("map['key2']", "value2");

    //// 一次性设置多个值（传入Map）
    //request.setParameters(new HashMap<String, Object>() {{
    //    put("name", "fsx");
    //    put("age", "18");
    //}});

    binder.bind(request);
    System.out.println(person);
}
```

```verilog
Person(name=fsx, age=18, flag=true, index=0, list=[4, 2, 3, 1], map={key1=value1, key2=value2})
```



#### ExtendedServletRequestDataBinder

它是对ServletRequestDataBinder的一个增强，它用于把URI template variables参数添加进来用于绑定。它会去从request的HandlerMapping.class.getName() + ".uriTemplateVariables";这个属性里查找到值出来用于绑定。

比如我们熟悉的@PathVariable它就和这相关：它负责把参数从url模版中解析出来，然后放在attr上，最后交给ExtendedServletRequestDataBinder进行绑定。

```java
// @since 3.1
public class ExtendedServletRequestDataBinder extends ServletRequestDataBinder {
	... // 沿用父类构造

	//本类的唯一方法
	@Override
	@SuppressWarnings("unchecked")
	protected void addBindValues(MutablePropertyValues mpvs, ServletRequest request) {
		// 它的值是：HandlerMapping.class.getName() + ".uriTemplateVariables";
		String attr = HandlerMapping.URI_TEMPLATE_VARIABLES_ATTRIBUTE;

		// 注意：此处是attr，而不是parameter
		Map<String, String> uriVars = (Map<String, String>) request.getAttribute(attr);
		if (uriVars != null) {
			uriVars.forEach((name, value) -> {
				
				// 若已经存在确切的key了，不会覆盖~~~~
				if (mpvs.contains(name)) {
					if (logger.isWarnEnabled()) {
						logger.warn("Skipping URI variable '" + name + "' because request contains bind value with same name.");
					}
				} else {
					mpvs.addPropertyValue(name, value);
				}
			});
		}
	}
}

```



### 如何注册自己的PropertyEditor来实现`自定义类型`数据绑定？

虽然默认注册支持的Editor众多，但是依旧发现它并没有对Date类型、以及Jsr310提供的各种事件、日期类型的转换（当然也包括我们的自定义类型）。
因此我相信小伙伴都遇到过这样的痛点：Date、LocalDate等类型使用自动绑定老不方便了，并且还经常傻傻搞不清楚。所以最终很多都无奈选择了语义不是非常清晰的时间戳来传递

#### Demo

```java
@Data
public class Person {
    private String name;
    private Integer age;

    // 以Date类型为示例
    private Date start;
    private Date end;
    private Date endTest;

    public static void main(String[] args) {
        Person person = new Person();
        DataBinder binder = new DataBinder(person, "person");

        // 设置属性
        MutablePropertyValues pvs = new MutablePropertyValues();
        pvs.add("name", "fsx");

        // 事件类型绑定
        pvs.add("start", new Date());
        pvs.add("end", "2019-07-20");
        // 试用试用标准的事件日期字符串形式~
        pvs.add("endTest", "Sat Jul 20 11:00:22 CST 2019");


        binder.bind(pvs);
        System.out.println(person);
    }
}
```

```verilog
// start有值，end没有，endTest却有值。
Person(name=fsx, age=null, start=Mon Jun 29 16:57:16 CST 2020, end=null, endTest=Sun Jul 21 01:00:22 CST 2019)
```

**步骤：**

1. BeanWrapper调用setPropertyValue()给属性赋值，传入的value值都会交给convertForProperty()方法根据get方法的返回值类型进行转换~（比如此处为Date类型）
2. 委托给this.typeConverterDelegate.convertIfNecessary进行类型转换（比如此处为string->Date类型）
3. 先this.propertyEditorRegistry.findCustomEditor(requiredType, propertyName);找到一个合适的PropertyEditor（显然此处我们没有自定义Custom处理Date的PropertyEditor，返回null）
4. 回退到使用ConversionService，显然此处我们也没有设置，返回null
5. 回退到使用默认的editor = findDefaultEditor(requiredType);（注意：此处只根据类型去找了，因为上面说了默认不处理了Date，所以也是返回null）
6. 最终的最终，回退到Spring对Array、Collection、Map的默认值处理问题，最终若是String类型，都会调用BeanUtils.instantiateClass(strCtor, convertedValue)也就是有参构造进行初始化~~~(请注意这必须是String类型才有的权利)
   1. 所以本例中，到最后一步就相当于`new Date("Sat Jul 20 11:00:22 CST 2019")`，因为该字符串是标准的时间日期串，所以是阔仪的，也就是endTest是能被正常赋值的~
     

**巧妙地例子：**

```java
@Getter
@Setter
@ToString
public class Person {
    private String name;
    // 备注：child是有有一个入参的构造器的
    private Child child;
}

@Getter
@Setter
@ToString
public class Child {
    private String name;
    private Integer age;
    public Child() {
    }
    public Child(String name) {
        this.name = name;
    }
}

    public static void main(String[] args) {
        Person person = new Person();
        DataBinder binder = new DataBinder(person, "person");

        // 设置属性
        MutablePropertyValues pvs = new MutablePropertyValues();
        pvs.add("name", "fsx");

        // 给child赋值，其实也可以传一个字符串就行了 非常的方便   Spring会自动给我们new对象
        pvs.add("child", "fsx-son");
        
        binder.bind(pvs);
        System.out.println(person);
    }

```



#### 自定义属性编辑器

我们知道`DataBinder`本身就是个`PropertyEditorRegistry`，因此我只需要自己注册一个自定义的`PropertyEditor`即可：

```java
// 通过继承PropertyEditorSupport实现一个自己的处理Date的编辑器
public class MyDatePropertyEditor extends PropertyEditorSupport {

    private static final String PATTERN = "yyyy-MM-dd";

    @Override
    public String getAsText() {
        Date date = (Date)super.getValue();
        return new SimpleDateFormat(PATTERN).format(date);
    }

    @Override
    public void setAsText(String text) throws IllegalArgumentException {
        try {
            super.setValue(new SimpleDateFormat(PATTERN).parse(text));
        } catch (ParseException e) {
            System.out.println("ParseException....................");
        }
    }
}
```

```java
// 注册进DataBinder并运行
 public static void main(String[] args) {
        Person person = new Person();
        DataBinder binder = new DataBinder(person, "person");
        binder.registerCustomEditor(Date.class, new MyDatePropertyEditor());

        // 设置属性
        MutablePropertyValues pvs = new MutablePropertyValues();
        pvs.add("name", "fsx");

        // 事件类型绑定
        pvs.add("start", new Date());
        pvs.add("end", "2019-07-20");
        // 试用试用标准的事件日期字符串形式~
        pvs.add("endTest", "Sat Jul 20 11:00:22 CST 2019");


        binder.bind(pvs);
        System.out.println(person);
    }
```

```verilog
// 1、输出了ParseException…
// 2、start有值，endTest值却为null了
ParseException....................
Person(name=fsx, age=null, start=Mon Jun 29 17:08:52 CST 2020, end=Sat Jul 20 00:00:00 CST 2019, endTest=null)
```



### WebBindingInitializer和WebDataBinderFactory

#### WebBindingInitializer

**WebBindingInitializer：**实现此接口重写initBinder方法注册的属性编辑器是全局的属性编辑器，对所有的Controller都有效。

可以简单粗暴的理解为：WebBindingInitializer为编码方式，@InitBinder为注解方式（当然注解方式还能控制到只对当前Controller有效，实现更细粒度的控制）

```java
// @since 2.5   Spring在初始化WebDataBinder时候的回调接口，给调用者自定义~
public interface WebBindingInitializer {

	// @since 5.0
	void initBinder(WebDataBinder binder);

	// @deprecated as of 5.0 in favor of {@link #initBinder(WebDataBinder)}
	@Deprecated
	default void initBinder(WebDataBinder binder, WebRequest request) {
		initBinder(binder);
	}

}

```

此接口它的内建唯一实现类为：`ConfigurableWebBindingInitializer`，若你自己想要扩展，建议继承它~

```java
public class ConfigurableWebBindingInitializer implements WebBindingInitializer {
	private boolean autoGrowNestedPaths = true;
	private boolean directFieldAccess = false; // 显然这里是false

	// 下面这些参数，不就是WebDataBinder那些可以配置的属性们吗？
	@Nullable
	private MessageCodesResolver messageCodesResolver;
	@Nullable
	private BindingErrorProcessor bindingErrorProcessor;
	@Nullable
	private Validator validator;
	@Nullable
	private ConversionService conversionService;
	// 此处使用的PropertyEditorRegistrar来管理的，最终都会被注册进PropertyEditorRegistry嘛
	@Nullable
	private PropertyEditorRegistrar[] propertyEditorRegistrars;

	... //  省略所有get/set
	
	// 它做的事无非就是把配置的值都放进去而已~~
	@Override
	public void initBinder(WebDataBinder binder) {
		binder.setAutoGrowNestedPaths(this.autoGrowNestedPaths);
		if (this.directFieldAccess) {
			binder.initDirectFieldAccess();
		}
		if (this.messageCodesResolver != null) {
			binder.setMessageCodesResolver(this.messageCodesResolver);
		}
		if (this.bindingErrorProcessor != null) {
			binder.setBindingErrorProcessor(this.bindingErrorProcessor);
		}
		// 可以看到对校验器这块  内部还是做了容错的
		if (this.validator != null && binder.getTarget() != null && this.validator.supports(binder.getTarget().getClass())) {
			binder.setValidator(this.validator);
		}
		if (this.conversionService != null) {
			binder.setConversionService(this.conversionService);
		}
		if (this.propertyEditorRegistrars != null) {
			for (PropertyEditorRegistrar propertyEditorRegistrar : this.propertyEditorRegistrars) {
				propertyEditorRegistrar.registerCustomEditors(binder);
			}
		}
	}
}

```

此实现类主要是提供了一些可配置项，方便使用。注意：此接口一般不直接使用，而是结合`InitBinderDataBinderFactory`、`WebDataBinderFactory`等一起使用~



#### WebDataBinderFactory

```java
// @since 3.1   注意：WebDataBinder 可是1.2就有了~
public interface WebDataBinderFactory {
	// 此处使用的是Spring自己的NativeWebRequest   后面两个参数就不解释了
	WebDataBinder createBinder(NativeWebRequest webRequest, @Nullable Object target, String objectName) throws Exception;
}

```

**实现类：**

```java
public class DefaultDataBinderFactory implements WebDataBinderFactory {
	@Nullable
	private final WebBindingInitializer initializer;
	// 注意：这是唯一构造函数
	public DefaultDataBinderFactory(@Nullable WebBindingInitializer initializer) {
		this.initializer = initializer;
	}

	// 实现接口的方法
	@Override
	@SuppressWarnings("deprecation")
	public final WebDataBinder createBinder(NativeWebRequest webRequest, @Nullable Object target, String objectName) throws Exception {

		WebDataBinder dataBinder = createBinderInstance(target, objectName, webRequest);
		
		// 可见WebDataBinder 创建好后，此处就会回调（只有一个）
		if (this.initializer != null) {
			this.initializer.initBinder(dataBinder, webRequest);
		}
		// 空方法 子类去实现，比如InitBinderDataBinderFactory实现了词方法
		initBinder(dataBinder, webRequest);
		return dataBinder;
	}

	//  子类可以复写，默认实现是WebRequestDataBinder
	// 比如子类ServletRequestDataBinderFactory就复写了，使用的new ExtendedServletRequestDataBinder(target, objectName)
	protected WebDataBinder createBinderInstance(@Nullable Object target, String objectName, NativeWebRequest webRequest) throws Exception 
		return new WebRequestDataBinder(target, objectName);
	}
}

```



#### InitBinderDataBinderFactory

它继承自`DefaultDataBinderFactory`，主要用于处理标注有`@InitBinder`的方法做初始绑定~

```java
public class InitBinderDataBinderFactory extends DefaultDataBinderFactory {
	
	// 需要注意的是：`@InitBinder`可以标注N多个方法~  所以此处是List
	private final List<InvocableHandlerMethod> binderMethods;

	// 此子类的唯一构造函数
	public InitBinderDataBinderFactory(@Nullable List<InvocableHandlerMethod> binderMethods, @Nullable WebBindingInitializer initializer) {
		super(initializer);
		this.binderMethods = (binderMethods != null ? binderMethods : Collections.emptyList());
	}

	// 上面知道此方法的调用方法生initializer.initBinder之后
	// 所以使用注解它生效的时机是在直接实现接口的后面的~
	@Override
	public void initBinder(WebDataBinder dataBinder, NativeWebRequest request) throws Exception {
		for (InvocableHandlerMethod binderMethod : this.binderMethods) {
			// 判断@InitBinder是否对dataBinder持有的target对象生效~~~（根据name来匹配的）
			if (isBinderMethodApplicable(binderMethod, dataBinder)) {
				// 关于目标方法执行这块，可以参考另外一篇@InitBinder的原理说明~
				Object returnValue = binderMethod.invokeForRequest(request, null, dataBinder);

				// 标注@InitBinder的方法不能有返回值
				if (returnValue != null) {
					throw new IllegalStateException("@InitBinder methods must not return a value (should be void): " + binderMethod);
				}
			}
		}
	}

	//@InitBinder有个Value值，它是个数组。它是用来匹配dataBinder.getObjectName()是否匹配的   若匹配上了，现在此注解方法就会生效
	// 若value为空，那就对所有生效~~~
	protected boolean isBinderMethodApplicable(HandlerMethod initBinderMethod, WebDataBinder dataBinder) {
		InitBinder ann = initBinderMethod.getMethodAnnotation(InitBinder.class);
		Assert.state(ann != null, "No InitBinder annotation");
		String[] names = ann.value();
		return (ObjectUtils.isEmpty(names) || ObjectUtils.containsElement(names, dataBinder.getObjectName()));
	}
}

```





## 面试问题

### Bean作用域

**早期版本只有两个作用域：**

- **singleton：**单例的方式存在。只会创建一个对象实例，而且是在Ioc容器初始化的时候就创建好了。多用于连接池啊之类的。
- **prototype：**多例，也称为原型模式。每次从容器中调用Bean时，都会返回一个新的实例，prototype通常会翻译为原型。只有当要调用时才会创建实例。



**2.x 版本中针对WebApplicationContext新增了3个作用域：**

- **request：**每次HTTP请求都会创建一个新的Bean。
- **session：**同一个HttpSession共享一个bean，不同的HttpSession使用不同的Bean。
- **globalSession：**同一个全局Session共享一个Bean。



单例模式和原型模式都是重要的设计模式。一般情况下，无状态或状态不可变得类适合使用单例模式。在传统开发中，由于DAO持有Connection这个非线程安全对象而没有使用单例模式；但在Spring环境下，所有DAO类可以采用单例模式，因为Spring利用AOP和Java API中的ThreadLocal对非线程安全的对象进行了特殊处理。



### 自动装配方式

如果当我们类中的依赖引用只有一个相同类型的Bean时，Spring会使用AutoWired帮我们自动装配，也不会存在什么冲突的问题，但是如果是多种相同类型的Bean存在，这时我们就需要Spring按照某种规则帮我们找到所需要的Bean进行装配。

- **no：**不进行自动装配，手动设置Bean的依赖关系。
- **byName：**根据Bean的名字进行自动装配。
- **byType：**根据Bean的类型进行自动装配。
- **constructor：**类似于byType，不过是应用于构造器的参数，如果正好有一个Bean与构造器的参数类型相同则可以自动装配，否则会导致错误。
- **autodetect：**如果有默认的构造器，则通过constructor的方式进行自动装配，否则使用byType的方式进行自动装配。



## BeanFactory & ApplicationContext

### BeanFactory

**BeanFactory**是Spring中比较原始，比较古老的Factory。所以它无法支持Spring插件，例如AOP、Web应用等功能。



### ApplicationContext

它是BeanFactory的子类，因为古老的BeanFactory无法满足不断更新的Spring的需求，于是ApplicationContext就基本上替代了BeanFactory的工作，以一种更面向框架的工作方式以及对上下文进行分层和实现继承，并在这个基础上对功能进行扩展：

- **MessageSource**提供国际化的消息访问
- 资源访问（如URL和文件）
- 事件传递
- Bean的自动装配
- 各种不同应用层的Context实现



### 区别

- 对于singleton实例对象，ApplicationContext不管你是不是想先加载，都会先实例化它。好处是可以预先加载，但坏处是浪费内存。
- BeanFactory实例化对象时不会马上实例化，而是等到你要调用bean的时候（getBean）才会被实例化。好在节约内存，坏在速度比较慢，多用于移动设备的开发。
- 现在基本上都是用ApplicationContext来完成，如果没有特殊情况，BeanFactory能完成的它都能完成。






























































































# SpringMVC

> 参考文档：https://juejin.im/post/6844904127059722253

## 概述

### 定义与理解

**Spring MVC**是一个基于Java的实现了MVC设计模式的请求驱动类型的轻量级Web框架，通过把**模型-视图-控制器**分离，将web层进行职责解耦，把复杂的web应用分成逻辑清晰的几部分，简化开发，减少出错，方便组内开发人员之间的配合。



### 优势

- 可以支持各种视图技术,而不仅仅局限于JSP；

- 与Spring框架集成（如IoC容器、AOP等）；

- 清晰的角色分配：前端控制器(dispatcherServlet) , 请求到处理器映射（handlerMapping), 处理器适配器（HandlerAdapter), 视图解析器（ViewResolver）。

- 支持各种请求资源的映射策略。



## 核心组件

### 主要组件

- 前端控制器 **DispatcherServlet**（不需要程序员开发）

  作用：接收请求、响应结果，相当于转发器，有了DispatcherServlet 就减少了其它组件之间的耦合度。

- 处理器映射器 **HandlerMapping**（不需要程序员开发）

  作用：根据请求的URL来查找Handler

- 处理器适配器 **HandlerAdapter**

  注意：在编写Handler的时候要按照HandlerAdapter要求的规则去编写，这样适配器HandlerAdapter才可以正确的去执行Handler。

- 处理器 **Handler**（需要程序员开发）

- 视图解析器 **ViewResolver**（不需要程序员开发）

  作用：进行视图的解析，根据视图逻辑名解析成真正的视图（view）

- 视图View（需要程序员开发jsp）

  View是一个接口， 它的实现类支持不同的视图类型（jsp，freemarker，pdf等等）



### Q & A

**1. 什么是DispatcherServlet**

Spring的MVC框架是围绕DispatcherServlet来设计的，它用来处理所有的HTTP请求和响应。



**2. 什么是Spring MVC框架的控制器？**

控制器提供一个访问应用程序的行为，此行为通常通过服务接口实现。控制器解析用户输入并将其转换为一个由视图呈现给用户的模型。Spring用一个非常抽象的方式实现了一个控制层，允许用户创建多种用途的控制器。



**3. Spring MVC的控制器是不是单例模式,如果是,有什么问题,怎么解决？**

是单例模式,所以在多线程访问的时候有线程安全问题,不要用同步,会影响性能的,解决方案是在控制器里面不能写字段。







## 工作流程

![](https://gitee.com/Wextree/Wex_imgs/raw/master/img/5220087-3c0f59d3c39a12dd.webp)

1. 用户发送请求至**前端控制器DispatcherServlet**
2. DispatcherServlet收到请求调用**处理器映射器HandlerMapping**。
3. 处理器映射器根据请求URL找到具体的处理器，**生成处理器执行链HandlerExecutionChain(**包括处理器对象和处理器拦截器)一并返回给DispatcherServlet。
4. DispatcherServlet根据处理器Handler获取**处理器适配器HandlerAdapter**执行。HandlerAdapter处理一系列的操作，如：**参数封装，数据格式转换，数据验证**等操作
5. **执行处理器Handler**(Controller，也叫页面控制器)。
6. Handler执行完成**返回ModelAndView**
7. **HandlerAdapter**将Handler执行结果ModelAndView**返回到DispatcherServlet**
8. DispatcherServlet将ModelAndView**传给ViewReslover视图解析器**
9. ViewReslover解析后**返回具体View**
10. DispatcherServlet**对View进行渲染视图**（即将模型数据model填充至视图中）。
11. DispatcherServlet**响应用户**



## 常用注解

### 注解原理是什么

- 注解本质是一个继承了**Annotation**的特殊接口，其具体实现类是Java运行时生成的动态代理类。我们通过反射获取注解时，返回的是Java运行时生成的动态代理对象。通过代理对象调用自定义注解的方法，会最终调用AnnotationInvocationHandler的invoke方法。该方法会从memberValues这个Map中索引出对应的值。而memberValues的来源是Java常量池。



### Spring MVC常用的注解有哪些？

- **@RequestMapping：**用于处理请求 url 映射的注解，可用于类或方法上。用于类上，则表示类中的所有响应请求的方法都是以该地址作为父路径。
- **@RequestBody：**注解实现接收http请求的json数据，将json转换为java对象。
- **@ResponseBody：**注解实现将conreoller方法返回对象转化为json对象响应给客户。
- **@Conntroller：**控制器的注解，表示是表现层,不能用用别的注解代替



### SpingMvc中的控制器的注解一般用哪个,有没有别的注解可以替代？

- 答：一般用@Controller注解,也可以使用@RestController,@RestController注解相当于@ResponseBody ＋ @Controller,表示是表现层,除此之外，一般不用别的注解代替。



### @Controller注解的作用

- 在Spring MVC 中，控制器Controller 负责处理由DispatcherServlet 分发的请求，它把用户请求的数据经过业务处理层处理之后封装成一个Model ，然后再把该Model 返回给对应的View 进行展示。在Spring MVC 中提供了一个非常简便的定义Controller 的方法，你**无需继承特定的类或实现特定的接口**，只需使用@Controller 标记一个类是Controller ，然后使用**@RequestMapping 和@RequestParam** 等一些注解用以定义URL 请求和Controller 方法之间的映射，这样的Controller 就能被外界访问到。此外Controller **不会直接依赖于HttpServletRequest 和HttpServletResponse 等HttpServlet 对象**，它们可以通过Controller 的**方法参数**灵活的获取到。
- @Controller 用于标记在一个类上，使用它标记的类就是一个Spring MVC Controller 对象。**分发处理器将会扫描使用了该注解的类的方法**，并检测该方法是否使用了@RequestMapping 注解。@Controller 只是定义了一个控制器类，而使用**@RequestMapping 注解的方法才是真正处理请求的处理器**。单单使用@Controller 标记在一个类上还不能真正意义上的说它就是Spring MVC 的一个控制器类，因为这个时候Spring 还不认识它。那么要如何做Spring 才能认识它呢？
- 这个时候就需要我们把这个控制器类交给Spring 来管理。有两种方式：
  - 在Spring MVC 的配置文件中定义MyController 的bean 对象。
  - 在Spring MVC 的配置文件中告诉Spring 该到哪里去找标记为@Controller 的Controller 控制器。



### @RequestMapping注解的作用

- RequestMapping是一个用来处理请求地址映射的注解，可用于类或方法上。用于类上，表示类中的所有响应请求的方法都是以该地址作为父路径。
- RequestMapping注解有六个属性，下面我们把她分成三类进行说明（下面有相应示例）。
- **value， method**
  - value： 指定请求的实际地址，指定的地址可以是URI Template 模式（后面将会说明）；
  - method： 指定请求的method类型， GET、POST、PUT、DELETE等；
- **consumes，produces**
  - consumes： 指定处理请求的提交内容类型（Content-Type），例如application/json, text/html;
  - produces: 指定返回的内容类型，仅当request请求头中的(Accept)类型中包含该指定类型才返回；
- **params，headers**
  - params： 指定request中必须包含某些参数值是，才让该方法处理。
  - headers： 指定request中必须包含某些指定的header值，才能让该方法处理请求。



### @ResponseBody注解的作用

- 作用： 该注解用于将Controller的方法返回的对象，通过适当的HttpMessageConverter转换为指定格式后，写入到Response对象的body数据区。
- 使用时机：返回的数据不是html标签的页面，而是其他某种格式的数据时（如json、xml等）使用；



### @PathVariable和@RequestParam的区别

`@RequestParam` 和 `@PathVariable` 注解是用于从request中接收请求的，两个都可以接收参数，关键点不同的是`@RequestParam` 是从request里面拿取值，而 `@PathVariable` 是从一个URI模板里面来填充。

对于这个网址：http://localhost:8080/springmvc/hello/101?param1=10&param2=20获取参数

```java
public String getDetails(
    @RequestParam(value="param1", required=true) String param1,
    @RequestParam(value="param2", required=false) String param2){
    ...
}

---------------------------------------------------------------------
@RequestMapping("/hello/{id}")
public String getDetails(
    @PathVariable(value="id") String id,
    @RequestParam(value="param1", required=true) String param1,
    @RequestParam(value="param2", required=false) String param2){
    .......
}
```



## 面试问题

### 1. Spring MVC怎么样设定重定向和转发的？

- （1）转发：在返回值前面加"forward:"，譬如"forward:user.do?name=method4"
- （2）重定向：在返回值前面加"redirect:"，譬如"redirect:[www.baidu.com](http://www.baidu.com/)"

> 转发是服务器行为，重定向是客户端行为。
>
> **转发过程：**客户浏览器发送http请求----》web服务器接受此请求--》调用内部的一个方法在容器内部完成请求处理和转发动作----》将目标资源发送给客户；在这里，转发的路径必须是同一个web容器下的url，其不能转向到其他的web路径上去，中间传递的是自己的容器内的request。在客户浏览器路径栏显示的仍然是其第一次访问的路径，也就是说客户是感觉不到服务器做了转发的。转发行为是浏览器只做了一次访问请求。
>
> **重定向过程：**客户浏览器发送http请求----》web服务器接受后发送302状态码响应及对应新的location给客户浏览器--》客户浏览器发现是302响应，则**自动**再发送一个新的http请求，请求url是新的location地址----》服务器根据此请求寻找资源并发送给客户。在这里location可以重定向到任意URL，既然是浏览器重新发出了请求，则就没有什么request传递的概念了。在客户浏览器路径栏显示的是其重定向的路径，客户可以观察到地址的变化的。重定向行为是浏览器做了至少两次的访问请求的。



### 2. Spring MVC怎么和AJAX相互调用的？

> ajax的全称是A synchronous + Javascript + XML
> 异步传输+js+xml。

- 通过Jackson框架就可以把Java里面的对象直接转化成Js可以识别的Json对象。具体步骤如下 ：

1. 加入Jackson.jar
2. 在配置文件中配置json的映射
3. 在接受Ajax方法里面可以直接返回Object,List等,但方法前面要加上@ResponseBody注解。



### 3. 如何解决POST请求中文乱码问题，GET的又如何处理呢？

- **解决post请求乱码问题：**

  在web.xml中配置一个CharacterEncodingFilter过滤器，设置成utf-8；

```xml
<filter>
    <filter-name>CharacterEncodingFilter</filter-name>
    <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
<init-param>
    <param-name>encoding</param-name>
    <param-value>utf-8</param-value>
</init-param>
</filter>

<filter-mapping>
    <filter-name>CharacterEncodingFilter</filter-name>
    <url-pattern>/*</url-pattern>
</filter-mapping>

```



- **get请求中文参数出现乱码解决方法有两个：**

1. 修改tomcat配置文件添加编码与工程编码一致，如下：

```
<ConnectorURIEncoding="utf-8" connectionTimeout="20000" port="8080" protocol="HTTP/1.1" redirectPort="8443"/>
```

2. 另外一种方法对参数进行重新编码：

   String userName = new String(request.getParamter(“userName”).getBytes(“ISO8859-1”),“utf-8”)

   ISO8859-1是tomcat默认编码，需要将tomcat编码后的内容按utf-8编码。



### 4. Spring MVC的异常处理？

答：可以将异常**抛给Spring框架**，由Spring框架来处理；我们只需要**配置简单的异常处理器**，在异常处理器中**添视图页面**即可。



### 5. 如果在拦截请求中，我想拦截get方式提交的方法,怎么配置

答：可以在@RequestMapping注解里面加上method=RequestMethod.GET。或者直接使用@GetMapping注解。



### 6. 怎样在方法里面得到Request,或者Session？

答：直接在方法的形参中声明request,Spring MVC就自动把request对象传入。像以下这样：

```java
public String test(HttpSession session){
    ......
}
```



### 7. 如果想在拦截的方法里面得到从前台传入的参数,怎么得到？

答：直接在形参里面声明这个参数就可以,但必须**名字和传过来的参数一样**。



### 8. 如果前台有很多个参数传入,并且这些参数都是一个对象的,那么怎么样快速得到这个对象？

答：直接在方法中声明这个对象,Spring MVC就自动会把属性赋值到这个对象里面。



### 9. Spring MVC中函数的返回值是什么？

答：返回值可以有很多类型,有**String, ModelAndView**。**ModelAndView类把视图和数据都合并的一起的**，但一般用String比较好。



### 10. Spring MVC用什么对象从后台向前台传递数据的？

答：通过ModelMap对象,可以在这个对象里面调用put方法,把对象加到里面,前台就可以通过el表达式拿到。也可以用Json格式直接将对象返回。



### 11. 怎么样把ModelMap里面的数据放入Session里面？

答：可以在类上面加上@SessionAttributes注解,里面包含的字符串就是要放入session里面的key。



### 12. Spring MVC里面拦截器是怎么写的

有两种写法,一种是实现HandlerInterceptor接口，另外一种是继承适配器类，接着在接口方法当中，实现处理逻辑；然后在Spring MVC的配置文件中配置拦截器即可：

```xml
<!-- 配置Spring MVC的拦截器 -->
<mvc:interceptors>
    <!-- 配置一个拦截器的Bean就可以了 默认是对所有请求都拦截 -->
    <bean id="myInterceptor" class="com.zwp.action.MyHandlerInterceptor"></bean>
    <!-- 只针对部分请求拦截 -->
    <mvc:interceptor>
       <mvc:mapping path="/modelMap.do" />
       <bean class="com.zwp.action.MyHandlerInterceptorAdapter" />
    </mvc:interceptor>
</mvc:interceptors>
```



### 13. 介绍一下 WebApplicationContext

WebApplicationContext **继承了ApplicationContext** 并增加了一些WEB应用必备的**特有功能**，它不同于一般的ApplicationContext ，因为它能处理主题，并找到被关联的**servlet**。












































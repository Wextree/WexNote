# SpringMVC

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
11. DispatcherServle**t响应用户**



## 数据绑定

> 参考文档：https://juejin.im/post/5d791f6e5188255de7351fe9

一般的string, int, long会自动绑定到参数，但是自定义的格式spring就不知道如何绑定了 .所以要继承`PropertyEditorSupport`，实现自己的属性编辑器`PropertyEditor`,绑定到`WebDataBinder ( binder.registerCustomEditor)`，覆盖方法`setAsText`



### @InitBinder原理






























































# Json

> JSON: **J**ava**S**cript **O**bject **N**otation(JavaScript 对象表示法)
>
> JSON 是存储和交换文本信息的语法。类似 XML。
>
> JSON 比 XML 更小、更快，更易解析。
>
> JSON 文本格式在语法上与创建 JavaScript 对象的代码相同。
>
> 由于这种相似性，无需解析器，JavaScript 程序能够使用内建的 eval() 函数，用 JSON 数据来生成原生的 JavaScript 对象



## 简介

### 与XML异同

**相同：**

- JSON 是纯文本
- JSON 具有"自我描述性"（人类可读）
- JSON 具有层级结构（值中存在值）
- JSON 可通过 JavaScript 进行解析
- JSON 数据可使用 AJAX 进行传输



**不同：**

- 没有结束标签
- 更短
- 读写的速度更快
- 能够使用内建的 **JavaScript eval()** 方法进行解析
- 使用数组
- 不使用保留字



## 语法

### JSON 语法规则

JSON 语法是 JavaScript 对象表示语法的子集。

- 数据在名称/值对中
- 数据由逗号分隔
- 大括号保存对象
- 中括号保存数组

------

### JSON 名称/值对

JSON 数据的书写格式是：名称/值对。

名称/值对包括字段名称（在双引号中），后面写一个冒号，然后是值：

"name" : "菜鸟教程"

这很容易理解，等价于这条 JavaScript 语句：

name = "菜鸟教程"

------

### JSON 值

JSON 值可以是：

- 数字（整数或浮点数）
- 字符串（在双引号中）
- 逻辑值（true 或 false）
- 数组（在中括号中）
- 对象（在大括号中）
- null



------

### JSON 数字

JSON 数字可以是整型或者浮点型：

{ "age":30 }



------

### JSON 对象

JSON 对象在大括号（{}）中书写：

对象可以包含多个名称/值对：

{ "name":"菜鸟教程" , "url":"www.runoob.com" }

这一点也容易理解，与这条 JavaScript 语句等价：

name = "菜鸟教程" url = "www.runoob.com"



------

### JSON 数组

JSON 数组在中括号中书写：

数组可包含多个对象：

{ "sites": [ { "name":"菜鸟教程" , "url":"www.runoob.com" },  { "name":"google" , "url":"www.google.com" },  { "name":"微博" , "url":"www.weibo.com" } ] }

在上面的例子中，对象 "sites" 是包含三个对象的数组。每个对象代表一条关于某个网站（name、url）的记录。



------

### JSON 布尔值

JSON 布尔值可以是 true 或者 false：

{ "flag":true }



------

### JSON null

JSON 可以设置 null 值：

{ "runoob":null }



------

### JSON 文件

- JSON 文件的文件类型是 ".json"
- JSON 文本的 MIME 类型是 "application/json"



## 对象和数组

### 对象语法

- JSON 对象使用**在大括号({})**中书写。

- 对象可以包含多个 **key/value（键/值）**对。

- key 必须是**字符串**，value 可以是合法的 JSON 数据类型（字符串, 数字, 对象, 数组, 布尔值或 null）。

- key 和 value 中使**用冒号(:)分割**。

- 每个 key/value 对使**用逗号(,)分割**。



### 操作对象

```javascript
// 访问对象
var myObj, x;
myObj = { "name":"runoob", "alexa":10000, "site":null };
x = myObj.name;  // x = myObj["name"];

// 循环遍历对象
var myObj = { "name":"runoob", "alexa":10000, "site":null };
for (x in myObj) {
    document.getElementById("demo").innerHTML += x + "<br>";
}

-----------------------------------------------------------------------
    
var myObj = { "name":"runoob", "alexa":10000, "site":null };
for (x in myObj) {
    document.getElementById("demo").innerHTML += myObj[x] + "<br>";
}

// 嵌套及访问
myObj = {
    "name":"runoob",
    "alexa":10000,
    "sites": {
        "site1":"www.runoob.com",
        "site2":"m.runoob.com",
        "site3":"c.runoob.com"
    }
}

x = myObj.sites.site1;
// 或者
x = myObj.sites["site1"];

// 修改和删除
myObj.sites.site1 = "www.google.com";

delete myObj.sites.site1;
```



### 数组语法

- JSON 数组在中**括号**中书写。

- JSON 中数组值必须是合法的 JSON 数据类型**（字符串, 数字, 对象, 数组, 布尔值或 null）**。

- **JavaScript** 中，数组值可以是以上的 JSON 数据类型，也可以是 **JavaScript 的表达式**，包括函数，日期，及 *undefined*。



### 操作数组

```javascript
// 对象中的数组及其访问
myObj=
{
"name":"网站",
"num":3,
"sites":[ "Google", "Runoob", "Taobao" ]
}

x = myObj.sites[0];

// 循环数组
for (i in myObj.sites) {
    x += myObj.sites[i] + "<br>";
}

--------------------------------------------

for (i = 0; i < myObj.sites.length; i++) {
    x += myObj.sites[i] + "<br>";
}

// 嵌套及其使用
myObj = {
    "name":"网站",
    "num":3,
    "sites": [
        { "name":"Google", "info":[ "Android", "Google 搜索", "Google 翻译" ] },
        { "name":"Runoob", "info":[ "菜鸟教程", "菜鸟工具", "菜鸟微信" ] },
        { "name":"Taobao", "info":[ "淘宝", "网购" ] }
    ]
}

for (i in myObj.sites) {
    x += "<h1>" + myObj.sites[i].name + "</h1>";
    for (j in myObj.sites[i].info) {
        x += myObj.sites[i].info[j] + "<br>";
    }
}

// 修改和删除数组值
myObj.sites[1] = "Github";

delete myObj.sites[1];
```



## 函数

### JSON.parse() 方法

我们可以使用 JSON.parse() 方法将数据转换为 JavaScript 对象。

- `JSON.parse(text[, reviver])`

- **text: **必需， 一个有效的 JSON 字符串。
- **reviver:** 可选，一个转换结果的函数， 将为对象的每个成员调用此函数。

```html
<p id="demo"></p>
 
<script>
 	// 解析前要确保你的数据是标准的 JSON 格式，否则会解析出错。
    var obj = JSON.parse('{ "name":"runoob", "alexa":10000, "site":"www.runoob.com" }');
    document.getElementById("demo").innerHTML = obj.name + "：" + obj.site;
</script>
```



- **异常：**
  - JSON 不能存储 Date 对象。
  - 如果你需要存储 Date 对象，需要将其转换为**字符串**。
  - 之后再将字符串转换为 Date 对象。
  - JSON 不允许包含函数，但你可以将函数作为字符串存储，之后再将字符串转换为函数。



- **解析处理：**

```javascript
var text = '{ "name":"Runoob", "initDate":"2013-12-14", "site":"www.runoob.com"}';
var obj = JSON.parse(text, function (key, value) {
    if (key == "initDate") {
        return new Date(value);
    } else {
        return value;
}});
 
document.getElementById("demo").innerHTML = obj.name + "创建日期：" + obj.initDate;
```



### JSON.stringify() 方法

我们可以使用 JSON.stringify() 方法将 JavaScript 对象转换为字符串。

- `JSON.stringify(value[, replacer[, space]])`

- **value:** 必需， 要转换的 JavaScript 值（通常为对象或数组）。

- **replacer:**  可选。用于转换结果的函数或数组。

  如果 replacer 为函数，则 JSON.stringify 将调用该函数，并传入每个成员的键和值。使用返回值而不是原始值。如果此函数返回 undefined，则排除成员。根对象的键是一个空字符串：""。

  如果 replacer 是一个数组，则仅转换该数组中具有键值的成员。成员的转换顺序与键在数组中的顺序一样。当 value 参数也为数组时，将忽略 replacer 数组。

- **space:** 可选，文本添加缩进、空格和换行符，如果 space 是一个数字，则返回值文本在每个级别缩进指定数目的空格，如果 space 大于 10，则文本缩进 10 个空格。space 也可以使用非数字，如：\t。



```javascript
var obj = { "name":"runoob", "alexa":10000, "site":"www.runoob.com"};
var myJSON = JSON.stringify(obj);

// JSON 不能存储 Date 对象。
// JSON.stringify() 会将所有日期转换为字符串。
// JSON 不允许包含函数，JSON.stringify() 会删除 JavaScript 对象的函数，包括 key 和 value。
var obj = { "name":"Runoob", "initDate":new Date(), "site":"www.runoob.com"};
var myJSON = JSON.stringify(obj);
document.getElementById("demo").innerHTML = myJSON;

```



# JWT

> 参考文档：https://www.jianshu.com/p/576dbf44b2ae

> Json web token (JWT), 是为了在网络应用环境间传递声明而执行的一种基于JSON的开放标准（[(RFC 7519](https://link.jianshu.com?t=https://tools.ietf.org/html/rfc7519)).该token被设计为紧凑且安全的，**特别适用于分布式站点的单点登录（SSO）场景**。JWT的声明一般被用来在身份提供者和服务提供者间传递被认证的用户身份信息，以便于从资源服务器获取资源，也可以增加一些额外的其它业务逻辑所必须的声明信息，该token也可直接被用于认证，也可被加密。



## 传统的Session认证

我们知道，http协议本身是一种**无状态**的协议，而这就意味着如果用户向我们的应用提供了用户名和密码来进行用户认证，那么下一次请求时，用户还要再一次进行用户认证才行，因为根据http协议，我们并不能知道是哪个用户发出的请求，所以为了让我们的应用能识别是哪个用户发出的请求，我们只能在服务器存储一份用户登录的信息，这份登录信息会在响应时传递给浏览器，**告诉其保存为cookie,**以便下次请求时发送给我们的应用，这样我们的应用就能识别请求来自哪个用户了,这就是传统的基于session认证。



### 问题

- **Session**: 每个用户经过我们的应用认证之后，我们的应用都要在服务端做一次记录，以方便用户下次请求的鉴别，通常而言session都是保存在内存中，而随着认证用户的增多，服务端的开销会明显增大。
- **扩展性**: 用户认证之后，服务端做认证记录，如果认证的记录被保存在内存中的话，这意味着用户下次请求还必须要请求在这台服务器上,这样才能拿到授权的资源，这样在分布式的应用上，相应的限制了负载均衡器的能力。这也意味着限制了应用的扩展能力。
- **CSRF**: 因为是基于cookie来进行用户识别的, cookie如果被截获，用户就会很容易受到跨站请求伪造的攻击。



## 基于token的鉴权机制

基于token的鉴权机制类似于http协议也是无状态的，它不需要在服务端去保留用户的认证信息或者会话信息。这就意味着基于token认证机制的应用不需要去考虑用户在哪一台服务器登录了，这就为应用的扩展提供了便利。



### 流程

- 用户使用用户名密码来请求服务器
- 服务器进行验证用户的信息
- 服务器通过验证发送给用户一个token
- 客户端存储token，并在每次请求时附送上这个token值
- 服务端验证token值，并返回数据

这个token必须要在每次请求时传递给服务端，它应该保存在请求头里， 另外，服务端要支持`CORS(跨来源资源共享)`策略，一般我们在服务端这么做就可以了`Access-Control-Allow-Origin: *`。



## JWT

### 构成

JWT是由三段信息构成的，将这三段信息文本用`.`链接一起就构成了Jwt字符串。就像这样:

```jwt
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ
```

- 第一部分我们称它为头部（header)
- 第二部分我们称其为载荷（payload, 类似于飞机上承载的物品)
- 第三部分是签证（signature).



### header

jwt的头部承载两部分信息：

- 声明类型，这里是jwt
- 声明加密的算法 通常直接使用 HMAC SHA256

```json
{
  'typ': 'JWT',
  'alg': 'HS256'
}

// 然后将头部进行base64加密（该加密是可以对称解密的)
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9
```



### playload

载荷就是**存放有效信息**的地方。这个名字像是特指飞机上承载的货品，这些有效信息包含三个部分：

- 标准中注册的声明
- 公共的声明
- 私有的声明

**标准中注册的声明** (建议但不强制使用) ：

- **iss**: jwt签发者
- **sub**: jwt所面向的用户
- **aud**: 接收jwt的一方
- **exp**: jwt的过期时间，这个过期时间必须要大于签发时间
- **nbf**: 定义在什么时间之前，该jwt都是不可用的.
- **iat**: jwt的签发时间
- **jti**: jwt的唯一身份标识，主要用来作为一次性token,从而回避重放攻击。



**公共的声明** ：
 公共的声明可以添加任何的信息，一般添加用户的相关信息或其他业务需要的必要信息.但**不建议添加敏感信息**，因为该部分在客户端可解密.



**私有的声明** ：
 **私有声明是提供者和消费者所共同定义的声明**，一般不建议存放敏感信息，因为base64是对称解密的，意味着该部分信息可以归类为明文信息。

```json
{
  "sub": "1234567890",
  "name": "John Doe",
  "admin": true
}

// 然后将其进行base64加密，得到Jwt的第二部分。
eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9
```



### signature

jwt的第三部分是一个签证信息，这个签证信息由三部分组成：

- **header (base64后的)**
- **payload (base64后的)**
- **secret**

这个部分需要base64加密后的header和base64加密后的payload使用`.`连接组成的字符串

然后通过header中声明的加密方式进行**加盐`secret`**组合加密，然后就构成了jwt的第三部分。

```javascript
// javascript
var encodedString = base64UrlEncode(header) + '.' + base64UrlEncode(payload);

var signature = HMACSHA256(encodedString, 'secret'); // TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ
```

注意：secret是保存在服务器端的，jwt的签发生成也是在服务器端的，secret就是用来进行jwt的签发和jwt的验证，所以，它就是你**服务端的私钥**，在任何场景都不应该流露出去。一旦客户端得知这个secret, 那就意味着客户端是可以自我签发jwt了。



### 应用

一般是在请求头里加入`Authorization`，并加上`Bearer`标注：



```bash
fetch('api/user/1', {
  headers: {
    'Authorization': 'Bearer ' + token
  }
})
```



服务端会验证token，如果验证通过就会返回相应的资源。整个流程就是这样的:

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/1821058-2e28fe6c997a60c9.webp)



### Summery

#### 优点

- 因为json的通用性，所以JWT是可以进行跨语言支持的，像JAVA,JavaScript,NodeJS,PHP等很多语言都可以使用。
- 因为有了payload部分，所以JWT可以在自身存储一些其他业务逻辑所必要的非敏感信息。
- 便于传输，jwt的构成非常简单，字节占用很小，所以它是非常便于传输的。
- 它不需要在服务端保存会话信息, 所以它易于应用的扩展

#### 安全相关

- 不应该在jwt的payload部分存放敏感信息，因为该部分是客户端可解密的部分。
- 保护好secret私钥，该私钥非常重要。
- 如果可以，请使用https协议






































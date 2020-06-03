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






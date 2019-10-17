# 爬虫

## 使用到的库

urllib\requests\bs4

（http的书推荐：图解http协议）



## respones常用属性

- r.status_code：HTTP请求返回的状态，200表示成功，404表示失败
- r.text：HTTP响应页面的字符串格式，即url的字符串内容
- r.encoding：从HTTP头部猜测出来的网页的编码方式
- r.apparent_encoding：从内容中分析出来的响应内容的编码方式
- r.content：HTTP响应内容的二进制形式



## requests的基本方法

- requests.request()：构造一个请求，支撑以下各方法的基本方法
- requests.get()：获取HTML网页的主要方法，对应HTTP的GET
- requests.head()：获取HTML网页头部信息的方法，对应于HTTP的HEAD
- requests.post()：向HTML网页提交POST请求的方法，对应于HTTP的POST
- requests.put()：向HTML网页提交PUT请求的方法，对应于HTTP的PUT
- requests.patch()：向HTML网页提交局部修改请求，对应于HTTP的PATCH
- requests.delete()：向HTTP网页提交删除请求，对应于HTTP的DELETE

### 对应参数

- params：字典或者字节序列，作为参数增加到url中
- data：字典、字节序列或文件对象，作为Request的内容
- json：JSON格式的数据，作为Request的内容
- headers：字典，HTTP定制头
- cookise：字典或者CookieJar，Request中的cookie
- auth：元组，支持HTTP认证功能
- file：字典类型，传输文件
- timeout：设定超时时间，秒为单位
- proxies：字典类型，设定访问代理服务器，可增加登录认证
- allow_redirects：True/False，默认为True，重定向开关
- stream：True/False，默认为True，获取内容立即下载
- verify：True/False，默认为True，认证SSL证书开关
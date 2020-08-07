# Nginx

> 参考文档：https://juejin.im/post/5d81906c518825300a3ec7ca

## 下载及安装

**官网下载地址：**https://nginx.org/en/download.html

下载对应的版本的nginx压缩包，解压到自己电脑上存放软件的文件夹中即可

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/20200728223641.png)



1. 直接双击该目录下的nginx.exe，即可启动nginx服务器

2.  命令行计入该文件夹，执行nginx命令，也会直接启动nginx服务器



打开浏览器，输入地址：[http://localhost](https://links.jianshu.com/go?to=http%3A%2F%2Flocalhost)，访问页面，出现如下页面表示访问成功（默认80端口）

![](https://gitee.com/wextree/Wex_imgs/raw/master/img/20200728224103.png)



**命令行进入nginx根目录，执行如下命令，停止服务器：**

- 强制停止nginx服务器，如果有未处理的数据，丢弃。

  `D:/resp_application/nginx-1.13.5> nginx -s stop`

- 如果有未处理的数据，等待处理完成之后停止

  D:/resp_application/nginx-1.13.5> nginx -s quit



### issuse

第一次启动nginx可能会造成闪退问题，我查了可能的原因有以下：

- nginx的端口被占用，修改nginx.conf文件中的 端口（我是这一种）
- 部署ssl证书配置问题
- nginx的启动路径中有中文



## nginx 服务架构

### 模块化架构

> nginx 服务器的开发`完全`遵循**模块化设计**思想
>
> 1. 单一职责原则，每个模块只负责一个功能
> 2. 将程序进行分解，自顶向下，逐步求精
> 3. 高内聚，低耦合



#### nginx的模块化结构

- **核心模块：** nginx最基本的核心模块，负责进程管理、权限控制和日志记录等。
- **标准HTTP模块：** nginx标准的HTTP功能。
- **可选的HTTP模块：** 处理一些特殊的HTTP请求。
- **邮件服务模块：** 邮件服务功能。
- **第三方模块：** 作为扩展，完成特殊功能。



### nginx 的模块清单

**核心模块：**

- ngx_core
- ngx_errlog
- ngx_conf
- ngx_events
- ngx_event_core
- ngx_epll
- ngx_regex



**标准 HTTP 模块：**

- ngx_http
- ngx_http_core             #配置端口，URI 分析，服务器相应错误处理，别名控制 (alias) 等
- ngx_http_log              #自定义 access 日志
- ngx_http_upstream         #定义一组服务器，可以接受来自 proxy, Fastcgi,Memcache 的重定向；主要用作负载均衡
- ngx_http_static
- ngx_http_autoindex        #自动生成目录列表
- ngx_http_index            #处理以`/`结尾的请求，如果没有找到 index 页，则看是否开启了`random_index`；如开启，则用之，否则用 autoindex
- ngx_http_auth_basic       #基于 http 的身份认证 (auth_basic)
- ngx_http_access           #基于 IP 地址的访问控制 (deny,allow)
- ngx_http_limit_conn       #限制来自客户端的连接的响应和处理速率
- ngx_http_limit_req        #限制来自客户端的请求的响应和处理速率
- ngx_http_geo
- ngx_http_map              #创建任意的键值对变量
- ngx_http_split_clients
- ngx_http_referer          #过滤 HTTP 头中 Referer 为空的对象
- ngx_http_rewrite          #通过正则表达式重定向请求
- ngx_http_proxy
- ngx_http_fastcgi          #支持 fastcgi
- ngx_http_uwsgi
- ngx_http_scgi
- ngx_http_memcached
- ngx_http_empty_gif        #从内存创建一个 1×1 的透明 gif 图片，可以快速调用
- ngx_http_browser          #解析 http 请求头部的 User-Agent 值
- ngx_http_charset          #指定网页编码
- ngx_http_upstream_ip_hash
- ngx_http_upstream_least_conn
- ngx_http_upstream_keepalive
- ngx_http_write_filter
- ngx_http_header_filter
- ngx_http_chunked_filter
- ngx_http_range_header
- ngx_http_gzip_filter
- ngx_http_postpone_filter
- ngx_http_ssi_filter
- ngx_http_charset_filter
- ngx_http_userid_filter
- ngx_http_headers_filter   #设置 http 响应头
- ngx_http_copy_filter
- ngx_http_range_body_filter
- ngx_http_not_modified_filter



**可选 HTTP 模块：**

- ngx_http_addition         #在响应请求的页面开始或者结尾添加文本信息
- ngx_http_degradation      #在低内存的情况下允许服务器返回 444 或者 204 错误
- ngx_http_perl
- ngx_http_flv              #支持将 Flash 多媒体信息按照流文件传输，可以根据客户端指定的开始位置返回 Flash
- ngx_http_geoip            #支持解析基于 GeoIP 数据库的客户端请求
- ngx_google_perftools
- ngx_http_gzip             #gzip 压缩请求的响应
- ngx_http_gzip_static      #搜索并使用预压缩的以.gz 为后缀的文件代替一般文件响应客户端请求
- ngx_http_image_filter     #支持改变 png，jpeg，gif 图片的尺寸和旋转方向
- ngx_http_mp4              #支持.mp4,.m4v,.m4a 等多媒体信息按照流文件传输，常与 ngx_http_flv 一起使用
- ngx_http_random_index     #当收到 / 结尾的请求时，在指定目录下随机选择一个文件作为 index
- ngx_http_secure_link      #支持对请求链接的有效性检查
- ngx_http_ssl              #支持 https
- ngx_http_stub_status
- ngx_http_sub_module       #使用指定的字符串替换响应中的信息
- ngx_http_dav              #支持 HTTP 和 WebDAV 协议中的 PUT/DELETE/MKCOL/COPY/MOVE 方法
- ngx_http_xslt             #将 XML 响应信息使用 XSLT 进行转换



**邮件服务模块：**

- ngx_mail_core
- ngx_mail_pop3
- ngx_mail_imap
- ngx_mail_smtp
- ngx_mail_auth_http
- ngx_mail_proxy
- ngx_mail_ssl



**第三方模块：**

- echo-nginx-module         #支持在 nginx 配置文件中使用 echo/sleep/time/exec 等类 Shell 命令
- memc-nginx-module
- rds-json-nginx-module     #使 nginx 支持 json 数据的处理
- lua-nginx-module



### nginx 的web 请求处理机制



作为一个服务器软件，必须能够处理多个客户端发来的请求，工作方式主要有以下三种：

- 多进程 (Apache)
  - 优点：设计和实现简单；子进程独立
  - 缺点：生成一个子进程要内存复制，在资源和时间上造成额外开销
- 多线程 (IIS)
  - 优点：开销小
  - 缺点：开发者自己要对内存进行管理；线程之间会相互影响
- 异步方式 (nginx)



**通信中的同步和异步：**

- **同步：** 发送方发送完一个请求之后，需要等待接收方的确认回应之后，才能发送下一个请求。
- **异步：** 发送方发送完一个请求之后，不需要等待，直接可以发送下一个请求。



## nginx配置实例

```properties
#定义 nginx 运行的用户和用户组
user www www;

#nginx 进程数，建议设置为等于 CPU 总核心数。
worker_processes 8;

#nginx 默认没有开启利用多核 CPU, 通过增加 worker_cpu_affinity 配置参数来充分利用多核 CPU 以下是 8 核的配置参数
worker_cpu_affinity 00000001 00000010 00000100 00001000 00010000 00100000 01000000 10000000;

#全局错误日志定义类型，[ debug | info | notice | warn | error | crit ]
error_log /var/log/nginx/error.log info;

#进程文件
pid /var/run/nginx.pid;

#一个 nginx 进程打开的最多文件描述符数目，理论值应该是最多打开文件数（系统的值 ulimit -n）与 nginx 进程数相除，但是 nginx 分配请求并不均匀，所以建议与 ulimit -n 的值保持一致。
worker_rlimit_nofile 65535;

#工作模式与连接数上限
events
{
    #参考事件模型，use [ kqueue | rtsig | epoll | /dev/poll | select | poll ]; epoll 模型是 Linux 2.6 以上版本内核中的高性能网络 I/O 模型，如果跑在 FreeBSD 上面，就用 kqueue 模型。
    #epoll 是多路复用 IO(I/O Multiplexing) 中的一种方式，但是仅用于 linux2.6 以上内核，可以大大提高 nginx 的性能
    use epoll;

    ############################################################################
    #单个后台 worker process 进程的最大并发链接数
    #事件模块指令，定义 nginx 每个进程最大连接数，默认 1024。最大客户连接数由 worker_processes 和 worker_connections 决定
    #即 max_client=worker_processes*worker_connections, 在作为反向代理时：max_client=worker_processes*worker_connections / 4
    worker_connections 65535;
    ############################################################################
}

#设定 http 服务器
http {
    include mime.types; #文件扩展名与文件类型映射表
    default_type application/octet-stream; #默认文件类型
    #charset utf-8; #默认编码

    server_names_hash_bucket_size 128; #服务器名字的 hash 表大小
    client_header_buffer_size 32k; #上传文件大小限制
    large_client_header_buffers 4 64k; #设定请求缓
    client_max_body_size 8m; #设定请求缓
    sendfile on; #开启高效文件传输模式，sendfile 指令指定 nginx 是否调用 sendfile 函数来输出文件，对于普通应用设为 on，如果用来进行下载等应用磁盘 IO 重负载应用，可设置为 off，以平衡磁盘与网络 I/O 处理速度，降低系统的负载。注意：如果图片显示不正常把这个改成 off。
    autoindex on; #开启目录列表访问，合适下载服务器，默认关闭。
    tcp_nopush on; #防止网络阻塞
    tcp_nodelay on; #防止网络阻塞

    ##连接客户端超时时间各种参数设置##
    keepalive_timeout  120;          #单位是秒，客户端连接时时间，超时之后服务器端自动关闭该连接 如果 nginx 守护进程在这个等待的时间里，一直没有收到浏览发过来 http 请求，则关闭这个 http 连接
    client_header_timeout 10;        #客户端请求头的超时时间
    client_body_timeout 10;          #客户端请求主体超时时间
    reset_timedout_connection on;    #告诉 nginx 关闭不响应的客户端连接。这将会释放那个客户端所占有的内存空间
    send_timeout 10;                 #客户端响应超时时间，在两次客户端读取操作之间。如果在这段时间内，客户端没有读取任何数据，nginx 就会关闭连接
    ################################

    #FastCGI 相关参数是为了改善网站的性能：减少资源占用，提高访问速度。下面参数看字面意思都能理解。
    fastcgi_connect_timeout 300;
    fastcgi_send_timeout 300;
    fastcgi_read_timeout 300;
    fastcgi_buffer_size 64k;
    fastcgi_buffers 4 64k;
    fastcgi_busy_buffers_size 128k;
    fastcgi_temp_file_write_size 128k;

    ###作为代理缓存服务器设置#######
    ###先写到 temp 再移动到 cache
    #proxy_cache_path /var/tmp/nginx/proxy_cache levels=1:2 keys_zone=cache_one:512m inactive=10m max_size=64m;
    ###以上 proxy_temp 和 proxy_cache 需要在同一个分区中
    ###levels=1:2 表示缓存级别，表示缓存目录的第一级目录是 1 个字符，第二级目录是 2 个字符 keys_zone=cache_one:128m 缓存空间起名为 cache_one 大小为 512m
    ###max_size=64m 表示单个文件超过 128m 就不缓存了  inactive=10m 表示缓存的数据，10 分钟内没有被访问过就删除
    #########end####################

    #####对传输文件压缩###########
    #gzip 模块设置
    gzip on; #开启 gzip 压缩输出
    gzip_min_length 1k; #最小压缩文件大小
    gzip_buffers 4 16k; #压缩缓冲区
    gzip_http_version 1.0; #压缩版本（默认 1.1，前端如果是 squid2.5 请使用 1.0）
    gzip_comp_level 2; #压缩等级，gzip 压缩比，1 为最小，处理最快；9 为压缩比最大，处理最慢，传输速度最快，也最消耗 CPU；
    gzip_types text/plain application/x-javascript text/css application/xml;
    #压缩类型，默认就已经包含 text/html，所以下面就不用再写了，写上去也不会有问题，但是会有一个 warn。
    gzip_vary on;
    ##############################

    #limit_zone crawler $binary_remote_addr 10m; #开启限制 IP 连接数的时候需要使用

    upstream blog.ha97.com {
        #upstream 的负载均衡，weight 是权重，可以根据机器配置定义权重。weigth 参数表示权值，权值越高被分配到的几率越大。
        server 192.168.80.121:80 weight=3;
        server 192.168.80.122:80 weight=2;
        server 192.168.80.123:80 weight=3;
    }

    #虚拟主机的配置
    server {
        #监听端口
        listen 80;

        #############https##################
        #listen 443 ssl;
        #ssl_certificate /opt/https/xxxxxx.crt;
        #ssl_certificate_key /opt/https/xxxxxx.key;
        #ssl_protocols SSLv3 TLSv1;
        #ssl_ciphers HIGH:!ADH:!EXPORT57:RC4+RSA:+MEDIUM;
        #ssl_prefer_server_ciphers on;
        #ssl_session_cache shared:SSL:2m;
        #ssl_session_timeout 5m;
        ####################################end

        #域名可以有多个，用空格隔开
        server_name www.ha97.com ha97.com;
        index index.html index.htm index.php;
        root /data/www/ha97;
        location ~ .*.(php|php5)?$ {
            fastcgi_pass 127.0.0.1:9000;
            fastcgi_index index.php;
            include fastcgi.conf;
        }

        #图片缓存时间设置
        location ~ .*.(gif|jpg|jpeg|png|bmp|swf)$ {
            expires 10d;
        }

        #JS 和 CSS 缓存时间设置
        location ~ .*.(js|css)?$ {
            expires 1h;
        }

        #日志格式设定
        log_format access '$remote_addr - $remote_user [$time_local] "$request" ' '$status $body_bytes_sent "$http_referer" ' '"$http_user_agent" $http_x_forwarded_for';

        #定义本虚拟主机的访问日志
        access_log /var/log/nginx/ha97access.log access;

        #对 "/" 启用反向代理
        location / {
            proxy_pass http://127.0.0.1:88;
            proxy_redirect off;
            proxy_set_header X-Real-IP $remote_addr;
            #后端的 Web 服务器可以通过 X-Forwarded-For 获取用户真实 IP
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            #以下是一些反向代理的配置，可选。
            proxy_set_header Host $host;
            client_max_body_size 10m; #允许客户端请求的最大单文件字节数
            client_body_buffer_size 128k; #缓冲区代理缓冲用户端请求的最大字节数，

            ##代理设置 以下设置是 nginx 和后端服务器之间通讯的设置##
            proxy_connect_timeout 90; #nginx 跟后端服务器连接超时时间（代理连接超时）
            proxy_send_timeout 90; #后端服务器数据回传时间（代理发送超时）
            proxy_read_timeout 90; #连接成功后，后端服务器响应时间（代理接收超时）
            proxy_buffering on;    #该指令开启从后端被代理服务器的响应内容缓冲 此参数开启后 proxy_buffers 和 proxy_busy_buffers_size 参数才会起作用
            proxy_buffer_size 4k;  #设置代理服务器（nginx）保存用户头信息的缓冲区大小
            proxy_buffers 4 32k;   #proxy_buffers 缓冲区，网页平均在 32k 以下的设置
            proxy_busy_buffers_size 64k; #高负荷下缓冲大小（proxy_buffers*2）
            proxy_max_temp_file_size 2048m; #默认 1024m, 该指令用于设置当网页内容大于 proxy_buffers 时，临时文件大小的最大值。如果文件大于这个值，它将从 upstream 服务器同步地传递请求，而不是缓冲到磁盘
            proxy_temp_file_write_size 512k; 这是当被代理服务器的响应过大时 nginx 一次性写入临时文件的数据量。
            proxy_temp_path  /var/tmp/nginx/proxy_temp;    ##定义缓冲存储目录，之前必须要先手动创建此目录
            proxy_headers_hash_max_size 51200;
            proxy_headers_hash_bucket_size 6400;
            #######################################################
        }

        #设定查看 nginx 状态的地址
        location /nginxStatus {
            stub_status on;
            access_log on;
            auth_basic "nginxStatus";
            auth_basic_user_file conf/htpasswd;
            #htpasswd 文件的内容可以用 apache 提供的 htpasswd 工具来产生。
        }

        #本地动静分离反向代理配置
        #所有 jsp 的页面均交由 tomcat 或 resin 处理
        location ~ .(jsp|jspx|do)?$ {
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_pass http://127.0.0.1:8080;
        }

        #所有静态文件由 nginx 直接读取不经过 tomcat 或 resin
        location ~ .*.(htm|html|gif|jpg|jpeg|png|bmp|swf|ioc|rar|zip|txt|flv|mid|doc|ppt|pdf|xls|mp3|wma)$
        { expires 15d; }

        location ~ .*.(js|css)?$
        { expires 1h; }
    }
}

```



## nginx服务器基础配置指令

### nginx.conf 文件的结构

> 所有指令都以 ; 结尾

- **Global：** nginx运行相关
- **events：** 与用户的网络连接相关
- **http**
  - **http Global：** 代理、缓存、日志以及第三方模块的配置
  - **server**
    - **server Global：** 虚拟机相关
    - **location：** 地址定向、数据缓存、应答控制，以及第三方模块的配置



### nginx 运行相关的Global部分

#### 配置运行nginx服务器的用户

`user nobody nobody`

#### 配置允许生成的 worker process 数

> 这个数字，跟电脑 CPU 核数要保持一致

`worker_processes auto; `

`worker_processes 4;`

#### 配置 nginx 进程 PID 存放路径

> 这里面保存的就是一个数字，nginx master 进程的进程号

`pid logs/nginx.pid;`

#### 配置错误日志的存放路径

`error_log logs/error.log;`

 `error_log logs/error.log error;`

#### 配置文件的引入

`include mime.types; `

`include fastcgi_params; include ../../conf/*.conf;`



### 与用户的网络连接相关的events

#### 设置网络连接的序列化

> 对多个 nginx 进程接收连接进行序列化，防止多个进程对连接的争抢（惊群）

`accept_mutex on;`

#### 设置是否允许同时接收多个网络连接

`multi_accept off;`

#### 事件驱动模型的选择

`use select|poll|kqueue|epoll|rtsig|/dev/poll|eventport`

#### 配置最大连接数

`worker_connections 512;`



### http

#### http Global 代理 - 缓存 - 日志 - 第三方模块配置

**定义 MIME-Type**

`include mime.types; `

`default_type application/octet-stream;`



**自定义服务日志**

`access_log logs/access.log main; `

`access_log off;`





#### server










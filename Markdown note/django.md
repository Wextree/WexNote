## 创建项目

django-admin startproject name 

- manage.py         一个命令行工具，用来交互

- _init_.py        一个空文件，告诉python这个目录是一个python包

- urls.py       项目的URL声明

- wsgi.py      项目与WSGI兼容的Web服务器入口

### 创建应用

  进入project文件里面

  python manage.py startapp name

**要在setting里面把APP的名字写进去INSTALLED_APPS**

在models里面定义模型

from django.db import models(要继承里面的model类)

### 迁移

python manage.py makemigrations			生成迁移文件，此时没有生成表

python manage.py migrate

## 启动服务器

python manage.py runserver ip:port

ip可以不写，默认主机IP

轻量级web服务器，仅仅在开发测试中



## 创建用户管理

python manage.py createsuperuser

依次输入用户名、邮箱、密码	



## 指定配置文件

例子（Windows shell）:

```
set DJANGO_SETTINGS_MODULE=mysite.settings
django-admin runserver
```

使用 `--settings` 命令行参数指定配置模块路径:

```
django-admin runserver --settings=mysite.settings
```

在服务器环境下，要通知WSGI应用当前使用的是哪个配置文件，用到 `os.environ`:

```
import os

os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'
```



## 查看你已经更改了哪个配置项

在命令行输入 `python manage.py diffsettings` 就会显示当前配置与默认配置中的不同
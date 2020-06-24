# Shiro

> 参考文档：https://zhuanlan.zhihu.com/p/54176956

## 简介

> **Apache Shiro™** is a powerful and easy-to-use Java security framework that performs authentication, authorization, cryptography, and session management. 

简而言之，Apache Shiro 是一个强大灵活的开源安全框架，可以完全处理身份验证、授权、加密和会话管理。

**功能：**

- 验证用户身份
- 用户访问权限控制，比如：1、判断用户是否分配了一定的安全角色。2、判断用户是否被授予完成某个操作的权限
- 在非 Web 或 EJB 容器的环境下可以任意使用Session API
- 可以响应认证、访问控制，或者 Session 生命周期中发生的事件
- 可将一个或以上用户安全数据源数据组合成一个复合的用户 “view”(视图)
- 支持单点登录(SSO)功能
- 支持提供“Remember Me”服务，获取用户关联信息而无需登录
  ···



## 使用原因与特性

**原因：**

- **易于使用**——易用性是项目的最终目标。应用程序安全非常令人困惑和沮丧,被认为是“不可避免的灾难”。如果你让它简化到新手都可以使用它,它就将不再是一种痛苦了。
- **全面**——没有其他安全框架的宽度范围可以同Apache Shiro一样,它可以成为你的“一站式”为您的安全需求提供保障。
- **灵活**——Apache Shiro可以**在任何应用程序环境中**工作。虽然在网络工作、EJB和IoC环境中可能并不需要它。但Shiro的授权也没有任何规范,甚至没有许多依赖关系。
- **Web支持**——Apache Shiro拥有令人兴奋的web应用程序支持,允许您基于应用程序的url创建灵活的安全策略和网络协议(例如REST),同时还提供一组JSP库控制页面输出。
- **低耦合**——Shiro干净的API和设计模式使它容易与许多其他框架和应用程序集成。你会看到**Shiro无缝地集成Spring这样的框架**, 以及Grails, Wicket, Tapestry, Mule, Apache Camel, Vaadin…等。
- **被广泛支持**——Apache Shiro是Apache软件基金会的一部分。项目开发和用户组都有友好的网民愿意帮助。这样的商业公司如果需要Katasoft还提供专业的支持和服务。



**特性：**

**Shiro功能的框架图：**

![](imgs\v2-e72930a8351ccf1590779ea87ac5cb65_720w.jpg)

**Authentication（认证）, Authorization（授权）, Session Management（会话管理）, Cryptography（加密）**被 Shiro 框架的开发团队称之为应用安全的四大基石：

- **Authentication（认证）：**用户身份识别，通常被称为用户“登录”
- **Authorization（授权）：**访问控制。比如某个用户是否具有某个操作的使用权限。
- **Session Management（会话管理）：**特定于用户的会话管理,甚至在非web 或 EJB 应用程序。
- **Cryptography（加密）：**在对数据源使用加密算法加密的同时，保证易于使用。



**其他功能的支持：**

- **Web支持：**Shiro的Web支持API有助于保护Web应用程序。
- **缓存：**缓存是Apache Shiro API中的第一级，以确保安全操作保持快速和高效。
- **并发性：**Apache Shiro支持具有并发功能的多线程应用程序。
- **测试：**存在测试支持，可帮助您编写单元测试和集成测试，并确保代码按预期得到保障。
- **“运行方式”：**允许用户承担另一个用户的身份(如果允许)的功能，有时在管理方案中很有用。
- **“记住我”：**记住用户在会话中的身份，所以用户只需要强制登录即可。



## 高级概述

在概念层，Shiro 架构包含三个主要的理念：Subject , SecurityManager 和 Realm。



![](imgs\v2-c0841dfc8cb19a94c322eef635371cf6_720w.jpg)

- **Subject：**当前用户，Subject 可以是一个人，但也可以是第三方服务、守护进程帐户、时钟守护任务或者其它–当前和软件交互的任何事件。
- **SecurityManager：**管理所有Subject，SecurityManager 是 Shiro 架构的核心，配合内部安全组件共同组成安全伞。
- **Realms：**用于进行权限信息的验证，我们自己实现。**Realm 本质上是一个特定的安全 DAO**：它封装与数据源连接的细节，得到Shiro 所需的相关的数据。在配置 Shiro 的时候，你必须指定至少一个Realm 来实现认证（authentication）和/或授权（authorization）。

我们需要实现Realms的Authentication 和 Authorization。其中 Authentication 是用来验证用户身份，Authorization 是授权访问控制，用于对用户进行的操作授权，证明该用户是否允许进行当前操作，如访问某个链接，某个资源文件等。





## 认证过程

![](imgs\v2-2531156d2e6fb3ec0702f1d1ed795f43_720w.jpg)





**实现：**（建立一个maven项目）



pom.xml

```xml
<dependencies>
    <dependency>
        <groupId>org.apache.shiro</groupId>
        <artifactId>shiro-core</artifactId>
        <version>1.5.3</version>
    </dependency>

    <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.13</version>
        <scope>test</scope>
    </dependency>
</dependencies>
```



新建一个测试类：

```java
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.mgt.DefaultSecurityManager;
import org.apache.shiro.realm.SimpleAccountRealm;
import org.apache.shiro.subject.Subject;
import org.junit.Before;
import org.junit.Test;


public class AuthenticationTest {

    // 创建一个验证对象
    SimpleAccountRealm simpleAccountRealm = new SimpleAccountRealm();

    @Before // 在方法开始前先添加一个用户
    public void addUser(){
        // 添加账号密码
        simpleAccountRealm.addAccount("wextree", "110325");
    }

    @Test
    public void testAuthentication(){
        // 1. 构建SecurityManager环境
        DefaultSecurityManager defaultSecurityManager = new DefaultSecurityManager();
        defaultSecurityManager.setRealm(simpleAccountRealm);

        // 2. 主题提交认证请求
        SecurityUtils.setSecurityManager(defaultSecurityManager);
        Subject subject = SecurityUtils.getSubject();

        // 模拟注入账户密码的token，并且登陆
        UsernamePasswordToken token = new UsernamePasswordToken("wextree", "110325");
        subject.login(token);

        // subject.isAuthenticated()方法返回一个boolean值,用于判断用户是否认证成功
        System.out.println("isAuthenticated:" + subject.isAuthenticated()); // 输出true

        subject.logout(); // 登出

        System.out.println("isAuthenticated:" + subject.isAuthenticated()); // 输出false
    }
}
```



**运行结果：**

```verilog
SLF4J: Failed to load class "org.slf4j.impl.StaticLoggerBinder".
SLF4J: Defaulting to no-operation (NOP) logger implementation
SLF4J: See http://www.slf4j.org/codes.html#StaticLoggerBinder for further details.
isAuthenticated:true
isAuthenticated:false

Process finished with exit code 0
```



**主要流程：**



![](E:\git\WexNote\Markdown note\imgs\v2-b18b91b5d198827d4140d0cc0a39dbe1_720w.jpg)



1. 首先调用` Subject.login(token) `进行登录，其会自动委托给 Security Manager，调用之前必须通`SecurityUtils.setSecurityManager() `设置；
2. SecurityManager 负责真正的身份验证逻辑；它会委托给 **Authenticator** 进行身份验证；
3. Authenticator 才是真正的身份验证者，Shiro API 中核心的身份认证入口点，此处可以自定义插入自己的实现；
4. Authenticator 可能会委托给相应的 AuthenticationStrategy 进行**多 Realm 身份验证**，默认 ModularRealmAuthenticator 会调用 AuthenticationStrategy 进行多 Realm 身份验证；
5. **Authenticator 会把相应的 token 传入 Realm**，从 Realm 获取身份验证信息，如果**没有返回 / 抛出异常表示身份验证失败**了。此处可以配置多个 Realm，将**按照相应的顺序及策略**进行访问。



## 授权过程

```java
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.mgt.DefaultSecurityManager;
import org.apache.shiro.realm.SimpleAccountRealm;
import org.apache.shiro.subject.Subject;
import org.junit.Before;
import org.junit.Test;


public class AuthenticationTest {

    // 创建一个验证对象
    SimpleAccountRealm simpleAccountRealm = new SimpleAccountRealm();

    @Before // 在方法开始前先添加一个用户
    public void addUser(){
        // 添加账号密码
        simpleAccountRealm.addAccount("wextree", "110325", "admin", "user");
    }

    @Test
    public void testAuthentication(){
        // 1. 构建SecurityManager环境
        DefaultSecurityManager defaultSecurityManager = new DefaultSecurityManager();
        defaultSecurityManager.setRealm(simpleAccountRealm);

        // 2. 主题提交认证请求
        SecurityUtils.setSecurityManager(defaultSecurityManager);
        Subject subject = SecurityUtils.getSubject();

        // 模拟注入账户密码的token，并且登陆
        UsernamePasswordToken token = new UsernamePasswordToken("wextree", "110325");
        subject.login(token);

        // subject.isAuthenticated()方法返回一个boolean值,用于判断用户是否认证成功
        System.out.println("isAuthenticated:" + subject.isAuthenticated()); // 输出true

        // 判断subject是否具有admin和user两个角色权限,如没有则会报错
                subject.checkRoles("admin","user");
        
        // org.apache.shiro.authz.UnauthorizedException: Subject does not have role [xxx]
        // subject.checkRole("xxx"); // 报错

    }
}

```





## 自定义Realm



Shiro 框架内部默认提供了两种实现Realm，一种是查询`.ini`文件的`IniRealm`，另一种是查询数据库的`JdbcRealm`。

先在合适的包下创建一个【MyRealm】类，继承 Shirot 框架的 AuthorizingRealm 类，并实现默认的两个方法：

```java
package wex.realm;

import org.apache.shiro.authc.AuthenticationException;
import org.apache.shiro.authc.AuthenticationInfo;
import org.apache.shiro.authc.AuthenticationToken;
import org.apache.shiro.authc.SimpleAuthenticationInfo;
import org.apache.shiro.authz.AuthorizationInfo;
import org.apache.shiro.authz.SimpleAuthorizationInfo;
import org.apache.shiro.realm.AuthorizingRealm;
import org.apache.shiro.subject.PrincipalCollection;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class MyRealm extends AuthorizingRealm {
    // 模拟数据库
    Map<String, String> userMap = new HashMap<>(16);

    {
        userMap.put("wextree", "110325");
        super.setName("myRealm"); // 设置自定义Realm的名称
    }

    // 授权
    @Override
    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principalCollection) {
        String userName = (String)principalCollection.getPrimaryPrincipal();
        // 从数据库获取角色和权限信息
        Set<String> roles = getRolesByUserName(userName);
        Set<String> permissions = getPermissionsByUserName(userName);

        SimpleAuthorizationInfo simpleAuthorizationInfo = new SimpleAuthorizationInfo();
        simpleAuthorizationInfo.setStringPermissions(permissions);
        simpleAuthorizationInfo.setRoles(roles);

        return simpleAuthorizationInfo;
    }

    private Set<String> getPermissionsByUserName(String userName) {
        Set<String> permissions = new HashSet<>();
        permissions.add("user:delete");
        permissions.add("user:add");
        return permissions;
    }

    private Set<String> getRolesByUserName(String userName) {
        Set<String> roles = new HashSet<>();
        roles.add("admin");
        roles.add("user");
        return roles;
    }

    // 认证
    @Override
    protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken authenticationToken) throws AuthenticationException {
        // 1. 从主体传过来的认证信息中，获取用户名
        String userName = (String)authenticationToken.getPrincipal();

        // 2. 通过用户名到数据库中获取凭证
        String password = getPasswordByUserName(userName);
        if (password == null)
            return null;

        SimpleAuthenticationInfo authenticationInfo = new SimpleAuthenticationInfo("wextree", password, "myRealm");

        return authenticationInfo;
    }

    private String getPasswordByUserName(String userName) {
        return userMap.get(userName);
    }
}

```



**写一个测试类去测试：**

```java
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.mgt.DefaultSecurityManager;
import org.apache.shiro.realm.SimpleAccountRealm;
import org.apache.shiro.subject.Subject;
import org.junit.Before;
import org.junit.Test;
import wex.realm.MyRealm;

import java.util.Set;


public class AuthenticationTest {

    @Test
    public void testAuthentication2(){
        MyRealm myRealm = new MyRealm();
        DefaultSecurityManager defaultSecurityManager = new DefaultSecurityManager();
        defaultSecurityManager.setRealm(myRealm);

        SecurityUtils.setSecurityManager(defaultSecurityManager);
        Subject subject = SecurityUtils.getSubject();

        UsernamePasswordToken token = new UsernamePasswordToken("wextree", "110325");
        subject.login(token);

        // subject.isAuthenticated()方法返回一个boolean值,用于判断用户是否认证成功
        System.out.println("isAuthenticated:" + subject.isAuthenticated()); // 输出true
        // 判断subject是否具有admin和user两个角色权限,如没有则会报错
        subject.checkRoles("admin", "user");
//        subject.checkRole("xxx"); // 报错
        // 判断subject是否具有user:add权限
        subject.checkPermission("user:add");
    }
}

```



## Shiro加密

在之前的学习中，我们在数据库中保存的密码都是明文的，一旦数据库数据泄露，那就会造成不可估算的损失，所以我们通常都会使用非对称加密，简单理解也就是**不可逆**的加密，而 md5 加密算法就是符合这样的一种算法。

123456 用 Md5 加密后，得到的字符串：**e10adc3949ba59abbe56e057f20f883e**，就无法通过计算还原回 123456，我们把这个加密的字符串保存在数据库中，等下次用户登录时我们把密码通过同样的算法加密后再从数据库中取出这个字符串进行比较，就能够知道密码是否正确了，这样既保留了密码验证的功能又大大增加了安全性，**但是问题是：虽然无法直接通过计算反推回密码，但是我们仍然可以通过计算一些简单的密码加密后的 Md5 值进行比较，推算出原来的密码**

比如我的密码是 123456，你的密码也是，通过 md5 加密之后的字符串一致，所以你也就能知道我的密码了，如果我们把常用的一些密码都做 md5 加密得到一本字典，那么就可以得到相当一部分的人密码，这也就相当于“破解”了一样，所以其实也没有我们想象中的那么“安全”。



### 加盐 + 多次加密



既然相同的密码 md5 一样，那么我们就让我们的原始密码再**加一个随机数**，然后再进行 md5 加密，这个随机数就是我们说的**盐(salt)**，这样处理下来就能得到不同的 Md5 值，当然我们需要把这个随机数盐也保存进数据库中，以便我们进行验证。

另外我们可以通过**多次加密**的方法，即使黑客通过一定的技术手段拿到了我们的密码 md5 值，但它并不知道我们到底加密了多少次，所以这也使得破解工作变得艰难。

在 Shiro 框架中，对于这样的操作提供了简单的代码实现：

```java
@Test
public void testCpr(){
    String password = "123456";
    String salt = new SecureRandomNumberGenerator().nextBytes().toString();
    int time = 2;  // 加密次数：2
    String algorithmName = "md5";  // 加密算法

    String encodePassword = new SimpleHash(algorithmName, password, salt, time).toString();
    System.out.printf("原始密码是 %s , 盐是： %s, 运算次数是： %d, 运算出来的密文是：%s ",password,salt,time,encodePassword);

}
```

**输出：**

```
原始密码是 123456 , 盐是： de7Tz18HRhens9UxUtjhaQ==, 运算次数是： 2, 运算出来的密文是：1cc7298271d4058f04695b89a5cbcf9d
```



## SpringBoot项目



### 搭建环境

**pom：**

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>8.0.19</version>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-thymeleaf</artifactId>
</dependency>

<dependency>
    <groupId>org.apache.shiro</groupId>
    <artifactId>shiro-spring</artifactId>
    <version>1.5.3</version>
</dependency>

<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.18.12</version>
</dependency>
```



**yaml:**

```yaml
#thymeleaf 配置
spring:
  thymeleaf:
    mode: HTML5
    encoding: UTF-8
    servlet:
      content-type: text/html
    #缓存设置为false, 这样修改之后马上生效，便于调试
    cache: false
  # 数据库
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://localhost:3306/score_system?useSSL=false&serverTimezone=UTC
    password: 110325
    username: root
  jpa:
    properties:
      hibernate:
        hbm2ddl:
          auto: update
    show-sql: true

logging:
  level:
    com:
      wex:
        shiro_demo: debug

```



### 新建实体类

**mysql建表和插入数据语句：**

```mysql
# user_info
create table user_info(
    id int primary key auto_increment,
    username varchar(50) unique,
    name varchar(50),
    password varchar(100),
    salt varchar(50)    
);
INSERT INTO `user_info` (`id`,`name`,`password`,`salt`,`username`) VALUES (1, '管理员','951cd60dec2104024949d2e0b2af45ae', 'xbNIxrQfn6COSYn1/GdloA==', 'wmyskxz');

# sys_role
create table sys_role(
    id int primary key auto_increment,
    description varchar(50),
    name varchar(20)
);
INSERT INTO `sys_role` (`id`,`description`,`name`) VALUES (1,'管理员','admin');

# sys_permission
create table sys_permission(
    id int primary key auto_increment,
    description varchar(50),
    name varchar(50),
    url varchar(50)
);
INSERT INTO `sys_permission` (`id`,`description`,`name`,`url`) VALUES (1,'查询用户','userInfo:view','/userList');
INSERT INTO `sys_permission` (`id`,`description`,`name`,`url`) VALUES (2,'增加用户','userInfo:add','/userAdd');
INSERT INTO `sys_permission` (`id`,`description`,`name`,`url`) VALUES (3,'删除用户','userInfo:delete','/userDelete');

# sys_user_role
create table sys_user_role(
    role_id int,
    uid int,
    primary key(role_id, uid),
    foreign key(role_id) references sys_role(id),
    foreign key(uid) references user_info(id)
);
INSERT INTO `sys_user_role` (`role_id`,`uid`) VALUES (1,1);

# sys_role_permission
create table sys_role_permission(
    role_id int,
    permission_id int,
    primary key(role_id, permission_id),
    foreign key(role_id) references sys_role(id),
    foreign key(permission_id) references sys_permission(id)
);
INSERT INTO `sys_role_permission` (`permission_id`,`role_id`) VALUES (1,1);
INSERT INTO `sys_role_permission` (`permission_id`,`role_id`) VALUES (2,1);
```



**建立对应实体类：**

```java
// 用户
@Entity
@Table(name = "user_info")
@Data
public class UserInfo {
    @Id
    @GeneratedValue
    private Long id;

    @Column(unique = true)
    private String username;

    private String name; // 用于UI显示
    private String password;
    private String salt;

    @JsonIgnoreProperties(value = {"userInfos"})
    @ManyToMany(fetch = FetchType.EAGER)
    @JoinTable(name = "SysUserRole", joinColumns = @JoinColumn(name = "uid"), inverseJoinColumns = @JoinColumn(name = "roleId"))
    private List<SysRole> roles;
}

// 角色
@Entity
@Table(name = "sys_role")
@Data
public class SysRole {
    @Id
    @GeneratedValue
    private Long id; // 主键.
    private String name; // 角色名称,如 admin/user
    private String description; // 角色描述,用于UI显示

    // 角色 -- 权限关系：多对多
    @JsonIgnoreProperties(value = {"roles"})
    @ManyToMany(fetch = FetchType.EAGER)
    @JoinTable(name = "SysRolePermission", joinColumns = {@JoinColumn(name = "roleId")}, inverseJoinColumns = {@JoinColumn(name = "permissionId")})
    private List<SysPermission> permissions;

    // 用户 -- 角色关系：多对多
    @JsonIgnoreProperties(value = {"roles"})
    @ManyToMany
    @JoinTable(name = "SysUserRole", joinColumns = {@JoinColumn(name = "roleId")}, inverseJoinColumns = {@JoinColumn(name = "uid")})
    private List<UserInfo> userInfos;// 一个角色对应多个用户

}

// 权限
@Entity
@Table(name = "sys_permission")
@Data
public class SysPermission {
    @Id
    @GeneratedValue
    private Long id; // 主键.
    private String name; // 权限名称,如 user:select
    private String description; // 权限描述,用于UI显示
    private String url; // 权限地址.
    @JsonIgnoreProperties(value = {"permissions"})
    @ManyToMany
    @JoinTable(name = "SysRolePermission", joinColumns = {@JoinColumn(name = "permissionId")}, inverseJoinColumns = {@JoinColumn(name = "roleId")})
    private List<SysRole> roles; // 一个权限可以被多个角色使用
}
```



### 准备DAO层和Service层

```java
@Repository
public interface UserInfoDao extends JpaRepository<UserInfo, Long> {

    public UserInfo findByUsername(String username);
}


@Service
public interface UserInfoService {
    /** 通过username查找用户信息；*/
    public UserInfo findByUsername(String username);
}

@Service
public class UserInfoServiceImpl implements UserInfoService {
    @Autowired
    private UserInfoDao userInfoDao;

    @Override
    public UserInfo findByUsername(String username) {
        return userInfoDao.findByUsername(username);
    }
}


```





### 配置Shiro

**MyShiroRealm：**

```java
@Component
public class MyShiroRealm extends AuthorizingRealm {
    @Autowired
    private UserInfoService userInfoService;

    @Override
    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principalCollection) {
        // 能进入这里说明用户已经通过验证了
        UserInfo userInfo = (UserInfo) principalCollection.getPrimaryPrincipal();
        SimpleAuthorizationInfo simpleAuthorizationInfo = new SimpleAuthorizationInfo();
        for (SysRole role : userInfo.getRoles()) {
            simpleAuthorizationInfo.addRole(role.getName());
            for (SysPermission permission : role.getPermissions()) {
                simpleAuthorizationInfo.addStringPermission(permission.getName());
            }
        }
        return simpleAuthorizationInfo;
    }

    @Override
    protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken authenticationToken) throws AuthenticationException {
        String username = (String)authenticationToken.getPrincipal();
        System.out.println(authenticationToken.getPrincipal());

        UserInfo userInfo = userInfoService.findByUsername(username);
        if (null == userInfo){
            return null;
        }

        SimpleAuthenticationInfo simpleAuthenticationInfo = new SimpleAuthenticationInfo(
            userInfo,
            userInfo.getPassword(),
            ByteSource.Util.bytes(userInfo.getSalt()),
            getName()
        );
        return simpleAuthenticationInfo;
    }
}

```

**ShiroConfig:**（定义一系列的Filter）

Apache Shiro 的核心通过 Filter 来实现，就好像 SpringMvc 通过 DispachServlet 来主控制一样。 既然是使用 Filter 一般也就能猜到，是通过URL规则来进行过滤和权限校验，所以我们需要定义一系列关于URL的规则和访问权限。

Filter Chain定义说明：

- 1、一个URL可以配置多个Filter，使用逗号分隔
- 2、当设置多个过滤器时，全部验证通过，才视为通过
- 3、部分过滤器可指定参数，如perms，roles

Shiro内置的FilterChain，因表格显示问题，麻烦移步简书观看完整版..

- anon:所有url都都可以匿名访问
- authc: 需要认证才能进行访问
- user:配置记住我或认证通过可以访问

```java
package com.wex.shiro_demo.config;

import com.wex.shiro_demo.realm.MyShiroRealm;
import org.apache.shiro.authc.credential.HashedCredentialsMatcher;
import org.apache.shiro.mgt.SecurityManager;
import org.apache.shiro.spring.security.interceptor.AuthorizationAttributeSourceAdvisor;
import org.apache.shiro.spring.web.ShiroFilterFactoryBean;
import org.apache.shiro.web.mgt.DefaultWebSecurityManager;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.handler.SimpleMappingExceptionResolver;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Properties;

@Configuration
public class ShiroConfig {
    @Bean
    public ShiroFilterFactoryBean shiroFilter(SecurityManager securityManager){
        System.out.println("ShiroConfiguration.shirFilter()");
        ShiroFilterFactoryBean shiroFilterFactoryBean = new ShiroFilterFactoryBean();
        shiroFilterFactoryBean.setSecurityManager(securityManager);
        // 拦截器
        Map<String, String> filterChainDefinitionMap = new LinkedHashMap<>();
        // 配置不会被拦截的路径 顺序判断
        filterChainDefinitionMap.put("/static/**", "anon");
        // 配置退出 过滤器,其中的具体的退出代码Shiro已经替我们实现了
        filterChainDefinitionMap.put("/logout", "logout");
        // <!-- 过滤链定义，从上向下顺序执行，一般将/**放在最为下边 -->:这是一个坑呢，一不小心代码就不好使了;
        // <!-- authc:所有url都必须认证通过才可以访问; anon:所有url都都可以匿名访问-->
        filterChainDefinitionMap.put("/**", "authc");
        // 如果不设置默认会自动寻找Web工程根目录下的"/login.jsp"页面
        shiroFilterFactoryBean.setLoginUrl("/login");
        // 登录成功后要跳转的链接
        shiroFilterFactoryBean.setSuccessUrl("/index");

        //未授权界面;
        shiroFilterFactoryBean.setUnauthorizedUrl("/403");
        shiroFilterFactoryBean.setFilterChainDefinitionMap(filterChainDefinitionMap);
        return shiroFilterFactoryBean;
    }

    /**
     * 凭证匹配器
     * （由于我们的密码校验交给Shiro的SimpleAuthenticationInfo进行处理了）
     *
     * @return
     */
    @Bean
    public HashedCredentialsMatcher hashedCredentialsMatcher() {
        HashedCredentialsMatcher hashedCredentialsMatcher = new HashedCredentialsMatcher();
        hashedCredentialsMatcher.setHashAlgorithmName("md5"); // 散列算法:这里使用MD5算法;
        hashedCredentialsMatcher.setHashIterations(2); // 散列的次数，比如散列两次，相当于 md5(md5(""));
        return hashedCredentialsMatcher;
    }

    @Bean
    public MyShiroRealm myShiroRealm() {
        MyShiroRealm myShiroRealm = new MyShiroRealm();
        myShiroRealm.setCredentialsMatcher(hashedCredentialsMatcher());
        return myShiroRealm;
    }


    @Bean
    public SecurityManager securityManager() {
        DefaultWebSecurityManager securityManager = new DefaultWebSecurityManager();
        securityManager.setRealm(myShiroRealm());
        return securityManager;
    }

    /**
     * 开启shiro aop注解支持.
     * 使用代理方式;所以需要开启代码支持;
     *
     * @param securityManager
     * @return
     */
    @Bean
    public AuthorizationAttributeSourceAdvisor authorizationAttributeSourceAdvisor(SecurityManager securityManager) {
        AuthorizationAttributeSourceAdvisor authorizationAttributeSourceAdvisor = new AuthorizationAttributeSourceAdvisor();
        authorizationAttributeSourceAdvisor.setSecurityManager(securityManager);
        return authorizationAttributeSourceAdvisor;
    }

    @Bean(name = "simpleMappingExceptionResolver")
    public SimpleMappingExceptionResolver
        createSimpleMappingExceptionResolver() {
        SimpleMappingExceptionResolver r = new SimpleMappingExceptionResolver();
        Properties mappings = new Properties();
        mappings.setProperty("DatabaseException", "databaseError"); // 数据库异常处理
        mappings.setProperty("UnauthorizedException", "403");
        r.setExceptionMappings(mappings);  // None by default
        r.setDefaultErrorView("error");    // No default
        r.setExceptionAttribute("ex");     // Default is "exception"
        //r.setWarnLogCategory("example.MvcLogger");     // No default
        return r;
    }
}
```



### Controller层

```java
@Controller
public class HomeController {

    @RequestMapping({"/", "/index"})
    public String index(){
        return "/index";
    }
    @RequestMapping("/login")
    public String login(HttpServletRequest request, Map<String, Object> map) throws Exception{
        System.out.println("HomeController.login()");
        // 登录失败从request中获取shiro处理的异常信息。
        // shiroLoginFailure:就是shiro异常类的全类名.
        String exception = (String) request.getAttribute("shiroLoginFailure");
        System.out.println("exception=" + exception);
        String msg = "";
        if (exception != null) {
            if (UnknownAccountException.class.getName().equals(exception)) {
                System.out.println("UnknownAccountException -- > 账号不存在：");
                msg = "UnknownAccountException -- > 账号不存在：";
            } else if (IncorrectCredentialsException.class.getName().equals(exception)) {
                System.out.println("IncorrectCredentialsException -- > 密码不正确：");
                msg = "IncorrectCredentialsException -- > 密码不正确：";
            } else if ("kaptchaValidateFailed".equals(exception)) {
                System.out.println("kaptchaValidateFailed -- > 验证码错误");
                msg = "kaptchaValidateFailed -- > 验证码错误";
            } else {
                msg = "else >> "+exception;
                System.out.println("else -- >" + exception);
            }
        }
        map.put("msg", msg);
        // 此方法不处理登录成功,由shiro进行处理
        return "/login";
    }

    @RequestMapping("/403")
    public String unauthorizedRole(){
        System.out.println("------没有权限-------");
        return "403";
    }
}
```

```java
@RestController
public class UserInfoController {

    @Autowired
    private UserInfoService userInfoService;

    /**
     * 按username账户从数据库中取出用户信息
     *
     * @param username 账户
     * @return
     */
    @GetMapping("/userList")
    @RequiresPermissions("userInfo:view") // 权限管理
    public UserInfo findUserInfoByUsername(@RequestParam String username) {
        return userInfoService.findByUsername(username);
    }

    /**
     * 简单模拟从数据库添加用户信息成功
     *
     * @return
     */
    @PostMapping("/userAdd")
    @RequiresPermissions("userInfo:add")
    public String addUserInfo() {
        return "addUserInfo success!";
    }

    /**
     * 简单模拟从数据库删除用户成功
     *
     * @return
     */
    @DeleteMapping("/userDelete")
    @RequiresPermissions("userInfo:delete")
    public String deleteUserInfo() {
        return "deleteUserInfo success!";
    }
}
```



### 页面

```html
<!-- index.html：首页 -->
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <title>首页</title>
</head>
<body>
index - 首页
</body>
</html>

<!-- login.html：登录页 -->
<!DOCTYPE html>
<html xmlns:th="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="UTF-8">
    <title>登录页</title>
</head>
<body>
错误信息：<h4 th:text="${msg}"></h4>
<form action="" method="post">
    <p>账号：<input type="text" name="username" value="wmyskxz"/></p>
    <p>密码：<input type="text" name="password" value="123456"/></p>
    <p><input type="submit" value="登录"/></p>
</form>
</body>
</html>

<!-- 403.html：没有权限的页面 -->
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <title>403错误页</title>
</head>
<body>
错误页面
</body>
</html>
```



### 测试

1. 编写好程序后就可以启动，首先访问`http://localhost:8080/userList?username=wmyskxz`页面，由于没有登录就会跳转到我们配置好的`http://localhost:8080/login`页面。登陆之后就会看到正确返回的JSON数据，上面这些操作时候触发`MyShiroRealm.doGetAuthenticationInfo()`这个方法，也就是登录认证的方法。
2. 登录之后，我们还能访问`http://localhost:8080/userAdd`页面，因为我们在数据库中提前配置好了权限，能够看到正确返回的数据，但是我们访问`http://localhost:8080/userDelete`时，就会返回错误页面.












































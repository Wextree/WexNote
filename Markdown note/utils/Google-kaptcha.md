# Google-kaptcha 验证码

## SpringBoot整合

**以Session代替存验证码，然后用工具检验**

**pom依赖导入**

```xml
<!--图片验证码-->
<dependency>
    <groupId>com.github.penggle</groupId>
    <artifactId>kaptcha</artifactId>
    <version>2.3.2</version>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```



### 配置类

> 主要是为了将DefaultKaptcha注入Ioc容器，并且进行基本的配置

```java
/**
 * 验证码配置类
 */
@Configuration
public class KaptchaConfiguration {

    @Bean(name = "captchaProducer")
    public DefaultKaptcha getKaptchaBean(){
        DefaultKaptcha defaultKaptcha = new DefaultKaptcha();
        Properties properties = new Properties();
        // 验证码是否带边框
        properties.setProperty("kaptcha.border", "no");
        //验证码字体颜色
        properties.setProperty("kaptcha.textproducer.font.color", "blue");
        //验证码整体宽度
        properties.setProperty("kaptcha.image.width", "400");
        //验证码整体高度
        properties.setProperty("kaptcha.image.height", "125");
        //文字个数
        properties.setProperty("kaptcha.textproducer.char.length", "4");
        //文字大小
        properties.setProperty("kaptcha.textproducer.font.size","120");
        //文字随机字体
        properties.setProperty("kaptcha.textproducer.font.names", "宋体,楷体,微软雅黑");
        //文字距离
        properties.setProperty("kaptcha.textproducer.char.space","16");
        //干扰线颜色
        properties.setProperty("kaptcha.noise.color","blue");

        Config config = new Config(properties);
        defaultKaptcha.setConfig(config);
        return defaultKaptcha;
    }
}
```



**附上官方基本配置：**

```properties
* Constant	描述	默认值
kaptcha.border	图片边框，合法值：yes , no	yes
kaptcha.border.color	边框颜色，合法值： r,g,b (and optional alpha) 或者 white,black,blue.	black
kaptcha.border.thickness	边框厚度，合法值：>0	1
kaptcha.image.width	图片宽	200
kaptcha.image.height	图片高	50
kaptcha.producer.impl	图片实现类	com.google.code.kaptcha.impl.DefaultKaptcha
kaptcha.textproducer.impl	文本实现类	com.google.code.kaptcha.text.impl.DefaultTextCreator
kaptcha.textproducer.char.string	文本集合，验证码值从此集合中获取	abcde2345678gfynmnpwx
kaptcha.textproducer.char.length	验证码长度	5
kaptcha.textproducer.font.names	字体	Arial, Courier
kaptcha.textproducer.font.size	字体大小	40px.
kaptcha.textproducer.font.color	字体颜色，合法值： r,g,b  或者 white,black,blue.	black
kaptcha.textproducer.char.space	文字间隔	2
kaptcha.noise.impl	干扰实现类	com.google.code.kaptcha.impl.DefaultNoise
kaptcha.noise.color	干扰颜色，合法值： r,g,b 或者 white,black,blue.	black
kaptcha.obscurificator.impl	图片样式：
水纹com.google.code.kaptcha.impl.WaterRipple
鱼眼com.google.code.kaptcha.impl.FishEyeGimpy
阴影com.google.code.kaptcha.impl.ShadowGimpy	com.google.code.kaptcha.impl.WaterRipple
kaptcha.background.impl	背景实现类	com.google.code.kaptcha.impl.DefaultBackground
kaptcha.background.clear.from	背景颜色渐变，开始颜色	light grey
kaptcha.background.clear.to	背景颜色渐变，结束颜色	white
kaptcha.word.impl	文字渲染器	com.google.code.kaptcha.text.impl.DefaultWordRenderer
kaptcha.session.key	session key	KAPTCHA_SESSION_KEY
kaptcha.session.date	session date	KAPTCHA_SESSION_DATE

```



### 去除默认背景和样式

> 这不是有必要的操作，如果有需要的话可以参考去自定义

定义两个实现类，继承**Configurable类**，并且继承对应的接口

```java
package com.wex.kaptchademo.impl;

import com.google.code.kaptcha.GimpyEngine;
import com.google.code.kaptcha.NoiseProducer;
import com.google.code.kaptcha.util.Configurable;
import com.jhlabs.image.RippleFilter;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.util.Random;

/**
 * 自定义样式实现类
 */
public class DisKaptchaCssImpl extends Configurable implements GimpyEngine {
    public DisKaptchaCssImpl() {
    }

    @Override
    public BufferedImage getDistortedImage(BufferedImage bufferedImage) {
        NoiseProducer noiseProducer = this.getConfig().getNoiseImpl();
        BufferedImage distortedImage = new BufferedImage(400, 125, 2);
        Graphics2D graph = (Graphics2D)distortedImage.getGraphics();
        Random rand = new Random();
        RippleFilter rippleFilter = new RippleFilter();
        rippleFilter.setXAmplitude(7.6F);
        rippleFilter.setYAmplitude(rand.nextFloat() + 1.0F);
        rippleFilter.setEdgeAction(1);
        BufferedImage effectImage = rippleFilter.filter(bufferedImage, (BufferedImage)null);
        graph.drawImage(effectImage, 0, 0, (Color)null, (ImageObserver)null);
        graph.dispose();
        noiseProducer.makeNoise(distortedImage, 0.1F, 0.1F, 0.25F, 0.25F);
        noiseProducer.makeNoise(distortedImage, 0.1F, 0.25F, 0.5F, 0.9F);
        return distortedImage;

    }
}
```

```java
package com.wex.kaptchademo.impl;

import com.google.code.kaptcha.BackgroundProducer;
import com.google.code.kaptcha.util.Configurable;

import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;

/**
 * 去除验证码背景实现类
 */
public class NoKaptchaBackhround extends Configurable implements BackgroundProducer {
    public NoKaptchaBackhround() {
    }

    @Override
    public BufferedImage addBackground(BufferedImage bufferedImage) {
        int width = 400;
        int height = 125;
        BufferedImage image = new BufferedImage(width, height, 1);
        Graphics2D graph = (Graphics2D)image.getGraphics();
        graph.fill(new Rectangle2D.Double(0.0D, 0.0D, (double)width, (double)height));
        graph.drawImage(bufferedImage, 0, 0, null);
        return image;
    }
}
```



**还要在刚才定义的配置类中，加入：**

```java
//自定义验证码样式
properties.setProperty("kaptcha.obscurificator.impl","com.wex.kaptchademo.impl.DisKaptchaCssImpl");
//自定义验证码背景
properties.setProperty("kaptcha.background.impl","com.wex.kaptchademo.impl.NoKaptchaBackhround");
```



### 生成验证码和对应图片

```java
/**
 * 验证码控制层
 */
@RestController
public class ImageCodeController {
    @Autowired
    // 这里必须和你配置累返回的对象同名
    private Producer captchaProducer;

    /**
     * 生成验证码
     * @param response
     * @param request
     * @throws Exception
     */
    @GetMapping("/kaptcha")
    public void getImageCode(HttpServletRequest request, HttpServletResponse response) throws Exception{
        HttpSession session = request.getSession();
        // 禁止缓存
        // 清空缓存
        response.setDateHeader("Expires", 0);
        response.setHeader("Cache-Control", "no-store, no-cache, must-revalidate");
        response.addHeader("Cache-Control", "post-check=0, pre-check=0");
        response.setHeader("Pragma", "no-cache");
        response.setContentType("image/jpeg");

        //生成验证码
        String capText = captchaProducer.createText();
        // 把验证码保存在服务器的会话中，模拟保存在缓存中
        session.setAttribute(Constants.KAPTCHA_SESSION_KEY, capText);
        //向客户端写出
        BufferedImage bi = captchaProducer.createImage(capText);
        ServletOutputStream out = response.getOutputStream();
        ImageIO.write(bi, "jpg", out);
        try {
            out.flush();
        } finally {
            out.close();
        }
    }

}
```



### 做验证码比对

> 设计一个工具类和一个控制类来判断用户前端输入的验证码是否和存入会话中的验证码对应

```java
package com.wex.kaptchademo.utils;

import javax.servlet.http.HttpServletRequest;

// 新建验证码比对工具类
public class CodeUtil {
    /**
     * 将获取到的前端参数转为string类型
     * @param request
     * @param key
     * @return
     */
    public static String getString(HttpServletRequest request, String key) {
        try {
            // 取出前端输入的验证码内容，key与前端对应的name对应
            String result = request.getParameter(key);
            if(result != null) {
                // 去掉首位空格
                result = result.trim();
            }
            if("".equals(result)) {
                result = null;
            }
            return result;
        }catch(Exception e) {
            return null;
        }
    }
    /**
     * 验证码校验
     * @param request
     * @return
     */
    public static boolean checkVerifyCode(HttpServletRequest request) {
        //获取生成的验证码
        String verifyCodeExpected = (String) request.getSession().getAttribute(com.google.code.kaptcha.Constants.KAPTCHA_SESSION_KEY);
        //获取用户输入的验证码
        String verifyCodeActual = CodeUtil.getString(request, "verifyCodeActual");
        if(verifyCodeActual == null ||!verifyCodeActual.equals(verifyCodeExpected)) {
            return false;
        }
        return true;
    }
}
```



```java
// 调用工具类进行判断
@RestController
public class HelloController {
    @RequestMapping("/hello")
    public String hello(HttpServletRequest request) {
        if (!CodeUtil.checkVerifyCode(request)) {
            return "验证码有误！";
        } else {
            return "hello,world";
        }
    }
}
```



**前端代码，我放在类路径下的static文件夹，因为这个事SpringBoot默认搜寻的路径之一：**

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Hello</title>
    <script type="text/javascript">
        function refresh() {
            // 加上最后的随机数让它会重新获取而不是用之前那张
            document.getElementById('captcha_img').src="/kaptcha?"+Math.random();
        }
    </script>
</head>
<body>
<form action="/hello" method="post">
    验证码:  <input type="text" placeholder="请输入验证码" name="verifyCodeActual">
    <div class="item-input">
        <img id="captcha_img" alt="点击更换" title="点击更换"
             onclick="refresh()" src="/kaptcha" />
    </div>
    <input type="submit" value="提交" />
</form>

</body>
</html>
```



### 访问

打开浏览器访问：http://localhost:8080/hello.html

![](E:\git\WexNote\Markdown note\imgs\1594628514(1).png)



**输入对或者错的答案会得到相应的返回**








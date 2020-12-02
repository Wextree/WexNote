[toc]



# Object-C 学习

1. 基于C语言开发，只保留了面向对象的精华部分，比C++简单许多，可以与C甚至是C++语言混合编程。
2. 没有像Java一样采用命名空间机制，也就是包名，而**使用前缀去防止类名冲突**。
3. 为了与C语言等其他编程语言发生冲突，OC所有的关键字都以@作为开头。

|      语法       |                      描述                      |
| :-------------: | :--------------------------------------------: |
|  基本数据类型   |     char，int，float，double，BOOL(YES/NO)     |
|       nil       |               相当于java中的null               |
|      self       |               相当于java中的this               |
|    基本语句     |    循环语句，条件语句，goto语句和C语言一致     |
| for循环快速遍历 |                for（xx in xx）                 |
|      基类       |                    NSOject                     |
|      继承       |       单继承，用super关键字饮用父类对象        |
|    异常处理     |      @try @catch @finally （一般不使用）       |
|      注释       |                 // 和 /*   */                  |
|    日志输出     | NSLog（@“Hello”）;  NSLog（@“age is %i”，27）; |

> oc的异常处理主要是采用block形式，传入error对象地址，进行赋值，然后在一个block里面进行操作。



## 语法基础学习

### 第一个OC程序

```objective-c
// .m为后缀是源代码文件，.h为后缀是头文件
// import可以避免重复包含
#import <Foundation/Foundation.h>

// 入口函数
int main(int argc, const char * argv[]) {
    // 自动回收的池
    @autoreleasepool {
        // 会自动换行
        NSLog(@"Hello, World!");
    }
    return 0;
}
```

```verilog
2020-10-22 10:56:29.545629+0800 firstOC[1259:37472] Hello, World!
Program ended with exit code: 0
```



### 定义一个类

```objective-c
// Student.h

// 引用该头文件可以使用很多基础的类
# import <Foundation/Foundation.h>

// 使用@interface代表一个类
// ：代表继承
@interface Student : NSObject { // 再大括号里面定义成员变量
    int age;
    int no;
}

// 在该地方声明的方法都是公共方法


// - 代表动态方法，也就是成员方法
// + 代表静态方法，也就是类方法
- (int)age;

-(int)no;

// 多少个：，多少个参数
- (void)setAge:(int)newAge;

// 这个andNo只是属于方法名的一部分
- (void)setAge:(int)newAge andNo:(int)newNo;

@end
```

```objective-c
// Student.m

#import "Student.h"

@implementation Student

- (int)age{
    NSLog(@"use getAge");
    
    return age;
}

- (int)no{
    return no;
}

- (void)setAge:(int)newAge{
    NSLog(@"use setAge");
    
    age = newAge;
}

- (void)setAge:(int)newAge andNo:(int)newNo{
    age = newAge;
    no = newNo;
}

@end
```



**创建一个对象：**

```objective-c
#import <Foundation/Foundation.h>
#import "Student.h"

int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        // 创建一个对象
        
        // 调用类的静态方法进行创建
        // 调用方式 [类名或对象 方法名],用一个对象指针去接收
        // Student *stu = [Student alloc];
        
        // 然后调用init方法进行一个初始化
        // stu = [stu init];
        
        Student *stu = [[Student alloc] init];
        
        [stu setAge:23];
        int age = [stu age];
        NSLog(@"age is %i", age);
        
        [stu setAge:44 andNo:1];
        NSLog(@"age is %i, no is %i", [stu age], [stu no]);
        
        
        // 释放内存 现在xcode的ARC功能会自动管理
         // [stu release];
        
    }
    return 0;
}

```

```verilog
2020-10-22 11:52:06.797373+0800 firstOC[1476:57297] use setAge
2020-10-22 11:52:06.798031+0800 firstOC[1476:57297] use getAge
2020-10-22 11:52:06.798114+0800 firstOC[1476:57297] age is 23
2020-10-22 11:52:06.798222+0800 firstOC[1476:57297] use getAge
2020-10-22 11:52:06.798271+0800 firstOC[1476:57297] age is 44, no is 1
Program ended with exit code: 0
```



#### 方法名

> 冒号是方法名的一部分

比如：**setAge:(int)newAge**，他的方法名就是**setAge:**。

而**setAge:(int)newAge andNo:(int)newNo**的方法名是**setAge:andNo:**



### 点语法

在OC语言中，我们可以像Java一样使用点语法去访问对象的属性或者进行赋值。

```objective-c
Student *stu = [[Student alloc] init];

stu.age = 3;
NSLog(@"age is %i", stu.age);
```

但是与Java不同的是，在Java中，当我们使用点语法去操作属性时，我们是直接对属性进行操作的，但是OC中，其实是默认调用它属性的get和set方法，而不是直接操作该对象的属性。

> 如果想要直接访问属性要用->

```objective-c
stu.age = 3; ---> [stu age:3];
int age = stu.age; ---> int age = [stu age];
```



对于OC成员属性命名的一种推荐方式，我们尽量以下划线开头，这是为了区别我们的参数和get方法。

因为get方法推荐是直接用属性名当做方法名的，没有使用get前缀。

```objective-c
@interface Student : NSObject {
  int _age;
}

- (int)age;

@end
```



OC中我们使用self可以代替Java中的this对本对象进行一个操作。

但是如果在get或者set方法中使用self加点方法，会导致死循环，因为点方法也是调用get或者set方法。



```objective-c
// 以下会造成死循环
- (int)age{
  return self.age;
}

- (void)setAge:(int)age{
  self.age = age;
}
```



### 构造方法和description函数

#### 构造方法

我们使用的构造方法默认是父类，也就是NSObject对象的init方法，我们可以自己写自定义的构造方法。

```objective-c
// Student.h
- (id)initWithAge:(int)age andNo:(int)no;
```

```objective-c
// Student.m
- (id)initWithAge:(int)age andNo:(int)no{
    if (self = [super init]){
        _age = age;
        _no = no;
    }
    
    return self;
}
```



```objective-c
int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        Student *stu = [[Student alloc] initWithAge:23 andNo:4];
        
        NSLog(@"age is %i, no is %i", stu.age, stu.no);
        
    }
    return 0;
}
```



#### description

这个就是当我们对象被打印的时候的字符串显示，跟Java中的toString()方法一致，默认是打印对象的地址。

```objective-c
// Student.h
- (NSString *)description;
```

```objective-c
// Student.m
- (NSString *)description{
    NSString *str = [NSString stringWithFormat:@"age is %i, no is %i", _age, _no];
    
    return str;
}
```

```objective-c
#import <Foundation/Foundation.h>
#import "Student.h"

int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        Student *stu = [[Student alloc] initWithAge:23 andNo:4];
        
      // %@可以引用oc对象
        NSLog(@"%@", stu);
        
    }
    return 0;
}
```



### 生命周期

- **@public**：公共声明，什么类都可以访问
- **@protect**：只有自己的子类可以访问，是在头文件中声明的默认实现
- **@private**：私有声明，只能给本类自己使用



可以用以下方式进行声明：

```objective-c
// Student.h
@interface Student : NSObject { // 再大括号里面定义成员变量
    @public
    int _age;
    int _no;
    
    @private
    int _name;
}

```



若在源文件中定义方法，但是在头文件中没有声明，那么默认为私有方法：

```objective-c
- (void)test1{
    NSLog(@"This is a private method.");
}
```



在Java中，this只能在动态方法中写，但是在OC中，我们可以在静态和动态方法中写self，但是表达的意思不一样，一个是类，一个是该对象：

```objective-c
- (void)test1{
    self.age = 11;
}

+ (id)test2{
    return [[self alloc] init];
}
```



> 后面加入语法new，相当于alloc+init，[Student new] = [[Student alloc] init]; 建议使用后者



### @property & @synthesize

可以使用这两个注解对类的成员变量做一个getter/setter的快速实现。



```objective-c
@interface Student : NSObject { // 再大括号里面定义成员变量
    int age;
    int no;
}

// 使用该注解会默认给它的同名变量做一个getter和setter的声明
@property int age;
@property int no;

@end
```

```objective-c
@implementation Student

// 默认会生成对应属性的getter/setter实现
// 如果检查不到对应的同名属性，那么会默认新建一个
@synthesize age, no;

@end
```

```objective-c
int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        Student *stu = [[Student alloc] init];
        stu.age = 23;
        
        NSLog(@"age is %i", stu.age);
        
    }
    return 0;
}
```



如果我们打算在方法生成时不使用对应的同名的成员变量，比如我们习惯在名字前面加上下划线，那么我们可以指定：

```objective-c
@synthesize age = _age;
```



但是在xcode 4.5以上版本，默认使用@property就会包含声明和实现，不用写**@synthesize**：

```objective-c
@interface Student : NSObject { // 再大括号里面定义成员变量
}

// 使用该注解会默认给它的同名变量做一个getter和setter的声明
// 不需要写@synthesize
// 而且默认生产的变量其实是_age
@property int age;
@property int no;

@end
```



## 内存管理

> 所有继承了NSObject的对象都要释放内存，但是基本类型不需要回收



### 原理

- 每个对象内部都保存了一个与之相关联的整数，称为引用计数器。
- 当使用alloc，new或者copy创建一个对象时，对象的引用计数器都会被设置为1
  - 只要运用到这些函数，不过有没有被变量引用，引用计数都会变化
- 给对象发送一条retain消息，可以使引用计数器值+1
- 给对象发送一条release消息，可以使引用计数器值-1
- 当一个对象的引用计数器值为0时，那么它将被销毁，其占用的内存被系统回收，OC也会自动向对象发送一条dealloc消息。一般会重写dealloc方法，在这里释放相关资源。一定不要直接调用dealloc方法。
- 可以给对象发送retainCount消息获得当前的引用计数器值



### 测试

重写dealloc方法：

```objective-c
// Student.m
-(void)dealloc{
    NSLog(@"%@ is dealloc.", self);
    [super dealloc];
}
```

```objective-c
int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        Student *stu = [[Student alloc] init];
      
        NSLog(@"stu is alived %@", stu);
        
        [stu release]; // ARC自动回收
        
    }
    return 0;
}
```



使用retainCount查看计数情况：

```objective-c
int main(int argc, const char * argv[]) {

  @autoreleasepool {
    Student *stu = [[Student alloc] init];  // 1

    NSLog(@"Count %zi", stu.retainCount);

    [stu retain];  // 2
    
    NSLog(@"Count %zi", stu.retainCount);

    [stu release]; // 1
    
    NSLog(@"Count %zi", stu.retainCount);
    
    [stu release]; // 0
  }
  return 0;
}
```



可以使用autorelease进行自动释放：

```objective-c
Student *stu = [[[Student alloc] init] autorelease];
```



### 内存管理原则

- 谁创建谁销毁，就是谁使用了alloc，new，copy，就需要谁去release，autorelease。
- 除了用以上方法创建的对象，我们默认它们是自动release的。
- 用了多少个retain，就需要多少个release进行抵消，以计数器为0作为标准。



> param mark xxxx. 可以方便注释标记



### 使用setter方法解决对象引用内存管理

因为我们有时候一个对象的属性引用其他对象，容易不知道在哪里释放内存，所以我们可以采用setter方法中retain，然后dealloc方法中release的方式进行内存的控制：



新建一个Book类给Student类进行引用：

```objc
// Book.h
#import <Foundation/Foundation.h>

@interface Book : NSObject{
    int _price;
}

@property int price;

@end
```



```objective-c
// Book.m
#import "Book.h"

@implementation Book

- (void)dealloc{
    NSLog(@"Book is release.");
    [super dealloc];
}

@end
```



在Student类中建立对Book的引用，并且在对应的setter操作和dealloc操作中对book进行一个**retain和release**：

```objective-c
@interface Student : NSObject { 
    Book *_book;
}

@property int age;
@property Book *book;

- (void)readBook;

@end
```

```objective-c
#import "Student.h"

@implementation Student

- (void)readBook{
    NSLog(@"I am reading %@", self.book);
}

- (Book *)book{
    return _book;
}

- (void)setBook:(Book *)book{
  // 先释放我们之前的变量，然后增加我们现在的变量
  	if (book != _book){
      [_book release];
    }
    _book = [book retain];
}

- (void)dealloc{
    [_book release];
    NSLog(@"Student is release.");
    [super dealloc];
}

@end
```



主文件调用：

```objective-c
#import <Foundation/Foundation.h>
#import "Student.h"
#import "Book.h"

void test1(Student *stu){
  // 哪里创建就要哪里release
    Book *book = [[Book alloc] init];
    book.price = 100;
    
    stu.book = book;
    
    [book release];
}

void test2(Student *stu){
    [stu readBook];
}

int main(int argc, const char * argv[]) {
    
    @autoreleasepool {
        Student *stu = [[Student alloc] init];
        stu.age = 23;
        
        test1(stu);
        test2(stu);
        
        [stu release];
    }
    return 0;
}
```



### @class

通常引用一个类有两种方法：

- 通过**#import方式**引入
- 通过**@class**引入



两种方式的区别：

- import方式会**包含被引用类的所有信息**，包括被引用类的变量和方法
- @class方式只是**告诉**编译器在某个头文件的声明中，对应的变量是一个类，至于这个类里面有什么信息，编译器不需要知道。
- 头文件使用@class标记类之后，在.m文件中如果要引用对应的方法什么的，**还是需要引入对应的头文件**
- 如果许多个文件引用一个头文件，当这个头文件更改的时候，所有引用它的头文件都需要重新编译，效率会非常低，但是如果用@class就不会。
- 还有就是循环依赖的问题，如:A–>B, B–>A这样的相互依赖关系，如果使用#import来相互包含，那么就会出现编译错误，如果使用@class在两个类的头文件中相互声明，则不会有编译错误出现。

 

### @property参数

**格式**：`@property (retain, nonatomic) Book *book;`



- **retain**：在setter中release旧值，retain新值。默认代替以下代码：

  ```objective-c
  - (void)setBook:(Book *)book{
    // 先释放我们之前的变量，然后增加我们现在的变量
    	if (book != _book){
        [_book release];
      }
      _book = [book retain];
  }
  ```



- 访问权限
  - **readonly**：不生成Setter
  - **readwrite**：同时生成，默认
- 内存访问
  - **strong**：对象默认，表示Retain+1内存语义
  - **assign**：标量默认，表示值拷贝内存语义
  - **weak**：对象弱引用，不影响引用计数原子策略
- 原子控制
  - **nonatomic**：Getter/Setter不加锁（IOS常用）
  - **atomic**：加锁，默认（macOS常用），保证线程安全
- 行为控制
  - **copy：**Setter触发copy方法



指定方法名：

```objective-c
@property(nonatomic, getter=isrich) BOOL rich;
```



### autorelease

#### 自动释放池 autorelease pool

- 自动释放池是OC里面的一种内存自动回收机制，一般可以将一些临时变量添加到自动释放池中，统一回收释放
- 当自动释放池销毁时，**池里面的所有对象都会调用一次release方法**



#### autorelease

- OC对象只需要发送一条autorelease消息，就会把这个对象添加到最近的自动释放池中（栈顶的释放池）
- autorelease实际上只是把对release的调用延迟了，对于每一次autorelease，系统只是把该对象放入了当前的释放池中，当该pool被释放的时候，该pool中的所有对象会被调用一次release。

```objective-c
int main(int argc, const char * argv[]) {
    
    // 自动创建一个释放池
    @autoreleasepool {
        Student *stu = [[Student alloc] init];
        // 加入池中
        [stu autorelease];
        
        Student *stu1 = [[Student alloc] init];
        
        [stu1 autorelease];
    }
    return 0;
}
```



因为我们创建对象的方式有点麻烦，**所以我们经常采用类的静态方法的方式去创建一个对象**，而为了该对象可以自动回收不造成内存泄漏，我们经常配合autorelease方法：

```objective-c
// Student.h
// 命名默认潜规则
+ (id)student;
```

```objective-c
+ (id)student{
    return [[[Student alloc] init] autorelease];
}
```

```objective-c
int main(int argc, const char * argv[]) {
    
    // 自动创建一个释放池
    @autoreleasepool {
        Student *stu = [Student student];
        
        stu.age = 22;
        NSLog(@"age is %i", stu.age);
    }
    return 0;
}
```



#### 注意事项

1. 在ARC下，不能使用`[[NSAutoreleasePool alloc] init]`，而是应当使用@autoreleasepool
2. **不要把大量循环操作放到同一个NSAutoreleasePool之间**，这样会造成内存峰值的上升
3. 尽量**避免对大内存使用该方法**，对于这种延迟释放机制，还是尽量少用
4. sdk中一般利用静态方法创建并返回的对象都是已经autorelease的，不需要再进行release操作



## 分类 Category

### 作用

- OC提供了一种与众不同的方式，可以动态的为已经存在的类添加新的行为（fangfa）
- 这样可以保证类的原始设计规模比较小，功能增加时再逐步扩展
- 使用Category对类进行扩展时，不需要创建子类
- Category使用简单的方式，实现了类的相关方法的模块化，把不同的类分配到不同的类模块下



### 使用

选择对应的类创建Category，会同时创建h文件和m文件，命名是“类名+分类名”

文件内部用**括号**代表分类：

```objective-c
// （）里面代表了分类
@interface Student (Test)

@end
```



使用，直接在分类中添加方法即可，但是**没办法添加成员变量**：

```objective-c
// Student+Test.h
// （）里面代表了分类
@interface Student (Test)

- (void)test2;

@end
```

```objective-c
// Student+Test.m
#import "Student+Test.h"
@implementation Student (Test)

-(void)test2{
    NSLog(@"test2");
}

@end
```

```objective-c
#import <Foundation/Foundation.h>
#import "Student.h"
#import "Student+Test.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        Student *stu = [[[Student alloc] init] autorelease];
        
        // 原始方法
        [stu test];
        // 分类扩展方法
        [stu test2];
    }
    return 0;
}
```

```verilog
2020-10-23 10:51:25.358528+0800 category[1351:45518] test
2020-10-23 10:51:25.359318+0800 category[1351:45518] test2
Program ended with exit code: 0
```



### 使用场景

- 在定义类时的某些情况下（例如需求变更），你可能想要为其中的某个或几个类中添加新的方法
- 一个类中包含了许多不同种类的方法需要实现，而这些方法可能需要不同的团队的成员实现
- 在使用基础类库中的类时，有可能希望这些类实现一些自己需要的方法。



## 协议 Protocol

### 规则

- 类似于Java的接口，描述了一组相关方法定义
  - 但是Java中实现某个接口，需要实现其中所有的方法，而OC中的协议没有这样的规定
  - 更多的应该是作为一个规范的约束，为了实现类似于多态的使用
- 抽象了类的功能，隐藏了具体的实现类
- 协议可以继承于其他协议，多继承



### 按钮示例使用

1. **先建立一个按钮类**

   ```objective-c
   // Button.h
   #import <Foundation/Foundation.h>
   
   NS_ASSUME_NONNULL_BEGIN
   
   // 按钮监听器，具有某种协议
   // 使用<>实现某个协议
   @protocol ButtonDelegate <NSObject>
   
   - (void)onClick;
   
   @end
   
   @interface Button : NSObject
   
   @property (nonatomic, retain) id<ButtonDelegate> delegate;
   
   - (void)click;
   
   @end
   
   NS_ASSUME_NONNULL_END
   
   ```

   ```objective-c
   // Button.m
   #import "Button.h"
   
   @implementation Button
   
   // 对按钮监听器进行调用，实现功能
   - (void)click{
       [_delegate onClick];
   }
   
   - (void)dealloc{
       NSLog(@"button release.");
       [super dealloc];
   }
   
   @end
   ```

2. **实现按钮监听器**

   ```objective-c
   // ButtonListener.h
   #import <Foundation/Foundation.h>
   #import "Button.h"
   
   NS_ASSUME_NONNULL_BEGIN
   
   @interface ButtonListener : NSObject <ButtonDelegate>
   
   @end
   
   NS_ASSUME_NONNULL_END
   ```

   ```objective-c
   // ButtonListener.m
   #import "ButtonListener.h"
   
   @implementation ButtonListener
   
   - (void)onClick{
       NSLog(@"按钮被点击了");
   }
   
   @end
   ```

3. **调用：**

   ```objective-c
   #import <Foundation/Foundation.h>
   #import "Button.h"
   #import "ButtonListener.h"
   
   int main(int argc, const char * argv[]) {
       @autoreleasepool {
           Button *button = [[[Button alloc] init] autorelease];
           ButtonListener *listener = [[[ButtonListener alloc] init] autorelease];
           
           button.delegate = listener;
           [button click];
       }
       return 0;
   }
   ```

   ```verilog
   2020-10-23 11:29:51.325980+0800 Protocol[1559:60156] 按钮被点击了
   2020-10-23 11:29:51.326570+0800 Protocol[1559:60156] button release.
   Program ended with exit code: 0
   ```

   

### 优化使用

因为我们一般点击这个按钮需要利用到本身按钮的信息，所以我们可以在onclick方法中传入我们的按钮对象信息：

```objective-c
#import <Foundation/Foundation.h>
@class Button;

NS_ASSUME_NONNULL_BEGIN

// 按钮监听器，具有某种协议
// 使用<>实现某个协议
@protocol ButtonDelegate <NSObject>

- (void)onClick:(Button *)btn;

@end

@interface Button : NSObject

@property (nonatomic, retain) id<ButtonDelegate> delegate;

- (void)click;

@end

NS_ASSUME_NONNULL_END
```



因为我们的协议是可以实现方法或者不实现，所以如果我们监听器继承了该协议，但是没有去实现对应的方法，到时候调用的时候就会报错，那么OC给我们提供了一种检验是否有该方法的机制：

```objective-c
// 对按钮监听器进行调用，实现功能
- (void)click{
    // 检测是否有对应的方法
    if ([_delegate respondsToSelector:@selector(onClick:)]){
        [_delegate onClick:self];
    }else {
        NSLog(@"No onClick method.");
    }
}
```



我们可以用@required和@optional去标注协议的方法，前者是要求实现，后者是不要求实现，可选的，那么默认是required

但是**OC是弱语法，即使是标注required不实现也不会报错**，但是开发过程还是要实现所有标注required的方法



检测一个类是否有实现对应的协议：

```objective-c
if (button.delegate conformsToProtocol:@protocol(ButtonDelegate)){
            
}
```





## 代码块 Block

- Block封装了一段代码，可以在任何时候执行
- Block可以作为函数参数或者函数的返回值，而其本身又可以带输入参数或返回值。他和传统的函数指针很类似，但是有区别：Block是inline（内联函数）的，并且默认情况下它对局部变量是只读的
- **可以访问外部的变量，不能改变量，除非使用__block关键字进行修饰**
- 苹果官方建议尽量多用block。在多线程，异步任务，集合遍历，集合排序，动画转场用的很多

> 感觉有点像封装在函数内部的简单函数



```objective-c
// 基础使用
int (^Sum) (int, int) = ^(int a, int b){
  return a+b;
};

int sum = Sum(2, 3);

NSLog(@"2 + 3 = %i", sum);
```



可以吧block当成一个变量来使用，配合typedef：

```objective-c
typedef int (^Sum) (int, int);

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        Sum sum = ^(int a, int b){
            return a + b;
        };
        
        NSLog(@"2 + 3 = %i", sum(2, 3));
    }
    return 0;
}
```



## Foundation

> 常用的结构和函数所在位置，类似于java.lang包



### 常用结构体

#### NSRange

- 这个结构体用来表示事物的一个范围，通常是字符串里的字符范围或者集合里的元素范围
- location代表该范围的起始位置
- length表示该范围内所含的元素个数

```objective-c
typedef struct _NSRange {
  NSUInteger location;
  NSInteger length;
} NSRange;
```



第一种使用：

```objective-c
NSRange range;
range.location = 10;
range.length = 3;
```



第二种使用：

```objective-c
NSRange range = {10, 3};
```



第三种使用：

```objective-c
NSRange range = {.location = 10, .length = 3};
```



第四种使用（建议）：

```objective-c
NSRange range = NSMakeRange(10, 3);
```



转为字符串：

```objective-c
NSString *str = NSStringFromRange(range);
```



#### NSPoint

- x，y坐标，多用于UI界面

```objective-c
typedef CGPoint NSPoint;
struct CGPoint {
    CGFloat x;
    CGFloat y;
};
```



使用：

```objective-c
NSPoint point;
point.x = 9;
point.y = 10;

point = NSMakePoint(9, 10);

NSString *str = NSStringFromPoint(point);
```



#### NSSize

```objective-c
typedef CGSize NSSize;
struct CGSize {
    CGFloat width;   // double
    CGFloat height;
};
```



使用：

```objective-c
NSSize size;
size.height = 3.0;
size.width = 10.0;

size = NSMakeSize(10.0, 3.0);

NSString *str = NSStringFromSize(size);
```



#### NSRect

```objective-c
typedef CGRect NSRect;
struct CGRect {
    CGPoint origin;
    CGSize size;
};
```



使用：

```objective-c
NSRect rect;
rect.origin.x = 10.0;
rect.origin.y = 12.0;

rect.size.width = 20;
rect.size.height = 30;

rect = NSMakeRect(10.0, 12.0, 20, 30)

  NSString *str = NSStringFromRect(rect);

```



### 常用类

#### NSString 不可变字符串

创建字符串：

```objective-c
void createString(){
    // 不需要释放内存
    NSString *str1 = @"A String!";
    
    NSString *str2 = [[NSString alloc] init];
    str2 = @"A String!";
    [str2 release];
    
    NSString *str3 = [[NSString alloc] initWithString:@"A String!"];
    [str3 release];
    str3 = [NSString stringWithString:@"A String!"];
    
    NSString *str4 = [[NSString alloc] initWithUTF8String:"A String!"];
    [str4 release];
    str4 = [NSString stringWithUTF8String:"A String!"];
    
    NSString *str5 = [[NSString alloc] initWithFormat:@"My age is %zi, and my height is %.2f.", 23, 180.0];
    [str5 release];
    str5 = [NSString stringWithFormat:@"My age is %zi, and my height is %.2f.", 23, 180.0];
}

```



或者文件内容和URL的内容：

```objective-c
NSError *error;
NSString *path = @"xxxxxxx";
NSString *str6 = [NSString stringWithContentsOfFile:path usedEncoding:NSUTF8StringEncoding error:&error];

if (error == nil){
  NSLog(@"%@", str6);
}else {
  NSLog(@"%@", error);
}

NSString *url = [NSURL URLWithString:@"xxxx"];
NSString *str7 = [NSString stringWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil];
```



字符串导出：

```objective-c
NSString *str8 = @"abcdefg";
// 会帮你创建文件
[str8 writeToFile:path atomically:YES encoding:NSUTF8StringEncoding error:&error];
```



其他常用方法：

```objective-c
#pragma mark StringOtherMethod
void otherMethod(){
    NSString *str = @"GuangDong";
    // 全部大写
    NSString *upper = [str uppercaseString];
    // 全部小写
    NSString *lower = [str lowercaseString];
    // 首字母大写，其余变小写
    NSString *firstUp = [str capitalizedString];
    // 比较两个字符串内容是否相等
    BOOL res1 = [@"abc" isEqualToString:@"abc"];
    // 比较两个字符串大小，三种情况，升序，降序，相等
    NSComparisonResult res2 = [@"abc" compare:@"Abc"];
    
    // 是否以什么开头
    BOOL res3 = [str hasPrefix:@"Guang"];
    // 是否以什么结尾
    BOOL res4 = [str hasSuffix:@"abc"];
    // 找到字符串的位置和长度，找不到默认location为int最大值
    NSRange range = [str rangeOfString:@"uan"];
    // 从后面反向找第一个（可以指定范围，多加一个参数）
    range = [str rangeOfString:@"Don" options:NSBackwardsSearch];
    
    NSString *str2 = @"123456";
    // 从指定索引截取到末尾，包含3
    NSString *sub1 = [str2 substringFromIndex:3];
    // 从开始索引截取到指定索引，不包含3
    NSString *sub2 = [str2 substringToIndex:3];
    // 指定范围截取
    NSString *sub3 = [str2 substringWithRange:NSMakeRange(2,3)];
    // 指定字符截取
    NSArray *array = [@"a,b,c" componentsSeparatedByString:@","];
    // 将数组拼接成url
    NSString *pathStr = [NSString pathWithComponents:array];
    // 将url分解成数组
    NSArray *pathArray = [pathStr pathComponents];
    // 判断是不是绝对路径
    BOOL isAbs = [pathStr isAbsolutePath];
    // 获取最后一个文件夹
    NSString *lastPath = [pathStr lastPathComponent];
    // 删除最后一个文件夹
    NSString *newPathByDeletingLastPathComponent = [pathStr stringByDeletingLastPathComponent];
    // 添加最后一个文件夹
    NSString *newPathByAppendingPathComponent = [pathStr stringByAppendingPathComponent:@"test"];
    
    NSString *string = @"wex";
    // 获取字符串的长度
    unsigned long len = string.length;
    // 获取对应索引位置的字符
    char c = [string characterAtIndex:2];
    // 类型转换
    double dv = string.doubleValue;
    float fv = string.floatValue;
    int iv = string.intValue;
    // 转换为C语言的字符串
    const char *cv = string.UTF8String;
}
```



#### NSMutableString 可变字符串

- NSString是不可变的，不能删除字符或者添加字符。NSString有一个子类NSMutableString，称为可变字符串。

- 创建可变字符串的常用方法：

  - `- (id)initWithCapacity:(NSUInteger)capacity`
  - `+ (id)stringWithCapacity:(NSUInteger)capacity`

  > capacity只是一个最优值，字符串的大小并不仅限于所提供的容量，设置了capacity，可以预分配一块内存来存储它，操作速度会快很多

```objective-c
#pragma mark NSMutableString
void createMutableString(){
    NSMutableString *str = nil;
    str = [str initWithCapacity:10];
    NSMutableString *mStr = [NSMutableString stringWithCapacity:10];
    
    [str appendString:@"wex"];
    [mStr appendFormat:@"my age is %i", 23];
    
    // 用一个字符串代替一段字符
    [str replaceCharactersInRange:NSMakeRange(0, 2) withString:@"ss"];
    
    // 插入字符串
    [str insertString:@" is good" atIndex:3];
    
    // 删除一段字符串(经常和rangeOfString一起使用删除特定的字符串)
    [str deleteCharactersInRange:NSMakeRange(0, 1)];
}
```



#### NSArray 不可变数组

- 用来存取对象的有序列表，它是不可变的
- 不能存储C语言中的基本数据类型，如int，float，enum，struct，也不能存储nil
- 创建NSArray的常用方法：

```objective-c
#pragma mark createNSArray
void createArray(){
    NSArray *arr1 = [NSArray array];
    
    NSArray *arr2 = [NSArray arrayWithObject:@"wex"];
    
    NSArray *arr3 = [NSArray arrayWithObjects:@"wex", @"best", @"good", nil];
    
    NSArray *arr4;
    [arr4 initWithObjects:@"wex", @"best", @"good", nil];
    
    NSArray *arr5;
    [arr5 initWithArray:arr2];
    
    // NSArray *arr6 = [NSArray arrayWithContentsOfFile:@"path/xx/xx"];
    
    // NSArray *arr7 = [NSArray arrayWithContentsOfURL:];
    
    // NSArray *arr8;
    // arr8 = [arr8 initWithContentsOfFile:];
    
    // NSArray *arr9;
    // arr9 = [arr9 initWithContentsOfURL:];
}
```



其他使用：

```objective-c
#pragma mark useArray
void useArray(){
    NSArray *arr = [NSArray arrayWithObjects:@"wex", @"best", @"good", nil];
    // 数组数量
    unsigned long count = [arr count];
    // 是否包含
    BOOL contain = [arr containsObject:@"wex"];
    // 最后一个元素
    id lastOb = [arr lastObject];
    // 对应位置的元素
    id obOfIndex = [arr objectAtIndex:2];
    // 查找元素索引
    unsigned long index = [arr indexOfObject:@"wex"];
    // 查找指定范围的元素的索引
    unsigned long index2 = [arr indexOfObject:@"wex" inRange:NSMakeRange(0, 2)];
}
```



**内存管理**：

```objective-c
#pragma mark arrayMemory
void arrayMemory(){
    Student *stu1 = [[Student alloc] init];
    Student *stu2 = [[Student alloc] init];
    Student *stu3 = [[Student alloc] init];

    // 加入数组之后计数器会加一
    NSArray *arr = [NSArray arrayWithObjects:stu1, stu2, stu3, nil];
    
    [stu1 release];
    [stu2 release];
    [stu3 release];
    
    // 释放时统一减一
    [arr release];
}

```



**比较**：

```objective-c
// 比较两个数组是否相等
BOOL isEqual = [arr isEqualToArray:<#(nonnull NSArray *)#>];
// 返回第一个相等的元素对象
id obj = [arr firstObjectCommonWithArray:<#(nonnull NSArray *)#>];
```



**发送消息（调用元素对象的方法）**：

```objective-c
#import "Student.h"

@implementation Student

+ (id)student{
    return [[[Student alloc] init] autorelease];
}

- (void)test{
    NSLog(@"%@ -> test", self);
}

- (void)test2:(NSString *)str{
    NSLog(@"%@ -> test -> %@", self, str);
}

- (void)dealloc{
    NSLog(@"%@ is released", self);
    [super dealloc];
}

@end
```

```objective-c
#pragma mark sendMessage
void sendMessage(){
    Student *stu1 = [Student student];
    Student *stu2 = [Student student];
    Student *stu3 = [Student student];
    
    NSArray *arr = [NSArray arrayWithObjects:stu1, stu2, stu3, nil];
    
    [arr makeObjectsPerformSelector:@selector(test)];
    // 只能带一个参数
    [arr makeObjectsPerformSelector:@selector(test2:) withObject:@"wex"];
}
```



**遍历方式**：

```objective-c
NSArray *arr = [NSArray arrayWithObjects:@"1", @"2", @"3", @"4", nil];

unsigned long count = arr.count;

for (int i = 0; i < count; i++) {
  id obj = [arr objectAtIndex:i];
  NSLog(@"index %i is %@", i, obj);
}
```



```objective-c
NSArray *arr = [NSArray arrayWithObjects:@"1", @"2", @"3", @"4", nil];
    
    int i = 0;
    
    for (id obj in arr) {
        NSLog(@"index %i is %@", i, obj);
        i++;
    }

```



```objective-c
// 利用block，推荐使用
NSArray *arr = [NSArray arrayWithObjects:@"1", @"2", @"3", @"4", nil];
    
    [arr enumerateObjectsUsingBlock:
     ^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        NSLog(@"index %i is %@", idx, obj);
        
        // 利用外面的布尔值停止循环
        if (idx == 2){
            *stop = YES;
        }
    }];
```



**迭代器**：

```objective-c
// NSEnumerator *enumerator = [arr objectEnumerator];
// 反序迭代器
NSEnumerator *enumerator = [arr reverseObjectEnumerator];
// 获取迭代器所有元素的数组
NSArray *objs = [enumerator allObjects];

id obj = nil;
while (obj = [enumerator nextObject]){
  NSLog(@"obj = %@", obj);
}
```



**新数组派生**：

```objective-c
#pragma mark newArray
void newArray(){
    NSArray *arr = [NSArray arrayWithObjects:@"1", @"2", @"3", nil];
    NSArray *arr1 = [arr arrayByAddingObject:@"4"];
    
    NSArray *arr2 = [arr arrayByAddingObjectsFromArray:[NSArray arrayWithObjects:@"4", @"5", nil]];
    
    // 截取数组的范围
    NSArray *arr3 = [arr2 subarrayWithRange:NSMakeRange(2, 2)];
}
```



**其他方法**：

```objective-c
#pragma mark arrayOtherMethod
void arrayOtherMethod(){
    NSArray *arr = [NSArray arrayWithObjects:@"1", @"2", @"3", @"4", nil];

    // 拼接字符串
    NSString *str = [arr componentsJoinedByString:arr];
    // 写到文件中去
    NSString *path = @"xx/xx/xx";
    BOOL writeToFile = [arr writeToFile:path atomically:YES];
}
```



**排序**：

```objective-c
#pragma mark arraySort
void arraySort(){
    NSArray *arr = [NSArray arrayWithObjects:@"2", @"1", @"4", @"3", nil];
    
  // 如果是自定义的类，我们应该去实现自己的比较方法
    NSArray *arr2 = [arr sortedArrayUsingSelector:@selector(compare:)];
    NSLog(@"%@", arr2);
}
```



```objective-c
// with block
NSArray *arr3 = [arr sortedArrayUsingComparator:
                     ^NSComparisonResult(NSString*  _Nonnull obj1, NSString*  _Nonnull obj2) {
        return [obj1 compare: obj2];
    }];
```





```objective-c
// 采用描述器进行高级排序
// 1. 先按照书名排序
NSSortDescriptor *bookNameDesc = [NSSortDescriptor sortDescriptorWithKey:@"book.name" ascending:YES];
// 2. 再按照姓排序
NSSortDescriptor *lastNameDesc = [NSSortDescriptor sortDescriptorWithKey:@"lastname" ascending:YES];
// 3. 再按照名排序
NSSortDescriptor *firstNameDesc = [NSSortDescriptor sortDescriptorWithKey:@"firstname" ascending:YES];
// 按顺序添加排序描述器
NSArray *descs = [NSArray arrrayWithObjects:bookNameDesc, lastnameDesc, firstnameDesc, nil];

NSArray *arr4 = [arr sortedArrayUsingDescriptors:descs];
```



#### NSMutableArray 可变数组

- 可变的NSArray，是NSArray的子类，可以随意的添加或者删除元素
- 创建方法：
  - `+ (id)arrayWithCapacity:(NSUInteger)numItems`
  - `- (id)initWithCapacity:(NSUInteger)numItems`
- 也可以使用创建NSArray的方法来创建NSMutableArray
- 当一个元素加到集合中时，会执行一次retain操作；当一个元素从集合中移除时，会执行一次release操作；当集合被销毁时，集合里的所有元素会执行一次release操作，这个原则也适用于字典或者其他集合。



**基本使用**：

```objective-c
#pragma mark mutableArray
void mutableArray(){
    NSMutableArray *arr = [NSMutableArray arrayWithCapacity:5];
    [arr addObject:@"w"];
    [arr addObject:@"e"];
    [arr addObject:@"x"];
    
    [arr removeObject:@"w"];
    [arr removeLastObject];
    [arr removeAllObjects];
}
```

其他操作都跟之前的差不多，需要使用的时候再看。



#### NSDictionary 字典

键值对，不可变。

```objective-c
#pragma mark createDict
void createDict(){
    NSDictionary *dict = [NSDictionary dictionary];
    
    dict = [NSDictionary dictionaryWithObject:@"v" forKey:@"k"];
    
    dict = [NSDictionary dictionaryWithObjectsAndKeys:@"v1", @"k1", @"v2", @"k2", @"v3", @"k3", nil];
    
    NSArray *values = [NSArray arrayWithObjects:@"v1", @"v2", @"v3", nil];
    NSArray *keys = [NSArray arrayWithObjects:@"k1", @"k2", @"k3", nil];
    dict = [NSDictionary dictionaryWithObjects:values forKeys:keys];
}
```



**基本使用**：

```objective-c
#pragma mark useDict
void useDict(){
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:@"v1", @"k1", @"v2", @"k2", @"v3", @"k3", nil];
    
    NSLog(@"count %zi", dict.count);
    
    // BOOL isEqual = [dict isEqualToDictionary:<#(nonnull NSDictionary *)#>];
    
    [dict writeToFile:@"xx/xx/xx" atomically:YES];
    
    NSArray *keys = [dict allKeys];
    NSArray *keysOfObj = [dict allKeysForObject:@"v1"];
    NSArray *values = [dict allValues];
    NSString *str = [dict objectForKey:@"k1"];
    NSArray *valuesOfKey = [dict objectForKey:@"k1"];
}
```



**遍历**：

```objective-c
#pragma mark visitDict
void visitDict(){
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:@"v1", @"k1", @"v2", @"k2", @"v3", @"k3", nil];
    
    for (id key in dict) {
        id value = [dict objectForKey:key];
        NSLog(@"%@ = %@", key, value);
    }
}
```



**迭代器**：

```objective-c
NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:@"v1", @"k1", @"v2", @"k2", @"v3", @"k3", nil];

NSEnumerator *keyEnumerator = [dict keyEnumerator];
// NSEnumerator *valueEnumerator = [dict objectEnumerator];

id key = nil;
while (key = [keyEnumerator nextObject]){
  id value = [dict objectForKey:key];
  NSLog(@"%@ = %@", key, value);
}
```



```objective-c
NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:@"v1", @"k1", @"v2", @"k2", @"v3", @"k3", nil];
    
    [dict enumerateKeysAndObjectsUsingBlock:
     ^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        NSLog(@"%@ = %@", key, value);
    }];

```



> 像之后的MutableDictionary，还有Set系列，整体和前面的差不多



#### NSNumber 数字类型

- NSNumber可以将基本数据类型包装成对象，这样就可以间接将基本数据类型存进NSArray，NSDictionary等集合中
- 常见的初始化方法：
  - `+ (NSNumber *)numberWithChar:(char)value`
  - `+ (NSNumber *)numberWithInt:(int)value`
  - `+ (NSNumber *)numberWithFloat:(float)value`
  - `+ (NSNumber *)numberWithBool:(BOOL)value`
  - `- (id)initWithChar:(char)value`
  - `- (id)initWithInt:(int)value`
  - `- (id)initWithFloat:(float)value`
  - `- (id)initWithBool:(BOOL)value`



```objective-c
#pragma mark useNumber
void useNumber(){
    NSNumber *number = [NSNumber numberWithInt:3];
    NSMutableArray *array = [NSMutableArray array];
    
    [array addObject:number];
    
    NSNumber *number1 = [array lastObject];
    
    int num = [number intValue];
}

```



- 其他常用方法：
  - `- (char)charValue`
  - `- (int)intValue`
  - `- (double)doubleValue`
  - `- (BOOL)boolValue`
  - `- (NSString *)stringValue`
  - `- (NSComparisionResult)compare:(NSNumber *)otherNumber`
  - `- (BOOL)isEqualToNumber:(NSNumber *)number`



#### NSNull 空对象

- 集合中不能存放nil对象的，因为nil在集合中有特殊含义，但有时确实需要存储一个表示“什么都没有”的值，那么就可以使用NSNull，它也是NSObject的一个子类
- 创建方法：
  - `+ (NSNull *)null`

> 该创建方法总是返回一个相同的值，可以说是一个单例对象



#### NSDate 时间对象

- 静态初始化方法：
  - `+ (id)date` ：返回当前时间
  - `+ (id)dateWithTimeIntervalSinceNow:(NSTimeInterval)secs`：返回以当前时间为基准，然后过了secs秒的时间
  - `+ (id)dateWithTimeIntervalSinceReferenceDate:(NSTimeInterval)secs`：和上面一样，只是以2001/1/1为基准
  - `+ (id)dateWithTimeIntervalSince1970:(NSTimeInterval)secs`：和上面一致，但是以1970/1/1为基准
  - `+ (id)distantFuture`：返回很多年后的未来的某一天（随机）
  - `+ (id)distantPast`：返回很多年以前的某一天

```objective-c
#pragma mark testDate
void testDate(){
    NSDate *date = [NSDate date];
    
    date = [NSDate dateWithTimeIntervalSinceNow:10];
    
    date = [NSDate dateWithTimeIntervalSince1970:10];
    
    date = [NSDate distantFuture];
    
    date = [NSDate distantPast];
}
```



**其他使用：**

```objective-c
// 返回从1970-1-1开始走过的毫秒数
NSTimeInterval interval = [date timeIntervalSince1970];
// 跟其他时间比较
// [date timeIntervalSinceDate:<#(nonnull NSDate *)#>]

NSDate *date2 = [NSDate date];
// 返回比较早的时间
[date earlierDate:date2];
// 返回比较晚的时间
[date laterDate:date2];
// 格式化日期
NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
formatter.dateFormat = @"yyyy-MM-dd hh:mm:ss";
NSString *str = [formatter stringFromDate:date];
[formatter dateFromString:@"2020-09-09 13:14:56"];
[formatter release];

```



#### NSObject 基类

常用方法：

```objective-c
#pragma mark useObject
void useObject(){
    Student *stu = [Student student];
    // 是否是Person或者是他的子类
    BOOL isPerson = [stu isKindOfClass:[Person class]];
    // 是否是Student类，不包含他的子类
    BOOL isStudent = [stu isMemberOfClass:[Student class]];
    // 是否实现了该协议
    BOOL canStudy = [stu conformsToProtocol:@protocol(study)];
    // 类是否有对应方法
    BOOL canTest = [Student instancesRespondToSelector:@selector(test)];
    // 对象是否有对应方法
    BOOL canTest2 = [stu respondsToSelector:@selector(test2:)];
    // 操作对象的方法
    [stu performSelector:@selector(test)];
}

```



**反射**：

```objective-c
#pragma mark reflect
void reflect(){
    // 类名的反射
    NSString *name = @"Person";
    Class class = NSClassFromString(name);
    
    Person *person = [[class alloc] init];
    
    [person release];
    
    NSString *str = NSStringFromClass([Person class]);
    
    // 方法的反射
    NSString *method = @"test";
    SEL selector =NSSelectorFromString(method);
    [person performSelector:selector];
    
    NSString *selectorName = NSStringFromSelector(selector);
}

```



## Copy

### copy & mutableCopy

- 一个对象使用copy或mutableCopy方法可以创建对象的副本
- copy - 需要实现NSCopying协议，创建的是不可变的副本
- mutableCopy - 需要实现NSMutableCopying协议，创建的是可变副本
- 深复制：内容拷贝，源对象和副本指向的是同一个对象。对象的引用计数器+1，其实相当于做了一次retain操作
- **只有不可变对象创建不可变副本（copy）才是浅复制，其他都是深复制**



### 字符串拷贝

```objective-c
#pragma mark stringCopy
void stringCopy(){
    NSString *str1 = [[NSString alloc] initWithString:@"abc"];
    
    // 浅拷贝，直接把原来的字符串返回，计数器加一
    NSString *str2 = [str1 copy];
    
    // 深拷贝，会复制出一个新的字符串，原来的字符串的计数器不变
    NSString *str3 = [str1 mutableCopy];
    
    [str1 release];
    [str2 release];
    [str3 release];
}
```

```objective-c
#pragma mark mutableStringCopy
void mutableStringCopy(){
    NSMutableString *str = [[NSMutableString alloc] initWithString:@"abc"];
    
    // 不管哪种copy都是深拷贝
    NSString *str1 = [str copy];
    NSMutableString *str2 = [str mutableCopy];
    
    [str release];
    [str1 release];
    [str2 release];
}

```



> NSDictionary，NSArray等都是跟上面一样



### 自定义类拷贝

#### 属性拷贝

```objective-c
@interface Person : NSObject

// copy代表set方法会release旧对象，copy新对象
// 修改外面的变量，并不会影响到内部的成员变量
// 建议：NSString一班用copy策略，其他对象一班用retain
@property (nonatomic, copy) NSString *name;

- (void)test;

@end
```

```objective-c
#pragma mark perNameCopy
void perNameCopy(){
    Person *person = [[Person alloc] init];
    
    NSMutableString *str = [[NSMutableString alloc] initWithString:@"wex"];
    person.name = str;
    
    [str appendString:@"ss"];
    
    // 不会改变name
    NSLog(@"person name : %@", person.name);
}
```



#### 类的拷贝

需要实现对应的协议：

```objective-c
@interface Person : NSObject <NSCopying>
```



实现对应的拷贝方法：

```objective-c
- (id)copyWithZone:(NSZone *)zone{
    Person *copy = [[Person allocWithZone:zone] init];
    copy.name = self.name;
    
    // 这里特殊，不需要释放内存
    return copy;
}
```


























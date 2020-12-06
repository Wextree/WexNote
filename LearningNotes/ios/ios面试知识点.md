# IOS面试知识点

## 一、Objective-C相关

### 1. #include与#import的区别、#import与@class 的区别

- `#include`和`#import` 其效果相同,都是导入类中定义的行为(方法);
-  `#import` 不会引起交叉编译,确保头文件只会被导入一次；
-  `@class` 表明只定义了类的名称,而具体类的行为是未知的,一般用于.h 文件
- ` @class`比`#import`编译效率更高。如果许多个文件引用一个头文件，当这个头文件更改的时候，所有引用它的头文件都需要重新编译，效率会非常低，但是如果用@class就不会。
- 此外`@class`和`#import`的主要区别在于解决引用死锁的问题。其实解释解决A引用B，B引用A的循环依赖问题。



### 2. 请分别说明@public、@protected、@private的含义与作用

- `@public`:对象的实例变量的作用域在任意地方都可以被访问 ;
-  `@protected`:对象的实例变量作用域在本类和子类都可以被访问 ;
- ` @private`:实例变量的作用域只能在本类(自身)中访问 .



### 3.  为啥这样写

```objc
- (instancetype)init
{
    if (self = [super init]) {
        // Custom initialization
    }
    return self;
}
```

我们知道`alloc`返回一个有效的未初始化的对象实例。对于`self`是`alloc` 返回的指针，同时可以在所有的方法作用域内访问。

但是对于 `super`，它只是一个"编译器指示符",告诉编译器在父类中搜索方法的实现。

优先调用`[super init]` 是为了使超类完成它们自己的初始化工作。

那么 `if (self = [super init])`又是做啥？

这里是担心父类初始化失败，如果初始化一个对象失败，就会返回`nil`，当返回`nil`的时候`self = [super init]`测试的主体就不会再继续执行。如果不这样做，你可能会操作一个不可用的对象，它的行为是不可预测的，最终可能会导致你的程序崩溃。



### 4. 理解 Self & Super

```objectivec
@implementation Son : Father
- (id)init
{
    self = [super init];
    if (self)
    {
        NSLog(@"%@", NSStringFromClass([self class]));
        NSLog(@"%@", NSStringFromClass([super class]));
    }
    return self;
}
@end
```

`self`表示当前这个类的对象，而`super`是一个编译器标示符，和`self`指向同一个消息接受者。在本例中，无论是`[self class]`还是`[super class]`，接受消息者都是`Son`对象，但`super`与`self`不同的是，`self`调用`class`方法时，是在子类`Son`中查找方法，而`super`调用`class`方法时，是在父类`Father`中查找方法。

当调用`[self class]`方法时，会转化为`objc_msgSend`函数，这个函数定义如下：



```objectivec
id objc_msgSend(id self, SEL op, ...)  
```

这时候就开始了消息传递和转发的过程，会先从`Cache` 中查找方法，然后当前类，如果还是查找不到会去父类，直至`NSObject`类

对于`NSObject`类中，`- (Class)class`的实现如下：



```kotlin
- (Class)class {  
    return object_getClass(self);  
}  
```

所以打印结果为`Son`

当调用`[super class]`方法时，会转化为`objc_msgSendSuper`，这个函数定义如下：



```objectivec
id objc_msgSendSuper(struct objc_super *super, SEL op, ...)  
```

`objc_msgSendSuper`函数第一个参数`super`的数据类型是一个指向`objc_super`的结构体



```objectivec
struct objc_super {
   __unsafe_unretained id receiver;
   __unsafe_unretained Class super_class;
};
```

结构体包含两个成员，第一个是`receiver`，表示类的实例。第二个成员是记录当前类的父类是什么,会优先从`Father`这个类里去找`- (Class)class`,然后进行消息传递的过程。

会发现不管是`self`、还是`super`指向消息接受者是一样的，并且经过消息传递，最终处理消息的方法都是`NSObject`中的`- (Class)class`方法。





### 5. 什么情况使用 weak 关键字，相比 assign 有什么不同？

1. 在 ARC 中,在有可能出现循环引用的时候,往往要通过让其中一端使用 weak 来解决,比如: delegate 代理属性
2. 自身已经对它进行一次强引用,没有必要再强引用一次,此时也会使用 weak,自定义 IBOutlet 控件属性一般也使用 weak；当然，也可以使用strong。
3. weak: 此特质表明该属性定义了一种“非拥有关系” (nonowning relationship)。为这种属性设置新值时，设置方法既不保留新值，也不释放旧值。此特质同assign类似， 然而在属性所指的对象遭到摧毁时，属性值也会清空(nil out)。 而 assign
    的“设置方法”只会执行针对“纯量类型” (scalar type，例如 CGFloat 或 NSlnteger 等)的简单赋值操作。
4. assigin 可以用非 OC 对象,而 weak 必须用于 OC 对象



### 6. @protocol 和 category 中如何使用 @property

● 在 protocol 中使用 property 只会生成 setter 和 getter 方法声明,我们使用属性的目的,是希望遵守我协议的对象能实现该属性
 ● category 使用 @property 也是只会生成 setter 和 getter 方法的声明,如果我们真的需要给 category 增加属性的实现,需要借助于运行时的两个函数：

```undefined
objc_setAssociatedObject  
objc_getAssociatedObject
```



### 7. 用@property声明的NSString（或NSArray，NSDictionary）经常使用copy关键字，为什么？如果改用strong关键字，可能造成什么问题？

1. 因为父类指针可以指向子类对象,使用 copy 的目的是为了让本对象的属性不受外界影响,使用 copy 无论给我传入是一个可变对象还是不可对象,我本身持有的就是一个不可变的副本.
2. 如果我们使用是 strong ,那么这个属性就有可能指向一个可变对象,如果这个可变对象在外部被修改了,那么会影响该属性.


> copy 此特质所表达的所属关系与 strong 类似。然而设置方法并不保留新值，而是将其“拷贝” (copy)。 当属性类型为 NSString 时，经常用此特质来保护其封装性，因为传递给设置方法的新值有可能指向一个 NSMutableString 类的实例。这个类是 NSString 的子类，表示一种可修改其值的字符串，此时若是不拷贝字符串，那么设置完属性之后，字符串的值就可能会在对象不知情的情况下遭人更改。所以，这时就要拷贝一份“不可变” (immutable)的字符串，确保对象中的字符串值不会无意间变动。只要实现属性所用的对象是“可变的” (mutable)，就应该在设置新属性值时拷贝一份。



### 8. 这个写法会出什么问题： @property (copy) NSMutableArray *array;

两个问题：

1. 添加,删除,修改数组内的元素的时候,程序会因为找不到对应的方法而崩溃.因为 copy 就是复制一个不可变 NSArray 的对象；
2. 使用了 atomic 属性会严重影响性能 ；（默认是atomic）



### 9. 如何为 Class 定义一个对外只读对内可读写的属性?

在头文件中将属性定义为readonly, 在.m文件中将属性重新定义为readwrite



### 10. 为什么很多内置类如UITableViewController的delegate属性都是assign而不是retain的？

所有的引用计数系统，都存在循环应用的问题。例如下面的引用关系：
 • 对象a创建并引用到了对象b.
 • 对象b创建并引用到了对象c.
 • 对象c创建并引用到了对象b.这时候b和c的引用计数分别是2和1。

> 当a不再使用b，调用release释放对b的所有权，因为c还引用了b，所以b的引用计数为1，b不会被释放。b不释放，c的引用计数就是1，c也不会被释放。从此，b和c永远留在内存中。这种情况，必须打断循环引用，通过其他规则来维护引用关系。我们常见的delegate往往是assign方式的属性而不是retain方式 的属性，赋值不会增加引用计数，就是为了防止delegation两端产生不必要的循环引用。如果一个UITableViewController 对象a通过retain获取了UITableView对象b的所有权，这个UITableView对象b的delegate又是a， 如果这个delegate是retain方式的，那基本上就没有机会释放这两个对象了。



### 11. 使用block有什么好处？请使用`NSTimer`写出一个使用block显示（在`UILabel`上）秒表的代码

- 代码紧凑，传值、回调都很方便，省去了写代理的很多代码。

```objectivec
NSTimer封装成的block，实现方法：
NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:1.0 repeats:YES callback:^() {
weakSelf.secondsLabel.text = ...
}
[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```



### 12. block和代理的区别，哪个更好？

- 代理回调更面向过程，block更面向结果。
-  如果需要在执行的不同步骤时被通知，你就要使用代理。 如果只需要请求的消息或者失败的详情，应该使用block。

- block更适合与状态无关的操作，比如被告知某些结果。block之间是不会相互影响的。

- 但是代理更像一个生产流水线，每个回调方法是生产线上的一个处理步骤，一个回调的变动可能会引起另一个回调的变动。要是一个对象有超过一个的不同事件，应该使用代理。

- 一个对象只有一个代理，要是某个对象是个单例对象，就不能使用代理。要是一个对象调用方法需要返回一些额外的信息，就可能需要使用代理。



### 13. 在block内如何修改block外部变量？

默认情况下，在block中访问的外部变量是复制过去的，即：写操作不对原变量生效。但是你可以加上__block来让其写操作生效，示例代码如下:

```cpp
__block int a = 0;
void  (^foo)(void) = ^{
a = 1;
}
f00();
```





### 14. 类别和类扩展的区别

- 一般的类扩展写到.m文件中;一般的私有属性写到类扩展中
- Category的小括号中有名字,而Extension没有;
- Category只能扩充方法,不能扩充成员变量和属性;
- 如果Category声明了声明了一个属性,**那么Category只会生成这个属性的set,get方法的声明**,也就不是会实现.



### 15. 分类的作用？分类和继承的区别？

- 分类可以在不获悉，不改变原来代码的情况下往里面添加新的方法，只能添加，不能删除修改，并且如果分类和原来类中的方法产生名称冲突，则分类将覆盖原来的方法，因为分类具有更高的优先级。
- 继承可以增加，修改或者删除方法，并且可以增加属性；但是分类只能添加方法，不能删除修改，也不能增加属性。












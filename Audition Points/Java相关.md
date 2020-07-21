## Java基础

### 封装、继承和多态

#### 封装

1. **定义：**隐藏对象的属性和实现细节，仅对外公开接口,控制在程序中属性的读和修改的访问级别。

2. **封装的目的是：**增强安全性和简化编程，使用者不必了解具体的实现细节，而只是要通过外部接口，一特定的访问权限来使用类的成员。

3. **封装的基本要求是:**把所有的属性私有化，对每个属性提供getter和setter方法，如果有一个带参的构造函数的话，那一定要写一个不带参的构造函数。在开发的时候经常要对已经编写的类进行测试，所以在有的时候还有重写toString方法，但这不是必须的。

4. **好处：**良好的封装能够减少耦合、类内部的结构可以自由修改、可以对成员进行更精确的控制、隐藏信息，实现细节。

#### 继承（强耦合）

1. **目的：**实现代码的复用。

2. **介绍：**当两个类具有**相同的特征（属性）和行为（方法）**时，可以将相同的部分抽取出来放到一个类中作为父类，其它两个类继承这个父类。继承后子类自动拥有了父类的属性和方法。
3. **继承原则：**
   - 子类拥有父类非private的属性和方法
   - 子类不能继承父类的构造器，调用父类的构造方法我们使用super()即可。（会调用父类默认的构造器）
   - 子类可以拥有自己属性和方法，即子类可以对父类进行扩展。
   - 子类可以用自己的方式实现父类的方法。

#### 多态

1.  **定义：**所谓多态就是指程序中定义的**引用变量所指向的具体类型和通过该引用变量发出的方法调用**在**编程时并不确定**，而是在**程序运行期间才确定**，即一个引用变量倒底会指向哪个类的实例对象，该引用变量发出的方法调用到底是哪个类中实现的方法，必须在由程序运行期间才能决定。因为在程序运行时才确定具体的类，这样，不用修改源程序代码，**就可以让引用变量绑定到各种不同的类实现上**，从而导致该引用调用的具体方法随之改变，即不修改程序代码就可以改变程序运行时所绑定的具体代码，让**程序可以选择多个运行状态**，这就是多态性。
2.  **原则：** 指向子类的父类引用由于**向上转型**了，它只能**访问父类中拥有的方法和属性**，而对于子类中存在而父类中不存在的方法，该引用是不能使用的，尽管是重载该方法。若子类**重写**了父类中的某些方法，在调用该些方法的时候，必定是**使用子类中定义的这些方法**（动态连接、动态调用）。
3.  **区分：**
    - **编译时多态：**是静态的，主要是指方法的重载，它是根据参数列表的不同来区分不同的函数，通过编辑之后会变成两个不同的函数，在运行时谈不上多态。
    - **运行时多态：**是动态的，它是通过动态绑定来实现的，也就是我们所说的多态性。
4.  **条件：**继承、重写、向上转型
5.  **实现形式：**继承和接口



### 抽象类

被**abstract修饰**的方法是抽象方法，抽象方法**没有方法体**。修饰符 abstract 返回值类型 函数名();抽象方法的修饰符只能用**public**或者**protected**或者没有修饰，不能被**final，static，private**修饰。、

#### 原则

1. 类即使**不包含抽象方法**，也可以定义成抽象类。
2. 类中含有抽象方法的类**一定**要定义成抽象类。
3. 抽象类中字段的定义和子类的访问与一般类没有变化。
4. 扩展抽象类有两种方法，第一种是在子类中**定义部分抽象方法或者抽象方法不定义**，这样子类也**必须定义成抽象类**，第二种是**定义全部的抽象方法**，这样子类就**可以不定义成抽象**的了。
5. 抽象类**不能被实例化**，但是可以定义一个抽象类的对象变量，这个变量可以引用非抽象子类的对象。
6. 抽象类中包含有构造方法，也可以显式书写构造方法，**构造方法在实例化子类的对象中调用**。

#### 与接口的不同

1. 接口可以**多实现**，而抽象类只能**单继承**
2. 抽象类可以有非抽象的方法和构造方法、变量，但是**接口只能有抽象方法，静态常量**。
3. **抽象类**和子类具有父子关系，**子类能拥有父类中一些属性**。**接口**虽然某个类实现一个接口，但是由于接口中的**变量都为静态常量**，**不存在继承**关系。

#### 与接口的共同点

1. 无论接口还是抽象类，都**无法直接实例化**，其自身实例化需要靠实现类或子类来实现。
2. 接口和抽象类都**必须实现其中的所有方法**。



### 序列化和反序列化

在Java中，我们可以通过多种方式来创建对象，并且只要对象没有被回收，我们都可以复用此对象。但是，我们**创建出来的这些对象都存在于JVM中的堆（heap）内存中**，只有JVM处于**运行状态**的时候，这些对象才可能存在。一旦JVM停止，这些对象也就随之消失；

但是在真实的应用场景中，我们需要将这些对象**持久化**下来，并且在需要的时候将对象重新读取出来，Java的序列化可以帮助我们实现该功能。

对象序列化机制（object serialization）是java语言内建的一种对象持久化方式，通过对象序列化，可以将**对象**的状态信息保存为**字节数组**，并且可以在有需要的时候将这个字节数组通过反序列化的方式转换成对象，对象的序列化可以很容易的在JVM中的活动对象和字节数组（流）之间进行转换。

#### Serialization接口

Java类通过实现java.io.Serialization接口来启用序列化功能，**未实现此接口的类将无法将其任何状态或者信息进行序列化或者反序列化**。可序列化类的所有子类型都是可以序列化的。序列化接口**没有方法或者字段**，仅用于**标识**可序列化的语义。

当试图对一个对象进行序列化时，如果遇到一个没有实现java.io.Serialization接口的对象时，将**抛出`NotSerializationException`异常**。

如果要序列化的类有**父类**，要想将在父类中定义过的变量序列化下来，那么父类也应该实现java.io.Serialization接口。

#### Externalizable接口

Externalizable**继承了Serializable**，该接口中定义了两个抽象方法：`writeExternal()`与`readExternal()`。当使用Externalizable接口来进行序列化与反序列化的时候需要开发人员重写`writeExternal()`与`readExternal()`方法。
在使用Externalizable进行序列化的时候，在读取对象时，会调用被序列化类的**无参构造器去创建一个新的对象**，然后再将被保存对象的字段的值分别填充到新对象中。所以，实现Externalizable接口的类**必须要提供一个public的无参的构造器**。

#### 静态变量的序列化

序列化保存的是**对象的状态**，静态变量属于**类的状态**，因此 序列化并不保存静态变量。

#### Transient 关键字使用

在变量声明前加上该关键字，可以**阻止该变量被序列化**到文件中，在被反序列化后，transient 变量的值被**设为初始值**，如 int 型的是 0，对象型的是 null。



### 集合类

#### Collection

- **size()** - 集合内的对象数量
- **add(E)/addAll(Collection)** - 向集合内添加单个/批量对象
- **remove(Object)/removeAll(Collection)** - 从集合内删除单个/批量对象
- **contains(Object)/containsAll(Collection)** - 判断集合中是否存在某个/某些对象
- **toArray()** - 返回包含集合内所有对象的数组

##### ArrayList

ArrayList基于数组来实现集合的功能，其内部维护了一个**可变长**的对象数组，集合内所有对象存储于这个数组中，并实现该数组长度的**动态伸缩**

##### LinkedList

LinkedList基于链表来实现集合的功能，其实现了静态类Node，集合中的每个对象都由一个Node保存，每个Node都拥有到自己的前一个和后一个Node的引用（**双向链表**）

##### ArrayList & LinkedList

- 随机获取ArrayList更快一些，因为LinkedList需要从头开始遍历，而ArrayList是直接访问。
- LinkedList在**非尾部**插入删除的效率更高。
- ArrayList扩容每次为1.5倍，而LinkedList所占用的空间会比较少
- 遍历效率差不多，但是LinkedList应该用**iterator**进行遍历，而不能用get()。

##### Vector

Vector和ArrayList很像，都是基于数组实现的集合，它和ArrayList的**主要区别**在于

- Vector是**线程安全**的，而ArrayList不是
- 由于Vector中的方法基本都是**synchronized**的，其**性能低**于ArrayList
- Vector可以**定义数组长度扩容的因子**，ArrayList不能

##### CopyOnWriteArrayList

- 也是一个线程安全版。适合**读多写少**的情况。性能比较好。
- 它在写的时候**复制一个副本**，然后再副本上操作，最后再**修改对应的引用**。
- 对读是不加锁的，所以只能保证读到已经完成的。只能保证最终数据的**最终一致性**，不能保证**实时一致**性。



#### Map

- **size()** - 集合内的对象数量
- **put(K,V)/putAll(Map)** - 向Map内添加单个/批量对象
- **get(K)** - 返回Key对应的对象
- **remove(K)** - 删除Key对应的对象
- **keySet()** - 返回包含Map中所有key的Set
- **values()** - 返回包含Map中所有value的Collection
- **entrySet()** - 返回包含Map中所有key-value对的EntrySet
- **containsKey(K)/containsValue(V)** - 判断Map中是否存在指定key/value

##### HashMap

HashMap将Entry对象存储在一个数组中，并通过**哈希表**来实现对Entry的快速访问（详见**底层结构**部分）

##### Hashtable

基本上已经被**弃用**，尽量避免使用。不允许传入key为null的键值对。线程安全，所有方法几乎都被**synchronized**修饰。

##### ConcurrentHashMap

而ConcurrentHashMap使用分段锁的思路解决并发性能，其将 Entry数组拆分至**16个Segment**中，以哈希算法决定Entry应该存储在哪个Segment。这样就可以实现在写操作时只**对一个Segment 加锁**，大幅提升了并发写的性能。

在进行读操作时，ConcurrentHashMap在绝大部分情况下都**不需要加锁**，其Entry中的**value是volatile**的，这保证了value被修改时的**线程可见性**，无需加锁便能实现线程安全的读操作。

##### LinkedHashMap

LinkedHashMap与HashMap非常类似，唯一的不同在于前者的Entry在HashMap.Entry的基础上增加了到前一个插入和后一个插入的Entry的引用，以实现能够按Entry的插入顺序进行遍历。

##### TreeMap

TreeMap是基于红黑树实现的Map结构，其Entry类拥有到左/右叶子节点和父节点的引用，同时还记录了自己的颜色。



#### Set

##### HashSet

1. 不能保证元素的排列顺序，顺序可能与添加顺序不同，顺序也有可能发生变化。

2. HashSet不是同步的，如果多个线程同时访问一个HashSet，则必须通过代码来保证其同步。
   3.集合元素值可以是null。

除此之外，HashSet判断两个元素是否相等的标准也是其一大特点。HashSet集合判断两个元素相等的标准是两个对象通过**equals()**方法比较相等，并且两个对象的**hashCode()**方法返回值也相等。

1. 只要是它们的hashCode()方法返回不相等，HashSet将会把它们存储在**不同的位置**。
2. 如果两个对象通过equals()方法比较不相等，hashCode()方法比较相等，HashSet将会把它们存储在相同的位置，在这个位置以链表式结构来保存多个对象。这是因为当向HashSet集合中存入一个元素时，HashSet会调用对象的hashCode()方法来得到对象的hashCode值，然后根据该hashCode值来决定该对象存储在HashSet中存储位置。

3. 如果有两个元素通过equal()方法比较返回true，但它们的hashCode()方法返回true，HashSet将不予添加。



##### LinkedHashSet

LinkedHashSet是HashSet对的子类，也是根据元素的hashCode值来决定元素的存储位置，同时使用链表维护元素的次序，使得元素是以插入的顺序来保存的。当遍历LinkedHashSet集合里的元素时，LinkedHashSet将会**按元素的添加顺序**来访问集合里的元素。但是由于要维护元素的插入顺序，在性能上略低与HashSet，但在迭代访问Set里的全部元素时有很好的性能。
 **注意：**LinkedHashSet依然不允许元素重复，**判断重复标准与HashSet一致**。

**补充：HashSet的实质是一个HashMap。**HashSet的所有集合元素，构成了HashMap的key，其value为一个静态Object对象。因此HashSet的所有性质，HashMap的key所构成的集合都具备。可以参考后续文章中HashMap的相关内容进行比对。



#####TreeSet

TreeSet是**SortedSet接口的实现类**，正如SortedSet名字所暗示的，TreeSet可以确保集合**元素处于排序状态**。此外，TreeSet还提供了几个额外的方法。

- **comparator():**返回对此 set 中的元素进行排序的比较器；如果此 set 使用其元素的自然顺序，则返回null。

- **first():**返回此 set 中当前第一个（最低）元素。

- **last():** 返回此 set 中当前最后一个（最高）元素。

- **lower(E e):**返回此 set 中严格小于给定元素的最大元素；如果不存在这样的元素，则返回 null。

- **higher(E e):**返回此 set 中严格大于给定元素的最小元素；如果不存在这样的元素，则返回 null。

- **subSet(E fromElement, E toElement):**返回此 set 的部分视图，其元素从 fromElement（包括）到 toElement（不包括）。

- **headSet(E toElement):**返回此 set 的部分视图，其元素小于toElement。

- **tailSet(E fromElement):**返回此 set 的部分视图，其元素大于等于 fromElement。

  对于TreeSet集合而言，判断两个对象是否相等的**唯一标准**是：**两个对象通过compareTo(Object obj)方法比较是否返回0**——如果通过compareTo(Object obj)方法比较返回0，TreeSet则会认为它们相等，不予添加入集合内；否则就认为它们不相等，添加到集合内。

TreeSet的自然排序是根据集合元素中compareTo(Object obj)比较的大小，以升序排列。而定制排序是通过Comparator接口的帮助。该接口包含一个int compare(T o1,T o2)方法，该方法用于比较o1,o2的大小：如果该方法返回正整数，则表明o1大于o2；如果该方法返回0，则表明o1等于o2;如果该方法返回负整数，则表明o1小于o2。
 如果要实现定制排序，则需要在创建TreeSet时，调用一个带参构造器，传入Comparator对象。并有该Comparator对象负责集合元素的排序逻辑，集合元素可以不必实现Comparable接口。下面具体演示一下这种用法：

```java
public static void main(String[] args){
    Person p1 = new Person();
    p1.age =20;
    Person p2 =new Person();
    p2.age = 30;
    Comparator<Person> comparator = new Comparator<Person>() {

        @Override
        public int compare(Person o1, Person o2) {
            //年龄越小的排在越后面
            if(o1.age<o2.age){
                return 1;
            }else if(o1.age>o2.age){
                return -1;
            }else{
                return 0;
            }

        }
    };
    TreeSet<Person> set = new TreeSet<Person>(comparator);
    set.add(p1);
    set.add(p2);
    System.out.println(set);
}
```



#### Queue

##### Queue/Deque类集合

Queue和Deque接口继承Collection接口，实现FIFO（先进先出）的集合。二者的区别在于，**Queue只能在队尾入队，队头出队**，而**Deque接口则在队头和队尾都可以执行出/入队操作**

- Queue接口**常用方法**：
  - **add(E)/offer(E)：**入队，即向队尾追加元素，二者的区别在于如果队列是有界的，**add方法在队列已满的情况下会抛出**`IllegalStateException`，而**offer方法只会返回false**
  - **remove()/poll()：**出队，即从队头移除1个元素，二者的区别在于如果队列是空的，**remove方法会抛出**`NoSuchElementException`，而**poll只会返回null**
  - **element()/peek()：**查看队头元素，二者的区别在于如果队列是空的，element方法会抛出`NoSuchElementException`，而peek只会返回null
- Deque接口**常用方法**：

`addFirst(E)` /` addLast(E) `/` offerFirst(E)` /` offerLast(E)`/`removeFirst()` /` removeLast() `/ `pollFirst() `/ `pollLast()`/`getFirst() `/ `getLast() `/ `peekFirst() `/` peekLast()`/
`removeFirstOccurrence(Object) `/ `removeLastOccurrence(Object)`

**注意：**Stack过于古老，并且实现地非常不好，因此现在基本已经不用了，可以直接用Deque来代替Stack进行栈操作。



##### Queue接口的常用实现类

- **ConcurrentLinkedQueue**
  - ConcurrentLinkedQueue是基于**链表实现**的队列，队列中每个Node拥有到下一个Node的引用：
  - 由于Node类的**成员都是volatile的**，所以ConcurrentLinkedQueue自然是线程安全的。能够保证**入队和出队操作的原子性和一致性**，但在**遍历和size()操作**时只能保证数据的**弱一致性**。
- **LinkedBlockingQueue**
  - 与ConcurrentLinkedQueue不同，LinkedBlockingQueue是一种**无界的阻塞队列**。
  - 所谓阻塞队列，就是在入队时如果队列已满，线程会被阻塞，**直到队列有空间供入队再返回**；
  - 同时在出队时，如果队列已空，线程也会被阻塞，**直到队列中有元素供出队时再返回**。
  - LinkedBlockingQueue同样基于链表实现，其出队和入队操作都会使用**Reentrant Lock进行加锁**。所以本身是线程安全的，但同样的，只能保证入队和出队操作的原子性和一致性，在遍历时只能保证数据的弱一致性。
- **ArrayDeque**
  - 用数组实现的Deque；既然是底层是数组那肯定也可以指定其capacity，也可以不指定，默认长度是16，然后根据添加的元素的个数，动态扩展。ArrayDeque由于是两端队列，所以其顺序是按照元素插入数组中对应位置产生的。
  - ArrayDeque为了满足可以同时在数组两端插入或删除元素的需求，其内部的动态数组还必须是循环的，即循环数组（circular array），也就是说数组的任何一点都可能被看作起点或者终点。ArrayDeque维护了两个变量，表示ArrayDeque的头和尾
- **ArrayBlockingQueue**
  - ArrayBlockingQueue是一种有界的阻塞队列，基于**数组实现**。
  - 其同步阻塞机制的实现与LinkedBlockingQueue基本一致，区别仅在于前者的**生产和消费使用同一个锁**，后者的生产和消费**使用分离的两个锁**。
- **ConcurrentLinkedQueue vs LinkedBlockingQueue vs ArrayBlockingQueue**
  - **ConcurrentLinkedQueue是非阻塞队列**，其他两者为阻塞队列
  - 三者都是**线程安全**的
  - LinkedBlockingQueue是无界的，**适合实现不限长度的队列**， ArrayBlockingQueue**适合实现定长的队列**
- **SynchronousQueue**
  - SynchronousQueue算是JDK实现的队列中比较奇葩的一个，它不能保存任何元素，**size永远是0**，**peek()永远返回null**。
  - **向其中插入元素的线程会阻塞**，直到有另一个线程将这个元素取走，**反之从其中取元素的线程也会阻塞**，直到有另一个线程插入元素。
  - 这种实现机制非常**适合传递性的场景**。也就是说如果生产者线程需要及时确认到自己生产的任务已经被消费者线程取走后才能执行后续逻辑的场景下，适合使用SynchronousQueue。
- **PriorityQueue & PriorityBlockingQueue**
  - 这两种Queue并不是FIFO队列，而是**根据元素的优先级进行排序**，保证最小的元素最先出队，也可以在构造队列时传入**Comparator实例**，这样PriorityQueue就会按照Comparator实例的要求对元素进行排序。
  - PriorityQueue是非阻塞队列，也不是线程安全的，PriorityBlockingQueue是阻塞队列，同时也是线程安全的
  - **不允许插入null值**



### Comparable 和 Comparator的区别

- **comparable接口**实际上是出自java.lang包 它有一个 `compareTo(Object obj)`方法用来排序
- **comparator接口**实际上是出自 java.util 包它有一个`compare(Object obj1, Object obj2)`方法用来排序
- 如果比较的方法**只要用在一个类**中，用该类实现Comparable接口就可以。
- 如果比较的方法在**很多类中**需要用到，就自己写个类实现Comparator接口，这样当要比较的时候把实现了Comparator接口的类传过去就可以，省得重复造轮子。这也是为什么Comparator会在java.util包下的原因。



### == 和 equals

#### ==

- 基本类型：比较的是值是否相同；
- 引用类型：比较的是引用是否相同；

#### equals

equals **本质上就是 ==**，只不过 String 和 Integer 等重写了 equals 方法，把它变成了值比较。



### collection & collections

1. Collection是集合的上级**接口**，继承它的有Set和List接口
2. Collections是集合的**工具类**，提供了一系列的静态方法对集合的搜索、查找、同步等操作
   1. **sort(Collection)方法的使用(含义：对集合进行排序)。**
   2. **reverse()方法的使用(含义：反转集合中元素的顺序)。**
   3. **shuffle(Collection)方法的使用(含义：对集合进行随机排序)。**
   4. **fill(List list,Object o)方法的使用(含义：用对象o替换集合list中的所有元素)**
   5. **copy(List m,List n)方法的使用(含义：将集合n中的元素全部复制到m中,并且覆盖相应索引的元素)。**
   6. **min(Collection),min(Collection,Comparator)方法的使用(前者采用Collection内含自然比较法，后者采用Comparator进行比较)。**
   7. **max(Collection),max(Collection,Comparator)方法的使用(前者采用Collection内含自然比较法，后者采用Comparator进行比较)。**
   8. **indexOfSubList(List list,List subList)方法的使用(含义：查找subList在list中首次出现位置的索引)。**
   9. **lastIndexOfSubList(List source,List target)方法的使用与上例方法的使用相同**
   10. **rotate(List list,int m)方法的使用(含义：集合中的元素向后移m个位置，在后面被遮盖的元素循环到前面来)。移动列表中的元素，负数向左移动，正数向右移动**
   11. **swap(List list,int i,int j)方法的使用(含义：交换集合中指定元素索引的位置)**
   12. **binarySearch(Collection,Object)方法的使用(含义：查找指定集合中的元素，返回所查找元素的索引)。**
   13. **replaceAll(List list,Object old,Object new)方法的使用(含义：替换批定元素为某元素,若要替换的值存在刚返回true,反之返回false)。**



### Iterator

![](imgs\20141127192847281.png)

在创建Collection接口的同时，集合类库也创建了Iterator接口，这个接口的对象是一个**迭代器**，他会**依次遍历**集合中所有的元素。在开始的时候，如果集合是有序的，那么通过Collection接口的iterator方法返回的迭代器对象会**在集合起始位置**。

Iterator对象工作的原理是把每个集合中的对象**看作一个块**，it在这些块之间跳跃。在开始的时候it在第一个块前（如果是有序集），调用一次next()方法it就会跳到下个块之后，并且跳完之后返回在it前面的块。如果在开始直接it.remove()会报错，**因为remove的原理是删除在it之前的这个块，所以需要先进行next()操作**。同理，**连续remove两次**也是会报错的。

```java
Queue<Integer> qe1 = new LinkedList<>();

qe1.add(null);  
qe1.add(1);
qe1.add(20);
System.out.println(qe1);

Iterator<Integer> it = qe1.iterator();

//-------------error-------------
it.reomve();  //不能直接调用remove()，这时it没有跳过块，it之前没有内容
//-------------------------------


//-------------error-------------
it.next();
it.remove();
it.reomve();  //不能连续调用remove()，it之前的块已被删除，再调用报错
//-------------------------------


//--------------ok---------------
it.next();
it.remove();
it.next();
it.remove();
//-------------------------------

System.out.println(qe1);
//输出：
//[20]
```



#### Enumeration 和 Iterator接口的区别

与Enumeration相比，Iterator更加安全，**因为当一个集合正在被遍历的时候，它会阻止其它线程去修改集合**。

- Iterator的方法名比Enumeration更科学
- Enumeration占用内存小，且速度快，但是不够安全
- Iterator有fail-fast机制，比Enumeration更安全(**fail-fast 机制是java容器（Collection和Map都存在fail-fast机制）中的一种错误机制**。在遍历一个容器对象时，当容器结构被修改，很有可能会抛出ConcurrentModificationException，产生fail-fast。)
- Iterator能够删除元素，Enumeration并不能删除元素



#### ListIterator

当时用链表的时候也就意味着我们需要进行大量的增添和删除功能。对于链表这种有序集合，Iterator迭代器无疑是描述位置最好的类，但是在Iterator中并没有add方法（因为有很多无序集不需要在特定的位置增添元素）所以我们从Iterator中实现了一个子类来进行增删操作——ListIterator。

- ListIterator**继承了**Iterator接口，它用于**遍历List集合的元素**。
- ListIterator可以实现**双向遍历,添加元素，设置元素**

![](imgs\640.webp)

```java
public class LinkedListE {

    public static void main(String[] args) {
        // TODO Auto-generated method stub

        List<String> letter = new LinkedList<>();
		// a -> b -> c -> d
        letter.add(0, "a"); //在索引为0的位置添加元素
        letter.add("b");
        letter.add("c");
        letter.add("d");

        ListIterator li = letter.listIterator(2);//在第3个元素“c”（索引为2）之前放置迭代器
        li.previous();//迭代器向前遍历
        li.remove();//删除刚刚跳过的元素“b”

        li.next();//迭代器向后遍历
        li.remove();//删除刚刚跳过的元素“c”

        li.next();//迭代器向后遍历
        li.set("e");//将刚刚跳过的元素“d”重新设置为“e”

        System.out.println(li.nextIndex());//输出： 2
        System.out.println(li.previousIndex());//输出：1

        System.out.println(letter);//输出：[a,e]

        letter.get(0);//可以使用，但是不要这样做
    }

}

```



### final

**final**在java中可以**修饰类，方法和变量**（包括成员变量和局部变量）。

- **修饰类：**
  - 该类不能被继承
  - 成员方法会被隐式地设置为final
- **修饰方法：**
  - 把方法锁定，防止继承类对它进行修改
  - 早期java将final方法内嵌实现，但是最近的版本，不再对此优化。
- **修饰变量：**
  - 修饰成员变量时作为一个常量，第一次赋值之后不能再改变。（当指向一个基本类型，那么值是不可以改变的，但是如果指向一个引用，那么这个引用是不能改变，但是引用的对象的内容是可以改变的）
  - **两种初始化方式**，一种是在变量声明的时候初始化；第二种方法是在声明变量的时候不赋初值，**但是要在这个变量所在的类的所有的构造函数中对这个变量赋初值**。
  - 局部变量只需要保证在使用之前被初始化赋值即可

**特点：**

- 当final变量是基本数据类型以及String类型时，如果在编译期间能知道它的确切值，则编译器会把它当做编译期常量使用。也就是说在用到该final变量的地方，相当于直接访问的这个常量，不需要在运行时确定。
- 引用变量被final修饰之后，虽然不能再指向其他对象，但是它指向的对象的内容是可变的。




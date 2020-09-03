package digit;

import java.util.Arrays;
import java.util.LinkedList;

// 测试LinkedList
public class LinkedListTest {

    public static void main(String[] args) {
        // 初始化构造方法和ArrayList差不多，但是因为这是一条链表，所以不能初始化容量（所以在赋值的时候一定要先进行越界检查）
        LinkedList<Integer> list = new LinkedList<>(Arrays.asList(1, 2, 3));


        // 与list相同的方法就不再测试了

        // 由于是双向链表，所以有特殊的对头尾的操作，所以一般可以用做队列
        list.removeLast();
        list.removeFirst();
        // 从尾或者头匹配第一个出现的元素进行删除
        list.removeLastOccurrence(3);
        list.removeFirstOccurrence(1);

        list.addFirst(1);
        list.addLast(4);

        list.getFirst();
        list.getLast();

        list.indexOf(1);
        list.lastIndexOf(1);

        // LinkedList实现了Deque接口，所以也有它的对应方法
        // 使用栈和队列都可以使用LinkedList进行实现
    }
}

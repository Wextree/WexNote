# 基础知识

### 定义

tensor：张量

Tensor("node:src_output", shape(), dtype)

> node是结点名称，src_output是对应结点的第几号输出
>
> shape是维度，零阶是标量，n阶就是n维数组
>
> dtype是张量的唯一类型（会进行强制的类型检查）





# 语法

定义一个结点常量：tf.constant(content, dtype, name)

> content表示结点的内容，dtype表示结点数据类型，name表示结点的名字



定义一个结点变量：tf.Variable(value, name)

> 变量使用前一定要初始化

- init = tf.name_Variable_initializer()    单个变量初始化
- init = tf.global_Variables_initializer()    单个变量初始化
- sess.run(init)
- 更新：tf.assign(value, new_val ue)



### Session 会话

```python
sess = tf.Session()
print(sess.run(tens1))
# 记得关闭，不占内存
sess.close()

# 或者
with tf.Session() as sess:
	print(sess.run(tens1))
```



```python
# 生成会话并设置为默认会话
sess = tf.InteractiveSession()
print(result.eval())
sess.close()
```



### tf.Placeholder占位符

x = tf.Placeholder(dtype, shape=None, name=None)

用feed_dist{}字典进行传递 
# DFA 格式的设计
我查阅了大部分的资料，发现现在已存的大部分实验代码或者是分析中`.dfa`的中转换表都是用表示边的方式来实现的。

但是弊端很大：文件的大小取决于DFA中的跳转边的个数。也就是在很多情况下，这个文件会膨胀的令人害怕。

因为这种实现方法的潜意识：表示跳转表是一维的方式。即每一条信息仅仅能包含一条边。这就是浪费，因为你要表示这个边的三个信息：
1. 从哪来
2. 为什么跳转
3. 到哪去
这种思维方式是一维的。而这种思维下占据空间S的表达式为：

$$
S = \sum_{i=1}^n V_i
$$

E<sub>i</sub>的意义是第 `i`个节点的出边数。

但是我们在学习了矩阵这种数学工具之后，思维应该不仅仅是一维的。

看一个表格：
||a|b|
|--|--|--|
|0|1|2|
|1|3|2|
|2|1|3|
|3|3|3|

这个表格本身已经表示很多信息了。

进一步优化：
1. 隐去跳转状态。因为可以默认是字符集的顺序
2. 隐去开始状态。因为可以默认是状态集的顺序

得到表：
|1|2|
|--|--|
|3|2|
|1|3|
|3|3|

当然，这个也有待优化的地方：对稀疏表的表现性能不好。

占据空间S的数学表达式S：

$$
S = E * V
$$

E是边数，V是节点数。

但是在有些情况下还是很大，所以希望能够再次压缩数据。

采用离散数学中学到的邻接表的思想。（PS：下面几段话是表述这个表的特点，不想看的直接跳过文字，看表）

于是我把目光看向了二进制，因为二进制在表示存在于不存在的集合的时候有着较好的性能。但是这里的二进制串最后的表现形式可以是16进制或者熟悉的10进制。但是为了方便采用了16进制。

那么原本的表格就变了。

原本二维表格可以表示信息的原因是：横坐标、纵坐标、元素。分别表示三个物理量（起始、结束、条件）。

现在元素被压缩为{0,1}，即现在只能表示横坐标、纵坐标（起始、结束）。而元素的{0,1}的物理量是用来表示这个横纵坐标的存在是有意义的。所以损失了表格本身的表现力。但是可以把原来的串压缩为一两个字符就很恐怖（2个16进制字符就可以表示255个状态），原来255*255 的大小变为255 * 2 ，这就是巨大的提升。

但是因为表格的表现力降维了，所以多家一个表，一次表示点之间的跳转关系。

~~~
-0123 
00110
10011
20101
30001
=>
6,a,b
3,b,a
5,a,b
1,a-b
~~~

因为临界表无法表示两个点间的多个边，但是这里用分隔符的不同表示来区分是下一个点的边，还是另一条边。(","表示下一个点，"-"表示还是这个边的点)。

在点较少的时候表现不是很好，但是状态数量稍微多一点，就可以有很明显的区别。


# 待优化

对待系数表的表现不好。
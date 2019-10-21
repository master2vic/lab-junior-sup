# 编译原理第二次实验报告

## 计科1703/石渊友/201708010303

## 目录

[TOC]

## 实验题目

词法分析程序

## 实验目的

学习和掌握词法分析程序手工构造状态图及其代码实现方法。

## 实验任务

（1）阅读已有编译器的经典词法分析源程序；

（2）用C或C++语言编写一门语言的词法分析器。

## 实验内容

1. 选择已有的编译器的经典词法分析源程序。
2. 确定一门今后要实现编译器的语言
3. 根据关键词、词法单元、注释，确立关键词表，画出所有词法单元和注释对应的DFA图
4. 编写词法分析器
5. 准备样例

## 系统设计

### 实验环境

**实验环境 Ubuntu16.04 + gcc 7.4.0**

运行方式：使用`make`编译，使用`./cifa`运行。

使用样例方法（如使用`aaaa.cl`），`ehco “aaaa.cl” > fileNameFile`。

其中“aaaa.cl”可以替换为其他文件名。

### 文件树

.
├── aaaa.cl
├── aim.txt
├── cankao
├── cankao.cpp
├── fileNameFile.txt
├── makefile
├── test.cl
├── tree.txt
└── tttt.cl

`cankao`: 可执行文件，fileNameFile.txt 是存放目标文件的文件

正例：`aaaa.cl` , `tttt.cl`

反例：`bbbb.cl`,`cccc.cl`

### 关键字表

- `integers`:数字0-9的非空字符串
- `type identifiers`: 类型标识符 -> 由字母，数字和下划线字符，以大写字母开头
- `object identifiers`: 对象标识符 -> 由字母，数字和下划线字符，以小写字母开头
- `strings`:字符串 -> "..." \t：tab，\n：newline
- `comments`-- 表示一行注释；(* ... *)表示多行注释，可以嵌套
- `key-words`:关键字 
- `white space`:空白符 -> ' ', \n , \t , \r\n
- `operator`:运算符 + - * / - < > = { } ( ) : => <- => @

cool 语言的关键字：

~~~c
class
else
false
fi
if
in
inherits
isvoid
let
loop
pool
then
while
case
esac
new
of
not
true
~~~

### DFA
![DFA.png](DFA.png)

### 算法

#### 词法分析

~~~python
lexical analysis
	if not EOF
		get a new line
		get Tokens and print each Token
~~~

#### 获取词性

~~~python
get Token
	put line into state machine
    get word from state machine
    map word -> Token
	return Token
~~~

## 样例

~~~cool
(*
        fafhiuadsufihaisfhi***
        rwe
*)
()()
123
int
123j3
nameIS_12w
NameIS123_er_
_123
12_dsf_"sdf"
<-
<=
<s
sf=df
sf/sf
~~~

~~~
(*
        fafhiuadsufihaisfhi***
        rwe
*)
()()
123
int
123j3
nameIS_12w
NameIS123_er_
_123
12_dsf_"sdf"
<-
<=
<s
sf=df
sf/sf
~~~

~~~
(*
        this is a MULTI COM
*)
-- this is a comalie
class Main inherits IO{
        main(): SELF_TYPE {
                let x : Int <- 1, y : Int in{
                        ;
                }
        }
}
~~~

~~~
(*  this is a error file *)

class classA IO{
        main(): {
                123;
                _sf;
        }
}
~~~

## 实验心得

我选择的语言是Cool：the Classroom Object-Oriented Language

在这次实验过程中，我是一边看着第一次实验的 `Scann.c`文件写的。在写的过程中学习到了很多c语言编程技巧。

一开始看到实验指导书中有一个不认识的词汇“增量编程”，在网上没有查到。但是看到相近词语“增量式开发”。起始就是平常写比较复杂代码的时候一个模块一个模块的写，然后每写一个模块就验证这个模块的正确性。这样检测到错误的时候就可以快速定位到错误。整个写代码的过程就是不断创建`TODO`完成`TODO`的过程

为了完成伪代码三行，我写了5个函数来做抽象层。最终达到了实现层仅仅有三行代码。然而我觉得最复杂的就是写那个状态机。因为DFA复杂，要有很多的条件判断，同时`VScode`的每次创建`case`之后不会自动不上`break`。所以导致我有很长一段时间都没有找到“宇宙射线”。
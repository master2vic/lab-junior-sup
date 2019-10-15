# 9.1
命令如下，加-c 验证
~~~shell
python2 ./lottery.py -j 3 -s 1
python2 ./lottery.py -j 3 -s 2
python2 ./lottery.py -j 3 -s 3
~~~
一下是在我的电脑上运行出来的结果
用1的随机种子出来的结果是:
~~~shell
Here is the job list, with the run time of each job:
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )
Here is the set of random numbers you will need (at most):
Random 651593
Random 788724
Random 93859
Random 28347
Random 835765
Random 432767
Random 762280
Random 2106
Random 445387
Random 721540
Random 228762
Random 945271
~~~
求解:

随机种子2
~~~shell
Here is the job list, with the run time of each job:
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )
Here is the set of random numbers you will need (at most):
Random 605944
Random 606802
Random 581204
Random 158383
Random 430670
Random 393532
Random 723012
Random 994820
Random 949396
Random 544177
Random 444854
Random 268241
Random 35924
Random 27444
Random 464894
Random 318465
Random 380015
Random 891790
Random 525753
Random 560510
Random 236123
Random 23858
Random 325143
~~~
随机种子3
~~~
Here is the job list, with the run time of each job:
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )
Here is the set of random numbers you will need (at most):
Random 13168
Random 837469
Random 259354
Random 234331
Random 995645
Random 470263
Random 836462
Random 476353
Random 639068
Random 150616
Random 634861
~~~

根据README文件，可知，每次随机因子会安排出三个作业，根据三个作业的总票数N。对随机数取余，得到的就是这次“中彩票的人”

随机因子为1的时候：
||作业1|作业2|作业3|
|--|--|--|--|
|票段|0-83|84-108|109-152|

得到执行作业的序列：
~~~
2 0 1 2 2 2 1 1 1 1 1 1
~~~
同理，随机因子为2和3的时候
~~~
2：2 0 0 2 0 1 0 2 0 0 0 1 0 0 1 2 1 1 1 2 1 1 2
3：1 1 0 1 0 2 2 2 2 2 2
~~~
# 9.2
命令如下，加-c验证
~~~
/lottery.py -l 10:1,10:100
~~~

如此不平衡的时候工作1会有很大的几率一直赢；有一定的机率运行，但是很低，只有1/100的几率运行；者中彩票不平衡对低彩票的工作不友好

# 9.2
命令如下可以对随机种子改变
~~~
python2 ./lottery.py -s 0 -l 100:100,100:100

python2 ./lottery.py -s 1 -l 100:100,100:100

python2 ./lottery.py -s 2 -l 100:100,100:100

python2 ./lottery.py -s 3 -l 100:100,100:100
~~~

结束时间分别为：
|随机种子|0|1|2|3|
|--|--|--|--|--|
|结束时间|192:200 | 196:200 | 190:200 | 196:200|
|提早时间占比|0.04|0.04|0.05|0.02|

看到两个作业的结束时间几乎相同。看上去很公平
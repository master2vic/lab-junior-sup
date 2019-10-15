# 5.1
~~~ c++
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
int main(){
    int x = 0;
    int rc = fork();
    if(rc < 0){
        printf("failed.\n");
    }else if (rc == 0)
    {
        printf ("child x is %d \n",x);
        x = 3;
        printf ("child x has changed to %d \n",x);
    }else
    {
        x = 100;
        printf("father x is %d \n",x);
    }
    exit(1);
    return 0;
}
~~~
这是我写的代码片段，输出的结果：
~~~
father x is 100 
child x is 0
child x has changed to 3
~~~

可以看到，虽然看起来执行了父进程，然后执行子进程。并且访问了`x`。但是子进程在一开始的时候所能看到的`x`的值依旧是0.

这就说明这种值得改变，并不会跨越进程实现
# 5.2
~~~c++
ifstream input;
ofstream output;
int main(){
    input.open("./5.2.txt");
    output.open("./5_2.txt");
    int rc = fork();
    string temp;
    if(rc < 0){
        printf("failed.\n");
    }else if (rc == 0)
    {
        input >> temp;
        cout << "child.\n" << temp << '\n';
        output << "child.\n";
        exit(1);
    }else
    {
        input >> temp;
        cout << "father.\n" << temp << '\n';
        output << "father.\n";
        exit(1);
    }
    return 0;
}
~~~
输出结果：
~~~
father.
child.
hello,world.

~~~
这个时候我们看到了，只有一个进程成功的获取了这个文件的内容。打开文件"5_2.txt"发现，两个进程都可以成功的写入文本
# 5.4

execl，execlp，execle（都带“l”）的参数个数是可变的，参数以一个空指针结束。
execv、execvp和execvpe的第二个参数是一个字符串数组，新程序在启动时会把在argv数组中给定的参数传递到main

所以我们构建的c程序：
~~~c++
int main(){
    int rc = fork();
    if(rc<0)
    {
        cout << "failed.\n";
        exit(1);
    } else if(rc==0){
        cout << "child.\n" ;
        execl("/bin/ls","ls","-l",NULL);
        execle("/bin/ls","ls","-l",NULL,NULL);
        execlp("/bin/ls","ls","-l",NULL);
        exit(1);
    } else{
        cout << "father\n" ;
        char *v[] = {"ls","-l",NULL};
        execve("/bin/ls",v,NULL);
        execv("/bin/ls",v);
        execvp("/bin/ls",v);
        exit(1);
    }
    return 0;
}
~~~

其中的`v`就是我设定的一个向量(vector),用来存储 `l` 系列函数中公用的部分`"ls","-l",NULL`。

这段程序在命令行下运行的结果：
~~~
father
child.
total 20
-rwxrwxrwx 1 syy syy   425 Oct 15 16:57 5.1.c
-rwxrwxrwx 1 syy syy   652 Oct 15 17:18 5.2.cpp
total 20
-rwxrwxrwx 1 syy syy    12 Oct 15 17:07 5.2.txt
-rwxrwxrwx 1 syy syy   628 Oct 15 18:56 5.4.cpp
-rwxrwxrwx 1 syy syy   425 Oct 15 16:57 5.1.c
-rwxrwxrwx 1 syy syy    15 Oct 15 17:14 5_2.txt
-rwxrwxrwx 1 syy syy   652 Oct 15 17:18 5.2.cpp
-rwxrwxrwx 1 syy syy  3288 Oct 15 18:57 A.md
-rwxrwxrwx 1 syy syy    12 Oct 15 17:07 5.2.txt
-rwxrwxrwx 1 syy syy 13192 Oct 15 18:57 a.out
-rwxrwxrwx 1 syy syy   628 Oct 15 18:56 5.4.cpp
-rwxrwxrwx 1 syy syy    15 Oct 15 17:14 5_2.txt
-rwxrwxrwx 1 syy syy  3288 Oct 15 18:57 A.md
-rwxrwxrwx 1 syy syy 13192 Oct 15 18:57 a.out
~~~

观察到这里虽然每一条输出的都有两次，但是并不是严格的一个进程执行之后再执行另一个。同时会发现，每次编译再运行出来的结果也是不太一样的。
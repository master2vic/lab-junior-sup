#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<iostream>
using namespace std;
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
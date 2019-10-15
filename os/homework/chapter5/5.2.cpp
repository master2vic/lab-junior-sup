#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
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
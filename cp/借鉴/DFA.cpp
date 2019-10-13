#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
ifstream input;       //输入文件
ofstream output;      //输出文件
set<char>char_set;    //字符集
set<char>state_set;   //状态集
set<char>end_state;   //接受状态集
map<char,int>change;  //下标转换
char table[100][100]; //装换表
char example[100];    //合法字符集合
int id=0;             //下标
int min1=100000;      //最小状态编号
int N;                //最大长度N
int max1=0;           //最大状态编号
char start_state=0;   //开始状态
vector<string>v;      //暂存转换表
bool read(){
   input.open("./DFA_in3.dfa.txt"); //输入文件
   output.open("./DFA_out.dfa.txt"); //输出文件
    string in,tem;
    memset(table,0,sizeof(table));
    int sum=0;
    while(getline(input,in)){
        int len=in.length();
          if(sum==0){
             for(int x=0;x<len;x++){
                 if(char_set.count(in[x])!=0) return 0;    //含义重复字符，DFA不正确
                 char_set.insert(in[x]);                   //字符集
             }
             set<char>::iterator it = char_set.begin();
             while(it!=char_set.end()){                     //下标转换
                    change[*it]=id;
                    id++;
                    it++;
             }
          }
          else if(sum==1){
             for(int x=0;x<len;x++){
                 if(state_set.count(in[x])!=0) return 0;   //含有重复状态，DFA不正确
                 state_set.insert(in[x]);                  //状态集
                 min1=min1>(in[x]-'0')?in[x]-'0':min1;     //最小状态编号
                 max1=max1<(in[x]-'0')?in[x]-'0':max1;     //最大状态编号
             }
          }
          else if(sum==2){
              if((len!=1)||(state_set.count(in[0])==0)) return 0;  //开始状态不唯一或者不包含在状态集合里面，DFA不正确
                start_state=in[0];                         //开始状态
          }
          else if(sum==3){
             for(int x=0;x<len;x++){
                    //接受状态为空或者接受状态有重复或者接受状态不在状态集合里面，DFA不正确
                 if((len==0)||(end_state.count(in[x])!=0)||(state_set.count(in[x])==0)) return 0;
                 end_state.insert(in[x]);   //接受集合
              }
          }
          else{
                //状态表不满足DFA要求，DFA不正确
                if((state_set.count(in[0])==0)||(state_set.count(in[2])==0)||(char_set.count(in[1])==0)) return 0;
                 table[in[0]-'0'][change[in[1]]]=in[2];  //状态表
                 v.push_back(in);
          }
          sum++;
    }
    return 1;
}
void show(){    //打印，并写入文件
     printf("字符集：\n");
     set<char>::iterator it = char_set.begin();
     while(it!=char_set.end()){
        printf("%c ",*it);
        output<<*it;
        it++;
     }
     output<<'\n';
     printf("\n");
     printf("状态集：\n");
     it=state_set.begin();
     while(it!=state_set.end()){
        printf("%c ",*it);
        output<<*it;
        it++;
     }
     output<<'\n';
     printf("\n");
     printf("开始状态: \n");
     printf("%c",start_state);
     output<<start_state;
     output<<'\n';
     printf("\n");
     printf("接受状态集：\n");
     it=end_state.begin();
     while(it!=end_state.end()){
        printf("%c ",*it);
        output<<*it;
        it++;
     }
     output<<'\n';
     for(int x=0;x<v.size();x++){
        output<<v[x];
        output<<'\n';
     }
     printf("\n");
     printf("状态转换表：\n");
     map<char,int>::iterator it1=change.begin();
     printf(" ");
     while(it1!=change.end()){printf(" %c",it1->first);it1++;}
     printf("\n");
     it=state_set.begin();
     for(int x=min1;x<=max1;x++){
        printf("%c ",*it); it++;
        for(int y=0;y<id;y++){
            printf("%c ",table[x][y]);
        }
        printf("\n");
     }
}
bool DFA_judge(){   //判断是否能识别输入字符串
     char state=start_state;
     string temp;
     printf("请输入识别字符串：\n");
     getline(cin,temp);
     int x=0;
     while(temp[x]!='\0'){
         int id=change[temp[x]];
          if(char_set.count(temp[x])==0&&char_set.count('|')==1)
           id=change['|'];
          state=table[state-'0'][id];
          if(state==' ') return 0;
          x++;
     }
     if(end_state.find(state)!=end_state.end())
           return 1;
     return 0;
}
void DFA_show(int N,int x1,char sta){   //打印所有合法的规则字符串
     if(x1==N){
        if(end_state.count(sta)!=1) return ;
        for(int x=0;x<N;x++)
        printf("%c",example[x]);
        printf("\n");
        return ;
     }
     for(int y=0;y<id;y++){
         if(table[sta-'0'][y]!=' '){
               map<char,int>::iterator it1=change.begin();
               for(int z=0;z<y;z++) it1++;
                example[x1]=it1->first;
            DFA_show(N,x1+1,table[sta-'0'][y]);
         }
     }
}
int main(){
    if(!read()){
        printf("DFA输入不正确");return 0;
    }
    cout<<table[1][0];
    show();
    printf("输入N的大小：");
    scanf("%d",&N);
    printf("所有可能的合法字符串：\n");
    for(int x=1;x<=N;x++){
        DFA_show(x,0,start_state);
    }
    getchar();
    if(DFA_judge()){
        printf("accept!!!!\n");
    }
    else
         printf("error!!!!\n");
}

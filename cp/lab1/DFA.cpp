#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map>
#include<vector>
#include<cstring>
#include<list>
#include<stack>
using namespace std;
char table[100][100];
ifstream input;       //输入文件
ofstream output;      //输出文件
int id = 0;

struct Edge{
    int s,e; // start and end
    char cond; // condition
};

list<Edge> edgeList;

class DFA
{
private:
    char startState;
    map<char,int> indexTable;
    set<char> charactersSet;
    set<char> statesSet,endStatesSet;
    vector<string> tempT;
    int minState,maxState;
public:
    char getStartState(){
        return startState;
    }
    DFA()
    {
        minState = 1000001;
        maxState = 0;
    }
    bool typeIn(); // input DFA by keyboard
    void storage(); // storage the DFA to the file named fileName
    bool readFile(); // read from the file named string fileName
    bool check();// the Correctness test of DFA
    void showWordList(int N); // display every possible word, which not longer than N
    void wordCheck(string );  //Judgment of legality
    void show(int N,int x1,char sta);
    void feedback();
};

bool DFA::readFile(){
    memset(table,0,sizeof(table));

    string temp;
    getline(input,temp);
    int len=temp.length();
    for(int x=0; x<len; x++)
    {
        if(charactersSet.count(temp[x])!=0)
            return 0;
        charactersSet.insert(temp[x]);
    }
    set<char>::iterator it = charactersSet.begin();
    while(it!=charactersSet.end())
    {
        indexTable[*it]=id;
        id++;
        it++;
    }

    getline(input,temp);
    len=temp.length();
    for(int i=0; i<len; i++)
    {
        if(statesSet.count(temp[i])!=0)
            return 0;
        statesSet.insert(temp[i]);
        minState=minState>(temp[i]-'0')?temp[i]-'0':minState;
        maxState=maxState<(temp[i]-'0')?temp[i]-'0':maxState;
    }
    getline(input,temp);
    len=temp.length();
    if((len!=1)||(statesSet.count(temp[0])==0))
        return 0;
    startState=temp[0];

    getline(input,temp);
    len=temp.length();
    for(int x=0; x<len; x++)
    {
        if((len==0)||(endStatesSet.count(temp[x])!=0)||(statesSet.count(temp[x])==0))
            return 0;
        endStatesSet.insert(temp[x]);
    }

    int n = statesSet.size();
    vector<string> chain[n];

    //1. 把读入的数转换位 n位的 bit 位
    for (int x = 0; x < n;x ++){
        int hexIn ,copyH ;
        input >> hex >> hexIn;
        copyH =hexIn;
# define debug1
#ifdef debug1
cout << "___:\n";
cout << "hex IN :" << hexIn << '\n';
#endif // debug1
        string tempInput;
        stack<string> s;
        for(int i = 0;i < n;i ++){
//cout << (hexIn&1) ;
            if (hexIn & 1 == 1){
                input >> tempInput;
                s.push(tempInput);
//cout << tempInput ;
                //chain[x].push_back(tempInput);
                //chain[x].insert(chain[x].begin(),tempInput);
            }
            else {
                //chain[x].insert(chain[x].begin(),"");
                //chain[x].push_back("");
            }
//cout <<  ' ';
            hexIn = hexIn >> 1;
        }

        for(int i = 0;i < n;i ++){
cout << (copyH&1) ;
            if (copyH & 1 == 1){

cout << s.top() ;
                //chain[x].push_back(tempInput);
                chain[x].insert(chain[x].begin(),s.top());
                s.pop();
            }
            else {
                chain[x].insert(chain[x].begin(),"");
                //chain[x].push_back("");
            }
cout <<  ' ';
            copyH = copyH >> 1;
        }

cout << '\n';
        for(int y = 0;y < n;y ++){
            if(chain[x][y] != ""){
                int num = chain[x][y].length();
                for(int i = 0;i < num;i ++){
cout << x << " "<< indexTable[chain[x][y][i]] << ' ' << y << '\n';
                    table[x+minState][indexTable[chain[x][y][i]]] = '0'+y + minState;

                }
            }
        }
cout << "----\n";
    }
    return 1;

}

bool DFA::typeIn()
{
    memset(table,0,sizeof(table));
    string temp;
    cout << "Please type the set of characters by letters.And do not SEPARATE by spaces.\n";
    getline(cin,temp);
    int len=temp.length();
    for(int x=0; x<len; x++)
    {
        if(charactersSet.count(temp[x])!=0)
            return 0;    //Some characters appear repeatedly
        charactersSet.insert(temp[x]);
    }
    set<char>::iterator it = charactersSet.begin();
    while(it!=charactersSet.end())
    {
        indexTable[*it]=id;
        id++;
        it++;
    }

    cout << "Please type the set of states by numbers.And do not SEPARATE by spaces.\n";
    getline(cin,temp);
    len=temp.length();
    for(int i=0; i<len; i++)
    {
        if(statesSet.count(temp[i])!=0) // repeat
            return 0;
        statesSet.insert(temp[i]);
        minState=minState>(temp[i]-'0')?temp[i]-'0':minState;
        maxState=maxState<(temp[i]-'0')?temp[i]-'0':maxState;
    }

    cout << "Please type the set of start states by numbers.And do not SEPARATE by spaces.\n";
    getline(cin,temp);
    len=temp.length();
    if((len!=1)||(statesSet.count(temp[0])==0))
        return 0;
    startState=temp[0];

    cout << "Please type the set of end states by numbers.And do not SEPARATE by spaces.\n";
    getline(cin,temp);
    len=temp.length();
    for(int x=0; x<len; x++)
    {
        if((len==0)||(endStatesSet.count(temp[x])!=0)||(statesSet.count(temp[x])==0))
            return 0;
        endStatesSet.insert(temp[x]);
    }

    // type in the edges of the DFA
    cout << "please type the edges of DFA using the pattern of \' 0a1 \', which means a edge from 0 to 1 when it gets a \' a \'. It will end when you type a \' -1 \' \n";
    string tempStr;
    int counter = 0;
    while(1)
    {
        getline(cin,temp);
        len=temp.length();
        if(temp == "-1")
            break;
        if((statesSet.count(temp[0])==0)||(statesSet.count(temp[2])==0)||(charactersSet.count(temp[1])==0))
            return 0;
        table[temp[0]-'0'][indexTable[temp[1]]]=temp[2];  //状态表
        tempT.push_back(temp);
    }
    return 1;
}

void DFA::feedback()     //打印，并写入文件
{
    printf("characters set\n");
    set<char>::iterator it = charactersSet.begin();
    while(it!=charactersSet.end())
    {
        printf("%c ",*it);
        output<<*it;
        it++;
    }
    output<<'\n';
    printf("\n");
    printf("state set\n");
    it=statesSet.begin();
    while(it!=statesSet.end())
    {
        printf("%c ",*it);
        output<<*it;
        it++;
    }
    output<<'\n';
    printf("\n");
    printf("start state set \n");
    printf("%c",startState);
    output<<startState;
    output<<'\n';
    printf("\n");
    printf("end state set\n");
    it=endStatesSet.begin();
    while(it!=endStatesSet.end())
    {
        printf("%c ",*it);
        output<<*it;
        it++;
    }
    output<<'\n';
    for(int x=0; x<tempT.size(); x++)
    {
        output<<tempT[x];
        output<<'\n';
    }
    printf("\n");
    printf("state transform set\n");
    map<char,int>::iterator it1=indexTable.begin();
    printf(" ");
    while(it1!=indexTable.end())
    {
        printf(" %c",it1->first);
        it1++;
    }
    printf("\n");
    it=statesSet.begin();
    for(int x=minState; x<=maxState; x++)
    {
        printf("%c ",*it);
        it++;
        for(int y=0; y<id; y++)
        {
            printf("%c ",table[x][y]);
        }
        printf("\n");
    }
}

bool DFA::check()
{
    char state=startState;
    string temp;
    cout << "please type in the word :\n";
    getline(cin,temp);
    int x=0;
    while(temp[x]!='\0')
    {
        int id=indexTable[temp[x]];
        if(charactersSet.count(temp[x])==0&&charactersSet.count('|')==1)
            id=indexTable['|'];
        state=table[state-'0'][id];
        if(state==' ')
            return 0;
        x++;
    }
    if(endStatesSet.find(state)!=endStatesSet.end())
        return 1;
    return 0;
}
char example[100];
void DFA::show(int N,int x1,char sta)
{
    if(x1==N)
    {
        if(endStatesSet.count(sta)!=1)
            return ;
        for(int x=0; x<N; x++)
            cout << example[x];
        cout << '\n';
        return ;
    }
    for(int y=0; y<id; y++)
    {
        if(table[sta-'0'][y]!=' ')
        {
            map<char,int>::iterator it1=indexTable.begin();
            for(int z=0; z<y; z++)
                it1++;
            example[x1]=it1->first;
            show(N,x1+1,table[sta-'0'][y]);
        }
    }
}


int main()
{
    char inputFileName[] = "./DFA_in3.dfa";
    char outputFileName[] = "./DFA_out2.dfa";
    char storeFileName[] = "./DFA.dfa";
    input.open(inputFileName);
    output.open(outputFileName);
    DFA dfa;
    int mode =0 ;
    cout << "type 1 for type, and type 2 for read for .dfa \n";
    cin >> mode;
    getchar();
    if (mode == 1){
        if(!dfa.typeIn())
        {
            cout << "Error. Check your DFA. \n";
        }
    }
    else if(mode == 2)
        dfa.readFile();
    else {
        return 0;
    }
    cout << table[1][0];
    dfa.feedback();
    cout << "type in the length of the word : ";
    int n = 0 ;
    cin >> n;
    char start = dfa.getStartState();
    getchar();
    for(int i = 0;i <= n;i ++){
        dfa.show(i,0,start);
    }
    dfa.check() ? cout << "Legal." : cout << "Invalid.";

    return 0;
}

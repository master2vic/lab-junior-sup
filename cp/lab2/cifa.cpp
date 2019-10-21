#include<iostream>
#include<fstream>
#include<string.h>
#include<assert.h>
using namespace std;

/* for debug */
// #define debug1
// #define debug2
//  #define debug3

// declare the functions
int next();
void getToken();
void getNewLine();
bool openFile();

// declare the global variable
static int Line = 0;
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static bool EOF_flag = false; /* corrects ungetNextChar behavior on EOF */
static bool lineEnd = false;
string tokenString = "";/* lexeme of identifier or reserved word */
ifstream inputFile;
ofstream outputFile;

typedef enum{
    // EOF , ERROR
    ENDFILE,ERROR,
    // Key word
    CLASS,ELSE,FALSE,FI,IF,IN,INHERITS,ISVOID,LET,LOOP,POOL,THEN,WHILE,CASE,ESAC,NEW,OF,NOT,TRUE,
    // ID , NUMBER
    ID,NUM,
    // single-character Operator + - * / = ; , ( ) < > { } : " <- <= => @ ~
    ADD,SUB,MUL,DIV,EQ,SEM,COMMA,LB,RB,LAB,RAB,LBB,RBB,COLON,QUO,LEFTARROW,LA,RA,ALTA,XI
}TokenType;

typedef enum{
    // start & done
    START,DONE,
    // number , object type , type , string
    INNUM,INOBJECT,INTYPE,INSTRING,
    // negative , comma line , less , common operator ,
    INNEGATIVE,INCOM_LINE,INLESS,INCOM_OP,INEQ,
    // left kuohao , comma lines , end comma
    INL_KUO,IN_COM,INPRE_END
}State;

bool num(char a){  // is number ?
	if(a>='0'&&a<='9')
        return true;
	return false;
}

bool alpha(char a){  // a~z A~Z _ 0~9
	if((a>='a'&&a<='z')||(a>='A'&&a<='Z') || (a == '_') || (a >= '0' && a <= '9'))
        return true ;
	return false ;
}

bool sup_c(char c){ // sup character{
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

bool sub_c(char c) // sub character
{
    if (c >= 'a' && c <= 'z')
        return true;
    return false;
}

void PrintToken(TokenType token,const string TokenString){
	if(TokenString!="\0")
        cout<<"\t"<<Line<<": ";
	switch(token)
    {
        case CLASS:
        case ELSE:
        case FALSE:
        case FI:
        case IF:
        case IN:
        case INHERITS:
        case ISVOID:
        case LOOP:
        case LET:
        case POOL:
        case THEN:
        case WHILE:
        case CASE:
        case ESAC:
        case NEW:
        case OF:
        case NOT:
        case TRUE:
        cout<<"reserved word,"<<TokenString<<endl;   //����Ԥ���ʶ��ǵ�����״̬
		break;

        case ADD: printf("+\n");  break;
        case SUB: printf("-\n");  break;
        case MUL: printf("*\n");  break;
        case DIV: printf("/\n");  break;
        case EQ:  printf("=\n");  break;
        case SEM: printf(";\n");  break;
        case COMMA: printf(",\n");break;
        case LB:  printf("(\n");  break;
        case RB:  printf(")\n");  break;
        case LAB:  printf("<\n"); break;
        case RAB:  printf(">\n"); break;
        case LBB: printf("{\n") ; break;
        case RBB: printf("}\n") ; break;
        case COLON :printf(":\n");break;
        case QUO :printf(":\n");  break;
        case LEFTARROW : printf("<-\n");break;
        case LA : printf("<=\n");break;
        case RA : printf("=>\n");break;
        case ALTA:printf("@\n");break;
        case XI : printf("~\n");break;

        case ENDFILE: if(TokenString!="\0") cout<<"COMMENT"<<TokenString<<endl;break;   
		case NUM:
	        cout<<"NUM, val="<<TokenString<<endl; break;
        case ID:
	        cout<<"ID, name="<<TokenString<<endl; break;
        case ERROR: // 
	        cout<<"ERROR unexpected token \""<<TokenString<<"\"\n"; break;
	}
}

TokenType Find(string a){
	if(a=="if")     return IF;
	else if(a=="else")   return ELSE;
	else if(a=="while")  return WHILE;
    else if(a=="class")  return CLASS;
    else if(a=="false")  return FALSE;
    else if(a=="fi")     return FI;
    else if(a=="if")     return IF;
    else if(a=="in")     return IN;
    else if(a=="inherits")return INHERITS;
    else if(a=="isvoid")  return ISVOID;
    else if(a=="let")     return LET;
    else if(a=="loop")   return LOOP;
    else if(a=="pool")   return POOL;
    else if(a=="then")   return THEN;
    else if(a=="case")   return CASE;
    else if(a=="esac")   return ESAC;
    else if(a=="new")    return NEW;
    else if(a=="of")     return OF;
    else if(a=="not")    return NOT;
    else if(a=="true")   return TRUE;
	else return ID;
}

void last(){
    if(!EOF_flag) linepos --;
}

int next(){
    if(linepos < bufsize){
        return lineBuf[linepos++];
    }else{
        lineEnd = true;
        return 0;
    }
}

bool single(char c){
    if(c == '~' || c == '+' || c == ')' || c == '/' || c == '>' || c == ';' || c == ',' || c == ':' || c == '{' || c == '}' || c == '@')
        return true;
    return false;
}

TokenType opFind(char c){
    TokenType ret ;
    switch (c)
    {
    case '~': ret = XI;break;
    case '+': ret =ADD;break;
    case ')': ret =RB;break;
    case '/': ret =DIV;break;
    case '>': ret =RAB;break;
    case ';': ret =SEM;break;
    case ',': ret =COMMA;break;
    case ':': ret =COLON;break;
    case '{': ret =LBB;break;
    case '}': ret =RBB;break;
    case '@': ret =ALTA;break;
    default:
        break;
    }
    return ret;
}

static bool multi_com_flag = false;

void getToken(){
    /* index for storing into tokenString */
    int tokenStringIndex = 0;
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state - always begins at START */
    State state = START;
    /* flag to indicate save to tokenString */
    int save;
    int c;

    if(multi_com_flag)
        state = IN_COM;

    while (1)
    {
        c = next();
        save = true;

        switch (state)
        {
        case START:
            if(c == '('){
                state = INL_KUO;
            } else if(num(c)){
                state = INNUM;
                currentToken = NUM;
            } else if(sup_c(c)) {
                state = INTYPE;
                currentToken = ID;
            } else if(sub_c(c)){
                state = INOBJECT;
                currentToken = ID;
            } else if(c == '\0'){
                state = START ;
                save = false ;
            } else if(c == '\"'){
                state = INSTRING;
                currentToken = ID;
            } else if(c == '-'){
                currentToken = SUB;
                state = INNEGATIVE;
            } else if(c == '<'){
                state = INLESS;
                currentToken = LAB;
            } else if(c == '='){
                state = INEQ;
            } else if(single(c)){
                state = DONE;
                currentToken = opFind(c);
            } else if( c == ' ' || c == '\t' || c == '\n'){
                state = START;
                save = false;
            }
            else {
                state = DONE;
                currentToken = ERROR;
            }
            break;
        case INEQ:
            if(c == '>'){
                state = DONE;
                currentToken = RA;
            }else
            {
                last();
                state = DONE;
                currentToken = EQ;
            }
            
            break;
        case INLESS :
            if(c == '-'){
                currentToken = LEFTARROW;
                state = DONE;
            }
            else if (c == '=')
            {
                currentToken = LA;
                state = DONE;
            }
            else
            {
                currentToken = SUB;
                last();
                save = false;
                state = DONE;
            }
            
            break;
        case INSTRING :
            if(c == '\"'){
                state = DONE;
                currentToken = ID;
            }
            break;
        case INNEGATIVE :
            if(c == '-'){
                state = INCOM_LINE;
                lineEnd = true;
                cout << '\n';
                return ;
            }else {
                currentToken = SUB;
                last();
                state = DONE;
                save = false;
            }
            break;
        case INTYPE:
            if(alpha(c)){
                state = INTYPE;
                currentToken = ID;
            } else {
                last();
                save = false;
                state = DONE;
            }
            break;
        case INOBJECT :
            if (alpha(c)){
                state = INOBJECT;
                currentToken = ID;
            }else {
                last();
                save = false;
                state = DONE;
            }
            break;
        case INNUM:
            if (num(c)){
                state = INNUM;
                currentToken = NUM;
            }
            else{
                save = false;
                state = DONE;
                last();
            }
            break;
        case INL_KUO: // 左括号 要判断是 multi ，还是下一个
            if(c != '*'){
                last();
                save = false;
                state = DONE;
                currentToken = LB;
            } else if(c == '*'){
                state = IN_COM;
                save = false;
                multi_com_flag = true;
            }
            break;
        case IN_COM:
            if (c == '*')
            {
                state = INPRE_END;
                save = false;
            } else
            {
                state = IN_COM;
                save = false;
            }
            break;
        case INPRE_END:
            if(c == '*'){
                state = INPRE_END;
                save = false;
            } else if(c == ')') {
                state = START;
                multi_com_flag = false;
                save = false;
            } else {
                state = IN_COM;
                save = false;
            }
            break;
        case DONE:
        default:
            save = false;
            state = DONE;
            currentToken = ERROR;
            tokenString = "or UNKNOWNED ERROR!";
            break;
        }

        if(save){
            tokenString += (char) c;
        }

#ifdef debug3
cout << "  state:" << state << "\n  save:" << save <<"\n"; 
cout << "  str  :\"" <<tokenString << "\"\n";
#endif // debug3
        if(state == DONE){
            if(currentToken == ID){
                currentToken = Find(tokenString);
            }

            PrintToken(currentToken,tokenString);
            tokenString = "";
            state = START;
        }
        if (lineEnd){
            cout << "\n";
            break;
        }
    }
}

void getNewLine(){
    if(inputFile.eof()){
        EOF_flag = true;
        return ;
    }
    Line ++ ;
    linepos = 0;
    inputFile.getline(lineBuf,256);
    bufsize = strlen(lineBuf);
    lineEnd = false;
    tokenString = "";
    cout << "line " << Line << " :\t\"" << lineBuf << "\", length:"<<bufsize<<"\n";
    return ;
}

bool openFile(){
    ifstream getFileName;
    string fileName = "";
    getFileName.open("fileNameFile.txt");
    getFileName >> fileName;
    inputFile.open(fileName.c_str());
    outputFile.open("aim.txt");
    return (inputFile.is_open() && outputFile.is_open());
}

void closeFile(){
    inputFile.close();
    outputFile.close();
}

int main(int argc,char* argv[]){
    if(openFile()){
        while(1){
            getNewLine();

#ifdef debug1
cout << "Line :" << Line << '\n';
cout << "\tbuffer size :" << bufsize << "\n\tline buffer :  " << lineBuf;
cout << "\n\tEOF flag : " << EOF_flag;
cout << "\n\tposition:" << linepos << '\n';
#endif // debug1
            if (EOF_flag)
                break;
            getToken();
        }
    }else{
        cout << " Open File Failed. Try again. \n";
    }
    closeFile();

	return 0;
}

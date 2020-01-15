#include<iostream>
#include<fstream>
#include <string>

using namespace std;


// THIS IS ALL THE TOKENS WE HAVE ON OUR LANGUAGE
//-----------------------------------------------
enum TokenType 
{
    STARTING,
	START_PROGRAM,
	END_PROGRAM,
	COMMENT,
    EXP,
	END_INSTRUCTION,
	RESERVED_WORD,
	STRING,
	INTEGER,
	FLOAT,
	IDENT,
	GIVE,
	COLON,
	DOUBLE_SEMICOLON,
	COMMA,
	AFFECT,
	RESERVED_CHARACHTER,
	IF,
	BIGGER,
	SMALLER,
	EQUAL,
	NOT_EQUAL,
	ELSE,
	START,
	FINISH,
	SHOW_MES,
	SHOW_VAL,
	TO,
	END_FILE,
	LTE,
	GTE,
	ERROR,
    INTEGER_NUMBER,
    SEQ,
    FLOAT_NUMBER,
    DOUBLE_MINUS
	};


// THIS IS ALL NODES WE NEED FOR PARSING
//-----------------------------------------------
enum NodeType
{
	START_N,
	PROGRAM_N,
	START_PROGRAM_N,
	COMMENT_N,
    END_PROGRAM_N,
	END_INSTRUCTION_N,
	STRING_N,
	INTEGER_N,
    SEQ_N,
    STARTING_N,
	FLOAT_N,
	IDENT_N,
    DOUBLE_MINUS_N,
	GIVE_N,
	COLON_N,
	SEMICOLON_N,
	COMMA_N,
	AFFECT_N,
	IF_N,
	BIGGER_N,
	SMALLER_N,
	EQUAL_N,
	NOT_EQUAL_N,
	ELSE_N,
	FINISH_N,
	SHOW_MES_N,
	SHOW_VAL_N,
	TO_N,
	END_FILE_N,
	LTE_N,
    DOUBLE_SEMICOLON_N,
	GTE_N,
	ERROR_N,
	NUMBER_N,
    TYPE_N,
    INTEGER_NUMBER_N,
    FLOAT_NUMBER_N,
    IDENT_LIST_N,
    IDENT_LIST_TAIL_N,
    COMMAND_N,
    TYPE_NUMBER_N,
    EXP_N,
    COMMAND_TAIL_N
};

// EVERY TOKEN HAS A NAME AND A TYPE
//----------------------------------
class Token {
public:
	TokenType type;
	string name;
	Token() {
		type = ERROR;
		name = "";
	}
};

//EVERY NODE HAS A TYPE AND A NAME{LEXEM}{TOKEN} AND CHILDES HERE WE HAVE 6 CHILDES BECAUSE 
// OUR GRAMMER HAS THE LONGEST TRANSITION WITH FIVE TOKENS SOO 6 NODES {CHLIDES}
// THE LONGEST TRANSACTION : <command>--> If -- <exp> -- <command> <commandTail>
//                                       [1] [2] [3]  [4]   [5]      [6]
//*********************************************************************************
class Node {
public:
	NodeType type;
	string Lexeme;
	Node* child[6];		// array of pointers (6 pointers)
	Node() {
		for (int i = 0; i < 6; i++) {
			child[i] = NULL;
		}
	}
	~Node() {
		for (int i = 0; i < 6; i++)
			delete child[i];
	}
};	

//*****************************************
/**/             Node* root;			 /* pointer that pointing to the root node to have the possibility to print and retrieve data from all other nodes  */
//*****************************************

class Parser 
{
private:
	fstream f;
	Token currentToken;				//currentToken has a token class --> (type,name)
	Token getToken();               //take a sequence of charachters and check is if it's a reserved charachter
    Token checkReserved(Token t);   //an function that helps the getToken() function to check
    void match(TokenType type);     // if the token readed from the file matches one of the token types
    void syntaxError(Token t);      // display that the token is not recognized or not in the right place
    Node* programe();
    Node* seq();
    Node* seqTail();
    Node* typeTest();
    Node* type();
    Node* identList();
    Node* identListTail();
    Node* command();
    Node* typeNumber();
    Node* exp();
    Node* commandTail();
    Node* op();
 
public:
    Parser(string fileName) {
        f.open(fileName);
        if (!f)
        {
            cout << "Unable to open file" << endl; //system("pause");
            //exit(1);
        }
    }
    string program[100];
    int index = 0;
    ~Parser() {
        f.close();
    }
    
    void  sampleParser() {
        Node* ptr = new Node();									//STARTING FUNCTION
        root = ptr;

        ptr->type = STARTING_N;
        currentToken = getToken();
        ptr->child[0] = programe();
        if (currentToken.type == END_FILE) {
            ptr->child[1] = new Node();
            ptr->child[1]->Lexeme = currentToken.name;
        }
        match(END_FILE);
    }

    //******************************************
    void displayTree(Node* Tree) {		//Tree = root
        if (Tree) {
            cout <<Tree->Lexeme<<"";
            program[index]= Tree->Lexeme;
            //cout << program[index];
            index++;
            if (Tree->Lexeme == ";;" || Tree->Lexeme == "Start_Program" || Tree->Lexeme == "End_Program"){cout << endl;}
            displayTree(Tree->child[0]);
            displayTree(Tree->child[1]);
            displayTree(Tree->child[2]);
            displayTree(Tree->child[3]);
            displayTree(Tree->child[4]);
            displayTree(Tree->child[5]);
        }
    }
    
    void displaySymantic()
    {
        string s=" ";
        int i = 0;
        while(i < 100)
        {
            if (program[i]=="Start_Program")
            {
                cout <<"\n"+ program[i]+" -----> Debut de program" << endl;
            }
            else if (program[i] == "Int_Number")
            {   
                cout << program[i];
                int j = i+1;
                while (program[j] != ";;")
                {
                    
                    s += program[j];
                    j++;
                }
                cout << " " + s + " ;; ----->  Declaration des variables \n";
                i = j;
            }
            else if (program[i] == "End_Program")
            {
                cout << program[i] + "-----> Fin de program" << endl;
            }
            i++;
        }
    }
};


Token Parser::getToken()
{
        char ch;
        Token t;
        f.get(ch);
        while (isspace(ch)) {
            f.get(ch);
            if (f.eof())
            {
                t.name = "$$";
                t.type = TokenType::END_FILE;
                return t;
            }
        }
        if (f.eof()) 
        {
            t.name = "$$";
            t.type = TokenType::END_FILE;
        }
        else if (ch == ';') 
        { 
            t.name = ch;  
            f.get(ch);
            if (ch == ';')   // ";;" FOR THE END OF INSTRUCTION
            {
                t.name += ch;
                t.type = TokenType::DOUBLE_SEMICOLON;
            }
            else
            {
                f.putback(ch);
                t.name += "ERROR!";
                t.type = TokenType::ERROR;
            }
        }
        else if (ch == '=') 
        { 
            t.name = ch; 
            f.get(ch);
            if (ch == '=')   // "==" for condition if
            {
                t.name += ch;
                t.type = TokenType::EQUAL;
            }
            else
            {
                f.putback(ch);
                t.name += "ERROR!";
                t.type = TokenType::ERROR;
            }
        }
        else if (ch == '-')
        {
            t.name = ch;
            f.get(ch);
            if (ch == '-')   // "--" for condition if
            {
                t.name += ch;
                t.type = TokenType::DOUBLE_MINUS;
            }
            else
            {
                f.putback(ch);
                t.name += "ERROR!";
                t.type = TokenType::ERROR;
            }
        }
        else if (ch == ',') { t.name = ch; t.type = TokenType::COMMA; }
        else if (ch == '<')
        {
            t.name = ch;
            f.get(ch);
            if (ch == '>') {
                t.name += ch;		//t.name="<>";
                t.type = TokenType::NOT_EQUAL;
            }
            else if (ch == '=') {
                t.name += ch;		//t.name = "<=";
                t.type = TokenType::LTE;
            }
            else {
                f.putback(ch);
                t.type = TokenType::SMALLER;	//t.name = "<";		return t >> t(name="<" , type=LESS)
            }
        }
        else if (ch == '>')
        {
            t.name = ch;
            f.get(ch);
            if (ch == '=') {
                t.name += ch;
                t.type = TokenType::GTE;
            }
            else {
                f.putback(ch);
                t.type = TokenType::BIGGER;
            }
        }
        else if (ch == ':') 
        {
                t.name += ch;
                t.type = TokenType::COLON;
        }
        else if (isalpha(ch))
        {
            t.name += ch;
            f.get(ch);
            while (isalpha(ch) || ch == '_' || isdigit(ch))
            {
                t.name += ch;
                f.get(ch);
            }
            f.putback(ch);
            return checkReserved(t);
        }
        else if (isdigit(ch))
        {
            t.name += ch;
            f.get(ch);
            while (isdigit(ch) || ch == '.')
            {
                t.name += ch;
                f.get(ch);
            }
            f.putback(ch);
            t.type = TokenType::FLOAT_NUMBER;
        }
        else if (isalpha(ch))
        {
            t.name = ch;
            f.get(ch);
            while (isalnum(ch))
            {
                t.name += ch;
                f.get(ch);
            }
            f.putback(ch);
            return checkReserved(t);
        }
        else if (isdigit(ch))
        {
            t.name = ch;
            f.get(ch);
            while (isdigit(ch))
            {
                t.name += ch;
                f.get(ch);
            }
            f.putback(ch);
            t.type = TokenType::INTEGER_NUMBER;
        }
        else {
            t.name = "ERROR !";
            t.type = ERROR;
        }
        //must be executed
        return t;
}

Token Parser::checkReserved(Token t)
{
    if (t.name == "Start_Program")    t.type = TokenType::START_PROGRAM;
    else if (t.name == "Int_Number")  t.type = TokenType::INTEGER;
    else if (t.name == "Give")        t.type = TokenType::GIVE;
    else if (t.name == "Real_Number") t.type = TokenType::FLOAT;
    else if (t.name == "if")          t.type = TokenType::IF;
    else if (t.name == "Affect")      t.type = TokenType::AFFECT;
    else if (t.name == "to")          t.type = TokenType::TO;
    else if (t.name == "ShowMes")     t.type = TokenType::SHOW_MES;
    else if (t.name == "ShowVal")     t.type = TokenType::SHOW_VAL;
    else if (t.name == "Start")       t.type = TokenType::START;
    else if (t.name == "Finish")      t.type = TokenType::FINISH;
    else if (t.name == "Else")        t.type = TokenType::ELSE;
    else if (t.name == "String")      t.type = TokenType::STRING;
    else if (t.name == "End_Program") t.type = TokenType::END_PROGRAM;
    else                              t.type = TokenType::IDENT;
    return t;
}

void Parser::match(TokenType type)
{
    if (currentToken.type == type) {
        cout << currentToken.name << "-----> Position Matched\n";			// the word is matched
    }
    else {
        syntaxError(currentToken);
    }
    if (type != END_FILE)
        currentToken = getToken();
}

void Parser::syntaxError(Token t) {
    cout << t.name << "----> Token not expected in this position\n";		            	//word is not expected
}

// for the first transaction : <programe> --> Start_Program <seq> End_Program
Node* Parser::programe()
{
    Node* ptr = new Node();   // creating the node program
    ptr->type = PROGRAM_N;

    if (currentToken.type == START_PROGRAM) {  // creating the first node childe if it's a start program
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = START_PROGRAM_N;
    }
    match(START_PROGRAM);

    ptr->child[1] = seq();    //creating the second child seq

    if (currentToken.type == END_PROGRAM) { // creating the third child
        ptr->child[2] = new Node();
        ptr->child[2]->Lexeme = currentToken.name;
        ptr->child[2]->type = END_PROGRAM_N;
    }
    match(END_PROGRAM);
    return ptr;
}

// for the seq transaction : <seq> --> E | <type>:<identList>;;<seqTail> 
//                                       | <command><seqTail> 
Node* Parser::seq()
{
    Node* ptr = new Node();
    if (currentToken.name == "Int_Number")
    {
        ptr->type = SEQ_N;
        ptr->child[0] = type();
        if (currentToken.type == COLON)
        {
            ptr->child[1] = new Node();
            ptr->child[1]->Lexeme = currentToken.name;
            ptr->child[1]->type = COLON_N;
        }
        match(COLON);
        ptr->child[2] = identList();
        if (currentToken.type == DOUBLE_SEMICOLON)
        {
            ptr->child[3] = new Node();
            ptr->child[3]->Lexeme = currentToken.name;
            ptr->child[3]->type = DOUBLE_SEMICOLON_N;
        }
        match(DOUBLE_SEMICOLON);
    }
        return ptr;
}

/*Node* Parser::seqTail()
{
}*/

// for type transaction : <type>--> Int_number | real_number
Node* Parser::type()
{
    Node* ptr = new Node();
    ptr->type = TYPE_N;
    switch (currentToken.type) {
    case INTEGER:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = INTEGER_N;
        match(INTEGER);
        break;
    case FLOAT:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = FLOAT_N;
        match(FLOAT);
        break;
    default:
        syntaxError(currentToken);
    }
    return ptr;
}

// for identList transaction : <identList>--> Ident <identListTail>
Node* Parser::identList()
{
    Node* ptr = new Node();
    ptr->type = IDENT_LIST_N;
    if (currentToken.type == IDENT)
    {
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = IDENT_N;
    }
    match(IDENT);
    ptr->child[1] = identListTail();
    return ptr;
}

//for identListTail : <identListTail>--> E | , <identList>
Node* Parser::identListTail()
{
    Node* ptr = new Node();
    ptr->type = IDENT_LIST_TAIL_N;
    if (currentToken.type == COMMA)
    {
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = COMMA_N;
        match(COMMA);
        ptr->child[1] = identList();
    }
    return ptr;
}

// for command transaction : <command>--> Give Ident : <typeNumber> ;; 
//                                      | ShowMes : String ;; 
//                                      | ShowVal : Ident ;; 
//                                      | comment 
//                                      | If -- <exp> -- <command> <commandTail>  
Node* Parser::command()
{
    Node* ptr = new Node();
    ptr->type = COMMAND_N;
    switch (currentToken.type) {

    case GIVE:  // Give Ident : <typeNumber> ;;
        ptr->child[0] = new Node();
        ptr->child[0]->type = GIVE_N;
        ptr->child[0]->Lexeme = currentToken.name;
        match(GIVE);
        if (currentToken.type == IDENT) {
            ptr->child[1] = new Node();
            ptr->child[1]->type = IDENT_N;
            ptr->child[1]->Lexeme = currentToken.name;
        }
        match(IDENT);
        if (currentToken.type == COLON) {
            ptr->child[2] = new Node();
            ptr->child[2]->type = COLON_N;
            ptr->child[2]->Lexeme = currentToken.name;
        }
        match(COLON);
        ptr->child[3] = typeNumber();
        if (currentToken.type == DOUBLE_SEMICOLON) {
            ptr->child[4] = new Node();
            ptr->child[4]->type = DOUBLE_SEMICOLON_N;
            ptr->child[4]->Lexeme = currentToken.name;
        }
        match(DOUBLE_SEMICOLON);
        break;


    case SHOW_MES:  // ShowMes : String ;;
        ptr->child[0] = new Node();
        ptr->child[0]->type = SHOW_MES_N;
        ptr->child[0]->Lexeme = currentToken.name;
        match(SHOW_MES);
        if (currentToken.type == COLON) {
            ptr->child[1] = new Node();
            ptr->child[1]->type = COLON_N;
            ptr->child[1]->Lexeme = currentToken.name;
        }
        match(COLON);
        if (currentToken.type == STRING) {
            ptr->child[2] = new Node();
            ptr->child[2]->type = STRING_N;
            ptr->child[2]->Lexeme = currentToken.name;
        }
        match(STRING);
        if (currentToken.type == DOUBLE_SEMICOLON) {
            ptr->child[3] = new Node();
            ptr->child[3]->type = DOUBLE_SEMICOLON_N;
            ptr->child[3]->Lexeme = currentToken.name;
        }
        match(DOUBLE_SEMICOLON);
        break;


    case SHOW_VAL:  // ShowVal: Ident;;
        ptr->child[0] = new Node();
        ptr->child[0]->type = SHOW_VAL_N;
        ptr->child[0]->Lexeme = currentToken.name;
        match(SHOW_VAL);
        if (currentToken.type == COLON) {
            ptr->child[1] = new Node();
            ptr->child[1]->type = COLON_N;
            ptr->child[1]->Lexeme = currentToken.name;
        }
        match(COLON);
        if (currentToken.type == IDENT) {
            ptr->child[2] = new Node();
            ptr->child[2]->type = IDENT_N;
            ptr->child[2]->Lexeme = currentToken.name;
        }
        match(IDENT);
        break;

    case COMMENT:   //  comment
        ptr->child[0] = new Node();
        ptr->child[0]->type = COMMENT_N;
        ptr->child[0]->Lexeme = currentToken.name;
        match(COMMENT);
        break;

    case IF:        //  If -- <exp> -- <command> <commandTail>
        
        if (currentToken.type == IF) {
            ptr->child[0] = new Node();
            ptr->child[0]->type = IF_N;
            ptr->child[0]->Lexeme = currentToken.name;
        }
        match(IF);
        if (currentToken.type == DOUBLE_MINUS) {
            ptr->child[1] = new Node();
            ptr->child[1]->type = DOUBLE_MINUS_N;
            ptr->child[1]->Lexeme = currentToken.name;
        }
        match(DOUBLE_MINUS);
        ptr->child[2] = exp();
        if (currentToken.type == DOUBLE_MINUS) {
            ptr->child[3] = new Node();
            ptr->child[3]->type = DOUBLE_MINUS_N;
            ptr->child[3]->Lexeme = currentToken.name;
        }
        match(DOUBLE_MINUS);
        ptr->child[4] = command();
        ptr->child[5] = commandTail();
        break;
    default: syntaxError(currentToken);
    }
    return ptr;
}

// transaction for typeNumber : <TypeNUMBER>--> integer | float
Node* Parser::typeNumber()
{
    Node* ptr = new Node();
    ptr->type = TYPE_N;
    switch (currentToken.type) {
    case INTEGER_NUMBER:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = INTEGER_NUMBER_N;
        match(INTEGER_NUMBER);
        break;
    case FLOAT_NUMBER:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = FLOAT_NUMBER_N;
        match(FLOAT_NUMBER);
        break;
    default:
        syntaxError(currentToken);
    }
    return ptr;
}

// transaction for exp : <exp>--> ident <op> ident 
Node* Parser::exp()
{
    Node* ptr = new Node();
    ptr->type = EXP_N;
    if (currentToken.type == IDENT)
    {
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = IDENT_N;
        match(IDENT);
    }
    ptr->child[1] = op();
    if (currentToken.type == IDENT)
    {
        ptr->child[2] = new Node();
        ptr->child[2]->Lexeme = currentToken.name;
        ptr->child[2]->type = IDENT_N;
        match(IDENT);
    }
    return ptr;
}

// transaction for commandtail : <commandTail>-->;; | Else Start <command> Finish 
Node* Parser::commandTail()
{
    Node* ptr = new Node();
    ptr->type = COMMAND_TAIL_N;
    switch (currentToken.type) {
    case DOUBLE_SEMICOLON:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = DOUBLE_SEMICOLON_N;
        match(DOUBLE_SEMICOLON);
        break;
    case ELSE:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = ELSE_N;
        match(ELSE);
        if (currentToken.type == START)
        {
            ptr->child[1] = new Node();
            ptr->child[1]->Lexeme = currentToken.name;
            ptr->child[1]->type = START_N;
            match(START);
        }
        ptr->child[2] = command();
        if (currentToken.type == FINISH)
        {
            ptr->child[3] = new Node();
            ptr->child[3]->Lexeme = currentToken.name;
            ptr->child[3]->type = FINISH_N;
            match(FINISH);
        }
        break;
    default:
        syntaxError(currentToken);
    }
    return ptr;
}

// transaction for op : <op>--> == | >= | <= | <> | < | > 
Node* Parser::op()
{
    Node* ptr = new Node();
    ptr->type = COMMAND_TAIL_N;
    switch (currentToken.type) {
    case EQUAL:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = EQUAL_N;
        match(EQUAL);
        break;
    case GTE:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = GTE_N;
        match(GTE);
        break;
    case LTE:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = LTE_N;
        match(LTE);
        break;
    case NOT_EQUAL:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = NOT_EQUAL_N;
        match(NOT_EQUAL);
        break;
    case SMALLER:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = SMALLER_N;
        match(SMALLER);
        break;
    case BIGGER:
        ptr->child[0] = new Node();
        ptr->child[0]->Lexeme = currentToken.name;
        ptr->child[0]->type = BIGGER_N;
        match(BIGGER);
        break;
    default:
        syntaxError(currentToken);
    }
    return ptr;
}

int main()
{
    string filename;
    cout << "Filename?: ";
    cin >> filename;
    Parser in(filename);
    in.sampleParser();
    cout << endl;
    in.displayTree(root);
    in.displaySymantic();
    return 0;
}
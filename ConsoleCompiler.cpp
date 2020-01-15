#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>

using namespace std;

enum Tokens
{
	START_PROGRAM,
	END_PROGRAM,
	COMMENT,
	END_INSTRUCTION,
	RESERVED_WORD,
	STRING,
	INTEGER,
	FLOAT,
	IDENT,
	GIVE,
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
	NUMBER
};

class Scanner
{
public:
	Scanner(string fileName);
	~Scanner();
	Tokens getToken1();
	void displayTokens1();
private:
	ifstream f;
	Tokens checkReserved1(string s);
};

Scanner::Scanner(string fileName)
{
	f.open(fileName.c_str());
}

Scanner::~Scanner()
{
	f.close();
}

Tokens Scanner::getToken1()
{
	char ch;
	string s;
	
	f.get(ch);
	
	while (isspace(ch)) f.get(ch);
	if (f.eof())        return Tokens::END_FILE;
	else if (ch == ',') return Tokens::RESERVED_CHARACHTER;
	else if (ch == ':') return Tokens::RESERVED_CHARACHTER;
	else if (ch == '<') 
	{
		f.get(ch);
		if (ch == '=')      return Tokens::LTE;
		else if (ch == '>') return Tokens::NOT_EQUAL;
		else
		{
			f.putback(ch);
			return Tokens::SMALLER;
		}
	}
	else if (ch == '>')
	{
		f.get(ch);
		if (ch == '=')      return Tokens::GTE;
		else
		{
			f.putback(ch);
			return Tokens::SMALLER;
		}
	}
	else if (ch == ';') 
	{
		f.get(ch);
		if (ch == ';') return Tokens::END_INSTRUCTION;
		else return Tokens::ERROR;
	}
	else if (ch == '-')
	{
		f.get(ch);
		if (ch == '-') return Tokens::RESERVED_CHARACHTER;
		else return Tokens::ERROR;
	}
	else if (ch == '/')
	{
		f.get(ch);
		if (ch == '/')
		{
			f.get(ch);
			if (ch == '.') return Tokens::COMMENT;
			else return Tokens::ERROR;
		}
		else return Tokens::ERROR;
	}
	else if (isalpha(ch))
	{
		s += ch;
		f.get(ch);
		while (isalpha(ch) || ch == '_' || isdigit(ch))
		{
			s += ch;
			f.get(ch);
		}
		f.putback(ch);
		return checkReserved1(s);
	}
	else if (isalpha(ch))
	{
	s = ch;
	f.get(ch);
	while (isalnum(ch))
	{
		s += ch;
		f.get(ch);
	}
	f.putback(ch);
	return checkReserved1(s);
	}
	else if (isdigit(ch))
	{
	s = ch;
	f.get(ch);
	while (isdigit(ch))
	{
		s += ch;
		f.get(ch);
	}
	f.putback(ch);
	return Tokens::NUMBER;
	}
		else if (isdigit(ch))
	{
	s = ch;
	f.get(ch);
	while (isdigit(ch))
	{
		s += ch;
		f.get(ch);
	}
	f.putback(ch);
	return Tokens::NUMBER;
	}
	else return Tokens::ERROR;
}

void Scanner::displayTokens1()
{
	Tokens a;

	if (f.eof()) cout << "END OF FILE \n";
	while (!f.eof())
	{
		a = getToken1();
		switch (a)
		{
		case START_PROGRAM:        cout << "Start_Program -----> START OF THE PROGRAM\n";
			break;
		case END_PROGRAM:          cout << "End_Program -----> END OF PROGRAM\n";
			break;
		case COMMENT:              cout << "//. -----> COMMENT STARTER\n";
			break;
		case END_INSTRUCTION:      cout << ";; -----> END OF INSTRUCRUCTION \n";
			break;
		case RESERVED_WORD:        cout << "RESERVED WORD\n";
			break;
		case STRING:               cout << "STRING WORD\n";
			break;
		case INTEGER:              cout << "Int_Number -----> word reserved charachter\n";
			break;
		case FLOAT:                cout << "Real_Number -----> word reserved charachter\n";
			break;
		case IDENT:                cout << "IDENTIFIRE\n";
			break;
		case GIVE:                 cout << "Give ----> Reserved Give Word \n";
			break;
		case AFFECT:               cout << "Affect ----> Reserved word for affectation\n";
			break;
		case RESERVED_CHARACHTER:  cout << "RESERVED CHARACHTER\n";
			break;
		case IF:                   cout << "IF -----> CONDITION\n";
			break;
		case BIGGER:               cout << "> -----> BIGGER\n";
			break;
		case SMALLER:              cout << "< -----> SMALLER\n";
			break;
		case EQUAL:                cout << "== -----> EQUAL\n";
			break;
		case NOT_EQUAL:            cout << "<> -----> NOT EQUAL\n";
			break;
		case ELSE:                 cout << "Else -----> ELSE CONDITION\n";
			break;
		case START:                cout << "Start -----> START OF THE BLOCK\n";
			break;
		case FINISH:               cout << "Finish -----> END OF THE BLOCK\n";
			break;
		case SHOW_MES:             cout << "ShowMes -----> RESERVED TO SHOW MESSAGES\n";
			break;
		case SHOW_VAL:             cout << "ShowVal -----> RESERVED TO SHOW VARIABLES VALUES\n";
			break;
		case TO:                   cout << "to -----> TO reserved charachter\n";
			break;
		case LTE:                  cout << "<= -----> LESS THEN OR EQUAL reserved charachter\n";
			break;
		case GTE:                  cout << ">= -----> GREATER THAN OR EQUAL reserved charachter\n";  
			break;
		case NUMBER:               cout << "NUMBER\n";
			break;
		case END_FILE:             cout << "\n ============== THE END OF THE FILE ======================\n";
			break;
		case ERROR:                cout << "ERROR\n";
			break;
		}
	}
}

inline Tokens Scanner::checkReserved1(string s)
{
	if (s == "Start_Program")    return Tokens::START_PROGRAM;
	else if (s == "Int_Number")  return Tokens::INTEGER;
	else if (s == "Give")        return Tokens::GIVE;
	else if (s == "Real_Number") return Tokens::FLOAT;
	else if (s == "If")          return Tokens::IF;
	else if (s == "Affect")      return Tokens::AFFECT;
	else if (s == "to")          return Tokens::TO;
	else if (s == "ShowMes")     return Tokens::SHOW_MES;
	else if (s == "ShowVal")     return Tokens::SHOW_VAL;
	else if (s == "Start")       return Tokens::START;
	else if (s == "Finish")      return Tokens::FINISH;
	else if (s == "Else")        return Tokens::ELSE;
	else if (s == "String")      return Tokens::STRING;
	else if (s == "End_Program") return Tokens::END_PROGRAM;
	else                         return Tokens::IDENT;
}

/*int main()
{
	string name;
	cout << "Enter the file name : ";
	cin >> name;
	Scanner s(name);
	s.displayTokens1();
	s.~Scanner();
}*/
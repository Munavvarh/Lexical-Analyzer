
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "lex.h"
#include <fstream>
#include <list>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <map>

using std::map;
using namespace std;


map<string, Token> dictionary = {
	
    { "WRITELN", WRITELN },
    { "PROGRAM", PROGRAM },
    { "INTEGER", INTEGER },
    { "BEGIN", BEGIN },
    { "REAL", REAL },
    { "STRING", STRING },
    { "REAL", REAL },
    { "IF", IF },
    { "THEN", THEN },
    { "END", END },
    { "TO", TO },
    { "DOWNTO", DOWNTO },
    { "FOR", FOR },
    { "ELSE", ELSE},
    { "VAR" , VAR},
    { "DO" , DO},
    
};

map <Token ,string> tokenData = {
	
  { PROGRAM,  "PROGRAM"},
  { WRITELN,  "WRITELN"},
  { INTEGER,  "INTEGER"},
  { END,      "END"},
  { VAR,      "VAR"},
  { DO,		  "DO"},
  { BEGIN,	  "BEGIN"},
  { IF,       "IF"},
  { REAL,     "REAL"},
  { TO,       "TO"},
  { DOWNTO,	  "DOWNTO"},
  { FOR,	  "FOR"},
  { THEN,	  "THEN"},
  { STRING,   "STRING"},
  { BEGIN,    "BEGIN"},
  { IDENT,    "IDENT"},
  { ELSE,     "ELSE"},
  { ICONST,   "ICONST"},
  { RCONST,   "RCONST"},
  { SCONST,   "SCONST"},
  { PLUS,     "PLUS"},
  { MINUS,    "MINUS"},
  { MULT,     "MULT"},
  { DIV,      "DIV"},
  { ASSOP,    "ASSOP"},
  { LPAREN,   "LPAREN"},
  { RPAREN,   "RPAREN"},
  { COMMA,    "COMMA"},
  { EQUAL,    "EQUAL"},
  { GTHAN,    "GTHAN"},
  { SEMICOL,  "SEMICOL"},
  { ERR,      "ERR"}, 
  { COLON,    "COLON"},
  { LTHAN,    "LTHAN" },
  { DONE,     "DONE"}
  
};


LexItem id_or_kw(const string& lex, int linenum) {
	
	
string val = lex;
	transform(val.begin(), val.end(), val.begin(), ::toupper);
	if(dictionary.find(val) == dictionary.end()){
		LexItem	result(IDENT, lex, linenum);
		return result;
	}

	LexItem result(dictionary[val], lex, linenum);
	return result;


    
}
	




LexItem getNextToken(istream& in, int& linenum) {


    enum TokState {

        START, INID, INSTRING, ININT, INREAL, INCOMMENT
    } tokState = START;
    string lex;
    char character;
    Token token;
    char nextChar ;
    char uppercaseChar;


    while (in.get(character)) {

        switch (tokState) { 

            case START:
                if( character == '\n')
                    linenum++;

                if(isspace(character) )
                    continue;
                    
                if(character == '"')
                    continue;



                lex = character;

                if (character == '(') {
                    if (in.peek() == '*') {
                        nextChar = in.get();
                        lex = character + nextChar;
                        tokState = INCOMMENT;
                }  else {
                     token = LPAREN;
					 	return LexItem(token,lex,linenum); 
					  }
            }

                else if(isdigit(character)){
                    tokState = ININT;
                    break;
                }
                else if( isalpha(character) || character == '_' ){
                    lex[0]=toupper(lex[0]);
                    tokState = INID;
                    break;

                }
                else if (character == '\''){
                    tokState = INSTRING;
                    break;
                }
                else if (character == '.'){ 

                    tokState = INREAL;
                    break;

                }

                else {
                    if (character == '+')
                        token = PLUS;

                    else if (character == '-')
                        token = MINUS;

                    else if (character == '*')
                        token = MULT;

                    else if (character == '/')
                        token = DIV;

                    else if (character == ':') {
                        if (in.peek() == '=') {
                            nextChar = in.get();
                            lex = character + nextChar;
                            token = ASSOP;


                        } else {
                            token = COLON;

                        }

                    }
                    
                    else if (character == '(')
                        token = LPAREN;

                    else if (character == ')')
                        token = RPAREN;

                    else if (character == '>')
                        token = GTHAN;
                        
                    else if (character == '<')
                        token = LTHAN;

                    else if (character == ';')
						token = SEMICOL;

                    else if (character == ',')
                        token = COMMA;
                        
                    else if (character == '=')
                        token = EQUAL;
                    
                    


                    else{
                        token = ERR;
                        return LexItem(token,lex,linenum);
                    }
                    return LexItem(token,lex,linenum);

                }

            case INCOMMENT:
                
                if (character == '\n'){
                    linenum++;
                    
                }
                else if (character == '*' ) {
                   	 if (in.peek() == ')') {
                        nextChar = in.get();
                        lex="*)";
                        lex = character + nextChar;
                        tokState = START;
                        
           			}
           		   } 
              
                break;
            case ININT:

                if(isdigit(character)){
                    lex += character;
                }

                else if(character == '.'){
                    in.putback(character);
                    tokState = INREAL;
                    break;

                }
                else{
                    in.putback(character);
                    tokState = START;
                    return LexItem(ICONST,lex,linenum);

                }
                break;

            case INREAL:


                if (character == '.'  && isdigit(in.peek())){
                    lex += character;

                }
                else if (isalpha(character)){


                    lex += character;

                    return LexItem(ERR,lex,linenum);


                }

                    
                else if(isdigit(character)) {
                    lex += character;
                }

                else if (character == '.' && !isdigit(in.peek())){

                    lex +=character;

                    return LexItem(ERR,lex,linenum);

                }
                else{
                    in.putback(character);
                    tokState = START;
                    return LexItem(RCONST,lex,linenum);
                }
                break;

            case INID:
                
                uppercaseChar = toupper(character);
                if(isalpha(character)|| isdigit(character)|| uppercaseChar == '_'){
                    lex += uppercaseChar;
                }



                else {
                    in.putback(character);
                    tokState = START;
                    return id_or_kw(lex,linenum);
                }
                break;
                
            case INSTRING:
                
                if (character == '\n') {
                    in.putback(character);
                    tokState = START;
                    return LexItem(ERR, lex, linenum);
                }
                if( character == '\'' ) 
          		 {
              		 lex = lex.substr(1, lex.length()-1);
              		 return LexItem(SCONST, lex, linenum );
          		 }  
                else{
                    lex += character;
                }
                break;


        }


    }

    if( in.eof() )
        return LexItem(DONE, "", linenum );
    return LexItem(ERR, "Error", linenum );

}
 






ostream& operator<< (ostream& out,const LexItem& tok){
	
	Token data = tok.GetToken();
	
	out<<tokenData[data];
	
	
	
	if(data == IDENT || data == ICONST || data == RCONST || data == SCONST){
		string val = tok.GetLexeme();
        if(data != SCONST)
		    transform(val.begin(), val.end(), val.begin(), ::toupper);
		out<<"("<<val<<")";
	}
	
	
	return out;
}












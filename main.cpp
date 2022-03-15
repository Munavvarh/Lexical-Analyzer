

#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <algorithm>
#include <sstream>
#include "lex.h"





#include <cctype>
#include <map>
using std::map;

using namespace std;

int main(int argc, char *argv[])
{
	LexItem	tokVal;
	
	
	
	int lineNum =0;
	ifstream  file ;

	
	map<string, bool> argVal {
		{"-v", false},
		{"-iconst", false},
		{"-rconst", false},
		{"-sconst", false},
		{"-ident", false},
	};
	
	bool multiFiles = false;

	//int i;
	
	if (argc == 1)
	{
		cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
		return 0;
	}
	for( int i=1; i<argc; i++ ) {
        string arg(argv[i]);
		if(argVal.find(arg) != argVal.end()){
			argVal[arg] = true;
            continue;
		}
		else if(arg[0] == '-'){
			cerr << "UNRECOGNIZED FLAG " << arg << endl;
			return 0;
		}
		if(multiFiles){
			cerr << "ONLY ONE FILE NAME ALLOWED." << endl;
			return 0;
		}
		file.open(arg);
		multiFiles = true;
		if(file.is_open() == false) {
			cerr << "CANNOT OPEN the File " << arg << endl;
			return 0;
		}
	}
	

	map<int, Token> intconsts = {};
	map<float, Token> rconsts = {};
	map<string, Token> sconsts = {};
	map<string, Token> identconsts = {};
	list<LexItem> all = {}; 

	int tokNum =0;

	while((tokVal = getNextToken(file, lineNum)) != DONE && tokVal != ERR){
		if(tokVal == ICONST){
			int i;
			istringstream (tokVal.GetLexeme()) >> i;
			intconsts[i] = ICONST;
		}
		if(tokVal == RCONST){
			float r;
			istringstream (tokVal.GetLexeme()) >> r;
			rconsts[r] = RCONST;
		}
		if(tokVal == SCONST) {
			sconsts[tokVal.GetLexeme()] = SCONST;
		}
		if(tokVal == IDENT) {
			string val = tokVal.GetLexeme();
			transform(val.begin(), val.end(), val.begin(), ::toupper);
			identconsts[val] = IDENT;
			
		}
		tokNum ++;
		all.push_back(tokVal);
		if(argVal["-v"]){
				cout<<tokVal<<endl;
			}
	}

    

  if(tokVal == ERR){
		cout<<"Error in line "<< tokVal.GetLinenum() + 1 <<" ("<<tokVal.GetLexeme()<<")"<<endl;
	}
  else{
  			
			cout<<"Lines: "<<lineNum<<endl;
      if(lineNum!=0)
        cout<<"Tokens: "<<tokNum<<endl;
        
      
      if(argVal["-sconst"]){
      	
      	
      			
					cout<<"STRINGS:"<<endl;
      	 	  	
				
				for (auto itr = sconsts.cbegin(); itr != sconsts.cend(); ++itr) {
						cout << "\'"<<(*itr).first <<"\'"<<endl;
				}
			
    	}
      
			if(argVal["-iconst"]){
				
			
					cout<<"INTEGERS:"<<endl;
				
				for (auto itr = intconsts.cbegin(); itr != intconsts.cend(); ++itr) {
        			cout << (*itr).first <<endl;
				}
		
    	}
			if(argVal["-rconst"]){
				
				
					cout<<"REALS:"<<endl;
      	 	  	
			
				for (auto itr = rconsts.cbegin(); itr != rconsts.cend(); ++itr) {
        			cout << (*itr).first <<endl;
				}
				
    	}
			
			if(argVal["-ident"]){
				
			
					cout<<"IDENTIFIERS:"<<endl;
				
				
				
				for (auto itr = identconsts.cbegin(); itr != identconsts.cend(); ) {
						cout<<(*itr).first ;
					if(++itr == identconsts.cend()){
						cout<<endl;
					}
					else {
						cout<<", ";
					}
				}
			
    		}
		}
	}
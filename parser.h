/* REUSING FOR PROJECT 3 / MODIFYING
Brian Cabantug
Grace Derderian

Program #3
CPSC 323-85
Oct 27 2017

Compilers used: Visual Studio 2013 (Visual C++ 12.0)
XCode (g++ Version 5.4)

Tested using both IDE and command line
*/
#ifndef PARSER_H //header file protectors
#define PARSER_H

//lexer header file and string library included
#include "lexer.h"
#include <string>
#include <vector>


vector <string> assemblyCommands;
//Function Prototypes

void Program(ifstream&, LexTok&); //
void DeclList(ifstream&, LexTok&); //
string Decl(ifstream&, LexTok&); //

string VarList(ifstream&, LexTok&);
string StmtList(ifstream&, LexTok&); //
void Stmt(ifstream&, LexTok&); //
void Assign(ifstream&, LexTok&); //
void Read(ifstream&, LexTok&); //
void Write(ifstream&, LexTok&); //
void If(ifstream&, LexTok&); //
void While(ifstream&, LexTok&); //

void Cond(ifstream&, LexTok&); //
void RelOp(ifstream&, LexTok&); // 
void Expr(ifstream&, LexTok&); //
void Term(ifstream&, LexTok&); //
void Factor(ifstream&, LexTok&); //


//parser function that starts the top down recursion for grammar rules that takes in the file
void parser(ifstream &file) {
	//calls the first lexeme/token combo in the file
	LexTok curToken = lexer(file);

	//calls the Program function, checking if "program" is present
	Program(file, curToken);

}

//Display the error message
void writeError(string s, LexTok& check, ifstream &file) {
	cout << "Error: unexpected string: " << check.lexeme << ", expected " << s << endl;
}

//lexeme check
bool accept(string s, LexTok& check, ifstream &file) {

	//checks the string with the lexeme
	if (s.compare(check.lexeme) == 0) {
		check = lexer(file);
		return true;
	}
	else {
		return false;
	}
}

void expect(string s, LexTok& check, ifstream &file) {
	if (!accept(s, check, file)) {
		writeError(s, check, file);
	}
}

//Grace and Brian
void Program(ifstream& file, LexTok& token) {
	//Consume token if present
	expect("program", token, file);

	//Check for DeclList
	if (token.token.compare("Type") == 0) {
		//Call DeclList
		DeclList(file, token);
	}
	
	//Consume token if present
	expect("begin", token, file);

	//Check for StmtList
	if ((!(token.lexeme.compare("end"))) == 0) {
		//Call StmtList
		StmtList(file, token);
	}

	//Consume token if present
	expect("end", token, file);

	//Consume token if present
	expect(".", token, file);

	
}

void DeclList(ifstream& file, LexTok& token) {

	//starts off the declarations of variables
	assemblyCommands.push_back(".data");

	//Check for initial declaration and loops back if there are more
	do {
		//Call Decl and adds the statement to the assembly command
		assemblyCommands.push_back(Decl(file, token));
	} while (token.token.compare("Type") == 0);

}

//Changing data type to return the data statements to save variable list
string Decl(ifstream& file, LexTok& token) {

	string decl = "";

	//Call Type function
	decl = decl + Type(file, token);

	//Call ValList function
	decl = decl + VarList(file, token);

	//Consume token if present
	expect(";", token, file);

	//returns decl to add it to the variables list
	return decl;
}


//Grace
string Type(ifstream& file, LexTok& token) {
	//check for type
	if (token.lexeme.compare("int") == 0)
	{
		//consume the token first
		token = lexer(file);
		//then output
		/*cout << "Type => int | real | string" << endl;*/
	}
}

//Grace
string VarList(ifstream& file, LexTok& token) {
	//consume Identifier
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}

	//if comma is found, then
	while (token.lexeme.compare(",") == 0) {
		//consume the token
		token = lexer(file);
		//check if next is another identifier
		if (token.token.compare("Identifier") == 0)
		{
			//get next token
			token = lexer(file);
		}
	}//loop again if there is a comma following the identifier

	 //output rule
	/*cout << "VarList => Ident {,Ident}" << endl;*/
}

//Grace
string StmtList(ifstream& file, LexTok& token) {
	//call Stmt function
	Stmt(file, token);

	//continues to call stmt function if lexemes are Ident, read, write, if, while, do, return
	while (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0) {
		Stmt(file, token);
	}

	//output rule
	/*cout << "StmtList => Stmt{Stmt}" << endl;*/
}

//Grace
void Stmt(ifstream& file, LexTok& token) {
	//if token is identifier
	if (token.token.compare("Identifier") == 0)
	{	//calls Assign function and output rule
		Assign(file, token);
		cout << "Stmt => Assign" << endl;
	}
	//if token is read
	else if (token.lexeme.compare("read") == 0)
	{	//calls Read function and output rule
		Read(file, token);
		cout << "Stmt => Read" << endl;
	}
	//if token is write
	else if (token.lexeme.compare("write") == 0)
	{
		//calls Write function and output rule
		Write(file, token);
		cout << "Stmt => Write" << endl;
	}
	//if token is if
	else if (token.lexeme.compare("if") == 0)
	{	//calls If function and output rule
		If(file, token);
		cout << "Stmt => If" << endl;
	}
	//if token is while
	else if (token.lexeme.compare("while") == 0)
	{	//calls While function and output rule
		While(file, token);
		cout << "Stmt => While" << endl;
	}
}

//Grace
void Assign(ifstream& file, LexTok& token) {
	//if token is Identifier, consume token
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}


	expect(":=", token, file);

	//call Expr function
	Expr(file, token);

	expect(";", token, file);
	//output rule
	/*cout << "Assign => Ident := Expr;" << endl;*/
}

//BRIAN
void Read(ifstream& file, LexTok& token) {

	expect("read", token, file);

	expect("(", token, file);
	//call VarList func
	VarList(file, token);

	expect(")", token, file);

	expect(";", token, file);
	//output rule
	/*cout << "Read => read ( VarList ) ; " << endl;*/
}
//Brian
void Write(ifstream& file, LexTok& token) {
	//after consuming write lexeme
	expect("write", token, file);

	expect("(", token, file);

	//keeps on finding an expression as long as the token matches with the comma
	Expr(file, token);

	while (token.lexeme.compare(",") == 0) {
		//consumes comma token
		token = lexer(file);
		Expr(file, token);
	}

	expect(")", token, file);

	expect(";", token, file);

	//output rule
	/*cout << "Write => write ( Expr {, Expr} ) ;" << endl;*/
}

//BRIAN 
void If(ifstream& file, LexTok& token) {
	//consumes if
	expect("if", token, file);

	expect("(", token, file);

	//calls Cond function
	Cond(file, token);

	expect(")", token, file);

	expect("begin", token, file);

	//cals StmtList function
	StmtList(file, token);

	expect("end", token, file);

	//elseif statements if there are any
	if (token.lexeme.compare("elsif") == 0) {

		do {
			//consume initial elsif
			expect("elsif", token, file);

			expect("(", token, file);
			//call Cond function
			Cond(file, token);

			expect(")", token, file);

			expect("begin", token, file);
			//call StmtList function
			StmtList(file, token);

			expect("end", token, file);

		} while (token.lexeme.compare("elsif") == 0); //continues the loop if elseif token is present

	}

	//checking for else statement

	if (token.lexeme.compare("else") == 0) {
		//consume else token
		expect("else", token, file);
		expect("begin", token, file);
		//call StmtList
		StmtList(file, token);
		expect("end", token, file);
	}
	//output If rule
	/*cout << "If => if ( Cond ) begin StmtList end { elsif ( Cond ) begin StmtList end } [else begin StmtList end ]" << endl;*/
}

void While(ifstream& file, LexTok& token) {
	//consume while token/keyword
	if (token.lexeme.compare("while") == 0) {
		token = lexer(file);
	}

	expect("(", token, file);
	//calls Cond function
	Cond(file, token);

	expect(")", token, file);

	expect("begin", token, file);

	//checks for stmtList if there
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0) {
		//call StmtList function
		StmtList(file, token);
	}

	expect("end", token, file);

	//Output rule
	/*cout << "While => while ( Cond ) begin [StmtList] end" << endl;*/
}

//Grace
void Cond(ifstream& file, LexTok& token) {

	//Call Expr function
	Expr(file, token);

	//Call RelOp function
	RelOp(file, token);

	//Call Expr function
	Expr(file, token);

	//Ouput rule
	/*cout << "Cond => Expr RelOp Expr" << endl;*/
}

//Grace
void RelOp(ifstream& file, LexTok& token) {

	//Check if it is a RelOp
	if (token.token.compare("RelOp") == 0)
	{
		//consume lexeme
		token = lexer(file);
	}

}

//Grace
void Expr(ifstream& file, LexTok& token) {

	//Call Term function
	Term(file, token);

	//If lexeme is + or -
	if (token.lexeme.compare("+") == 0 || token.lexeme.compare("-") == 0) {
		//consume token
		token = lexer(file);

		//call the factor if it is factor
		Term(file, token);
	}

	//Output rule
	cout << "Expr => Term { (+|-) Term }" << endl;
}

//Grace
void Term(ifstream& file, LexTok& token) {

	//Call Factor function
	Factor(file, token);

	//Check if lexeme is * or /
	if (token.lexeme.compare("*") == 0 || token.lexeme.compare("/") == 0) {
		//consume token
		token = lexer(file);

		//call the factor if it is factor
		Factor(file, token);
	}

	//Output rule
	cout << "Term => Factor { (*|/) Factor } " << endl;
}

//Grace
void Factor(ifstream& file, LexTok& token) {

	//Check if identifier, intConst, realConst, strConst
	if (token.token.compare("Identifier") == 0)
	{
		//consume token
		token = lexer(file);

		////check if funcCall or Identifiers
		//if (token.lexeme.compare("(") == 0) {
		//	//Call FuncCall
		//	FuncCall(file, token);
		//	//Output rule for function call
		//	cout << "Factor => FuncCall" << endl;
		//}

		/*else {*/
			//Output rule for identifier
			/*cout << "Factor => Ident" << endl;*/
		/*}*/
	}
	else if (token.token.compare("IntConst") == 0)
	{
		//Consume token
		token = lexer(file);
		//Output rule
		/*cout << "Factor => IntConst" << endl;*/
	}
	
	else if (token.lexeme.compare("(") == 0)
	{

		expect("(", token, file);
		//Call Expr function
		Expr(file, token);

		expect(")", token, file);

		//Output rule
		/*cout << "Factor => ( Expr )" << endl;*/
	}

}

//Grace
//void FuncCall(ifstream& file, LexTok& token) {
//	//assuming the Identifier lexeme has already been consumed
//
//	expect("(", token, file);
//
//	//Check if there is an ArgList
//	if ((!token.lexeme.compare(")")) == 0) {
//		//Call ArgList function if there is
//		ArgList(file, token);
//	}
//
//	expect(")", token, file);
//
//	//Output rule
//	cout << "FuncCall => Ident ( [ArgList] ) " << endl;
//}
//
////Grace
//void ArgList(ifstream& file, LexTok& token) {
//
//	//Call Expr function
//	Expr(file, token);
//
//	//Check if there is another expression
//	while (token.lexeme.compare(",") == 0) {
//		expect(",", token, file);
//
//		//Call Expr function
//		Expr(file, token);
//	}
//
//	//Output rule
//	cout << "ArgList => Expr { ,Expr} " << endl;
//}



#endif
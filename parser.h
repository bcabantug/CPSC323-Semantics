/*
Brian Cabantug
Grace Derderian

Program #2
CPSC 323-85
Sept 29 2017

Compilers used: Visual Studio 2013 (Visual C++ 12.0)
XCode (g++ Version 5.4)

Tested using both IDE and command line
*/
#ifndef PARSER_H //header file protectors
#define PARSER_H

//lexer header file and string library included
#include "lexer.h"
#include <string>

//Function Prototypes
void writeError(string, LexTok&, ifstream&);
bool accept(string, LexTok&, ifstream&);
void expect(string, LexTok&, ifstream&);
void Program(ifstream&, LexTok&);
void DeclList(ifstream&, LexTok&);
void Decl(ifstream&, LexTok&);
void FuncList(ifstream&, LexTok&);
void Func(ifstream&, LexTok&);
void ParamList(ifstream&, LexTok&);
void Param(ifstream&, LexTok&);
void Type(ifstream&, LexTok&);
void VarList(ifstream&, LexTok&);
void StmtList(ifstream&, LexTok&);
void Stmt(ifstream&, LexTok&);
void Assign(ifstream&, LexTok&);
void Read(ifstream&, LexTok&);
void Write(ifstream&, LexTok&);
void If(ifstream&, LexTok&);
void While(ifstream&, LexTok&);
void Do(ifstream&, LexTok&);
void Return(ifstream&, LexTok&);
void Cond(ifstream&, LexTok&);
void RelOp(ifstream&, LexTok&);
void Expr(ifstream&, LexTok&);
void Term(ifstream&, LexTok&);
void Factor(ifstream&, LexTok&);
void FuncCall(ifstream&, LexTok&);
void ArgList(ifstream&, LexTok&);

//parser function that starts the top down recursion for grammar rules that takes in the file
void parser(ifstream &file) {
	//calls the first lexeme/token combo in the file
	LexTok curToken = lexer(file);

	//calls the Program function, checking if "program" is present
	Program(file, curToken);

}

//Display the error message
void writeError(string s, LexTok& check, ifstream &file){
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
	//Check for FuncList
	if (token.lexeme.compare("function") == 0) {
		//Call FuncList
		FuncList(file, token);
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

	//Output rule
	cout << "Program => program [DeclList] [FuncList] begin [StmtList] end." << endl;
}

//Grace and Brian
void DeclList(ifstream& file, LexTok& token) {

	//Check for initial declaration and loops back if there are more
	do {
		//Call Decl function
		Decl(file, token);
	} while (token.token.compare("Type") == 0);

	//Output rule
	cout << "DeclList => Decl {Decl}" << endl;
}

//Grace and Brian
void Decl(ifstream& file, LexTok& token) {

	//Call Type function
	Type(file, token);

	//Call ValList function
	VarList(file, token);

	//Consume token if present
	expect(";", token, file);


	//Output rule
	cout << "Decl => Type VarList ;" << endl;
}

//Grace
void FuncList(ifstream& file, LexTok& token) {
	//checks for initial function and loops back if more functions are present
	do {
		//calls Func function
		Func(file, token);
	} while (token.lexeme.compare("function") == 0);
	//output rule
	cout << "FuncList =>Func {Func}" << endl;
}

//Grace
void Func(ifstream& file, LexTok& token) {

	expect("function", token, file);

	//checks for name of the function and consumes it
	if (token.token.compare("Identifier") == 0)
	{
		//consumes token
		token = lexer(file);
	}

	expect("(", token, file);

	//checks if parameter list is present, consume paramList
	if ((!token.lexeme.compare(")")) == 0) {
		//calls ParamList function
		ParamList(file, token);
	}

	expect(")", token, file);

	expect(":", token, file);

	//calls Type function
	Type(file, token);

	expect(";", token, file);

	//while begin is not token, call DeclList function
	if ((!(token.lexeme.compare("begin"))) == 0) {
		DeclList(file, token);
	}

	expect("begin", token, file);
	
	//while end is not token, call StmtList function
	if ((!(token.lexeme.compare("end"))) == 0) {
		StmtList(file, token);
	}

	expect("end", token, file);
	//output rule
	cout << "Func => function Ident ( [ParamList] ): Type ; [DeclList] begin [StmtList] end " << endl;
}

//Grace
void ParamList(ifstream& file, LexTok& token) {
	//call Param function
	Param(file, token);
	
	//if comma is next token, indicates another Parameter and continues to 
	while (token.lexeme.compare(",") == 0) {
		//consume token
		expect(",", token, file);
		//call Param function
		Param(file, token);
	}
	//output rule
	cout << "ParamList => Param {, Param}" << endl;
}

//Grace
void Param(ifstream& file, LexTok& token) {
	//call type function
	Type(file, token);

	//consumes identifier
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}
	//output rule
	cout << "Param => Type Ident" << endl;
}

//Grace
void Type(ifstream& file, LexTok& token) {
	//check for type
	if (token.lexeme.compare("int") == 0 || token.lexeme.compare("real") == 0 || token.lexeme.compare("string") == 0)
	{
		//consume the token first
		token = lexer(file);
		//then output
		cout << "Type => int | real | string" << endl;
	}
}

//Grace
void VarList(ifstream& file, LexTok& token) {
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
	cout << "VarList => Ident {,Ident}" << endl;
}

//Grace
void StmtList(ifstream& file, LexTok& token) {
	//call Stmt function
	Stmt(file, token);

	//continues to call stmt function if lexemes are Ident, read, write, if, while, do, return
	while (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		Stmt(file, token);
	}

	//output rule
	cout << "StmtList => Stmt{Stmt}" << endl;
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
	//if token is do
	else if (token.lexeme.compare("do") == 0)
	{	//call Do functions and output rule
		Do(file, token);
		cout << "Stmt => Do" << endl;
	}
	//if token is return
	else if (token.lexeme.compare("return") == 0)
	{	//calls Return function and output rule
		Return(file, token);
		cout << "Stmt => Return" << endl;
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
	cout << "Assign => Ident := Expr;" << endl;
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
	cout << "Read => read ( VarList ) ; " << endl;
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
	cout << "Write => write ( Expr {, Expr} ) ;" << endl;
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
	cout << "If => if ( Cond ) begin StmtList end { elsif ( Cond ) begin StmtList end } [else begin StmtList end ]" << endl;
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
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		//call StmtList function
		StmtList(file, token);
	}

	expect("end", token, file);

	//Output rule
	cout << "While => while ( Cond ) begin [StmtList] end" << endl;
}
//Brian 9/25
void Do(ifstream& file, LexTok& token) {

	//consume do lexeme (will check if implemented)
	expect("do", token, file);

	//Check for stmt list and call StmtList
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		StmtList(file, token);
	}

	expect("until", token, file);

	expect("(", token, file);

	//Call Cond function
	Cond(file, token);

	expect(")", token, file);

	expect(";", token, file);

	//Output rule
	cout << "do => do [StmtList] until ( Cond ) ;" << endl;
}
//Brian
void Return(ifstream& file, LexTok& token) {

	expect("return", token, file); //token to be consumed

	//Call Expr function
	Expr(file, token);

	expect(";", token, file);

	//Output rule
	cout << "Return => return Expr ;" << endl;
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
	cout << "Cond => Expr RelOp Expr" << endl;
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

		//check if funcCall or Identifiers
		if (token.lexeme.compare("(") == 0) {
			//Call FuncCall
			FuncCall(file, token);
			//Output rule for function call
			cout << "Factor => FuncCall" << endl;
		}

		else {
			//Output rule for identifier
			cout << "Factor => Ident" << endl;
		}
	}
	else if (token.token.compare("IntConst") == 0)
	{
		//Consume token
		token = lexer(file);
		//Output rule
		cout << "Factor => IntConst" << endl;
	}
	else if (token.token.compare("RealConst") == 0)
	{
		//Consume token
		token = lexer(file);
		//Output rule
		cout << "Factor => RealConst" << endl;
	}
	else if (token.token.compare("StrConst") == 0)
	{
		//Consume token
		token = lexer(file);
		//Output rule
		cout << "Factor => StrConst" << endl;
	}
	else if (token.lexeme.compare("(") == 0)
	{

		expect("(", token, file);
		//Call Expr function
		Expr(file, token);

		expect(")", token, file);

		//Output rule
		cout << "Factor => ( Expr )" << endl;
	}

}

//Grace
void FuncCall(ifstream& file, LexTok& token) {
	//assuming the Identifier lexeme has already been consumed

	expect("(", token, file);

	//Check if there is an ArgList
	if ((!token.lexeme.compare(")")) == 0) {
		//Call ArgList function if there is
		ArgList(file, token);
	}

	expect(")", token, file);

	//Output rule
	cout << "FuncCall => Ident ( [ArgList] ) " << endl;
}

//Grace
void ArgList(ifstream& file, LexTok& token) {

	//Call Expr function
	Expr(file, token);

	//Check if there is another expression
	while (token.lexeme.compare(",") == 0) {
		expect(",", token, file);

		//Call Expr function
		Expr(file, token);
	}

	//Output rule
	cout << "ArgList => Expr { ,Expr} " << endl;
}



#endif
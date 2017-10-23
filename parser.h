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
//
string tRegister[10] = { "", "", "", "", "", "", "", "", "", "" };

int whileCount = 0;

//Function Prototypes

void Program(ifstream&, LexTok&); //
void DeclList(ifstream&, LexTok&); //
vector<string> Decl(ifstream&, LexTok&); //
string Type(ifstream&, LexTok&);//

vector<string> VarList(ifstream&, LexTok&);
void StmtList(ifstream&, LexTok&); //
void Stmt(ifstream&, LexTok&); //
void Assign(ifstream&, LexTok&); //
void Read(ifstream&, LexTok&); //
void Write(ifstream&, LexTok&); //
void If(ifstream&, LexTok&); //
void While(ifstream&, LexTok&); //

string Cond(ifstream&, LexTok&); //
string RelOp(ifstream&, LexTok&); // 
string Expr(ifstream&, LexTok&); //
string Term(ifstream&, LexTok&); //
string Factor(ifstream&, LexTok&); //


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
	//begins the assembly assignment to the 
	assemblyCommands.push_back(".text");
	assemblyCommands.push_back("main:");

	//Check for StmtList
	if ((!(token.lexeme.compare("end"))) == 0) {
		//Call StmtList
		StmtList(file, token);
	}

	//Consume token if present
	expect("end", token, file);

	//Consume token if present
	expect(".", token, file);

	assemblyCommands.push_back("li $v0, 10");
	assemblyCommands.push_back("syscall");

	//output commands at end of programs
	for (vector<string>::iterator it = assemblyCommands.begin(); it != assemblyCommands.end(); it++){
		cout << *it << endl;
	}


}

void DeclList(ifstream& file, LexTok& token) {

	vector<string> declL;

	//starts off the declarations of variables
	assemblyCommands.push_back(".data");

	//Check for initial declaration and loops back if there are more
	do {
		//Call Decl and takes the list of declarations given to add to the main vector of commands
		declL = Decl(file, token);


		//loop to go through the vector 
		for (vector<string>::iterator it = declL.begin(); it != declL.end(); it++) {

			//adds them to the main assembly Command vector
			assemblyCommands.push_back(*it);
		}

	} while (token.token.compare("Type") == 0);

}

//Changing data type to return the data statements to save variable list
vector<string> Decl(ifstream& file, LexTok& token) {
	vector<string> decl;

	vector<string> vars;
	string t = "";

	//Call Type function
	t = Type(file, token);

	//Call ValList function
	vars = VarList(file, token);

	//Consume token if present
	expect(";", token, file);

	//loop to set all the declarations

	for (vector<string>::iterator it = vars.begin(); it != vars.end(); it++) {
		string d = "";
		//gets the var
		d = d + *it;

		d = d + ":    ";
		d = d + t;
		d = d + "0";

		//saves it in the decl vector to pass back
		decl.push_back(d);

	}

	//returns decl to add it to the variables list
	return decl;
}


//Grace
string Type(ifstream& file, LexTok& token) {
	//check for type
	string t = "";

	//if the type is int, then return
	if (token.lexeme.compare("int") == 0)
	{

		t = ".word	";

		//consume the token first
		token = lexer(file);
	}
	return t;
}

//Grace
vector<string> VarList(ifstream& file, LexTok& token) {
	//vector to hold the identifier list and 
	vector<string> ident;

	if (token.token.compare("Identifier") == 0)
	{
		//adds the identifier to the temp vector
		ident.push_back(token.lexeme);

		//moves to the next token
		token = lexer(file);
	}

	//if comma is found, then
	while (token.lexeme.compare(",") == 0) {
		//consume the token
		token = lexer(file);

		//check if next is another identifier
		if (token.token.compare("Identifier") == 0)
		{
			//push the next identifier to the vector
			ident.push_back(token.lexeme);
			//get next token
			token = lexer(file);
		}
	}//loop again if there is a comma following the identifier

	//return the vector
	return ident;
}

//Grace
void StmtList(ifstream& file, LexTok& token) {

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
		/*cout << "Stmt => Assign" << endl;*/
	}
	//if token is read
	else if (token.lexeme.compare("read") == 0)
	{	//calls Read function and output rule
		Read(file, token);
		/*cout << "Stmt => Read" << endl;*/
	}
	//if token is write
	else if (token.lexeme.compare("write") == 0)
	{
		//calls Write function and output rule
		Write(file, token);
		/*cout << "Stmt => Write" << endl;*/
	}
	//if token is if
	else if (token.lexeme.compare("if") == 0)
	{	//calls If function and output rule
		If(file, token);
		/*cout << "Stmt => If" << endl;*/
	}
	//if token is while
	else if (token.lexeme.compare("while") == 0)
	{	//calls While function and output rule
		While(file, token);
		/*cout << "Stmt => While" << endl;*/
	}
}

//Grace
void Assign(ifstream& file, LexTok& token) {
	vector<string> assign;

	string ident = "";
	string reg = "";



	//if token is Identifier, consume token
	if (token.token.compare("Identifier") == 0)
	{
		ident = token.lexeme;

		token = lexer(file);
	}


	expect(":=", token, file);

	//call Expr function
	reg = Expr(file, token);

	assemblyCommands.push_back("sw " + reg + ", " + ident);

	expect(";", token, file);
	//output rule
	/*cout << "Assign => Ident := Expr;" << endl;*/
}

//BRIAN
void Read(ifstream& file, LexTok& token) {

	vector<string> idents;


	expect("read", token, file);

	expect("(", token, file);
	//call VarList func
	idents = VarList(file, token);

	for (vector<string>::iterator it = idents.begin(); it != idents.end(); it++){
		assemblyCommands.push_back("li $v0, 5");
		assemblyCommands.push_back("syscall");
		assemblyCommands.push_back("sw $v0, " + *it);
	}

	expect(")", token, file);

	expect(";", token, file);
	//output rule
	/*cout << "Read => read ( VarList ) ; " << endl;*/
}



//Brian
void Write(ifstream& file, LexTok& token) {
	
	vector<string> exprList;
	
	//after consuming write lexeme
	expect("write", token, file);

	expect("(", token, file);

	//keeps on finding an expression as long as the token matches with the comma
	exprList.push_back(Expr(file, token));

	while (token.lexeme.compare(",") == 0) {
		//consumes comma token
		token = lexer(file);
		exprList.push_back(Expr(file, token));
	}

	for (vector<string>::iterator it = exprList.begin(); it != exprList.end(); it++){
		assemblyCommands.push_back("li $v0, 1");
		assemblyCommands.push_back("move $a0, " + *it);
		assemblyCommands.push_back("syscall");
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
	whileCount++;

	string whil = "while" + to_string(whileCount) + ":";
	string endWhil = "endWhl" + to_string(whileCount);


	string cond = "";

	if (token.lexeme.compare("while") == 0) {
		//starts the while loop
		assemblyCommands.push_back(whil);
		
		token = lexer(file);
	}

	expect("(", token, file);
	//calls Cond function
	cond = Cond(file, token);
	//adds the endWhile jump to the condition statement
	cond = cond + endWhil;
	//add condition statement to the commands list
	assemblyCommands.push_back(cond);


	expect(")", token, file);

	expect("begin", token, file);

	//checks for stmtList if there
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0) {
		//call StmtList function
		StmtList(file, token);
	}

	expect("end", token, file);

	assemblyCommands.push_back("b " + whil);
	assemblyCommands.push_back(endWhil + ":");


	//Output rule
	/*cout << "While => while ( Cond ) begin [StmtList] end" << endl;*/
}

//Grace
string Cond(ifstream& file, LexTok& token) {

	string cond = "";

	string r1 = "";
	string r2 = "";

	string relo = "";

	//Call Expr function
	r1 = Expr(file, token);

	//Call RelOp function
	relo =  RelOp(file, token);

	//Call Expr function
	r2 = Expr(file, token);

	//complete initial statement of branch
	cond = relo + " " + r1 + ", " + r2 + ", ";

	return cond;
	//Ouput rule
	/*cout << "Cond => Expr RelOp Expr" << endl;*/
}

//Grace
string RelOp(ifstream& file, LexTok& token) {

	string relator = "";

	//Check if it is a RelOp
	if (token.token.compare("RelOp") == 0)
	{
		//equal
		if (token.lexeme.compare("=") == 0){
			//return opposite branch for the comparison
			relator = "bne";
		}
		//not equal
		else if (token.lexeme.compare("<>") == 0){
			relator = "beq";
		}
		//less than
		else if (token.lexeme.compare("<") == 0){
			relator = "bge";
		}
		//greater than
		else if (token.lexeme.compare(">") == 0){
			relator = "ble";
		}
		//less or equal
		else if (token.lexeme.compare("<=") == 0){
			relator = "bgt";
		}
		//greater or equal
		else{
			relator = "blt";
		}


		//consume lexeme
		token = lexer(file);
	}

	return relator;

}

//Grace
string Expr(ifstream& file, LexTok& token) {

	string r1 = "";
	string r2 = "";

	//Call Term function
	r1 = Term(file, token);

	//If lexeme is + or -
	if (token.lexeme.compare("+") == 0 || token.lexeme.compare("-") == 0) {
		if (token.lexeme.compare("+") == 0) {
			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Term(file, token);

			//gets add command
			assemblyCommands.push_back("add " + r1 + ", " + r1 + ", " + r2);
			//clears the register that is unused after calc
			tRegister[r2[2]] = "";

		}
		else if (token.lexeme.compare("-") == 0) {
			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Term(file, token);

			//gets sub command
			assemblyCommands.push_back("sub " + r1 + ", " + r1 + ", " + r2);
			//clears the register that is unused after calc
			tRegister[r2[2]] = "";
		}


	}

	//return the register with the value
	return r1;

	//Output rule
	/*cout << "Expr => Term { (+|-) Term }" << endl;*/
}

//Grace
string Term(ifstream& file, LexTok& token) {
	string r1 = "";
	string r2 = "";


	//Call Factor function
	r1 = Factor(file, token);

	//Check if lexeme is * or /
	if (token.lexeme.compare("*") == 0 || token.lexeme.compare("/") == 0) {
		//multiply
		if (token.lexeme.compare("*") == 0) {


			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Factor(file, token);
			//set commands
			assemblyCommands.push_back("mult " + r1 + ", " + r2);
			assemblyCommands.push_back("mflo " + r1);
			//clear unused register after use
			tRegister[r2[2]] = "";
		}
		//divide
		else if (token.lexeme.compare("/") == 0) {

			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Factor(file, token);
			//set commands
			assemblyCommands.push_back("div " + r1 + ", " + r2);
			assemblyCommands.push_back("mflo " + r1);
			//clear unused register after use
			tRegister[r1[2]] = "";
		}

	}

	//return register holding the value
	return r1;

	//Output rule
	/*cout << "Term => Factor { (*|/) Factor } " << endl;*/

}


//Grace
string Factor(ifstream& file, LexTok& token) {

	string reg = "$t";
	string in = "";

	//Check if identifier, intConst, realConst, strConst
	if (token.token.compare("Identifier") == 0)
	{
		in = token.lexeme;
		//consume token
		token = lexer(file);

		for (int i = 0; i < 10; i++) {
			//if the temp register is empty/not used yet
			if (tRegister[i] == "") {
				//assign it as the register to use for assignment
				reg = reg + to_string(i);
				tRegister[i] = in;
				break;
			}

		}

		assemblyCommands.push_back("lw " + reg + ", " + in);


	}
	else if (token.token.compare("IntConst") == 0)
	{
		in = token.lexeme;

		//Consume token
		token = lexer(file);

		for (int i = 0; i < 10; i++) {
			//if the temp register is empty/not used yet
			if (tRegister[i] == "") {
				//assign it as the register to use for assignment
				reg = reg + to_string(i);
				tRegister[i] = in;
				break;
			}

		}

		assemblyCommands.push_back("li " + reg + ", " + in);


		//Output rule
		/*cout << "Factor => IntConst" << endl;*/
	}

	else if (token.lexeme.compare("(") == 0)
	{

		expect("(", token, file);
		//Call Expr function
		reg = Expr(file, token);

		expect(")", token, file);

		//Output rule
		/*cout << "Factor => ( Expr )" << endl;*/
	}
	//returns the register the value was stored in
	return reg;

}

#endif
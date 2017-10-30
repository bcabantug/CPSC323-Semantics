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
string tRegister[10] = {};

vector<string> list;

vector <string> ifOrder;

vector<int> location;

int whileCount = 0;
int ifCount = 0;
int elsifCount = 0;


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
	for (vector<string>::iterator it = assemblyCommands.begin(); it != assemblyCommands.end(); it++) {
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
	bool error = false;

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

		if (list.empty() == true){
			list.push_back(d);
		}
		else{
			for (vector<string>::iterator il = list.begin(); il != list.end(); il++){
				if (il->compare(d) == 0){
					error = true;
					break;
				}
			}
		}
		
		if (error == true){
			cout << "error:" << d << " is declared more than once" << endl;
		}
		else{
			list.push_back(d);
		}
		

		d = d + ":    ";
		d = d + t;
		d = d + "0";

		//saves it in the decl vector to pass back
		decl.push_back(d);

	}

	//returns decl vector
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

		//checks if identifier was declared
		bool err = false;
		for (vector<string>::iterator it = list.begin(); it != list.end(); it++){
			if (token.lexeme.compare(*it) != 0){
				err = true;
			}
			else{
				err = false;
				break;
			}
		}//outputs error if found
		if (err == true){
			cout << "error: using " << token.lexeme << " without declaring first" << endl;
		}


		Assign(file, token);
	}
	//if token is read
	else if (token.lexeme.compare("read") == 0)
	{	//calls Read function and output rule
		Read(file, token);
	}
	//if token is write
	else if (token.lexeme.compare("write") == 0)
	{
		//calls Write function and output rule
		Write(file, token);
	}
	//if token is if
	else if (token.lexeme.compare("if") == 0)
	{	//calls If function and output rule
		//saves the count of if that is called
		ifCount++;
		If(file, token);
	}
	//if token is while
	else if (token.lexeme.compare("while") == 0)
	{	//calls While function and output rule
		While(file, token);
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

	//clear register after storing value back into the variable
	int rNum = reg[2] - '0';

	tRegister[rNum] = "";

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
	string errorVar = "";

	//first check if the identifier is present from the declaration list
	bool err = false;
	for (vector<string>::iterator ito = idents.begin(); ito != idents.end(); ito++){
		for (vector<string>::iterator it = list.begin(); it != list.end(); it++){
			if (ito->compare(*it) != 0){
				err = true;
				errorVar = *ito;
			}
			else{
				err = false;
				break;
			}
		}
	}
	if (err == true){
		cout << "error: using " << errorVar << " without declaring first" << endl;
	}

	for (vector<string>::iterator it = idents.begin(); it != idents.end(); it++) {
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

	int i = 0;

	//after consuming write lexeme
	expect("write", token, file);

	expect("(", token, file);

	//keeps on finding an expression as long as the token matches with the comma
	exprList.push_back(Expr(file, token));

	assemblyCommands.push_back("li $v0, 1");
	assemblyCommands.push_back("move $a0, " + exprList[i]);
	assemblyCommands.push_back("syscall");

	//clear register used
	int regNum = exprList[i][2] - '0';
	tRegister[regNum] = "";

	while (token.lexeme.compare(",") == 0) {
		//consumes comma token
		i++;
		token = lexer(file);
		exprList.push_back(Expr(file, token));

		assemblyCommands.push_back("li $v0, 1");
		assemblyCommands.push_back("move $a0, " + exprList[i]);
		assemblyCommands.push_back("syscall");
		//clear register used
		int regNum = exprList[i][2] - '0';
		tRegister[regNum] = "";
	}

	expect(")", token, file);

	expect(";", token, file);

}

//BRIAN 
void If(ifstream& file, LexTok& token) {
	string cond = "";

	//keeps the current count of how many if's have been encountered locally
	int countOfIf = ifCount;


	//consumes if
	expect("if", token, file);

	expect("(", token, file);

	//calls Cond function
	cond = Cond(file, token);

	expect(")", token, file);

	//pushes the first endIf to fulfill the first condition (incomplete statement, will add branch to go to after checking)
	assemblyCommands.push_back(cond);
	location.push_back(assemblyCommands.size() - 1);

	ifOrder.push_back("");

	expect("begin", token, file);

	//cals StmtList function
	StmtList(file, token);

	expect("end", token, file);


	if (token.lexeme.compare("elsif") == 0 || token.lexeme.compare("else") == 0){
		assemblyCommands.push_back("b endIf" + to_string(countOfIf));
	}

	//elseif statements if there are any
	if (token.lexeme.compare("elsif") == 0) {

		do {
			elsifCount++;
			//consume initial elsif
			expect("elsif", token, file);

			assemblyCommands.push_back("elseif" + to_string(elsifCount) + ":");

			//add the label to the associated branch
			assemblyCommands[location.back()] += "elseif" + to_string(elsifCount);
			location.pop_back();


			expect("(", token, file);
			//call Cond function
			cond = Cond(file, token);

			expect(")", token, file);

			assemblyCommands.push_back(cond);
			location.push_back(assemblyCommands.size() - 1);

			expect("begin", token, file);
			//call StmtList function
			StmtList(file, token);

			expect("end", token, file);

			if (token.lexeme.compare("elsif") == 0 || token.lexeme.compare("else") == 0){
				assemblyCommands.push_back("b endIf" + to_string(countOfIf));
			}


		} while (token.lexeme.compare("elsif") == 0); //continues the loop if elseif token is present

	}

	//checking for else statement

	if (token.lexeme.compare("else") == 0) {
		assemblyCommands.push_back("else" + to_string(countOfIf) + ":");
		//consume else token
		expect("else", token, file);
		//add the label to the associated branch
		assemblyCommands[location.back()] += "else" + to_string(countOfIf);
		location.pop_back();

		expect("begin", token, file);
		//call StmtList
		StmtList(file, token);
		expect("end", token, file);

		//assemblyCommands.push_back("b endIf" + to_string(countOfIf));

	}
	else{
		assemblyCommands[location.back()] += "endIf" + to_string(countOfIf);
		location.pop_back();

	}


	//branch to end the complete ifStatement
	assemblyCommands.push_back("endif" + to_string(countOfIf) + ":");
}

void While(ifstream& file, LexTok& token) {
	//consume while token/keyword
	whileCount++;

	string whil = "while" + to_string(whileCount);
	string endWhil = "endWhl" + to_string(whileCount);


	string cond = "";

	if (token.lexeme.compare("while") == 0) {
		//starts the while loop
		assemblyCommands.push_back(whil + ":");

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

	//clear registers used in comparison for use in the loop



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
	relo = RelOp(file, token);

	//Call Expr function
	r2 = Expr(file, token);

	//complete initial statement of branch
	cond = relo + " " + r1 + ", " + r2 + ", ";

	//clears the registers used in the conditional to free up for the following function
	int mo = r1[2] - '0';
	tRegister[mo] = "";

	mo = r2[2] - '0';
	tRegister[mo] = "";


	return cond;
}

//Grace
string RelOp(ifstream& file, LexTok& token) {

	string relator = "";

	//Check if it is a RelOp
	if (token.token.compare("RelOp") == 0)
	{
		//equal
		if (token.lexeme.compare("=") == 0) {
			//return opposite branch for the comparison
			relator = "bne";
		}
		//not equal
		else if (token.lexeme.compare("<>") == 0) {
			relator = "beq";
		}
		//less than
		else if (token.lexeme.compare("<") == 0) {
			relator = "bge";
		}
		//greater than
		else if (token.lexeme.compare(">") == 0) {
			relator = "ble";
		}
		//less or equal
		else if (token.lexeme.compare("<=") == 0) {
			relator = "bgt";
		}
		//greater or equal
		else {
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
	while (token.lexeme.compare("+") == 0 || token.lexeme.compare("-") == 0) {
		if (token.lexeme.compare("+") == 0) {
			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Term(file, token);

			//gets add command
			assemblyCommands.push_back("add " + r1 + ", " + r1 + ", " + r2);
			//clears the register that is unused after calc
			int mo = r2[2] - '0';
			tRegister[mo] = "";

		}
		else if (token.lexeme.compare("-") == 0) {
			//consume token
			token = lexer(file);

			//call the factor if it is factor
			r2 = Term(file, token);

			//gets sub command
			assemblyCommands.push_back("sub " + r1 + ", " + r1 + ", " + r2);
			//clears the register that is unused after calc
			int mo = r2[2] - '0';
			tRegister[mo] = "";
		}


	}

	//return the register with the value
	return r1;
}

//Grace
string Term(ifstream& file, LexTok& token) {
	string r1 = "";
	string r2 = "";


	//Call Factor function
	r1 = Factor(file, token);

	//Check if lexeme is * or /
	while (token.lexeme.compare("*") == 0 || token.lexeme.compare("/") == 0) {
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
			int mo = r2[2] - '0';

			tRegister[mo] = "";
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
			int mo = r2[2] - '0';
			tRegister[mo] = "";
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

		//check if variable is declared
		bool err = false;

		for (vector<string>::iterator it = list.begin(); it != list.end(); it++){
			if (token.lexeme.compare(*it) != 0){
				err = true;
			}
			else{
				err = false;
				break;
			}
		}
		if (err == true){
			cout << "error: using " << token.lexeme << " without declaring first" << endl;
		}
		

		//consume token
		token = lexer(file);

		for (int i = 0; i < 10; i++) {
			//if the temp register is empty/not used yet
			if (tRegister[i].compare("") == 0) {
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
			if (tRegister[i].compare("") == 0) {
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
#include "SHeader.h"
string er[7] = {"Parenthesis Error", "No Variable", "", "No list size", "No Semicolon", "No Equation", "Comma Error"};

int main() {
	try{
		changeToBuffer();
		Execute(1000);
	}
	catch(int exception) {
		cout<<er[exception]; 
	}
}

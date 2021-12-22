#include <bits/stdc++.h>
using namespace std;
enum TknKind {
	Lparen = 0, Rparen, Plus, Minus, Multiple, Divide = 5,
	Percent, Exp, Letter, Comma, If = 10,
	Else, Elif, For, While, Number = 15, 
	Quot, Semi, Var, Equal, Lmparen = 20,
	Rmparen, String, Typeint, Typefloat, Typestring=25,
	Typebool, Plpl, Mama, Pleq, Mieq=30,
	Muleq, Diveq, Pereq, Expeq, Print=35,
	Bigeq, Smalleq, Big, Small, And=40,
	Or, Eqeq, Differ, Input, Dot=45,
	Others
};
struct KeyW {
	const char *keyName;
	TknKind keyKind;
};
struct Token {
	TknKind tk;
	string txt = "";
	int intVal = 0;
	int endnum = 0;
	int renum = 0;
	int inputisint = 0;
};
enum SymKind {
	Intkind, Floatkind, Stringkind, Boolkind
};
struct SymTbl {
	string name;
	SymKind myKind;
	int adrs;
	int valueint = 0; string valuestring = "";
	void clear() {
		name=""; myKind = Intkind;
	}
};
Token buffer[1000];
TknKind a[256];
KeyW Tble[11] = {{"if", If}, {"for", For}, {"while", While}, {"elif", Elif}, {"else", Else},
{"int", Typeint}, {"float", Typefloat}, {"bool", Typebool}, {"string", Typestring}, 
{"sout", Print}, {"sin", Input}};
TknKind isTwo(int cnt, char c) {
	if(cnt==0) return Others;
	string x = buffer[cnt-1].txt + c;
	if(x=="++" && (cnt>1 && buffer[cnt-2].tk==Var)) return Plpl;
	if(x=="--" && (cnt>1 && buffer[cnt-2].tk==Var)) return Mama;
	if(x=="+=") return Pleq;
	if(x=="-=") return Mieq;
	if(x=="*=") return Muleq;
	if(x=="/=") return Diveq;
	if(x=="%=") return Pereq;
	if(x=="^=") return Expeq;
	if(x=="<=") return Bigeq;
	if(x==">=") return Smalleq;
	if(x=="&&") return And;
	if(x=="||") return Or;
	if(x=="==") return Eqeq;
	if(x=="!=") return Differ;
	return Others;
}
void initTkn() {
	for(int i = 0; i<256; i++) a[i] = Others;
	for(int i = 'a'; i<='z'; i++) a[i] = Letter;
	for(int i = 'A'; i<='Z'; i++) a[i] = Letter;
	a['_'] = Letter;
	for(int i = '0'; i<='9'; i++) a[i] = Number;
	a['"'] = Quot; a['('] = Lparen; a[')'] = Rparen; a['+'] = Plus; a['-'] = Minus;
	a['*'] = Multiple; a['/'] = Divide; a['%'] = Percent; a['^'] = Exp;
	a[','] = Comma; a[';'] = Semi; a['='] = Equal;  a['{'] = Lmparen; a['}'] = Rmparen;
	a['<'] = Big; a['>'] = Small; a['.'] = Dot;
}
vector<SymTbl> tbl;

/*class MymemoryInt {
	public:
		vector<int> mem;
		void set(int adrs, int dt) { if(mem.size()>adrs) mem[adrs] = dt; else mem.push_back(dt);}
		void set(int adrs) { mem[adrs] = 0; }
		void add(int adrs, int dt) { mem[adrs] = dt; }
		int get(int adrs) { return mem[adrs]; }
};

MymemoryInt Dmem;*/

int Search(string x) {
	for(int i = 0; i<tbl.size(); i++) {
		if(tbl[i].name==x) {
			return i;
		}
	}
	return -1;
}

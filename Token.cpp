#include "MainHeader.h"
using namespace std;
ifstream fin;
vector<int> endfind, parenendfind;
Token buffer[1000];
TknKind a[256];
KeyW Tble[10] = {{"if", If}, {"for", For}, {"while", While}, {"elif", Elif}, {"else", Else},
{"int", Typeint}, {"float", Typefloat}, {"bool", Typebool}, {"string", Typestring}, 
{"print", Print}};
TknKind isTwo(int cnt, char c) {
	if(cnt==0) return Others;
	string x = buffer[cnt-1].txt + c;
	if(x=="++") return Plpl;
	if(x=="--") return Mama;
	if(x=="+=") return Pleq;
	if(x=="-=") return Mieq;
	if(x=="*=") return Muleq;
	if(x=="/=") return Diveq;
	if(x=="%=") return Pereq;
	if(x=="^=") return Expeq;
	if(x=="<=") return Bigeq;
	if(x==">=") return Smalleq;
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
	a['<'] = Big; a['>'] = Small;
}
int changeToBuffer() {
	fin.open("Source.txt"); if(!fin) exit(1);
	int cnt = 0;
	initTkn();
	char c = fin.get();
	while(true) {
		if(c==EOF) break;
		if(c==' ' || c=='\n') {
			c = fin.get();
			continue;
		}
		if(a[c]==Letter) {
			string intxt = "";
			while(a[c]==Letter) {
				intxt += c;
				c = fin.get();
			}
			if(a[c]==Number) return 0;
			TknKind t = Var;
			for(int i = 0; i<10; i++) {
				if(intxt==(string)Tble[i].keyName) {
					t = Tble[i].keyKind;
					break;
				}
			}
			Token cur;
			cur.tk = t;
			cur.txt = intxt;
			buffer[cnt] = cur;
			cnt++;
		}
		else if(a[c]==Number) {
			string intxt = "";
			int val = 0;
			while(a[c]==Number) {
				intxt += c;
				val = val*10+c-48;
				c = fin.get();
			}
			if(a[c]==Letter) return 0;
			TknKind t = Number;
			Token cur;
			cur.tk = t;
			cur.txt = intxt;
			cur.intVal = val;
			buffer[cnt] = cur;
			cnt++;
		}
		else {
			if(c=='(') parenendfind.push_back(cnt);
			if(c==')') {
				int last = parenendfind.back();
				parenendfind.pop_back();
				buffer[last].endnum = cnt;
			}
			if(c=='{') endfind.push_back(cnt);
			if(c=='}') {
				int last = endfind.back();
				endfind.pop_back();
				buffer[last].endnum = cnt;
			}
			if(c=='"') {
				Token cur;
				cur.tk = Quot;
				cur.txt = c;
				buffer[cnt] = cur;
				buffer[cnt+2] = cur;
				cnt++;
				string strtxt = "";
				c = fin.get();
				while(c!='"') {
					strtxt+=c;
					c = fin.get();
				}
				cur.tk = String;
				cur.txt = strtxt;
				buffer[cnt] = cur; cnt+=2;
				c = fin.get();
				continue;
			}
			TknKind ist = isTwo(cnt, c);
			if(ist!=Others) {
				Token cur;
				cur.tk = ist;
				cur.txt = buffer[cnt-1].txt+c;
				cnt--;
				buffer[cnt] = cur;
				cnt++;
				c = fin.get();
				continue;
			}
			Token cur;
			cur.tk = a[c];
			cur.txt = c;
			buffer[cnt] = cur;
			cnt++;
			c = fin.get();
		}
	}
	for(int i = 0; i<1000; i++) {
		if(buffer[i].txt=="") break;
		cout<<i<<" "<<buffer[i].tk<<" "<<buffer[i].intVal<<" "<<buffer[i].txt;
		if(buffer[i].endnum!=0) cout<<" "<<buffer[i].endnum;
		cout<<"\n";
	}
}

//	NOTIFICATION
//		It is simple-versioned-Header of Language S
//				Not for developing

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
	Lbparen, Rbparen, Typelist, Others
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
	Intkind, Floatkind, Stringkind, Boolkind, Autokind
};
struct SymTbl {
	string name;
	SymKind myKind = Autokind;
	int adrs;
	int valueint = 0; string valuestring = "";
	int isList = 0;
	void clear() {
		name=""; myKind = Intkind;
	}
};
Token buffer[1000];
TknKind a[256];
KeyW Tble[12] = {{"if", If}, {"for", For}, {"while", While}, {"elif", Elif}, {"else", Else},
{"int", Typeint}, {"float", Typefloat}, {"bool", Typebool}, {"string", Typestring}, 
{"sout", Print}, {"sin", Input}, {"list", Typelist}};
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
	a['<'] = Big; a['>'] = Small; a['.'] = Dot; a['['] = Lbparen; a[']'] = Rbparen;
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
	throw 1;
}

//Calc
int pri[1000] = {0,};
int index = 0;
int perc();
int exp();
int exp(int a, int jisu) {
	int result = 1;
	for(int i = 0; i<jisu; i++) {
		result *= a;
	}
	return result;
}
struct forinput {
	int type = 0; //0:int 1:string
	int valint = 0;
	string valstring = "";
};
forinput inp() {
	forinput inpret;
	if(buffer[index+1].tk!=Dot) {
		string ret = "";
		getline(cin, ret);
		inpret.type = 1;
		inpret.valstring = ret;
		index++;
		return inpret;
	}
	else {
		index+=2;
		if(buffer[index].tk==Typeint) {
			index++;
			int ret;
			cin>>ret;
			inpret.type = 0;
			inpret.valint = ret;
			return inpret;
		}
		return inpret;
	}
}
forinput middleexpression();
forinput smallexpression();
//num=0: <,>,<=,>=  num=1: +,- num=2: *, /  num=3: %  num=4: ^
forinput express(int num) {
	if(num==5) return smallexpression();
	forinput result = express(num+1);
	if(num==-1) {
		if(buffer[index].tk==And) {
			index++;
			if(result.type==1) result.valint = 1;
			result.valint = express(num+1).valint && result.valint;
			result.type = 0;
		}
		else if(buffer[index].tk==Or) {
			index++;
			if(result.type==1) result.valint = 1;
			result.valint = express(num+1).valint || result.valint;
			result.type = 0;
		}
		return result;
	}
	else if(num==0) {
		if(buffer[index].tk==Big) {
			index++;
			result.valint = express(num+1).valint > result.valint;
		}
		else if(buffer[index].tk==Small) {
			index++;
			result.valint = express(num+1).valint < result.valint;
		}
		else if(buffer[index].tk==Bigeq) {
			index++;
			result.valint = express(num+1).valint >= result.valint;
		}
		else if(buffer[index].tk==Smalleq) {
			index++;
			result.valint = express(num+1).valint <= result.valint;
		}
		else if(buffer[index].tk==Eqeq) {
			index++;
			forinput ex = express(num+1);
			if(result.type!=ex.type) throw(1);
			result.valint = (ex.valint == result.valint) && (ex.valstring==result.valstring);
			result.valstring = "";
			result.type = 0;
		}
		else if(buffer[index].tk==Differ) {
			index++;
			forinput ex = express(num+1);
			if(result.type!=ex.type) throw(1);
			result.valint = (ex.valint != result.valint) || (ex.valstring!=result.valstring);
			result.valstring = "";
			result.type = 0;
		}
		return result;
	}
	else if(num==1) {
		while(buffer[index].tk==Plus || buffer[index].tk==Minus) {
			if(buffer[index].tk==Plus) {
				index++;
				if(result.type==0)
				result.valint += express(num+1).valint;
				else result.valstring += express(num+1).valstring;
			}
			else {
				index++;
				result.valint -= express(num+1).valint;
			}
		}
	}
	else if(num==2) {
		while(buffer[index].tk==Multiple || buffer[index].tk==Divide) {
			if(buffer[index].tk==Multiple) {
				index++;
				int qwer = express(num+1).valint;
				if(result.type==0)
				result.valint *= qwer;
				else {
					string firstval = result.valstring;
					for(int newi = 1; newi<qwer; newi++) {
						result.valstring += firstval;
					}
				}
			}
			else {
				index++;
				result.valint /= express(num+1).valint;
			}
		}
	}
	else if(num==3) {
		while(buffer[index].tk==Percent) {
			index++;
			result.valint %= express(num+1).valint;
		}
	}
	else if(num==4) {
		while(buffer[index].tk==Exp) {
			index++;
			forinput x = smallexpression();
			if(result.type!=0) throw(1);
			result.valint = exp(result.valint, x.valint);
		}
	}
	return result;
}
forinput smallexpression() {
	forinput res;
	if(buffer[index].tk==Var) {
		int serresult = Search(buffer[index].txt);
		if(tbl[serresult].isList==0) {
			index++;
		}
		else {
			index+=2;
			if(buffer[index-1].tk!=Lbparen) throw(3);
			serresult += express(-1).valint;
			index++;
		}
		res.valint = tbl[serresult].valueint;
		res.valstring = tbl[serresult].valuestring;
		if(res.valstring!="") res.type = 1;
	}
	else if(buffer[index].tk==Number) {
		int ret = buffer[index].intVal;
		index++;
		res.type = 0; res.valint = ret;
	}
	else if(buffer[index].tk==Plus || buffer[index].tk==Minus) {
		int fl = 1;
		while(buffer[index].tk==Plus || buffer[index].tk==Minus) {
			if(buffer[index].tk==Minus) fl*=-1;
			index++;
		}
		res.valint = fl*smallexpression().valint;
	}
	else if(buffer[index].tk==Lparen) {
		index++;
		forinput bigrs = express(-1);
		index++;
		return bigrs;
	}
	else if(buffer[index].tk==Input) {
		return inp();
	}
	else if(buffer[index].tk==Quot) {
		res.valstring = buffer[index+1].txt;
		res.type = 1;
		index+=3;
	}
	else {
		throw(1);
	}
	return res;
}

//MainHeader
ifstream fin;
vector<int> endfind, parenendfind;
int changeToBuffer() {
	fin.open("Source.txt"); if(!fin) exit(1);
	int cnt = 0;
	initTkn();
	char c = fin.get();
	while(true) {
		if(c==EOF) break;
		if(c==' ' || c=='\n' || c=='\t') {
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
			for(int i = 0; i<12; i++) {
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
			if(c=='\"') {
				Token cur;
				cur.tk = Quot;
				cur.txt = c;
				buffer[cnt] = cur;
				buffer[cnt+2] = cur;
				cnt++;
				string strtxt = "";
				c = fin.get();
				char d = '\0';
				while(c!='\"' || d!='\0') {
					if(c=='\\') {
						d = fin.get();
						if(d=='\\') c = '\\';
						else if(d=='n') c = '\n';
						else if(d=='t') c = '\t';
						else if(d=='\"') c = '\"';
						else if(d=='\'') c = '\'';
					}
					strtxt+=c;
					c = fin.get();
					d = '\0';
					while(c=='\n') {
						c = fin.get();
					}
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
	if(endfind.size()!=0 || parenendfind.size()!=0) throw(0);
	for(int i = 0; i<1000; i++) {
		if(buffer[i].txt=="") break;
		cout<<i<<" "<<buffer[i].tk<<" "<<buffer[i].intVal<<" "<<buffer[i].txt;
		if(buffer[i].endnum!=0) cout<<" "<<buffer[i].endnum;
		cout<<"\n";
	}
}
struct loopnum {
	int renum = 0;
	int endnum = 0;
};
int Execute(int lastin) {
	vector<loopnum> loopcnt;
	while(buffer[index].txt!="" && index<lastin) {
		//cout<<index<<" ";
		switch(buffer[index].tk) {
			case Typeint:
				{
				SymTbl tb;
				tb.myKind = Intkind;
				tb.name = buffer[index+1].txt;
				tb.adrs = tbl.size();
				index+=3;
				if(buffer[index].tk!=Semi) {
					tb.valueint = express(-1).valint;
					if(buffer[index].tk!=Semi) throw(1);
					index+=1;
				}
				tbl.push_back(tb);
				break;}
			case Typestring:
				{SymTbl tb;
				tb.myKind = Stringkind;
				tb.name = buffer[index+1].txt;
				tb.adrs = tbl.size();
				index+=3;
				if(buffer[index].tk!=Semi) {
					tb.valuestring = express(-1).valstring;
					if(buffer[index].tk!=Semi) throw(1);
					index+=1;
				}
				tbl.push_back(tb);
				break;}
			case Typelist:
				{
				if(buffer[index+1].tk!=Lbparen) throw(3);
				index+=2;
				int s = express(-1).valint;
				if(s<1) throw(3);
				for(int lz = 0; lz<s; lz++) {
					SymTbl tb;
					tb.name = buffer[index+1].txt;
					tb.myKind = Autokind;
					tb.adrs = tbl.size(); tb.isList = 1;
					tbl.push_back(tb);
				}
				if(buffer[index+2].tk!=Semi) throw(4);
				index+=3;
				break;}
			case Print:
				{index+=2; if(buffer[index-1].tk!=Lparen) throw(0);
				string soutresult = "";
				forinput x = express(-1);
				//cout<<'|'<<x.valstring<<'|';
				if(x.type==1) {
					soutresult += x.valstring;
				}
				else /*(buffer[index].tk==Number || (buffer[index].tk==Var && tbl[Search(buffer[index].txt)].myKind==Intkind))*/ {
					soutresult += to_string(x.valint);
				}
				while(buffer[index].tk==Comma) {
					soutresult += " ";
					index++;
					forinput x = express(-1);
					if(x.type==1) {
						soutresult += x.valstring;
					}
					else {
						soutresult += to_string(x.valint);
					}
				}
				cout<<soutresult;
				index+=1;
 				if(buffer[index].tk!=Semi) throw(1);
 				index++;
				break;}
			case If: case Elif:
				{index+=2;
				if(buffer[index-1].txt!="(") throw(1);
				if(express(-1).valint) {
					if(buffer[index+1].txt!="{") throw(1);
					int lastone = buffer[index+1].endnum;
					index+=2;
					Execute(lastone);
					index++;
					while(buffer[index].tk==Elif || buffer[index].tk==Else) {
						if(buffer[index].tk==Else)
						index = buffer[index+1].endnum+1;
						else {
							index = buffer[index+1].endnum+1;
							index = buffer[index].endnum+1;
						}
					}
				}
				else {
					index = buffer[index+1].endnum+1;
				}
				break;}
			case Else:
				{
				if(buffer[index+1].txt!="{") throw(1);
				int lastone = buffer[index+1].endnum;
				index+=2;
				Execute(lastone);
				index++;
				break;}
			case Var:
				{
				int ad = Search(buffer[index].txt);
				if(tbl[ad].isList) {
					index+=2;
					ad += express(-1).valint;
				}
				if(tbl[ad].myKind==Intkind) {
					if(buffer[index+1].tk==Equal) {
						index+=2;
						tbl[ad].valueint = express(-1).valint;
					}
					else if(buffer[index+1].tk==Pleq) {
						index+=2;
						tbl[ad].valueint += express(-1).valint;
					}
					else if(buffer[index+1].tk==Mieq) {
						index+=2;
						tbl[ad].valueint -= express(-1).valint;
					}
					else if(buffer[index+1].tk==Muleq) {
						index+=2;
						tbl[ad].valueint *= express(-1).valint;
					}
					else if(buffer[index+1].tk==Diveq) {
						index+=2;
						tbl[ad].valueint /= express(-1).valint;
					}
					else if(buffer[index+1].tk==Pereq) {
						index+=2;
						tbl[ad].valueint %= express(-1).valint;
					}
					else if(buffer[index+1].tk==Expeq) {
						index+=2;
						tbl[ad].valueint = exp(tbl[ad].valueint, express(-1).valint);
					}
					else if(buffer[index+1].tk==Plpl) {
						index+=2;
						tbl[ad].valueint += 1;
					}
					else if(buffer[index+1].tk==Mama) {
						index+=2;
						tbl[ad].valueint -= 1;
					}
				}
				else if(tbl[ad].myKind==Stringkind) {
					index+=2;
					tbl[ad].valuestring = express(-1).valstring;
				}
				else {
					if(buffer[index+1].tk!=Equal) throw(5);
					index+=2;
					forinput newex = express(-1);
					if(newex.valstring!="") {
						tbl[ad].myKind = Stringkind;
						tbl[ad].valuestring = newex.valstring;
					}
					else {
						tbl[ad].myKind = Intkind;
						tbl[ad].valueint = newex.valint;
					}
				}
				if(buffer[index].tk!=Semi) throw(1);
				index++;
				break;}
			case While:
				{
				int re = index;
				index++;
				if(buffer[index].tk!=Lparen) throw(1);
				index++;
				int va = express(-1).valint;
				//cout<<"!"<<va<<"!";
				if(va) {
					if(buffer[index+1].txt!="{") throw(1);
					index+=2;
					loopnum ln; ln.endnum = buffer[index-1].endnum; ln.renum = re;
					loopcnt.push_back(ln);
					Execute(buffer[index-1].endnum);
					index = re;
					loopcnt.pop_back();
				}
				else {
					index = buffer[index+1].endnum+1;
				}
				break;}
			case For:
				{
				index+=2;
				if(buffer[index-1].tk!=Lparen) throw(0);
				int ad = Search(buffer[index].txt);
				if(buffer[index+1].tk!=Comma) throw(6);
				index+=2;
				tbl[ad].valueint = express(-1).valint;
				if(buffer[index].tk!=Comma) throw(6);
				index+=1;
				forinput until = express(-1);
				if(buffer[index].tk!=Rparen) throw(0);
				if(buffer[index+1].tk!=Lmparen) throw(0);
				if(until.valint<=tbl[ad].valueint) {
					index = buffer[index+1].endnum+1; break;
				}
				index+=2;
				while(tbl[ad].valueint<until.valint) {
					int re = index;
					loopnum ln; ln.endnum = buffer[index-1].endnum; ln.renum = re;
					loopcnt.push_back(ln);
					Execute(buffer[index-1].endnum);
					index = re;
					loopcnt.pop_back();
					tbl[ad].valueint = tbl[ad].valueint+1;
				}
				index=buffer[index-1].endnum+1;
				break;}
			default:
				cout<<"No command name with "<<buffer[index].txt;
				throw(2);
		}
		//cout<<"!!";
	}
	//cout<<"??";
}

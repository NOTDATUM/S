#include "Calc.h"
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
			for(int i = 0; i<11; i++) {
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
				{SymTbl tb;
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
			case Print:
				{index+=2;
				forinput x = express(-1);
				//cout<<'|'<<x.valstring<<'|';
				if(x.type==1) {
					cout<<x.valstring;
					index+=1;
				}
				else /*(buffer[index].tk==Number || (buffer[index].tk==Var && tbl[Search(buffer[index].txt)].myKind==Intkind))*/ {
					int ser = x.valint; index+=1; cout<<ser;
				}
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
			default:
				cout<<"No command name with "<<buffer[index].txt;
				throw(1);
		}
		//cout<<"!!";
	}
	//cout<<"??";
}

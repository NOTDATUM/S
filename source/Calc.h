#include "MainStruct.h"
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
				result.valint *= express(num+1).valint;
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
		index++;
		res.valint = tbl[serresult].valueint;
		res.valstring = tbl[serresult].valuestring;
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


//#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
//#include "C:/Wichtig/System/Static/Container/Tree.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Container/Tree.h"

#define ARG_X				1.0E-40f

#define MFUNCTION_NONE		0
#define MFUNCTION_VALUE		1
#define MFUNCTION_VARIABLE	2
#define MFUNCTION_X			3
#define MFUNCTION_CALC		4
#define MFUNCTION_OP		5
#define MFUNCTION_OP_ADD	6
#define MFUNCTION_OP_SUB	7
#define MFUNCTION_OP_MUL	8
#define MFUNCTION_OP_DIV	9
#define MFUNCTION_OP_NEG	10
#define MFUNCTION_OP_POW	11
#define MFUNCTION_OP_LBK	12
#define MFUNCTION_OP_RBK	13
#define MFUNCTION_OP_COM	14

typedef struct Token {
	char* str;
	int Type;
} Token;

char Token_OpState(int State){
	if(State==MFUNCTION_OP)		return 1;
	if(State==MFUNCTION_OP_ADD) return 1;
	if(State==MFUNCTION_OP_SUB) return 1;
	if(State==MFUNCTION_OP_MUL) return 1;
	if(State==MFUNCTION_OP_DIV) return 1;
	if(State==MFUNCTION_OP_NEG) return 1;
	if(State==MFUNCTION_OP_POW) return 1;
	if(State==MFUNCTION_OP_LBK) return 1;
	if(State==MFUNCTION_OP_RBK) return 1;
	if(State==MFUNCTION_OP_COM) return 1;
	return 0;
}

char Token_Op(char* str){
	if(CStr_Cmp(str,"+")) return 1;
	if(CStr_Cmp(str,"-")) return 1;
	if(CStr_Cmp(str,"*")) return 1;
	if(CStr_Cmp(str,"/")) return 1;
	if(CStr_Cmp(str,"^")) return 1;
	if(CStr_Cmp(str,"(")) return 1;
	if(CStr_Cmp(str,")")) return 1;
	if(CStr_Cmp(str,",")) return 1;
	return 0;
}

int Token_GetOp(char* str,int argc){
	if(CStr_Cmp(str,"+") && argc==2) 	return MFUNCTION_OP_ADD;
	if(CStr_Cmp(str,"-") && argc==2) 	return MFUNCTION_OP_SUB;
	if(CStr_Cmp(str,"*") && argc==2) 	return MFUNCTION_OP_MUL;
	if(CStr_Cmp(str,"/") && argc==2) 	return MFUNCTION_OP_DIV;
	if(CStr_Cmp(str,"-") && argc==1) 	return MFUNCTION_OP_NEG;
	if(CStr_Cmp(str,"^") && argc==2) 	return MFUNCTION_OP_POW;
	if(CStr_Cmp(str,"(")) 				return MFUNCTION_OP_LBK;
	if(CStr_Cmp(str,")")) 				return MFUNCTION_OP_RBK;
	if(CStr_Cmp(str,",")) 				return MFUNCTION_OP_COM;
	return MFUNCTION_NONE;
}

int Token_OpArc(int Tok){
	if(Tok==MFUNCTION_OP_ADD) return 2;
	if(Tok==MFUNCTION_OP_SUB) return 2;
	if(Tok==MFUNCTION_OP_MUL) return 2;
	if(Tok==MFUNCTION_OP_DIV) return 2;
	if(Tok==MFUNCTION_OP_NEG) return 1;
	if(Tok==MFUNCTION_OP_POW) return 2;
	if(Tok==MFUNCTION_OP_LBK) return 0;
	if(Tok==MFUNCTION_OP_RBK) return 0;
	if(Tok==MFUNCTION_OP_COM) return 0;
	return 0;
}

int Token_PrecOp(int Tok){
	if(Tok==MFUNCTION_OP_ADD) 	return 2;
	if(Tok==MFUNCTION_OP_SUB) 	return 3;
	if(Tok==MFUNCTION_OP_MUL) 	return 4;
	if(Tok==MFUNCTION_OP_DIV) 	return 5;
	if(Tok==MFUNCTION_OP_NEG) 	return 6;
	if(Tok==MFUNCTION_OP_POW) 	return 7;
	if(Tok==MFUNCTION_OP_LBK) 	return 1;
	if(Tok==MFUNCTION_OP_RBK) 	return 1;
	if(Tok==MFUNCTION_OP_COM) 	return 1;
	return 0;
}

int Token_Type(char* str){
	if(!str) 													return MFUNCTION_NONE;
	if(Double_Parse(str,DOUBLE_UNSIGNED)!=DOUBLE_PARSE_ERROR) 	return MFUNCTION_VALUE;
	if(Token_Op(str)) 											return MFUNCTION_OP;
	if(CStr_Cmp(str,"x")) 										return MFUNCTION_X;
	return MFUNCTION_VARIABLE;
}

typedef struct Operation {
	float Const1;
	float (*Func)(Vector*);
} Operation;

Operation Operation_New(float Const1,float Const2,float (*Func)(Vector*)){
	Operation op;
	op.Const1 = Const1;
	op.Func = Func;
	return op;
}

typedef Tree MFunction;

MFunction MFunction_New(){
	MFunction Func = Tree_New();	
	return Func;
}

float MFunction_Add(float Left,float Right){
	return Left + Right;
}
float MFunction_Sub(float Left,float Right){
	return Left - Right;
}
float MFunction_Mul(float Left,float Right){
	return Left * Right;
}
float MFunction_Div(float Left,float Right){
	return Left / Right;
}
float MFunction_Neg(float Left,float Right){
	return -Right;
}
float MFunction_Pow(float Left,float Right){
	return powf(Left,Right);
}

float MFunction_Sqrt(float a){
	return sqrtf(a);
}
float MFunction_Sin(float a){
	return sinf(a);
}
float MFunction_Cos(float a){
	return cosf(a);
}
float MFunction_Tan(float a){
	return tanf(a);
}

void MFunction_Set(MFunction* Funcs,int Layer,int* Indices,float Const1,float Const2,float (*Func)(Vector*)){
	Tree_Set(Funcs,Layer,Indices,(Operation[]){ {Const1,Func} },sizeof(Operation));
}

void MFunction_Push(MFunction* Funcs,int Layer,int* Indices,float Const1,float Const2,float (*Func)(Vector*)){
	Tree_Add(Funcs,Layer,Indices,(Operation[]){ {Const1,Func} },sizeof(Operation));
}

void* MFunction_GetFunc(int State){ // -> float (*)(float,float)
	if(State==MFUNCTION_OP_ADD) return (void*)MFunction_Add;
	if(State==MFUNCTION_OP_SUB) return (void*)MFunction_Sub;
	if(State==MFUNCTION_OP_MUL) return (void*)MFunction_Mul;
	if(State==MFUNCTION_OP_DIV) return (void*)MFunction_Div;
	if(State==MFUNCTION_OP_NEG) return (void*)MFunction_Neg;
	if(State==MFUNCTION_OP_POW) return (void*)MFunction_Pow;
	return NULL;
}

void MFunction_PrintToken(Vector* Toks){
	printf("Tokens -----------\n");
	for(int i = 0;i<Toks->size;i++){
		Token* t = (Token*)Vector_Get(Toks,i);
		if(t->Type!=MFUNCTION_CALC) printf("Tok: %s -> %d\n",t->str,t->Type);
		else 						printf("Tok: %p -> %d\n",t->str,t->Type);
	}
	printf("-----------------\n");
}

float MFunction_CalculateBranch(Branch* b,float x){
	float Out = 0.0f;

	Operation* op = (Operation*)b->Memory;
    if(op){
		if(op->Func){
			Vector v = Vector_New(sizeof(float));
			for(int i = 0;i<b->Childs.size;i++){
				Branch* branch = (Branch*)Vector_Get(&b->Childs,i);
				float out = MFunction_CalculateBranch(branch,x);
				Vector_Push(&v,&out);
			}
			Out = op->Func(&v);
			Vector_Free(&v);
		}else if(op->Const1==ARG_X)	Out = x;
		else  						Out = op->Const1;
	}
	return Out;
}

float MFunction_Calculate(MFunction* Func,float x){
	float Out = MFunction_CalculateBranch(Func->Root,x);
	return Out;
}

void Token_Push(Vector* Tokens,String* Buffer,int* State,int* Argc){
	char* out = String_CStr(Buffer);
	int StateIn = *State;
	if(StateIn==MFUNCTION_OP) StateIn = Token_GetOp(out,*Argc+1);
	Vector_Push(Tokens,(Token[]){out,StateIn});
	String_Clear(Buffer);
	*State = MFUNCTION_NONE;
	(*Argc)++;
	if(Token_OpState(StateIn) && StateIn!=MFUNCTION_OP_RBK) 		*Argc = 0;
	else if(Token_OpState(StateIn) && StateIn==MFUNCTION_OP_RBK)	*Argc = 1;
}

Vector MFunction_Parse(char* str){
	int Size = CStr_Size(str);
	Vector Tokens = Vector_New(sizeof(Token));
	String Buffer = String_New();
	
	int Argc = 0;
	int State = MFUNCTION_NONE;
	for(int i = 0;i<Size;i++){
		char c = str[i];
		if(c==' '){
			if(!Token_OpState(State) && State!=MFUNCTION_NONE && Buffer.size>0){
				Token_Push(&Tokens,&Buffer,&State,&Argc);
			}
		}else if(State==MFUNCTION_NONE){
			String_AppendChar(&Buffer,c);
			State = Token_Type((char[]){c,'\0'});
		}else if(State==MFUNCTION_VALUE){
			String_AppendChar(&Buffer,c);
			char* cstr = String_CStr(&Buffer);
			if(Double_Parse(cstr,DOUBLE_UNSIGNED)==DOUBLE_PARSE_ERROR){
				String_Pop(&Buffer);
				i--;
				Token_Push(&Tokens,&Buffer,&State,&Argc);
			}
			free(cstr);
		}else if(State==MFUNCTION_OP){
			String_AppendChar(&Buffer,c);
			char* cstr = String_CStr(&Buffer);
			if(!Token_Op(cstr)){
				String_Pop(&Buffer);
				i--;
				Token_Push(&Tokens,&Buffer,&State,&Argc);
			}
			free(cstr);
		}else if(State==MFUNCTION_VARIABLE || State==MFUNCTION_X){
			if(Token_Op((char[]){c,'\0'})){
				Token_Push(&Tokens,&Buffer,&State,&Argc);
				i--;
			}else{
				String_AppendChar(&Buffer,c);
			}
		}
	}
	Token_Push(&Tokens,&Buffer,&State,&Argc);

	String_Free(&Buffer);
	return Tokens;
}

Vector MFunction_SYA(Vector Toks){
	Vector Stack = Vector_New(sizeof(Token));
    Vector Out = Vector_New(sizeof(Token));
    for(int i = 0;i<Toks.size;i++){
        Token* t = (Token*)Vector_Get(&Toks,i);
		if(!Token_OpState(t->Type)){ 
			Vector_Push(&Out,t); 
		}else{
            if(t->Type==MFUNCTION_OP_RBK){
				while(Stack.size>0 && ((Token*)Vector_Get(&Stack,Stack.size-1))->Type!=MFUNCTION_OP_LBK){
                	Vector_Push(&Out,(Token*)Vector_Get(&Stack,Stack.size-1));
                	Vector_PopTop(&Stack);
            	}
				Vector_PopTop(&Stack);
			}else if(t->Type==MFUNCTION_OP_LBK){
				Vector_Push(&Stack,t);
			}else{
				while(Stack.size>0 && Token_PrecOp(t->Type)<=Token_PrecOp(((Token*)Vector_Get(&Stack,Stack.size-1))->Type)){
                	Vector_Push(&Out,(Token*)Vector_Get(&Stack,Stack.size-1));
                	Vector_PopTop(&Stack);
            	}
            	Vector_Push(&Stack,t);
			}
        }
		if(i==Toks.size-1){
			while(Stack.size>0){
            	Vector_Push(&Out,(Token*)Vector_Get(&Stack,Stack.size-1));
            	Vector_PopTop(&Stack);
            }
		}
    }
	Vector_Free(&Stack);
	return Out;
}

MFunction MFunction_ToFunction(Vector Toks){ // After SYA -> in right precedence order
	Vector Solver = Vector_New(sizeof(Token));
	for(int i = 0;i<Toks.size;i++){
        Token* t = (Token*)Vector_Get(&Toks,i);

		if(!Token_OpState(t->Type)) Vector_Push(&Solver,t);
        else{
			Token* First = NULL;
			float arg1 = ARG_DONTCARE;
			if(Token_OpArc(t->Type)>1){
				First = (Token*)Vector_Get(&Solver,Solver.size-2);
				arg1 = (float)Double_Parse(First->str,DOUBLE_UNSIGNED);
				if(CStr_Cmp(First->str,"x")) 		arg1 = ARG_X;
				if(First->Type==MFUNCTION_CALC) 	arg1 = ARG_DONTCARE;
			}
			
			Token* Second = (Token*)Vector_Get(&Solver,Solver.size-1);
			float arg2 = (float)Double_Parse(Second->str,DOUBLE_UNSIGNED);
			if(CStr_Cmp(Second->str,"x")) 		arg2 = ARG_X;
			if(Second->Type==MFUNCTION_CALC)	arg2 = ARG_DONTCARE;
			
            Branch* branch = Branch_New((Operation[]){Operation_New(arg1,arg2,(float (*)(float,float))MFunction_GetFunc(t->Type))},sizeof(Operation));
			if(First && First->Type==MFUNCTION_CALC){
				branch->First = (Branch*)First->str;
				branch->First->Parent = branch;
			}
			if(Second && Second->Type==MFUNCTION_CALC){
				branch->Second = (Branch*)Second->str;
				branch->Second->Parent = branch;
			}
			for(int i = 0;i<Token_OpArc(t->Type) && Solver.size>0;i++) Vector_PopTop(&Solver);

			Vector_Push(&Solver,(Token[]){(char*)branch,MFUNCTION_CALC});
        }
    }
	Branch* Root = NULL;
	Token* tok = (Token*)Vector_Get(&Solver,Solver.size-1);
	if(tok->Type==MFUNCTION_CALC){
		Root = (Branch*)tok->str;
	}else{
		float arg2 = (float)Double_Parse(tok->str,DOUBLE_UNSIGNED);
		if(CStr_Cmp(tok->str,"x")) 		arg2 = ARG_X;
		if(tok->Type==MFUNCTION_CALC)	arg2 = ARG_DONTCARE;
		Root = BinBranch_New((Operation[]){Operation_New(arg2,0.0f,NULL)},sizeof(Operation));
	}
	Vector_Free(&Solver);
	
	MFunction Func;
	Func.Root = Root;
	return Func;
}

MFunction MFunction_Build(char* str){ // After SYA -> in right precedence order
	Vector Parsed = MFunction_Parse(str);
	Vector SYA = MFunction_SYA(Parsed);
	MFunction Func = MFunction_ToFunction(SYA);

	for(int i = 0;i<Parsed.size;i++){
		Token* t = (Token*)Vector_Get(&Parsed,i);
		free(t->str);
	}
	Vector_Free(&Parsed);
	Vector_Free(&SYA);
	return Func;
}

void MFunction_Free(MFunction* Func){
	//Tree_ForEach(Func,(void (*)(void*)));
    BinTree_Free(Func);
}


typedef struct GFunction {
	MFunction f;
	char* Name;
	Pixel Color;
} GFunction;

GFunction GFunction_New(char* Name,char* str,Pixel Color){
	GFunction gf;
	gf.f = MFunction_Build(str);
	gf.Name = CStr_Cpy(Name);
	gf.Color = Color;
	return gf;
}

void GFunction_Free(GFunction* Func){
	MFunction_Free(&Func->f);
	free(Func->Name);
}

Vector Functions;
TextBox tb;
int FunctionFocused = -1;

float fOffsetX = 0.0f;
float fOffsetY = 0.0f;
float fScaleX = 0.0f;
float fScaleY = 0.0f;

float fStartPanX = 0.0f;
float fStartPanY = 0.0f;

float fSelectedCellX = 0.0f;
float fSelectedCellY = 0.0f;

Vec2 fMouseWorld_BeforeZoom = { 0.0f,0.0f };
Vec2 fMouseWorld_AfterZoom  = { 0.0f,0.0f };

float fWorldLeft = 0.0f;
float fWorldTop = 0.0f;
float fWorldRight = 0.0f; 
float fWorldBottom = 0.0f;

Vec2 ScreenToWorld(float x,float y) {
	return (Vec2){ (x / fScaleX) + fOffsetX,(y / fScaleY) + fOffsetY };
}
Vec2 WorldToScreen(float x,float y) {
	return (Vec2){ (x - fOffsetX) * fScaleX,(y - fOffsetY) * fScaleY };
}

void RenderFunction(float (*Func)(float),Pixel c){
	float fWorldPerScreenWidthPixel = (fWorldRight - fWorldLeft) / GetWidth();
	float fWorldPerScreenHeightPixel = (fWorldBottom - fWorldTop) / GetHeight();
	Vec2 s = { 0.0f,0.0f };
	Vec2 e = { 0.0f,0.0f };
	
	e = WorldToScreen(fWorldLeft-fWorldPerScreenWidthPixel, -Func((fWorldLeft - fWorldPerScreenWidthPixel)));
	for (float x = fWorldLeft; x < fWorldRight; x+=fWorldPerScreenWidthPixel)
	{
		float y = -Func(x);
		s = WorldToScreen(x,y);
		if(F32_Abs(s.y - e.y) < GetHeight()) RenderLine(s,e,c,1.0f);
		e = s;
	}
}

void RenderFunctionEx(MFunction* Func,Pixel c){
	float fWorldPerScreenWidthPixel = (fWorldRight - fWorldLeft) / GetWidth();
	float fWorldPerScreenHeightPixel = (fWorldBottom - fWorldTop) / GetHeight();
	Vec2 s = { 0.0f,0.0f };
	Vec2 e = { 0.0f,0.0f };
	
	e = WorldToScreen(fWorldLeft-fWorldPerScreenWidthPixel, -MFunction_Calculate(Func,(fWorldLeft - fWorldPerScreenWidthPixel)));
	for (float x = fWorldLeft; x < fWorldRight; x+=fWorldPerScreenWidthPixel)
	{
		float y = -MFunction_Calculate(Func,x);
		s = WorldToScreen(x,y);
		if(F32_Abs(s.y - e.y) < GetHeight()) RenderLine(s,e,c,1.0f);
		e = s;
	}
}

void PrintBinBranch(BinBranch* b){
	printf(" { ");
	if(b){
		Operation* op = (Operation*)b->Memory;
		if(op){
			if(op->Const1==ARG_DONTCARE) 	printf("DC,");
			else if(op->Const1==ARG_X) 		printf("X,");
			else 							printf("%10.10f,",op->Const1);
			if(op->Const2==ARG_DONTCARE) 	printf("DC,");
			else if(op->Const2==ARG_X) 		printf("X,");
			else 							printf("%10.10f,",op->Const2);
			if(op->Func==MFunction_Add) 		printf("add}");
			else if(op->Func==MFunction_Sub) 	printf("sub}");
			else if(op->Func==MFunction_Mul) 	printf("mul}");
			else if(op->Func==MFunction_Div) 	printf("div}");
			else if(op->Func==MFunction_Pow) 	printf("pow}");
			else 								printf("%p",op->Func);
		}
	}
	printf(" }");
}

void Setup(AlxWindow* w){
	SetFont(Sprite_Load("C:/Wichtig/System/Fonts/Alx_Font_Yanis.png"),16,16,16,16);
	ResizeFont(100,100);

	fScaleX = 100;
	fScaleY = 100;
	fOffsetX = (float)-GetWidth()  / fScaleX * 0.5f;
	fOffsetY = (float)-GetHeight() / fScaleY * 0.5f;

	//Function = MFunction_New();
	//MFunction_Set(&Function,0,BRANCH_DONTCARE,0,ARG_DONTCARE,1.0f,MFunction_Sub);
	//MFunction_Push(&Function,0,BRANCH_DONTCARE,BRANCH_FIRST,ARG_DONTCARE,ARG_DONTCARE,MFunction_Add);
	//MFunction_Push(&Function,1,(int[]){BRANCH_FIRST},BRANCH_FIRST,ARG_DONTCARE,ARG_DONTCARE,MFunction_Sub);
	//MFunction_Push(&Function,1,(int[]){BRANCH_FIRST},BRANCH_SECOND,3.0f,ARG_X,MFunction_Mul);
	//MFunction_Push(&Function,2,(int[]){BRANCH_FIRST,BRANCH_FIRST},BRANCH_FIRST,4.0f,ARG_DONTCARE,MFunction_Mul);
	//MFunction_Push(&Function,2,(int[]){BRANCH_FIRST,BRANCH_FIRST},BRANCH_SECOND,2.0f,ARG_DONTCARE,MFunction_Mul);
	//MFunction_Push(&Function,3,(int[]){BRANCH_FIRST,BRANCH_FIRST,BRANCH_FIRST},BRANCH_SECOND,ARG_X,3.0f,MFunction_Pow);
	//MFunction_Push(&Function,3,(int[]){BRANCH_FIRST,BRANCH_FIRST,BRANCH_SECOND},BRANCH_SECOND,ARG_X,2.0f,MFunction_Pow);

	tb = TextBox_New(Input_New(50,1),(Rect){0.0f,0.0f,2300.0f,100.0f},"C:/Wichtig/System/Fonts/Alx_Font_Yanis.png",16,16,16,16,50,50,BLACK);

	Functions = Vector_New(sizeof(GFunction));
}

void Update(AlxWindow* w){
    if(Stroke(VK_LBUTTON).RELEASED) {
		fSelectedCellX = floorf(fMouseWorld_AfterZoom.x + 1.0f);
		fSelectedCellY = floorf(fMouseWorld_AfterZoom.y + 1.0f);
	}
	if(Stroke(VK_LBUTTON).PRESSED) {
		if(Overlap_Rect_Point(tb.r,(Vec2){GetMouse().x,GetMouse().y})){
			tb.In.Enabled = 1;
		}else{
			tb.In.Enabled = 0;
		}
	}
	if(Stroke(VK_RBUTTON).PRESSED){
		FunctionFocused = -1;
		
		Vec2 p = ScreenToWorld(GetMouse().x,GetMouse().y);
		p.y *= -1;
		for(int i = 0;i<Functions.size;i++){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,i);
			float y = MFunction_Calculate(&gf->f,p.x);
			if(F32_Abs(p.y-y)<0.1f){
				FunctionFocused = i;
				break;
			}
		}
	}
	if(Stroke(VK_UP).DOWN) {
		if(FunctionFocused>=0){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,FunctionFocused);
			gf->Color += 0x1;
		}
	}
	if(Stroke(VK_DOWN).DOWN) {
		if(FunctionFocused>=0){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,FunctionFocused);
			gf->Color -= 0x1;
		}
	}

	Input_DefaultReact(&tb.In);
	TextBox_Update(&tb,GetMouse());

	if(Stroke(VK_RETURN).PRESSED) {
		String_RemoveAll(&tb.In.Buffer,'\n');

		if(tb.In.Buffer.str.size>0){
			char* str = String_CStr(&tb.In.Buffer);
			char* Name = CStr_ChopTo(str,'(');
			char* Function = CStr_ChopFrom(str,'=');
			int Size = CStr_Size(Function);
			if(Size>0){
				int Found = -1;
				for(int i = 0;i<Functions.size;i++){
					GFunction* gf = (GFunction*)Vector_Get(&Functions,i);
					if(CStr_Cmp(gf->Name,Name)){
						Found = i;
						break;
					}
				}
				if(Found>=0){
					GFunction* gf = (GFunction*)Vector_Get(&Functions,Found);
					MFunction_Free(&gf->f);
					gf->f = MFunction_Build(Function);
				}else{
					Vector_Push(&Functions,(GFunction[]){ GFunction_New(Name,Function,GREEN) });
				}
			}
			free(str);
			free(Name);
			free(Function);
		}
	}

	if(Stroke(VK_MBUTTON).PRESSED) {
		fStartPanX = GetMouse().x;
		fStartPanY = GetMouse().y;
	}

    if(Stroke(VK_MBUTTON).DOWN) {
		fOffsetX -= (GetMouse().x - fStartPanX) / fScaleX;
		fOffsetY -= (GetMouse().y - fStartPanY) / fScaleY;
		fStartPanX = GetMouse().x;
		fStartPanY = GetMouse().y;
	}
	
	fMouseWorld_BeforeZoom = ScreenToWorld(GetMouse().x,GetMouse().y);
	if(Stroke('Q').DOWN) {
		fScaleX *= 1.01f;
		fScaleY *= 1.01f;
	}
	if(Stroke('A').DOWN) {
		fScaleX *= 0.99f;
		fScaleY *= 0.99f;
	}

	fMouseWorld_AfterZoom = ScreenToWorld(GetMouse().x,GetMouse().y);	
	fOffsetX += (fMouseWorld_BeforeZoom.x - fMouseWorld_AfterZoom.x);
	fOffsetY += (fMouseWorld_BeforeZoom.y - fMouseWorld_AfterZoom.y);
	
	fWorldLeft = (0.0f / fScaleX) + fOffsetX;
	fWorldTop  = (0.0f / fScaleY) + fOffsetY;
	fWorldRight  = (GetWidth()  / fScaleX) + fOffsetX;
	fWorldBottom = (GetHeight() / fScaleY) + fOffsetY;

    Clear(BLACK);
	
	int nLinesDrawn = 0;
	for(float y = -GetHeight()/2;y<=GetHeight()/2;y++) {
		if(y >= fWorldTop && y <= fWorldBottom){
			Vec2 s = { -GetWidth()/2,y };
			Vec2 e = {  GetWidth()/2,y };
			RenderLine(WorldToScreen(s.x,s.y),WorldToScreen(e.x,e.y),LIGHT_GRAY,1.0f);
			nLinesDrawn++;
		}
	}
	for(float x = -GetWidth()/2;x<=GetWidth()/2;x++) {
		if(x >= fWorldLeft && x <= fWorldRight){
			Vec2 s = { x,-GetHeight()/2 };
			Vec2 e = { x, GetHeight()/2 };
			RenderLine(WorldToScreen(s.x,s.y),WorldToScreen(e.x,e.y),LIGHT_GRAY,1.0f);
			nLinesDrawn++;
		}
	}
	
	Vec2 s = (Vec2){ -GetWidth()/2,0.0f };
	Vec2 e = (Vec2){  GetWidth()/2,0.0f };
	RenderLine(WorldToScreen(s.x,s.y),WorldToScreen(e.x,e.y),RED,1.0f);
	nLinesDrawn++;
	
	s = (Vec2){ 0.0f,-GetHeight()/2 };
	e = (Vec2){ 0.0f, GetHeight()/2 };
	RenderLine(WorldToScreen(s.x,s.y),WorldToScreen(e.x,e.y),RED,1.0f);
	nLinesDrawn++;
	
	RenderCircle(WorldToScreen(fSelectedCellX - 0.5f,fSelectedCellY - 0.5f),(int)(0.3*fScaleX),WHITE);
	
	//RenderFunction(sinf,RED);
	//RenderFunction(cosf,GREEN);
	//RenderFunction(tanf,BLUE);

	for(int i = 0;i<Functions.size;i++){
		GFunction* gf = (GFunction*)Vector_Get(&Functions,i);
		RenderFunctionEx(&gf->f,gf->Color);
		
		int Size = CStr_Size(gf->Name);
		if(FunctionFocused==i) 	RenderRect(0.0f,tb.r.d.y+i*GetFont()->CharSizeY,Size * GetFont()->CharSizeX,GetFont()->CharSizeY,YELLOW);
		else 					RenderRect(0.0f,tb.r.d.y+i*GetFont()->CharSizeY,Size * GetFont()->CharSizeX,GetFont()->CharSizeY,gf->Color);
		RenderCStr(gf->Name,0.0f,tb.r.d.y+i*GetFont()->CharSizeY,BLACK);
	}

	RenderTextBox(&tb);
}

void Delete(AlxWindow* w){
	for(int i = 0;i<Functions.size;i++){
		GFunction* mf = (GFunction*)Vector_Get(&Functions,i);
		GFunction_Free(mf);
	}
	Vector_Free(&Functions);
	TextBox_Free(&tb);
}

int main(){
    if(Create("Game Test",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
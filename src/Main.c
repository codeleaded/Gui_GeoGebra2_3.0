#if defined(__linux__)
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/MInterpreter.h"
#elif defined(_WINE)
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/MInterpreter.h"
#elif defined(_WIN32)
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "F:/home/codeleaded/System/Static/Library/Random.h"
#include "F:/home/codeleaded/System/Static/Library/TransformedView.h"
#include "F:/home/codeleaded/System/Static/Library/MInterpreter.h"
#elif defined(__APPLE__)
    #error "Apple not supported!"
#else
    #error "Platform not supported!"
#endif

#define GFUNCTION_STDVARIABLE 	"x"
#define TOKEN_MFUNCTION_ADD 	(TOKEN_START + 0)
#define TOKEN_MFUNCTION_SUB 	(TOKEN_START + 1)
#define TOKEN_MFUNCTION_MUL 	(TOKEN_START + 2)
#define TOKEN_MFUNCTION_DIV 	(TOKEN_START + 3)
#define TOKEN_MFUNCTION_NEG 	(TOKEN_START + 4)
#define TOKEN_MFUNCTION_POW 	(TOKEN_START + 5)
#define TOKEN_MFUNCTION_SIN 	(TOKEN_START + 6)
#define TOKEN_MFUNCTION_COS 	(TOKEN_START + 7)
#define TOKEN_MFUNCTION_TAN 	(TOKEN_START + 8)
#define TOKEN_MFUNCTION_EXP 	(TOKEN_START + 9)
#define TOKEN_MFUNCTION_LOG 	(TOKEN_START + 10)

MType MFunction_Add(Vector* args){
	return ((MType*)Vector_Get(args,0))[0] + ((MType*)Vector_Get(args,1))[0];
}
MType MFunction_Sub(Vector* args){
	return ((MType*)Vector_Get(args,0))[0] - ((MType*)Vector_Get(args,1))[0];
}
MType MFunction_Mul(Vector* args){
	return ((MType*)Vector_Get(args,0))[0] * ((MType*)Vector_Get(args,1))[0];
}
MType MFunction_Div(Vector* args){
	return ((MType*)Vector_Get(args,0))[0] / ((MType*)Vector_Get(args,1))[0];
}
MType MFunction_Neg(Vector* args){
	return -((MType*)Vector_Get(args,0))[0];
}

MType MFunction_Pow(Vector* args){
	return pow(((MType*)Vector_Get(args,0))[0],((MType*)Vector_Get(args,1))[0]);
}
MType MFunction_Sin(Vector* args){
	return sin(((MType*)Vector_Get(args,0))[0]);
}
MType MFunction_Cos(Vector* args){
	return cos(((MType*)Vector_Get(args,0))[0]);
}
MType MFunction_Tan(Vector* args){
	return tan(((MType*)Vector_Get(args,0))[0]);
}
MType MFunction_Exp(Vector* args){
	return exp(((MType*)Vector_Get(args,0))[0]);
}
MType MFunction_Log(Vector* args){
	return log(((MType*)Vector_Get(args,0))[0]);
}


typedef struct GFunction {
	MFunction f;
	char* Name;
	Pixel Color;
} GFunction;

GFunction GFunction_New(MInterpreter* mi,char* Name,char* expr,Pixel Color){
	GFunction gf;
	gf.f = MFunction_New(mi,expr);
	gf.Name = CStr_Cpy(Name);
	gf.Color = Color;
	return gf;
}
void GFunction_Render(unsigned int* Target,int Width,int Height,GFunction* gf,TransformedView* tv,MInterpreter* mi){
	Vec2 PosBefore = { 0.0f,0.0f };
	for(int i = 0;i<GetWidth();i++){
		Vec2 Pos = { i,0.0f };
		
		float x = TransformedView_ScreenWorldX(tv,i);
		MVariableMap_SetVariable(&mi->variables,GFUNCTION_STDVARIABLE,x);
		
		float y = (float)MInterpreter_Execute(mi,&gf->f);
		Pos.y = TransformedView_WorldScreenY(tv,-y);
		
		//if(F32_Abs(Pos.y - PosBefore.y) < GetHeight())
		RenderLine(PosBefore,Pos,gf->Color,1.0f);

		PosBefore = Pos;
	}
}
void GFunction_Free(GFunction* gf){
	MFunction_Free(&gf->f);
	CStr_Free(&gf->Name);
}


MInterpreter mi;
Vector Functions;
TextBox tb;
int FunctionFocused = -1;
TransformedView tv;

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ -25.0f,-50.0f },
		(Vec2){ 0.01f,0.01f },
		(float)GetHeight() / (float)GetWidth()
	);

	tb = TextBox_New(Input_New(50,1),(Rect){0.0f,0.0f,2300.0f,100.0f},ALXFONT_PATHS_HIGH,50,100,BLACK);
	Functions = Vector_New(sizeof(GFunction));
	mi = MInterpreter_New(
		MVariableMap_Make((MVariable[]){
			MVariable_New(GFUNCTION_STDVARIABLE,0.0),
			MVariable_Null()
		}),
		MFuncMap_Make((MFunc[]){
			MFunc_New(TOKEN_MFUNCTION_ADD,2,(MType(*)(Vector*))MFunction_Add),
			MFunc_New(TOKEN_MFUNCTION_SUB,2,(MType(*)(Vector*))MFunction_Sub),
			MFunc_New(TOKEN_MFUNCTION_MUL,2,(MType(*)(Vector*))MFunction_Mul),
			MFunc_New(TOKEN_MFUNCTION_DIV,2,(MType(*)(Vector*))MFunction_Div),
			MFunc_New(TOKEN_MFUNCTION_NEG,1,(MType(*)(Vector*))MFunction_Neg),
			MFunc_New(TOKEN_MFUNCTION_POW,2,(MType(*)(Vector*))MFunction_Pow),
			MFunc_New(TOKEN_MFUNCTION_SIN,1,(MType(*)(Vector*))MFunction_Sin),
			MFunc_New(TOKEN_MFUNCTION_COS,1,(MType(*)(Vector*))MFunction_Cos),
			MFunc_New(TOKEN_MFUNCTION_TAN,1,(MType(*)(Vector*))MFunction_Tan),
			MFunc_New(TOKEN_MFUNCTION_EXP,1,(MType(*)(Vector*))MFunction_Exp),
			MFunc_New(TOKEN_MFUNCTION_LOG,1,(MType(*)(Vector*))MFunction_Log),
			MFunc_Null(),
		}),
		KeywordMap_Make((KeywordRP[]){
			KeywordRP_New("add",TOKEN_MFUNCTION_ADD),
			KeywordRP_New("sub",TOKEN_MFUNCTION_SUB),
			KeywordRP_New("mul",TOKEN_MFUNCTION_MUL),
			KeywordRP_New("div",TOKEN_MFUNCTION_DIV),
			KeywordRP_New("neg",TOKEN_MFUNCTION_NEG),
			KeywordRP_New("pow",TOKEN_MFUNCTION_POW),
			KeywordRP_New("sin",TOKEN_MFUNCTION_SIN),
			KeywordRP_New("cos",TOKEN_MFUNCTION_COS),
			KeywordRP_New("tan",TOKEN_MFUNCTION_TAN),
			KeywordRP_New("exp",TOKEN_MFUNCTION_EXP),
			KeywordRP_New("log",TOKEN_MFUNCTION_LOG),
			KeywordRP_Null(),
		}),
		OperatorMap_Make((OperatorRP[]){
			OperatorRP_Make((TT_Type[]){ TOKEN_PLUS_SIGN,TOKEN_END },	TOKEN_MFUNCTION_ADD,ARGS_IGNORE),
			OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_END },	TOKEN_MFUNCTION_SUB,1),
			OperatorRP_Make((TT_Type[]){ TOKEN_ASTERISK,TOKEN_END },	TOKEN_MFUNCTION_MUL,ARGS_IGNORE),
			OperatorRP_Make((TT_Type[]){ TOKEN_SLASH,TOKEN_END },		TOKEN_MFUNCTION_DIV,ARGS_IGNORE),
			OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_END },	TOKEN_MFUNCTION_NEG,0),
			OperatorRP_Make((TT_Type[]){ TOKEN_CARET,TOKEN_END },		TOKEN_MFUNCTION_POW,ARGS_IGNORE),
			OperatorRP_Null(),
		}),
		ShutingYard_New(
			PrecedenceMap_Make((Precedencer[]){
				Precedencer_New(TOKEN_MFUNCTION_ADD,1),
				Precedencer_New(TOKEN_MFUNCTION_SUB,2),
				Precedencer_New(TOKEN_MFUNCTION_MUL,3),
				Precedencer_New(TOKEN_MFUNCTION_DIV,4),
				Precedencer_New(TOKEN_MFUNCTION_NEG,6),
				Precedencer_New(TOKEN_MFUNCTION_POW,5),
				Precedencer_New(TOKEN_MFUNCTION_SIN,7),
				Precedencer_New(TOKEN_MFUNCTION_COS,7),
				Precedencer_New(TOKEN_MFUNCTION_TAN,7),
				Precedencer_New(TOKEN_MFUNCTION_EXP,7),
				Precedencer_New(TOKEN_MFUNCTION_LOG,7),
                Precedencer_New(TOKEN_PARENTHESES_L,PRECEDENCE_BRACKL),
                Precedencer_New(TOKEN_PARENTHESES_R,PRECEDENCE_BRACKR),
				Precedencer_End,
			}),
			ExecuteMap_Make((Executer[]){
				Executer_New(TOKEN_MFUNCTION_ADD,2,NULL),
				Executer_New(TOKEN_MFUNCTION_SUB,2,NULL),
				Executer_New(TOKEN_MFUNCTION_MUL,2,NULL),
				Executer_New(TOKEN_MFUNCTION_DIV,2,NULL),
				Executer_New(TOKEN_MFUNCTION_NEG,1,NULL),
				Executer_New(TOKEN_MFUNCTION_POW,2,NULL),
				Executer_New(TOKEN_MFUNCTION_SIN,1,NULL),
				Executer_New(TOKEN_MFUNCTION_COS,1,NULL),
				Executer_New(TOKEN_MFUNCTION_TAN,1,NULL),
				Executer_New(TOKEN_MFUNCTION_EXP,1,NULL),
				Executer_New(TOKEN_MFUNCTION_LOG,1,NULL),
				Executer_End,
			}),
			PreexecuteMap_Make((Preexecuter[]){
				Preexecuter_End,
			})
		),
		ReseterMap_Make((Reseter[]){ 
            // operators who might have two versions: 1 arg vs. 2 args are not listed: -
            Reseter_New(TOKEN_NONE,RESETER_NONE),
            Reseter_New(TOKEN_NUMBER,RESETER_ADD1),
            Reseter_New(TOKEN_FLOAT,RESETER_ADD1),
            Reseter_New(TOKEN_STRING,RESETER_ADD1),
            Reseter_New(TOKEN_SPACE,RESETER_NONE),
            Reseter_New(TOKEN_NEWLINE,RESETER_RST),// without semicolon like python
            Reseter_New(TOKEN_CARTURN,RESETER_NONE),
            Reseter_New(TOKEN_HTAB,RESETER_NONE),
            Reseter_New(TOKEN_VTAB,RESETER_NONE),
            Reseter_New(TOKEN_COMMA,RESETER_RST),
            Reseter_New(TOKEN_PARENTHESES_R,RESETER_TO1),
            Reseter_New(TOKEN_SQUARE_BRACKETS_R,RESETER_NONE),
            Reseter_New(TOKEN_CURLY_BRACES_R,RESETER_NONE),
            Reseter_New(TOKEN_DOUBLE_ANGLE_BRACKETS_R,RESETER_NONE),
            Reseter_New(TOKEN_WHITE_SQUARE_BRACKETS_R,RESETER_NONE),
            Reseter_New(TOKEN_PARENTHESES_L,RESETER_RST),
            Reseter_New(TOKEN_SQUARE_BRACKETS_L,RESETER_RST),
            Reseter_New(TOKEN_CURLY_BRACES_L,RESETER_RST),
            Reseter_New(TOKEN_DOUBLE_ANGLE_BRACKETS_L,RESETER_RST),
            Reseter_New(TOKEN_WHITE_SQUARE_BRACKETS_L,RESETER_RST),
            Reseter_New(TOKEN_SEMICOLON,RESETER_RST),
            Reseter_New(TOKEN_COLON,RESETER_RST),
            Reseter_New(TOKEN_EXCLAMATION_MARK,RESETER_RST),
            Reseter_New(TOKEN_QUESTION_MARK,RESETER_RST),
            Reseter_New(TOKEN_ELLIPSIS,RESETER_RST),
            Reseter_New(TOKEN_PLUS_SIGN,RESETER_RST),
            Reseter_New(TOKEN_MULTIPLICATION_SIGN,RESETER_RST),
            Reseter_New(TOKEN_DIVISION_SIGN,RESETER_RST),
            Reseter_New(TOKEN_EQUAL_SIGN,RESETER_RST),
            Reseter_New(TOKEN_NOT_EQUAL_SIGN,RESETER_RST),
            Reseter_New(TOKEN_LESS_THAN,RESETER_RST),
            Reseter_New(TOKEN_GREATER_THAN,RESETER_RST),
            Reseter_New(TOKEN_LESS_THAN_OR_EQUAL_TO,RESETER_RST),
            Reseter_New(TOKEN_GREATER_THAN_OR_EQUAL_TO,RESETER_RST),
            Reseter_New(TOKEN_APPROXIMATION_SIGN,RESETER_RST),
            Reseter_New(TOKEN_PLUS_MINUS_SIGN,RESETER_RST),
            Reseter_New(TOKEN_AMPERSAND,RESETER_RST),
            Reseter_New(TOKEN_AT_SIGN,RESETER_RST),
            Reseter_New(TOKEN_ASTERISK,RESETER_RST),
            Reseter_New(TOKEN_CARET,RESETER_RST),
            Reseter_New(TOKEN_TILDE,RESETER_RST),
            Reseter_New(TOKEN_SLASH,RESETER_RST),
            Reseter_New(TOKEN_BACKSLASH,RESETER_RST),
            Reseter_New(TOKEN_UNDERSCORE,RESETER_RST),
            Reseter_New(TOKEN_PIPE,RESETER_RST),
            Reseter_New(TOKEN_PERCENT,RESETER_RST),
            Reseter_New(TOKEN_HASH_POUND_SIGN,RESETER_RST),
            Reseter_New(TOKEN_SINGLE_QUOTE,RESETER_RST),
            Reseter_New(TOKEN_DOUBLE_QUOTE,RESETER_RST),
            Reseter_New(TOKEN_BACKTICK,RESETER_RST),
            Reseter_New(TOKEN_PRIME,RESETER_RST),
            Reseter_New(TOKEN_DOUBLE_PRIME,RESETER_RST),
            Reseter_New(TOKEN_SECTION_SYMBOL,RESETER_RST),
            Reseter_New(TOKEN_DEGREE_SYMBOL,RESETER_RST),
            Reseter_New(TOKEN_FRACTION_SLASH,RESETER_RST),
            Reseter_New(TOKEN_INTERROBANG,RESETER_RST),
            Reseter_New(TOKEN_BULLET_POINT,RESETER_RST),
            Reseter_New(TOKEN_CAST,RESETER_ADD1),
            Reseter_New(TOKEN_FUNCTION,RESETER_ADD1),
            Reseter_New(TOKEN_CONSTSTRING_SINGLE,RESETER_ADD1),
            Reseter_New(TOKEN_CONSTSTRING_DOUBLE,RESETER_ADD1),
            Reseter_New(TOKEN_END,RESETER_RST)
        })
	);

	//MInterpreter_Print(&mi);
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
	
	if(Stroke(ALX_MOUSE_R).PRESSED){
		FunctionFocused = -1;
		
		Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
		p.y *= -1;
		for(int i = 0;i<Functions.size;i++){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,i);

			MVariableMap_SetVariable(&mi.variables,GFUNCTION_STDVARIABLE,p.x);
			float y = (float)MInterpreter_Execute(&mi,&gf->f);

			if(F32_Abs(p.y-y)<0.1f){
				FunctionFocused = i;
				break;
			}
		}
	}
	if(Stroke(ALX_KEY_UP).DOWN) {
		if(FunctionFocused>=0){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,FunctionFocused);
			gf->Color += 0x1;
		}
	}
	if(Stroke(ALX_KEY_DOWN).DOWN) {
		if(FunctionFocused>=0){
			GFunction* gf = (GFunction*)Vector_Get(&Functions,FunctionFocused);
			gf->Color -= 0x1;
		}
	}

	if(Stroke(ALX_KEY_ENTER).PRESSED) {
		if(tb.In.Buffer.size>0){
			char* str = String_CStr(&tb.In.Buffer);
			if(str){
				char* Name = CStr_ChopTo(str,'(');
				char* Function = CStr_ChopFrom(str,'=');
				
				if(CStr_Find(str,'(')>=0 && CStr_Find(str,'=')>=0 && CStr_Size(Function) > 0){
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
						gf->f = MFunction_New(&mi,Function);
					}else{
						Vector_Push(&Functions,(GFunction[]){ GFunction_New(&mi,Name,Function,GREEN) });
					}
				}

				CStr_Free(&str);
				CStr_Free(&Name);
				CStr_Free(&Function);
			}
		}
		tb.In.Enabled = 0;
	}else{
		TextBox_Update(&tb,w->Strokes,GetMouse());
	}

    Clear(BLACK);

	Vec2 wvls = { 0.0f,TransformedView_ScreenWorldY(&tv,0.0f) };
	Vec2 wvle = { 0.0f,TransformedView_ScreenWorldY(&tv,GetHeight()) };
	Vec2 whls = { TransformedView_ScreenWorldX(&tv,0.0f),0.0f };
	Vec2 whle = { TransformedView_ScreenWorldX(&tv,GetWidth()),0.0f };

	Vec2 svls = TransformedView_WorldScreenPos(&tv,wvls);
	Vec2 svle = TransformedView_WorldScreenPos(&tv,wvle);
	Vec2 shls = TransformedView_WorldScreenPos(&tv,whls);
	Vec2 shle = TransformedView_WorldScreenPos(&tv,whle);

	RenderLine(svls,svle,WHITE,1.0f);
	RenderLine(shls,shle,WHITE,1.0f);

	for(int i = 0;i<Functions.size;i++){
		GFunction* gf = (GFunction*)Vector_Get(&Functions,i);
		GFunction_Render(WINDOW_STD_ARGS,gf,&tv,&mi);
		
		int Size = CStr_Size(gf->Name);
		if(FunctionFocused==i) 	RenderRect(0.0f,tb.r.d.y+i*GetAlxFont()->CharSizeY,Size * GetAlxFont()->CharSizeX,GetAlxFont()->CharSizeY,YELLOW);
		else 					RenderRect(0.0f,tb.r.d.y+i*GetAlxFont()->CharSizeY,Size * GetAlxFont()->CharSizeX,GetAlxFont()->CharSizeY,gf->Color);
		RenderCStr(gf->Name,0.0f,tb.r.d.y+i*GetAlxFont()->CharSizeY,BLACK);
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
	MInterpreter_Free(&mi);
}

int main(){
    if(Create("Geogebra 2.3",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
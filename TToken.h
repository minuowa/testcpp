#ifndef Token_h__
#define Token_h__

class TToken
{
public:
	CXIndex mType;
	int mLine;
	GString mStr;
public:
	TToken();
	void clear();
};
enum eInnerToken
{
	ITK_None,
	ITK_ID,					//id
	ITK_TypeName,			//ÀàÐÍ
	ITK_ConstNumber,		//Êý×Ö
	ITK_LeftSquareBracket,	//(
	ITK_RightSuqareBracket, //)
	ITK_LeftBracket,		//[
	ITK_RightBracket,		//]
	ITK_LeftAngleBracket,	//<
	ITK_RightAngleBracket,	//>
	ITK_LeftBrace,			//{
	ITK_RightBrace,			//}
	ITK_Colon,				//:
	ITK_Simicolon,			//;
	ITK_Comma,				//,
	ITK_Wave,				//~
	ITK_Sharp,				//#
	ITK_LineFeed,			//\n
	ITK_Equal,				//=
	ITK_Add,				//+
	ITK_Sub,				//-
	ITK_Multiply,			//*
	ITK_Divide,				///
	ITK_Dot,				//.
	ITK_And,				//&

	ITK_EndOfFile,			//\0
	ITK_Void,				//void

	ITK_Const,				//const
	ITK_Decorate,			//const,virtual,static,inline,unsigned
	ITK_LimitOfPower,		//public,protected,private
	ITK_PreProcess,			//
	ITK_Typedef,			//typedef
	ITK_Include,			//include
	ITK_Program,			//program
	ITK_Ifdef,				//ifdef
	ITK_Ifndef,				//ifndef
	ITK_Endif,				//endif

	ITK_Define,				//define

	ITK_Switch,				//switch
	ITK_Break,				//break

	ITK_If,					//if
	ITK_Else,				//else

	ITK_Do,					//do
	ITK_While,				//while

	ITK_Return,				//return
	ITK_True,				//true
	ITK_False,				//false

	ITK_Class,				//class
	ITK_Sturct,				//struct
	ITK_Enum,				//enum

	ITK_Max,
};
#endif // Token_h__
#pragma once
#include "TToken.h"
enum eGrammarState
{
	eGS_Something,
	eGS_Expression,
	eGS_If,
	eGS_Switch,
	eGS_DoWhile,
	eGS_WhileDo,
	eGS_For,
	eGS_Class,
	eGS_Struct,
	eGS_Enum,
	eGS_PreProcess,
};
class TGrammarElement
{
public:
	TGrammarElement ( eGrammarState state, const char* args );
	TGrammarElement();
	virtual bool checkTag( const TToken& token );
	void reset();
	bool isEnd();
	TGrammarElement* clone();
private:
	eGrammarState mState;
	CXIndex mCurToken;
	CXDynaArray<GString> mTags;
	bool mEnd;
};


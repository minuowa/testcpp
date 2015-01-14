#ifndef TGrammar_h__
#define TGrammar_h__

#include "TToken.h"
enum eGrammarState
{
    eGS_Something,
    eGS_Expression,
    eGS_If,
	eGS_IfElse,
    eGS_Switch,
    eGS_DoWhile,
    eGS_WhileDo,
	eGS_For,
	eGS_Class,
	eGS_Enum,
	eGS_Struct,
};
class TGrammarElement
{
public:
	TGrammarElement(eGrammarState state,const char* args);
	TGrammarElement();
	bool nextState(const char* token);
	void reset();
	bool isEnd();
	TGrammarElement* clone();
private:
	eGrammarState mState;
	CXIndex mCurToken;
	CXDynaArray<GString> mTags;
	bool mEnd;
};
class TGrammar
{
public:
    typedef TToken MyToken;
    TGrammar ( void );
    ~TGrammar ( void );

    void nextState ( const char* token );
    void nextState ( char c );

	CXDynaArray<TGrammarElement*> mElements;

	CXStack<TGrammarElement*> mBufferState;

    eGrammarState mGrammarState;
};


#endif // TGrammar_h__

# ifndef TGrammar_h__
#define TGrammar_h__

#include "TToken.h"
#include "TSymbols.h"
#include "TGrammarElement.h"
class TLexer;
class TSymbolMacro;

struct reduce
{
	vector<string> mTokenlist;
};

class TGrammar
{
public:
    typedef TToken MyToken;
    TGrammar ( void );
    ~TGrammar ( void );
	/**
	ID:±êÊ¶·û
	DIG:Êý×Ö
	EMPTY:¿Õ
	*/
	bool loadProduction(const char* production);

	bool parserToken(const TToken& token);


	void parserPreprocess();

	void parseEnum();

	void parseDefine(TSymbolMacro* symbol);

	void goUntil(eInnerToken type);

	void fillUntil();

	TLexer* mLexer;

    CXDynaArray<TGrammarElement*> mSysElements;

    CXStack<TGrammarElement*> mBufferElement;

    CXStack<TSymbolBase*> mBufferSymbol;

    CXDynaArray<GString> mBufferDecorate;

    eGrammarState mGrammarState;

	eInnerToken mTargetToken;


	GString* mContent;

	CXDynaArray<reduce> mReducelist;
	TSymbols mSymbols;

};


#endif // TGrammar_h__

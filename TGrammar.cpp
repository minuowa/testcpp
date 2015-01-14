#include "stdafx.h"
#include "TGrammar.h"
#include "TSymbolBase.h"
#include "TLexer.h"
struct TGrammarPara
{
    eGrammarState mState;
    const char* mParas;
};
static const TGrammarPara gParas[] =
{
    {eGS_If, "if,(,)" },
    {eGS_DoWhile, "do,while,(,)"},
    {eGS_WhileDo, "while,(,)"},
    {eGS_For, "for,(,;,;,)"},
    {eGS_Class, "class,{,},;"},
    {eGS_Enum, "enum,{,},;"},
    {eGS_Struct, "struct,{,},;"},
};

TGrammar::TGrammar ( void )
    : mGrammarState ( eGS_Something )
    , mTargetToken ( ITK_None )
{
for ( const TGrammarPara & para: gParas )
    {
        TGrammarElement* ele = new TGrammarElement ( para.mState, para.mParas );
        mSysElements.push_back ( ele );
    }
}


//bool TGrammar::nextState ( char c )
//{
//
//    return nextState ( ch, false );
//}
//
//bool TGrammar::nextState ( const char* token, bool checkSymbol/*=true*/ )
//{
//for ( TGrammarElement * ele: mSysElements )
//    {
//        if ( ele->checkTag ( token ) )
//        {
//            ele->reset();
//            TGrammarElement* clone = ele->clone();
//            mBufferElement.push ( clone );
//            return true;
//        }
//    }
//    if ( !mBufferElement.empty() )
//    {
//        TGrammarElement* ele = mBufferElement.top();
//        if ( ele->checkTag ( token ) )
//        {
//            if ( ele->isEnd() )
//            {
//                dSafeDelete ( ele );
//                mBufferElement.pop();
//            }
//            return true;
//        }
//    }
//    //if ( checkSymbol )
//    if ( 0 )
//    {
//        TSymbolBase* symbol = mSymbols.getSymbol ( token );
//        if ( symbol == nullptr )
//        {
//            assert ( mBufferSymbol.size() );
//            TSymbolBase* typeSymbol = mBufferSymbol.top();
//            TSymbolBase* theSymbol = new TSymbolBase;
//            std::copy ( mBufferDecorate.begin(), mBufferDecorate.end(), theSymbol->mDecorates.begin() );
//            theSymbol->mName = token;
//            theSymbol->mSymbolType = TST_Var;
//            theSymbol->mTypeID = typeSymbol->mName;
//            mSymbols.addSymbol ( token, theSymbol );
//        }
//    }
//
//    return false;
//}

TGrammar::~TGrammar ( void )
{
    dSafeDeleteVector ( mSysElements );
}

bool TGrammar::parserToken ( const TToken& token )
{
    if ( mTargetToken != ITK_None )
    {
        if ( token.mType.Value() != mTargetToken )
        {
            return false;
        }
    }
    switch ( token.mType.Value() )
    {
    case ITK_Sharp:
    {
        mGrammarState = eGS_PreProcess;
        parserPreprocess();
    }
    break;
    case ITK_Enum:
    {
        mGrammarState = eGS_Enum;
        parseEnum();
    }
    break;
    default:
        break;
    }
    return true;
}

void TGrammar::parserPreprocess()
{
    assert ( mGrammarState == eGS_PreProcess );

    TToken token;

    switch ( token.mType.Value() )
    {
    case ITK_Ifndef:
    {
        TSymbolBase* symbol = mSymbols.getSymbol ( token.mStr );
        if ( symbol )
            goUntil ( ITK_Sharp );
    }
    break;
    case ITK_Ifdef:
    {
        TSymbolBase* symbol = mSymbols.getSymbol ( token.mStr );
        if ( !symbol )
            goUntil ( ITK_Sharp );
    }
    break;
    case ITK_Endif:
        break;
    case ITK_Define:
    {
        TSymbolBase* symbol = mSymbols.getSymbol ( token.mStr );
        if ( !symbol )
        {
            TSymbolMacro* macro = new TSymbolMacro;
            macro->mName = token.mStr;
            parseDefine ( macro );
        }
    }
    break;
    default:
        break;
    }
}

void TGrammar::goUntil ( eInnerToken type )
{
    mTargetToken = type;
}

void TGrammar::parseDefine ( TSymbolMacro* symbol )
{
    TToken token;
    switch ( token.mType.Value() )
    {
    case ITK_LineFeed:
    {

    }
    break;
    default:
        break;
    }
}

void TGrammar::parseEnum()
{
    TToken token;
    assert ( !token.mType.IsValid() );

    TSymbolEnum* symbol = new TSymbolEnum;
}

bool TGrammar::loadProduction ( const char* production )
{
    ifstream fin;
    string str;
    reduce r;
    fin.open ( production, ios_base::in );
    if ( !fin.is_open() )
        CXASSERT_RETURN_FALSE ( 0 );
    while ( getline ( fin, str ) )
    {
        int len = str.size();
        int num = 0;
        int x = 0, y = 0;
        string tempstr;
        while ( y < len )
        {
            while ( isspace ( str[y] ) && y < len )
                y++;
            if ( y == len )
                return false;
            x = y;
            while ( isalpha ( str[y] ) || str[y] == '_' )
                y++;
            r.mTokenlist.push_back ( str.substr ( x, y - x ) );
            while ( isspace ( str[y] ) && y < len )
                y++;
            if ( str[y] == '=' && str[y + 1] == '>' )
                y += 2;
            while ( isspace ( str[y] ) && y < len )
                y++;
            while ( y < len )
            {
                x = y;
                while ( isalpha ( str[y] ) || str[y] == '_' )
                    y++;
                tempstr = str.substr ( x, y - x );
                r.mTokenlist.push_back ( tempstr );
                num++;
                while ( isspace ( str[y] ) && y < len )
                    y++;
            }
        }
        mReducelist.push_back ( r );
        r.mTokenlist.clear();
    }
    fin.close();
	return true;
}


#include "stdafx.h"
#include "TGrammarElement.h"


TGrammarElement* TGrammarElement::clone()
{
    TGrammarElement* ele = new TGrammarElement;
    ele->mState = this->mState;
    ele->mTags = this->mTags;
    ele->mCurToken++;
    if ( ele->mCurToken >= mTags.size() )
    {
        ele->mCurToken = mTags.size() - 1;
        ele->mEnd = true;
    }
    return ele;
}

TGrammarElement::TGrammarElement()
    : mCurToken ( 0 )
    , mEnd ( false )
{

}

TGrammarElement::TGrammarElement ( eGrammarState state, const char* args )
    : mCurToken ( 0 )
    , mEnd ( false )
{
    mState = state;
    CXASSERT ( args );
    GString str = args;
    str.Splite ( ',', mTags );
    CXASSERT ( mTags.size() );
}

bool TGrammarElement::checkTag ( const TToken& token )
{
    assert ( token.mType.IsValid() );

    if ( mTags[mCurToken].equal ( token.mStr ) )
    {
        dDebugOut ( "Token:%d,%s", mCurToken, token.mStr );
        mCurToken++;
        if ( mCurToken >= mTags.size() )
        {
            mCurToken = mTags.size() - 1;
            mEnd = true;
        }
        return true;
    }
    return false;
}

bool TGrammarElement::isEnd()
{
    return mEnd;
}

void TGrammarElement::reset()
{
    mCurToken = 0;
    mEnd = false;
}

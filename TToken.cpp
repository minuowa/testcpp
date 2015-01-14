#include "stdafx.h"
#include "TToken.h"

TToken::TToken()
    : mLine ( 0 )
{

}

void TToken::clear()
{
    mStr.clear();
    mLine = 0;
    mType = CXIndex::InvalidIndex;
}

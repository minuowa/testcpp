#include "stdafx.h"
#include "TSymbols.h"
#include "TSymbolBase.h"

#define AddInnerSymbol(name) mSymbolRepository->Insert(#name,new TSymbolBase(#name,#name,TST_Number))

TSymbols::TSymbols ( void )
{
    auto mSymbolRepository = new CXSymbolMap;
    mSymbolArray.push_back ( mSymbolRepository );
    AddInnerSymbol ( int );
    AddInnerSymbol ( char );
    AddInnerSymbol ( long );
    AddInnerSymbol ( double );
    AddInnerSymbol ( float );
    AddInnerSymbol ( short );
    AddInnerSymbol ( bool );

    AddInnerSymbol ( void );
}


TSymbols::~TSymbols ( void )
{
for ( auto s: mSymbolArray )
    {
        dSafeDeleteMap2 ( *s );
    }
}

TSymbolBase* TSymbols::getSymbol ( const char* name ) const
{
    TSymbolBase* symbol = nullptr;
for ( auto s: mSymbolArray )
    {
        if ( s->Get ( name, symbol ) )
            return symbol;
    }

    return symbol;
}

void TSymbols::addSymbol ( const char* name, TSymbolBase* symbol )
{
    assert ( mSymbolArray.size() );
    CXSymbolMap* last = mSymbolArray[mSymbolArray.size() - 1];
    assert ( last );
    TSymbolBase* pnull = nullptr;
    assert ( !last->Get ( name, pnull ) );
    assert ( symbol );
    last->Insert ( name, symbol );
}

void TSymbols::enterField()
{
	this->showMe();
	mSymbolArray.push_back ( new CXSymbolMap );
}

void TSymbols::leaveField()
{
    assert ( mSymbolArray.size() );
    CXSymbolMap* last = mSymbolArray[mSymbolArray.size() - 1];
    dSafeDeleteMap2 ( *last );
    mSymbolArray.pop_back();
	this->showMe();
}

void TSymbols::showMe()
{
	dDebugOut("=================Symbol Begin=================");
	for ( auto s: mSymbolArray )
    {
for ( auto p: *s )
        {
			dDebugOut("Symbol:%s",p.first.c_str());
        }
    }
	dDebugOut("=================Symbol End=================");
}

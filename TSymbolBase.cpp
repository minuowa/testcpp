#include "stdafx.h"
#include "TSymbolBase.h"


TSymbolBase::TSymbolBase ( void )
{
}

TSymbolBase::TSymbolBase ( const char* name, const char* typeID, TSymbolType type )
    : mName ( name )
    , mTypeID ( typeID )
    , mSymbolType ( type )
{

}

TSymbolBase::~TSymbolBase()
{

}



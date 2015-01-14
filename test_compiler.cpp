#include "stdafx.h"
#include "test_compiler.h"
#include "TGrammar.h"
#include "TLexer.h"


test_compiler::test_compiler ( void )
{
    TLexer lexer;
    TToken token;
    if ( lexer.parserFile ( "TGrammar2.h" ) )
    {
        bool res = lexer.file_header();
        CXASSERT ( res );
    }
}


test_compiler::~test_compiler ( void )
{
}

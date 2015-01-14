#include "stdafx.h"
#include "TLexer.h"
#include "TSymbolBase.h"

#define TEat_Token(token) if(!eat ( token )) {;return false;}

#define TDRINK(expr) if(!(expr)) return false

#define TLIST(express,_pos) if(express) continue;else backTo(_pos);

#define TLIST_TEST(express,_pos) if(express){ addSymbol(symbol);continue;} else { deleteSymbol(symbol);backTo(_pos);}

#define TLIST_END(exp) if(exp){return true;}

#define TSINGLE(exp) if(exp) return true;

#define TSINGLE_MULTI(exp,_pos) if(exp) return true;else backTo(_pos)

TLexer::TLexer ( void )
{
    mBuffer.setElementByteCount ( sizeof ( CH ) );

    mWord[0] = mWord[1] = '\0';

    mGrammar.mLexer = this;

    //CXASSERT(mGrammar.loadProduction("product.txt"));

    bind ( ITK_LeftSquareBracket,	 "(" );
    bind ( ITK_RightSuqareBracket,   ")" );
    bind ( ITK_LeftBracket,	 "[" );
    bind ( ITK_RightBracket,	 "]" );
    bind ( ITK_LeftAngleBracket,	 "<" );
    bind ( ITK_RightAngleBracket,	 ">" );
    bind ( ITK_LeftBrace,			 "{" );
    bind ( ITK_RightBrace,			 "}" );
    bind ( ITK_Colon,				 ":" );
    bind ( ITK_Simicolon,			 ";" );
    bind ( ITK_Comma,				 "," );
    bind ( ITK_Wave, "~" );
    bind ( ITK_Sharp,				 "#" );

    bind ( ITK_LineFeed, "\n" );

    bind ( ITK_Equal, "=" );
    bind ( ITK_Add, "+" );
    bind ( ITK_Sub, "-" );
    bind ( ITK_Multiply, "*" );
    bind ( ITK_Divide, "/" );

    bind ( ITK_Dot, "." );
    bind ( ITK_And, "&" );

    bind ( ITK_Void, "void" );

    bind ( ITK_Const, "const" );
    bind ( ITK_Decorate, "virtual" );
    bind ( ITK_Decorate, "static" );
    bind ( ITK_Decorate, "inline" );
    bind ( ITK_Decorate, "unsigned" );

    bind ( ITK_LimitOfPower, "public" );
    bind ( ITK_LimitOfPower, "protected" );
    bind ( ITK_LimitOfPower, "private" );

    bind ( ITK_Typedef, "typedef" );
    bind ( ITK_Include, "include" );
    bind ( ITK_Program, "program" );


    bind ( ITK_Ifdef, "ifdef" );
    bind ( ITK_Ifndef, "ifndef" );
    bind ( ITK_Endif, "endif" );

    bind ( ITK_Define, "define" );

    bind ( ITK_Switch, "switch" );
    bind ( ITK_Break, "break" );

    bind ( ITK_If, "if" );
    bind ( ITK_Else, "else" );

    bind ( ITK_Do, "do" );
    bind ( ITK_While, "while" );

    bind ( ITK_Return, "return" );
    bind ( ITK_True, "true" );
    bind ( ITK_False, "false" );

    bind ( ITK_Class, "class" );
    bind ( ITK_Sturct, "struct" );
    bind ( ITK_Enum, "enum" );
}
int TLexer::getTokenType ( const CH* str ) const
{
    TSymbolBase* symbol = mSymbols.getSymbol ( str );
    if ( symbol )
    {
        switch ( symbol->mSymbolType )
        {
        case TST_Class:
        case TST_Number:
        case TST_Enum:
        {
            return ITK_TypeName;
        }
        break;
        case TST_Var:
        {
            return ITK_ID;
        }
        break;
        default:
            break;
        }
    }

    int type = -1;
    mTokenMap.Get ( str, type );
    return type;
}

bool TLexer::isEnd()
{
    return mCursor.mPosition >= ( int ) mBuffer.capacity() - 1;
}



TLexer::CH TLexer::getNextChar()
{
    assert ( mCursor.mPosition < ( int ) mBuffer.capacity() );
    mCursor.mPosition++;
    return mBuffer.get<CH> ( mCursor.mPosition );
}

void TLexer::bind ( int type, const CH* str )
{
    assert ( !mTokenMap.findkey ( str ) );
    mTokenMap.Insert ( str, type );
}

bool TLexer::parser ( const CH* content )
{
    assert ( content );
    u32 len = dStrLen ( content );
    mBuffer.reallocateByElementCount ( len + 1 );
    mBuffer.copyFrom ( ( void* ) content );
    return true;
}

bool TLexer::parserFile ( const char* fileName )
{
    char* buffer = nullptr;
    CXFileLoader fileloader;
    CXASSERT_RETURN_FALSE ( fileloader.OpenFile ( fileName ) );
    buffer = new Char[fileloader.GetBufferSize() + 1];
    memset ( buffer, 0, fileloader.GetBufferSize() + 1 );
    fileloader.ReadToBuffer ( buffer );
    fileloader.Close();

    return parser ( ( const char* ) buffer );
}

TLexer::~TLexer ( void )
{

}

int TLexer::fillToken ( const CH* word )
{
    mCurToken.mStr = word;
    mCurToken.mType = getTokenType ( word );
    mCurToken.mLine = mLastCursor.mLine;
    return mCurToken.mType.Value();
}

int TLexer::nextToken()
{
    mLastCursor = mCursor;
    mCurString.clear();
    mCurToken.clear();

    while ( !isEnd() )
    {
        CH c = getNextChar();
        mCursor.mColoumns++;
        switch ( c )
        {
        case '{':
        case '}':
        case '(':
        case ')':
        case '<':
        case '>':
        case '#':
        case ';':
        case '~':
        case ':':
        case '+':
        case '-':
        case '*':
        case '/':
        {
            if ( !mCurString.isEmpty() )
            {
                fillToken ( mCurString );
                //mLastCursor = mCursor;
                mCursor.mPosition--;
                mCursor.mColoumns--;
                return mCurToken.mType;
            }
            mWord[0] = c;
            return fillToken ( mWord );
        }
        case '\n':
        {
            mCursor.mColoumns = 0;
            if ( !mCurString.isEmpty() )
            {
                fillToken ( mCurString );
                mCursor.mLine++;
                return mCurToken.mType;
            }
            else
            {
                mCursor.mLine++;
            }
        }
        case ' ':
        case '\t':
        case ',':
        case '&':
        {
            if ( !mCurString.isEmpty() )
                return fillToken ( mCurString );
        }
        break;
        case '\0':
        {
            if ( !mCurString.isEmpty() )
            {
                fillToken ( mCurString );
                //mLastCursor = mCursor;
                mCursor.mPosition--;
                mCursor.mColoumns--;
                return mCurToken.mType;
            }
            mCurToken.clear();
            mCurToken.mType = ITK_EndOfFile;
            return mCurToken.mType;
        }
        break;
        default:
        {
            if ( isdigit ( c ) && mCursor.mPosition < ( int ) mBuffer.capacity() )
            {
                char nextc = mBuffer.get<CH> ( mCursor.mPosition + 1 );

                if ( isspace ( nextc ) )
                {
                    return fillToken ( mCurString );
                }
            }
            mCurString.append ( 1, c );
        }
        break;
        }
    }
    return mCurToken.mType;
}

bool TLexer::eat ( int token )
{
    if ( mCurToken.mType.Value() == token )
    {
        nextToken();
        return true;
    }
    //reportError ( token );
    return false;
}

void TLexer::reportError ( int token )
{
    dDebugOut ( "Eat Error:%d,%s", token, mCurToken.mStr.c_str() );
}

bool TLexer::expr ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSymbolBase* symbol = nullptr;
        TSINGLE_MULTI ( expr_assign ( tpos ), tpos );
        TSINGLE_MULTI ( expr_declare ( tpos, symbol ) , tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}


bool TLexer::block ( const TCursor& pos )
{
    TEat_Token ( ITK_LeftBrace );
    mSymbols.enterField();
    TDRINK ( something ( pos ) );
    TEat_Token ( ITK_RightBrace );
    mSymbols.leaveField();
    return true;
}

bool TLexer::file_header()
{
    bool res = true;
    do
    {
        TCursor pos = mLastCursor;
        TSymbolBase* symbol = nullptr;
        TLIST_TEST ( class_delcare ( pos, symbol ), pos );
        TLIST_TEST ( class_impl ( pos, symbol ), pos );
        TLIST_TEST ( func_impl ( pos, symbol ) , pos );
        TLIST_TEST ( func_declare ( pos, symbol, TST_FuncDecl ), pos );
        TLIST_END ( mCurToken.mType.Value() == ITK_EndOfFile );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}


bool TLexer::class_delcare ( const TCursor& pos, TSymbolBase*& symbol )
{
    TEat_Token ( ITK_Class );
    fillVarName ( TST_Class , symbol );
    TEat_Token ( ITK_Simicolon );

    return true;
}


bool TLexer::class_impl ( const TCursor& pos, TSymbolBase*& symbol )
{
    TEat_Token ( ITK_Class );
    fillVarName ( TST_Class , symbol );
    TEat_Token ( ITK_LeftBrace );
    mSymbols.enterField();
    TDRINK ( class_impl_list ( mCursor ) );
    TEat_Token ( ITK_RightBrace );
    mSymbols.leaveField();
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::class_func_declare ( const TCursor& pos, TSymbolBase*& outSymbol )
{
    TDRINK ( func_declare ( pos , outSymbol, TST_ClassFuncDecl ) );
    return true;
}

void TLexer::backTo ( const TCursor& pos )
{
    mCursor = pos;
    nextToken();
}

bool TLexer::var ( const TCursor& pos, TSymbolBase*& symbol, TSymbolType type )
{
    TDRINK ( type_name ( pos ) );
    fillVarName ( type , symbol );
    return true;
}

void TLexer::fillVarName ( TSymbolType type, TSymbolBase*& symbol )
{
    mCurVar = mCurString;
    assert ( mSymbols.getSymbol ( mCurString ) == nullptr );
    switch ( type )
    {
    case TST_Number:
        break;
    case TST_Class:
        if ( symbol == nullptr )
        {
            symbol = new TSymbolClass;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
        break;
    case TST_ClassFuncDecl:
    {
        if ( symbol == nullptr )
        {
            symbol = new TSymbolClassFuncDecl;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
    }
    break;
    case TST_ClassFuncImpl:
    {
        if ( symbol == nullptr )
        {
            symbol = new TSymbolClassFuncImpl;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
    }
    break;
    case TST_ClassMember:
    {
        if ( symbol == nullptr )
        {
            symbol = new TSymbolClassMember;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
    }
    break;
    case TST_Enum:
        if ( symbol == nullptr )
        {
            symbol = new TSymbolEnum;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
        break;
    case TST_Var:
    {
        if ( symbol == nullptr )
        {
            symbol = new TSymbolVariable;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
    }
    break;
    case TST_Macro:
        break;
    case TST_FuncDecl:
        break;
    case TST_FuncImpl:
    {
        if ( symbol == nullptr )
        {
            symbol = new TSymbolFuncImpl;
            symbol->mName = mCurString;
            mSymbols.addSymbol ( mCurString, symbol );
        }
    }
    break;
    default:
        break;
    }
    nextToken();
}

bool TLexer::type_name ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE ( type_name_common_const ( tpos ) );
        TSINGLE ( type_name_common ( tpos )  );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::type_name_raw_single ( const TCursor& pos )
{
    TEat_Token ( ITK_TypeName );
    return true;
}

bool TLexer::type_name_raw_templeate ( const TCursor& pos )
{
    TEat_Token ( ITK_TypeName );
    TEat_Token ( ITK_LeftAngleBracket );
    TEat_Token ( ITK_TypeName );
    TEat_Token ( ITK_RightAngleBracket );
    return true;
}

bool TLexer::class_func_construct ( const TCursor& pos, TSymbolBase*& outSymbol )
{
    if ( !mCurToken.mStr.equal ( mCurVar ) )
        return false;
    nextToken();
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( para ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::para ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        if ( para_null ( tpos ) )
            return true;
        if ( para_void ( tpos ) )
            return true;
        TLIST ( para_be ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::para_null ( const TCursor& pos )
{
    TEat_Token ( ITK_RightSuqareBracket );
    backTo ( pos );
    return true;
}
bool TLexer::para_void ( const TCursor& pos )
{
    //TEAT ( ITK_Void );
    if ( mCurToken.mStr == "void" )
        nextToken();
    else
        return false;
    return true;
}
bool TLexer::para_be ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSymbolBase* symbol = nullptr;
        TLIST ( para_be_mid ( tpos, symbol ), tpos );
        TSINGLE ( para_be_last ( tpos, symbol ) );
        res = false;
        break;
    }
    while ( 1 );
    return res;

}

bool TLexer::para_be_mid ( const TCursor& pos, TSymbolBase*& symbol )
{
    TDRINK ( var ( pos, symbol, TST_Para ) );
    TEat_Token ( ITK_Comma );
    return true;
}

bool TLexer::para_be_last ( const TCursor& pos, TSymbolBase*& symbol )
{
    TDRINK ( var ( pos, symbol, TST_Para ) );
    TCursor tpos = mLastCursor;
    TEat_Token ( ITK_RightSuqareBracket );
    backTo ( tpos );
    return true;
}

bool TLexer::class_func_deconstruct ( const TCursor& pos, TSymbolBase*& outSymbol )
{
    TEat_Token ( ITK_Wave );
    if ( !mCurVar == mCurString )
        return false;
    nextToken();
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( para ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::class_func_impl ( const TCursor& pos, TSymbolBase*& outSymbol )
{
    TDRINK ( func_impl ( pos, outSymbol ) );
    return true;
}

bool TLexer::sth_if_else ( const TCursor& pos )
{
    TEat_Token ( ITK_If );
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( expr ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TDRINK ( something ( mCursor ) );
    TEat_Token ( ITK_Else );
    TDRINK ( something ( mCursor ) );
    return true;
}

bool TLexer::something ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TLIST ( sth_if_else ( tpos ), tpos );
        TLIST ( sth_if ( tpos ), tpos );
        TLIST ( sth_common ( tpos ), tpos );
        TLIST_END ( mCurToken.mType.Value() == ITK_RightBrace );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::sth_if ( const TCursor& pos )
{
    TEat_Token ( ITK_If );
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( expr ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TDRINK ( something ( mCursor ) );
    return true;
}

bool TLexer::sth_common ( const TCursor& pos )
{
    TDRINK ( expr ( pos ) );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::expr_assign ( const TCursor& pos )
{
    TDRINK ( expr_assign_left ( pos ) );
    TEat_Token ( ITK_Equal );
    TDRINK ( expr_assign_right ( pos ) );
    return true;
}

bool TLexer::expr_assign_common ( const TCursor& pos )
{
    TEat_Token ( ITK_ID );
    TEat_Token ( ITK_Equal );
    TDRINK ( expr ( pos ) );
    TEat_Token ( ITK_Simicolon );
    return true;
}

//bool TLexer::expr_assign_declare ( const TCursor& pos )
//{
//    TDRINK ( var ( pos, TODO, TST_Var ) );
//    TEAT ( ITK_Equal );
//    TDRINK ( expr ( pos ) );
//    TEAT ( ITK_Simicolon );
//    return true;
//}
bool TLexer::expr_declare ( const TCursor& pos, TSymbolBase*& symbol )
{
    TDRINK ( var ( pos, symbol, TST_Var ) );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::expr_cal_var_common ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_var_number ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var_id ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::expr_cal_var_number_int ( const TCursor& pos )
{
    size_t cnt = mCurToken.mStr.size();
    for ( size_t i = 0; i < cnt; ++i )
    {
        if ( !isdigit ( mCurToken.mStr[i] ) )
            return false;
    }
    nextToken();
    return true;
}

bool TLexer::expr_cal_var_number_float ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_var_number_int ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var_number_float_dot_n ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var_number_float_n_dot_n ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var_number_float_n_dot ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::expr_cal_var_number_float_dot_n ( const TCursor& pos )
{
    TEat_Token ( ITK_Dot );
    TDRINK ( expr_cal_var_number_int ( mCursor ) );
    nextToken();
    return true;
}

bool TLexer::expr_cal_var_number_float_n_dot_n ( const TCursor& pos )
{
    TDRINK ( expr_cal_var_number_int ( mCursor ) );
    TEat_Token ( ITK_Dot );
    TDRINK ( expr_cal_var_number_int ( mCursor ) );
    nextToken();
    return true;
}

bool TLexer::expr_cal_var_number_float_n_dot ( const TCursor& pos )
{
    TDRINK ( expr_cal_var_number_int ( mCursor ) );
    TEat_Token ( ITK_Dot );
    nextToken();
    return true;
}

bool TLexer::expr_cal_var_number ( const TCursor& pos )
{
    TDRINK ( expr_cal_var_number_float ( pos ) );
    return true;
}

bool TLexer::expr_cal_var_id ( const TCursor& pos )
{
    TEat_Token ( ITK_ID );
    return true;
}

bool TLexer::expr_cal_argthom_common ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_argthom_add ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_argthom_sub ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_argthom_multiply ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_argthom_divide ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}


bool TLexer::expr_cal_var_b ( const TCursor& pos )
{
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( expr_cal_var_common ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    return true;
}

bool TLexer::expr_cal_var ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_var_b ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var_common ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::expr_cal_argthom_add ( const TCursor& pos )
{
    TDRINK ( expr_cal_var ( pos ) );
    TEat_Token ( ITK_Add );
    TDRINK ( expr_cal_var ( mCursor ) );
    return true;
}

bool TLexer::expr_cal_argthom_sub ( const TCursor& pos )
{
    TDRINK ( expr_cal_var ( pos ) );
    TEat_Token ( ITK_Sub );
    TDRINK ( expr_cal_var ( mCursor ) );
    return true;
}

bool TLexer::expr_cal_argthom_multiply ( const TCursor& pos )
{
    TDRINK ( expr_cal_var ( pos ) );
    TEat_Token ( ITK_Multiply );
    TDRINK ( expr_cal_var ( mCursor ) );
    return true;
}

bool TLexer::expr_cal_argthom_divide ( const TCursor& pos )
{
    TDRINK ( expr_cal_var ( pos ) );
    TEat_Token ( ITK_Divide );
    TDRINK ( expr_cal_var ( mCursor ) );
    return true;
}

bool TLexer::expr_assign_left ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSymbolBase* symbol = nullptr;
        TSINGLE_MULTI ( var ( tpos, symbol, TST_Var ), tpos );
        TSINGLE_MULTI ( expr_cal_var_id ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::expr_assign_right ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_argthom ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_var ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::expr_cal_argthom_b ( const TCursor& pos )
{
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( expr_cal_argthom_common ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    return true;
}

bool TLexer::expr_cal_argthom ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( expr_cal_argthom_b ( tpos ), tpos );
        TSINGLE_MULTI ( expr_cal_argthom_common ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::class_member ( const TCursor& pos, TSymbolBase*& outSymbol )
{
    TDRINK ( var ( pos, outSymbol, TST_ClassMember ) );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::func_declare ( const TCursor& pos, TSymbolBase*& symbol, TSymbolType type )
{
    TDRINK ( var ( mCursor, symbol, type ) );
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( para ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TEat_Token ( ITK_Simicolon );
    return true;
}

bool TLexer::func_impl ( const TCursor& pos, TSymbolBase*& symbol )
{
    TEat_Token ( ITK_TypeName );
    fillVarName ( TST_FuncImpl , symbol );
    TEat_Token ( ITK_LeftSquareBracket );
    TDRINK ( para ( mCursor ) );
    TEat_Token ( ITK_RightSuqareBracket );
    TDRINK ( block ( mCursor ) );
    return true;
}

bool TLexer::type_name_common ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( type_name_common_ref ( tpos ), tpos );
        TSINGLE_MULTI ( type_name_common_point ( tpos ) , tpos );
        TSINGLE_MULTI ( type_name_raw ( tpos ), tpos );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::type_name_common_const ( const TCursor& pos )
{
    TEat_Token ( ITK_Const );
    TDRINK ( type_name_common ( pos ) );
    return true;
}

bool TLexer::type_name_raw ( const TCursor& pos )
{
    bool res = true;
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE ( type_name_raw_single ( tpos ) );
        TSINGLE ( type_name_raw_templeate ( tpos ) );
        res = false;
        break;
    }
    while ( 1 );
    return res;
}

bool TLexer::type_name_common_ref ( const TCursor& pos )
{
    TDRINK ( type_name_raw ( pos ) );
    TEat_Token ( ITK_And );
    return true;
}

bool TLexer::type_name_common_point ( const TCursor& pos )
{
    TDRINK ( type_name_raw ( pos ) );
    TEat_Token ( ITK_Multiply );
    return true;
}

bool TLexer::class_impl_list ( const TCursor& pos )
{
    do
    {
        TCursor tpos = mLastCursor;
        TSymbolBase* symbol = nullptr;
        TLIST_TEST ( class_func_construct ( tpos , symbol ), tpos );
        TLIST_TEST ( class_func_deconstruct ( tpos, symbol ), tpos );
        TLIST_TEST ( class_member ( tpos, symbol ), tpos );
        TLIST_TEST ( class_func_impl ( tpos , symbol ), tpos );
        TLIST_TEST ( class_func_declare ( tpos, symbol ), tpos );
        TLIST_END ( mCurToken.mType.Value() == ITK_RightBrace );
        break;
    }
    while ( 1 );
    return true;
}

bool TLexer::return_sth ( const TCursor& pos )
{
    TEat_Token ( ITK_Return );
    TDRINK ( expr ( mCursor ) );
    return true;
}

bool TLexer::return_void ( const TCursor& pos )
{
    TEat_Token ( ITK_Return );
    return true;
}

bool TLexer::sth_return ( const TCursor& pos )
{
    do
    {
        TCursor tpos = mLastCursor;
        TSINGLE_MULTI ( return_sth ( tpos ), tpos );
        TSINGLE_MULTI ( return_void ( tpos ), tpos );
        break;
    }
    while ( 1 );
    return true;
}







TCursor::TCursor()
    : mPosition ( -1 )
    , mLine ( 1 )
    , mColoumns ( 0 )
{

}

#pragma once
#include "XString.h"
#include "XHashMap.h"
#include "TToken.h"
#include "TGrammar.h"
#include "TSymbols.h"
#include "TSymbolBase.h"

struct TCursor
{
    int mPosition;
    int mLine;
    int mColoumns;
    TCursor();
};

class TLexer
{
public:
    typedef TToken MyToken;
    typedef TGrammar MyGrammar;
    typedef char CH;
    static const int ErrorToken = -1;
public:
    TLexer ( void );
    ~TLexer ( void );

    bool parserFile ( const char* fileName );
    bool parser (const CH* content );
    void bind ( int type, const CH* str );

    int nextToken();

    bool eat ( int token );

    bool file_header();

    bool block ( const TCursor& pos );
    bool expr ( const TCursor& pos );

    bool expr_cal_argthom ( const TCursor& pos );
    bool expr_cal_argthom_common ( const TCursor& pos );
    bool expr_cal_argthom_b ( const TCursor& pos );
    bool expr_cal_argthom_add ( const TCursor& pos );
    bool expr_cal_argthom_sub ( const TCursor& pos );
    bool expr_cal_argthom_multiply ( const TCursor& pos );
    bool expr_cal_argthom_divide ( const TCursor& pos );


    bool expr_cal_var ( const TCursor& pos );
    bool expr_cal_var_common ( const TCursor& pos );
    bool expr_cal_var_b ( const TCursor& pos );

    //bool expr_cal_var_braket ( const TCursor& pos );

    bool expr_cal_var_id ( const TCursor& pos );

    bool expr_cal_var_number ( const TCursor& pos );

    bool expr_cal_var_number_int ( const TCursor& pos );
    bool expr_cal_var_number_float ( const TCursor& pos );
    bool expr_cal_var_number_float_dot_n ( const TCursor& pos );
    bool expr_cal_var_number_float_n_dot_n ( const TCursor& pos );
    bool expr_cal_var_number_float_n_dot ( const TCursor& pos );

    bool expr_declare ( const TCursor& pos, TSymbolBase*& symbol );
    bool expr_assign ( const TCursor& pos );
    bool expr_assign_left ( const TCursor& pos );
    bool expr_assign_right ( const TCursor& pos );
    bool expr_assign_common ( const TCursor& pos );
    //bool expr_assign_declare ( const TCursor& pos );

    bool something ( const TCursor& pos );
    bool sth_common ( const TCursor& pos );
    bool sth_if ( const TCursor& pos );
    bool sth_if_else ( const TCursor& pos );
    bool sth_do ( const TCursor& pos );
    bool sth_do_while ( const TCursor& pos );
    bool sth_while_do ( const TCursor& pos );
    bool sth_for ( const TCursor& pos );
	bool sth_return(const TCursor& pos);

	bool return_sth(const TCursor& pos);
	bool return_void(const TCursor& pos);

    bool class_delcare ( const TCursor& pos, TSymbolBase*& symbol );
    bool class_impl ( const TCursor& pos, TSymbolBase*& symbol );

	bool class_impl_list(const TCursor& pos);
    bool class_member ( const TCursor& pos, TSymbolBase*& outSymbol );
    bool class_func_declare ( const TCursor& pos, TSymbolBase*& outSymbol );
    bool class_func_impl ( const TCursor& pos, TSymbolBase*& outSymbol );
    bool class_func_construct ( const TCursor& pos, TSymbolBase*& outSymbol );
    bool class_func_deconstruct ( const TCursor& pos, TSymbolBase*& outSymbol );

    bool func_declare ( const TCursor& pos, TSymbolBase*& symbol, TSymbolType type );
    bool func_impl ( const TCursor& pos, TSymbolBase*& symbol );

    bool var ( const TCursor& pos, TSymbolBase*& symbol, TSymbolType type );
    bool para ( const TCursor& pos );
    bool para_null ( const TCursor& pos );
    bool para_void ( const TCursor& pos );
    bool para_be ( const TCursor& pos );
    bool para_be_mid ( const TCursor& pos, TSymbolBase*& symbol );
    bool para_be_last ( const TCursor& pos, TSymbolBase*& symbol );

    bool type_name ( const TCursor& pos );

    bool type_name_common_ref ( const TCursor& pos );
    bool type_name_common_point ( const TCursor& pos );
    bool type_name_common_const ( const TCursor& pos );

    bool type_name_common ( const TCursor& pos );

    bool type_name_raw ( const TCursor& pos );

    bool type_name_raw_single ( const TCursor& pos );
    bool type_name_raw_templeate ( const TCursor& pos );

    inline bool isEnd();
    inline bool isTokenTypeValid() const;
protected:
	void fillVarName(TSymbolType type, TSymbolBase*& symbol);

    int getTokenType ( const CH* str ) const;
    inline CH getNextChar();
    int fillToken ( const CH* word );
    void selse();
    void reportError ( int token );

    void backTo ( const TCursor& pos );
	void addSymbol(TSymbolBase* symbol);
	void deleteSymbol(TSymbolBase* symbol);

    CXHashMap<CXCharString<CH>, int> mTokenMap;
    CXCharString<CH> mCurString;
    CXBuffer mBuffer;

    CH mWord[2];


    TToken mCurToken;
    TCursor mLastCursor;
    TCursor mCursor;
    MyGrammar mGrammar;

    GString mCurVar;

    TSymbols mSymbols;
};

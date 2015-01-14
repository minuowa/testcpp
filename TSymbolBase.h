#pragma once
enum TSymbolType
{
    TST_Number,
    TST_Class,
	TST_ClassFuncDecl,
	TST_ClassFuncImpl,
	TST_ClassMember,
	TST_Enum,
    TST_Var,
	TST_Para,
    TST_Macro,
    TST_FuncDecl,
    TST_FuncImpl,
};
enum TClassField
{
	TCF_Public,
	TCF_Protected,
	TCF_Private,
	TCF_Max,
};
#define TSymbolCreator(type) TSymbolBase* create()\
	{\
		return new type;\
	}
class TSymbolBase
{
public:
    TSymbolBase ( const char* name, const char* typeID, TSymbolType type );
    TSymbolBase();
	~TSymbolBase();
    GString mName;
    GString mTypeID;
    TSymbolType mSymbolType;
    CXDynaArray<GString> mDecorates;
};
class TSymbolVariable: public TSymbolBase
{
public:
    TSymbolVariable()
    {
        mSymbolType = TST_Var;
    }
};
class TSymbolClassFiled
{
public:
	TClassField mField;
};
class TSymbolFuncDecl: public TSymbolBase
{
public:
	TSymbolFuncDecl()
	{
		mSymbolType = TST_FuncDecl;
	}
};
class TSymbolFuncImpl: public TSymbolBase
{
public:
	TSymbolFuncImpl()
	{
		mSymbolType = TST_FuncImpl;
	}
};
class TSymbolClassFuncImpl: public TSymbolClassFiled,public TSymbolFuncImpl
{
public:
    TSymbolClassFuncImpl()
    {
        mSymbolType = TST_ClassFuncImpl;
    }
};
class TSymbolClassFuncDecl: public TSymbolClassFiled,public TSymbolFuncDecl
{
public:
	TSymbolClassFuncDecl()
	{
		mSymbolType = TST_ClassFuncDecl;
	}
};
class TSymbolClassMember: public TSymbolClassFiled, public TSymbolBase
{
public:
    TSymbolClassMember()
    {
        mSymbolType = TST_ClassMember;
    }
};
class TSymbolClass: public TSymbolBase
{
public:
    TSymbolClass()
    {
        mSymbolType = TST_Class;
    }
	CXDynaArray<GString> mMethods;
	CXDynaArray<GString> mMembers;
};
class TSymbolMacro: public TSymbolBase
{
public:
    TSymbolMacro()
    {
        mSymbolType = TST_Macro;
    }
    GString mContent;
};
struct TEnumStruct
{
	GString mName;
	int mValue;
};
class TSymbolEnum: public TSymbolBase
{
public:
    TSymbolEnum()
    {
        mSymbolType = TST_Enum;
    }
	CXDynaArray<TEnumStruct*> mContents;
};
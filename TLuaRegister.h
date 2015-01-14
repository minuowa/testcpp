#ifndef TLuaRegister_h__
#define TLuaRegister_h__

#include "XBase.h"
#include "TLexer.h"
struct TLuaObj
{
    GString mBaseClass;
    GString mThisClass;
    GStringArr mFunctions;
    bool mOut;
    TLuaObj()
    {
        mOut = false;
    }
};
//-------------------------------------------------------------------------
/**
	@brief
*/
class TLuaRegister
{
public:
    TLuaRegister ( void );
    ~TLuaRegister ( void );
    bool init ( const char* path, const char* ext , const char* saveFile );
private:
    bool parserFile ( const char* fileName );
    bool parseCurTokens();
    void saveToFile ( const char* file );
	void outRawLuaObj(std::ofstream& ofs,TLuaObj* rawObj);
	void outLuaObj(std::ofstream& ofs,TLuaObj* pObj);
	void  sortToStack(TLuaObj* rawObj,CXStack<GString>& stack);

    GStringArr mHeaders;
    GStringArr mGlobalFunctions;
    CXMap<GString, TLuaObj*> mLuaObjs;
    TCharLexer mLexer;
    GString mCurClass;
    char* mBuffer;
};

#endif // TLuaRegister_h__

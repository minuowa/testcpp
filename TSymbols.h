#pragma once
class TSymbolBase;
typedef CXMap<GString,TSymbolBase*> CXSymbolMap;
class TSymbols
{
public:
	TSymbols(void);
	~TSymbols(void);
	TSymbolBase* getSymbol(const char* name) const;
	void addSymbol(const char* name,TSymbolBase* symbol);
	void enterField();
	void leaveField();
private:
	void showMe();

	CXDynaArray<CXSymbolMap*> mSymbolArray;
};


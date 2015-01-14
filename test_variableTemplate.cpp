#include "stdafx.h"
#include "test_variableTemplate.h"

//template<typename T,typename ...Args>
//class TAddAll  
//{
//public:
//	TAddAll()
//	{
//		static const int cnt=sizeof...(Args);
//		for (int i=0;i<cnt;++i)
//		{
//			addType<Args i>(mSum);
//		}
//	}
//	 int get()
//	{
//		return mSum;
//	}
//	int mSum;
//};
test_variableTemplate::test_variableTemplate(void)
{
	int voidTypeCount=0;
	addType<test_variableTemplate>(voidTypeCount);

	//TAddAll<int,bool,char,long> addall;
	//int x=addall.get();

}


test_variableTemplate::~test_variableTemplate(void)
{
}

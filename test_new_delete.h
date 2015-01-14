#pragma once
#include <vector>
class test_new_delete
{
public:
    test_new_delete ( void );
    ~test_new_delete ( void );
};
static const int ChildCount = 4;
class TTerNode
{
public:
    TTerNode();
    ~TTerNode();
    void setLevel ( int lv )
    {
        mLevel = lv;
        if ( mLevel > 1 )
        {
            //for ( int i = 0; i < ChildCount; ++i )
            //{
            //    mChildren[i] = new TTerNode();
            //    mChildren[i]->setLevel ( mLevel - 1 );
            //}
        }
    }
    void* operator new ( unsigned int n )
    {
        return ::operator new ( n );
    }
    void operator delete ( void* p )
    {
        return ::delete p;
    }
	void operator delete ( void* p,unsigned int n )
	{
		return ::delete p;
	}
    TTerNode* mChildren[ChildCount];
private:
    int mLevel;
	std::vector<int> mArray;
};

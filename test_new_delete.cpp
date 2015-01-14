#include "stdafx.h"
#include "test_new_delete.h"


test_new_delete::test_new_delete ( void )
{
    TTerNode* p = new TTerNode;
    p->setLevel ( 3 );
    delete p;
}


test_new_delete::~test_new_delete ( void )
{
}

TTerNode::TTerNode()
{
    for ( int i = 0; i < ChildCount; ++i )
        mChildren[i] = nullptr;
}

TTerNode::~TTerNode()
{
	mArray.clear();
    for ( int i = 0; i < ChildCount; ++i )
    {
        if ( mChildren[i] != nullptr )
        {
            delete mChildren[i];
        }
    }
}

#include "stdafx.h"
#include "test_regex.h"
#include <regex>
#include <assert.h>
#include <iosfwd>
test_regex::test_regex ( void )
{
    std::ifstream testfile ( "regex.test" );
    string buffer;
    if ( testfile.is_open() )
    {
        testfile.seekg ( 0, ios_base::end ); //移动到文件尾
        unsigned int cnt = testfile.tellg();//取得当前位置的指针长度->即文件长度
        testfile.seekg ( ios_base::beg ); //移动到原来的位置
        buffer.assign ( cnt, 0 );
        testfile.read ( ( char* ) buffer.c_str(), cnt );
    }
    {
        //test header
        regex pattern ( ".*#include .*", regex_constants::extended  );
        match_results<string::const_iterator> result;
        bool valid = regex_search ( buffer, result, pattern );
for ( auto s: result )
        {
            std::cout << s << std::endl;
        }
        assert ( valid );
    }
    {
        cout << "TestHeaders:" << endl;
        regex pattern ( "#include .*", regex_constants::basic  );
        const std::sregex_token_iterator end;

        for ( std::sregex_token_iterator i ( buffer.begin(), buffer.end(), pattern ); i != end ; ++i )
        {
            std::cout << *i << std::endl;
        }
    }

    {
        cout << "TestClass:" << endl;
		//regex pattern ( "class .*", 
		//regex pattern ( "class .*\n", 
		//regex pattern ( "class .*\n\\{", 
		regex pattern ( "class .*\n\\{[.\\s\\S\\w\\W]*\\}", 
			regex_constants::ECMAScript
			/*||regex_constants::ECMAScript*/);
        const std::sregex_token_iterator end;

        for ( std::sregex_token_iterator i ( buffer.begin(), buffer.end(), pattern ); i != end ; ++i )
        {
            std::cout << *i << std::endl;
        }
    }
}


test_regex::~test_regex ( void )
{
}

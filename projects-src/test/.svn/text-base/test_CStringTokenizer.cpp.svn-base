/* 
 * Testing CStringTokenizer
 *
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <vector>

#include "CStringTokenizer.hpp"
using namespace std;

//------------------------------------------------------------------------------
//
bool cmp( const char* str1, const string &str2 )
{
    return strcmp( str1, str2.c_str() ) == 0;
}

//------------------------------------------------------------------------------
//
void Test_1()
{
    //cout << "> Basic test 1" << endl;

    // Data
    string str = "This is a test string : 1 2 3 . , :: ñ    ' '  *  end    ";
    char const* charTokens[] = { "This", "is", "a", "test", "string",
                                 ":", "1", "2", "3", ".",
                                 ",", "::", "ñ", "'", "'",
                                 "*", "end" };
    vector<string> list( charTokens, charTokens + 17 );

    // Tests...
    CStringTokenizer tokenizer( str.c_str() );

    assert( tokenizer.numTokens() == 17 );

    assert( tokenizer.hasMoreTokens() == true );
    assert( cmp( tokenizer.peekNextToken(), "This" ) == true );
    assert( cmp( tokenizer.peekNextToken(), "This" ) == true );

    int num = 0;
    while ( tokenizer.hasMoreTokens() )
    {
        assert( cmp( tokenizer.getNextToken(), list[ num ] ) == true );
        ++num;
    }

    assert( tokenizer.hasMoreTokens() == false );
//    assert( tokenizer.getVector() == list );
}

//------------------------------------------------------------------------------
//
void Test_2()
{
    //cout << "> Basic test 2: avoid repeated delimiters = false" << endl;

    string str = "This is a test string : 1 2 3 . , :: ñ    ' '  *  end    ";
    char const* charTokens2[] = { "This", "is", "a", "test", "string",
                                 ":", "1", "2", "3", ".",
                                 ",", "::", "ñ", "", "",
                                 "", "'", "'", "", "*",
                                 "", "end", "", "", "" };
    vector<string> list2( charTokens2, charTokens2 + 25 );


    CStringTokenizer tokenizer2( str.c_str(), ' ', false );

    assert( tokenizer2.numTokens() == 25 );

    assert( tokenizer2.hasMoreTokens() == true );
    assert( cmp( tokenizer2.peekNextToken(), "This" ) == true);
    assert( cmp( tokenizer2.peekNextToken(), "This" ) == true );

    int num = 0;
    while ( tokenizer2.hasMoreTokens() )
    {
        assert( cmp( tokenizer2.getNextToken(), list2[ num ] ) == true );
        ++num;
    }

    assert( tokenizer2.hasMoreTokens() == false );
//    assert( tokenizer2.getVector() == list2 );
}

//------------------------------------------------------------------------------
//
void Test_3()
{
    //cout << "> Basic test 3: Changing delimiter" << endl;

    string str = "This is a test string : 1 2 3 . , :: ñ    ' '  *  end    ";
    char const* charTokens3[] = { "This is a test string : 1 2 3 . ",
                                 " :: ñ    ' '  *  end    " };
    vector<string> list3( charTokens3, charTokens3 + 2 );

    CStringTokenizer tokenizer3( str.c_str(), ',', true );


    assert( tokenizer3.numTokens() == 2 );

    assert( tokenizer3.hasMoreTokens() == true );
    assert( cmp( tokenizer3.peekNextToken(), "This is a test string : 1 2 3 . " ) == true );
    assert( cmp( tokenizer3.peekNextToken(), "This is a test string : 1 2 3 . " ) == true );

    int num = 0;
    while ( tokenizer3.hasMoreTokens() )
    {
        assert( cmp( tokenizer3.getNextToken(), list3[ num ] ) == true );
        ++num;
    }

    assert( tokenizer3.hasMoreTokens() == false );
//    assert( tokenizer3.getVector() == list3 );
}

//------------------------------------------------------------------------------
//
void Test_4()
{
    //cout << "> Basic test 4" << endl;

    // Data
    string str = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20";
    char const* charTokens[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                                 "11","12","13","14","15","16","17","18","19","20" };
    vector<string> list( charTokens, charTokens + 20 );

    // Tests...
    CStringTokenizer tokenizer( str.c_str(), ',', true );


    assert( tokenizer.numTokens() == 20 );

    assert( tokenizer.hasMoreTokens() == true );
    assert( cmp( tokenizer.peekNextToken(), "1" ) == true );
    assert( cmp( tokenizer.peekNextToken(), "1" ) == true );

    int num = 0;
    while ( tokenizer.hasMoreTokens() )
    {
        assert( cmp( tokenizer.getNextToken(), list[ num ] ) == true );
        ++num;
    }

    assert( tokenizer.hasMoreTokens() == false );
//    assert( tokenizer.getVector() == list );
}

//------------------------------------------------------------------------------
//
void Test_5()
{
    //cout << "> Basic test 5" << endl;

    // Data
    string str = "1,2,3,4,5,6,7,8,9,10,11,,,,,,12,13,14,15,16,17,18,19,20";
    char const* charTokens[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                                 "11","",  "",  "",  "",  "",  "12","13","14","15",
                                 "16","17","18","19","20" };
    vector<string> list( charTokens, charTokens + 25 );

    // Tests...
    CStringTokenizer tokenizer( str.c_str(), ',', false );


    assert( tokenizer.numTokens() == 25 );

    assert( tokenizer.hasMoreTokens() == true );
    assert( cmp( tokenizer.peekNextToken(), "1" ) == true );
    assert( cmp( tokenizer.peekNextToken(), "1" ) == true );

    int num = 0;
    while ( tokenizer.hasMoreTokens() )
    {
        assert( cmp( tokenizer.getNextToken(), list[ num ] ) == true );
        ++num;
    }

    assert( tokenizer.hasMoreTokens() == false );
//    assert( tokenizer.getVector() == list );
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "-----------------------------------------------------------" << endl;
    cout << "> Testing CStringTokenizer" << endl;

    Test_1();

    Test_2();

    Test_3();

    Test_4();

    Test_5();

    cout << "> All tests have been successful!" << endl;
    cout << "-----------------------------------------------------------" << endl;

    return (EXIT_SUCCESS);
}

/**
*/

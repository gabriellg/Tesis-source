/* -----------------------------------------------------------------------------
*  test_CArgumentParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "CArgumentParser.hpp"
#include "CString.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
void test1()
{
    class CArgumentParser *parser;
    int numArgs;
    bool valueFlag;
    char valueChar;
    char *valueStr;
    int valueInt;
    unsigned long valueULong;
    double valueDouble;

    numArgs = 12;
    const char* argList[] = {   "program name",
                                "-flag",
                                "-char",
                                "c",
                                "-str",
                                "valueStr",
                                "-int",
                                "-2",
                                "-ulong",
                                "1",
                                "-double",
                                "2.2" };

    parser = new CArgumentParser( "Program description" );
    parser->addArgFlag( "-flag", "Flag argument description" );
    parser->addArgChar( "-char", "Char argument description" );
    parser->addArgStr( "-str", "Str argument description" );
    parser->addArgInt( "-int", "Int argument description" );
    parser->addArgULong( "-ulong", "ULong argument description" );
    parser->addArgDouble( "-double", "Double argument description" );
    parser->addDescription( "auxiliar description" );

    if( parser->validateParse( numArgs, argList, NULL ) == true )
    {
        valueFlag = parser->getArgFlag( "-flag" );
        valueChar = parser->getArgChar( "-char" );
        valueStr = parser->getArgStr( "-str" );
        valueInt = parser->getArgInt( "-int" );
        valueULong = parser->getArgULong( "-ulong" );
        valueDouble = parser->getArgDouble( "-double" );
    }
    else
    {
        assert_message( "Error in validate-parse function" );
    }

    assert( valueFlag == true );
    assert( valueChar == 'c' );
    assert( CString::equal( valueStr, "valueStr" ) == true );
    assert( valueInt == -2 );
    assert( valueULong == 1 );
    assert( valueDouble == 2.2 );

    delete valueStr;
    delete parser;
}

//------------------------------------------------------------------------------
//
int main( int argc, const char** argv )
{
    test1();

    return 0;
}

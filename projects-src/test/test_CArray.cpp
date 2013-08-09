/* 
 * File:   test_CArray.cpp
 * Author: javi
 *
 * Created on October 13, 2010, 11:49 PM
 */

#include <iostream>
#include <stdlib.h>
#include "CArray.hpp"
#include "CMatrixData.hpp"
#include "asrtbas.h"
using namespace std;


//------------------------------------------------------------------------------
//
class CTest
{
    public:
        CTest( unsigned int value ) { mValue = value; };
        CTest(const class CTest *test) { mValue = test->mValue;}

        ~CTest() { cout << "delete CTest de " << mValue << endl; };

        unsigned int getValue() const { return mValue; };

    private:
        unsigned int mValue;
};

//------------------------------------------------------------------------------
//
void test_values(const class CArray<CTest> *vector, unsigned long ind, unsigned long value)
{
    class CTest *test;

    assert_no_null(vector);
    test = vector->get(ind);
    assert(test->getValue() == value);
}

//------------------------------------------------------------------------------
//
static void prv_test_create_destroy_and_access(void)
{
    class CTest *test;
    class CArray<CTest> *vector;

    vector = new CArray<CTest>();
    assert( vector->size() == 0 );

    test = new CTest( 7 );
    vector->add( test );
    test = new CTest( 6 );
    vector->add( test );
    test = new CTest( 3 );
    vector->add( test );

    assert( vector->size() == 3 );

    test_values(vector, 0, 7);
    test_values(vector, 1, 6);
    test_values(vector, 2, 3);

    delete vector;
}

//------------------------------------------------------------------------------
//
static void prv_test_copy(void)
{
    class CTest *test;
    class CArray<CTest> *vector, *vector_duplicate;

    vector = new CArray<CTest>();
    assert( vector->size() == 0 );

    test = new CTest( 5 );
    vector->add( test );
    test = new CTest( 7 );
    vector->add( test );
    test = new CTest( 9 );
    vector->add( test );

    vector_duplicate = new CArray<CTest>(vector);

    assert( vector_duplicate->size() == 3 );

    test_values(vector_duplicate, 0, 5);
    test_values(vector_duplicate, 1, 7);
    test_values(vector_duplicate, 2, 9);

    delete vector_duplicate;
    delete vector;
}

//------------------------------------------------------------------------------
//
static void prv_testMatrixData(void)
{
    class CMatrixData<CTest> *matrix;
    class CTest *valueTest;
    unsigned long width, height;

    cout << "-----------------------------------------------------------" << endl;
    cout << "> Testing CMatrixData" << endl;

    matrix = new CMatrixData<CTest>(2,3);

    valueTest = new CTest(1);
    matrix->set(0, 0, valueTest);
    valueTest = new CTest(2);
    matrix->set(0, 1, valueTest);

    matrix->size(&width, &height);

    for (unsigned long i = 0; i < width; i++)
    {
        for (unsigned long j = 0; j < height; j++)
        {
            cout << "(" << j << "," << i << ")";

            if (matrix->hasData(j, i) == true)
            {
                valueTest = matrix->get(j, i);
                cout << valueTest->getValue();
            }
            else
                cout << "Vacio";

            cout << endl;
        }
    }

    delete matrix;
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "-----------------------------------------------------------" << endl;
    cout << "> Testing CArray" << endl;

    prv_test_create_destroy_and_access();

    prv_test_copy();

    cout << "> All tests have been successful!" << endl;
    cout << "-----------------------------------------------------------" << endl;

    prv_testMatrixData();

    return (EXIT_SUCCESS);
}

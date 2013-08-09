/* -----------------------------------------------------------------------------
*  test_read3d.cpp
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CReaderModel3d.hpp"
#include "CModel3d.hpp"
#include "stdlib.h"
#include "stdio.h"

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        class CModel3d *model;

        model = CReaderModel3d::readModel3d(argv[1]);

        printf("OK\n");

        delete model;
    }
    else
        printf("%s namefile\n", argv[0]);

    return (EXIT_SUCCESS);
}

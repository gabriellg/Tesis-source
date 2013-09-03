/* -----------------------------------------------------------------------------
*  CLogTxt.cpp
*  Author: Gabriel López García (gabriel DOT lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CLogTxt.hpp"

#include "asrtbas.h"
#include "stdio.h"

//------------------------------------------------------------------------------
//
void CLogTxt::lineText(const char *text)
{
	assert_no_null(text);
	printf("%s\n", text);
}

/* -----------------------------------------------------------------------------
*  GMVCompiler
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "asrtbas.h"
#include "CLogTxt.hpp"
#include "CGMVWorld.hpp"

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
	int result;
	class CLogTxt *logTxt;

	logTxt = new CLogTxt();

	if (argc != 2)
	{
		logTxt->line("%s nameFile.gmv", argv[0]);
		result = -1;
	}
	else
	{
		class CGMVWorld *world;

		world = NULL;

		if (CGMVWorld::readXML(argv[1], logTxt, &world) == true)
		{
			delete world;
			result = 0;
		}
		else
		{
			assert(world == NULL);
			result = -1;
		}
	}

	delete logTxt;

    return result;
}

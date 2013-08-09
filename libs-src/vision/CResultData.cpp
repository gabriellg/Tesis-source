#include "CResultData.hpp"

#include "asrtbas.h"
#include "strbas.h"
#include "array.h"

//---------------------------------------------------------
	
bool CResultData::isType(const char *typeData) const
{
	const char *typeClass;
	
	typeClass = type();
	return strbas_equalsStrings(typeClass, typeData);
}

//---------------------------------------------------------

static bool i_isElementType(const class CResultData *result, const char *typeResult)
{
	assert_no_null(result);
	return result->isType(typeResult);
}

//---------------------------------------------------------
	
class CResultData *cresultdata_searchDataResultPrivate(const ArrClass(CResultData) *results, const char *type)
{
	bool existe;
	unsigned long ind;
	
	existe = array_BuscarUnicaClass(CResultData, results, char, type, i_isElementType, &ind);
	assert(existe == true);
	
	return array_GetClass(CResultData, results, ind);
}

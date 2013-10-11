/*
 * CAgentPrimitive.cpp
 *
 *  Created on: 11/10/2013
 *      Author: gabi
 */

#include "CAgentPrimitive.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CString.hpp"
#include "CArray.hpp"
#include "CFigure.hpp"

//---------------------------------------------------------------

CAgentPrimitive::CAgentPrimitive(const char *symbol)
{
    m_dataPrivate = (struct SPrvAgentPrimitive *)CString::copy(symbol);
}

//---------------------------------------------------------------

CAgentPrimitive::~CAgentPrimitive()
{
    char *strPrimitive;

    strPrimitive = (char *)m_dataPrivate;
    FREE_T(&strPrimitive, char);
}

//---------------------------------------------------------------

class CArray<IObjectDraw> *CAgentPrimitive::createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt)
{
    class CArray<IObjectDraw> *objectsDraw;
    class IObjectDraw *primitive;

    assert(childsOpt == NULL);

    primitive = new CFigure((const char *)m_dataPrivate);

    objectsDraw = new CArray<IObjectDraw>(1);
    objectsDraw->set(0, primitive);

    return objectsDraw;
}

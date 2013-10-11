/*
 * CAgentPrimitive.hpp
 *
 *  Created on: 11/10/2013
 *      Author: gabi
 */

#include "CAgentNoEvolution.hpp"

class CAgentPrimitive: public CAgentNoEvolution
{
public:
    CAgentPrimitive(const char *symbol);
    virtual ~CAgentPrimitive();

    virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt);

private:

    struct SPrvAgentPrimitive *m_dataPrivate;

};

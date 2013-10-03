// CCircuit.hpp: Agent Circuit.

#include "CAgentNoEvolution.hpp"

class CCircuit: public CAgentNoEvolution
{
public:

    CCircuit() {;}

private:

    virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt);

};

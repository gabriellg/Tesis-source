//CAgentCamera.cpp : Agent to handle camera.

#include "CAgent.hpp"

class CAgentCamera: public CAgent
{
public:

    CAgentCamera(class CPositionCamera **positionCameraInitial);
    virtual ~CAgentCamera();

private:

    virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
    virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **nextGenerationSons);

    virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt);

    struct SPrvAgentCamera *m_dataPrivate;
};

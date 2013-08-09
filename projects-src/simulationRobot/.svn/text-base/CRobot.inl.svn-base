/* -----------------------------------------------------------------------------
 *  CRobot.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentWithRepresentation.hpp"

class CRobot : public CAgentWithRepresentation
{
    public:

        CRobot();
        virtual ~CRobot();

    private:

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDraw);

        struct SPrvDataPrivateRobot *m_dataPrivate;

};

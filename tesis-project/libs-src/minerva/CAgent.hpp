/* -----------------------------------------------------------------------------
 *  CAgent.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef CAGENT_HPP_
#define CAGENT_HPP_

class CAgent
{
    public:

        CAgent(void);
        virtual ~CAgent();

        void appendChild(class CAgent **son);
        void appendChildren(class CArrayRef<CAgent> **sons);

        class CAgent *nextGeneration(class CCollectionEventsSystem *allEvents);
        class CArrayRef<CAgent> *getRepresentation(class CTypeDescription *evtDescription);
        void traslateRepresentationToDescription(class CTypeDescription *evtDescription);

        static void deleteAllAgentNotRepeted(class CAgent **agent);
        static void eraseOldObjects(class CAgent *newAgent, class CAgent **oldAgent);

    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) = 0;
        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents) = 0;
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) = 0;

        virtual class CAgent *representation(class CTypeDescription *evtDescription) = 0;

        virtual void beginRepresentation(class CTypeDescription *evtDescription) = 0;
        virtual void drawRepresentation(class CTypeDescription *evtDescription)  = 0;
        virtual void endRepresentation(class CTypeDescription *evtDescription) = 0;

        class CAgent *prv_nextGenerationRecursive(unsigned long numRecursive, class CCollectionEventsSystem *allEvents);
        void prv_getRepresentationRecursive(
                                unsigned long numRecursive,
                                class CArrayRef<CAgent> *stringRepresentation,
                                class CTypeDescription *evtDescription);

        void prv_traslateRepresentationToDescription(unsigned long numRecursive, class CTypeDescription *description);

        void prv_appendRefNotRepeated(unsigned long numRecursive, class CArrayRef<CAgent> *agentsNotRepeated);

        static class CArrayRef<CAgent> *prv_allReferenceNotRepeated(class CArrayRef<CAgent> *agents);

        struct SPrvDataPrivateAgent *m_dataPrivate;
};

#endif /* CAGENT_HPP_ */

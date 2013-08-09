/* -----------------------------------------------------------------------------
*  CEventSimulationRobot.inl
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CEventSystem.hpp"

class CEventLaser: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventLaser(double distance, double angle);
        virtual ~CEventLaser();

        void saveInMemory(class CMemoryRobot *memory);

    private:

        struct prv_dataPrivateEventLaser_t *m_dataPrivate;
};

class CEventMarkObjective: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventMarkObjective(const char *mark);
        virtual ~CEventMarkObjective();

        const char *markObjective(void) const;

    private:

        struct prv_dataPrivateEventVideo_t *m_dataPrivate;
};

class CEventVideo: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventVideo(const char *mark);
        virtual ~CEventVideo();

        void saveInMemory(class CMemoryRobot *memory);

        bool isObjetive(const char *markObjetive) const;

    private:

        struct prv_dataPrivateEventVideo_t *m_dataPrivate;
};

class CEventDecide: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventDecide(void) : CEventSystem(ID_EVENT) {;}
};


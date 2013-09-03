/* -----------------------------------------------------------------------------
 *  CEventKey.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventSystem.hpp"

class CEventKey: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventKey(const struct EvtKey_t *evtKey);
        virtual ~CEventKey();

        unsigned int key(void) const;

    private:

        struct SPrvDataPrivateEventKey *m_dataPrivate;
};

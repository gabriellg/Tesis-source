/* -----------------------------------------------------------------------------
 *  CGeneratorAcceleration.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentGeneratorEvent.hpp"
#include "CGeneratorKey.hpp"

class CGeneratorAccelerationKey: public CGeneratorKey
{
public:

    CGeneratorAccelerationKey();
    virtual ~CGeneratorAccelerationKey();

    void appendCarKey(const char *id, unsigned int keyAccelerator, unsigned int keyDesacelerator);

private:

    virtual void generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events);

    struct SPrvGeneratorAccelerationKey *m_dataPrivate;
};

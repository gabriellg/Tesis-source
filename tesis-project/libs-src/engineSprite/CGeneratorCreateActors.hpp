//CGeneratorCreateActors.hpp:

#include "CAgentGeneratorEvent.hpp"

class CGeneratorCreateActors: public CAgentGeneratorEvent
{
	public:

		CGeneratorCreateActors(
					class CWorldGeometry2DPixels *worldGeometry2DPixels,
					class CCollectionPositionCreation *collectionPositionCreation,
					class ICreatorActors *createActors);

		virtual ~CGeneratorCreateActors();

	private:

        virtual void beginEvolution(class CCollectionEventsSystem *events);

		struct prv_dataPrivateGeneratorCreateActors_t *m_dataPrivate;
};

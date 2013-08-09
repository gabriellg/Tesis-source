//CGeneratorCollisionSprite.hpp: Generador de colisiones sprite.

#include "CAgentGeneratorEvent.hpp"

class CGeneratorCollisionSprite: public CAgentGeneratorEvent
{
	public:

		CGeneratorCollisionSprite(class CWorldGeometry2DPixels *worldGeometry2D);
		virtual ~CGeneratorCollisionSprite();

		void appendSymbolFigureToConsider(const char *symbol, class CImg **img);

	private:

        virtual void beginEvolution(class CCollectionEventsSystem *events);
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        struct prv_dataPrivateGeneratorCollisionSprite_t *m_dataPrivate;
};

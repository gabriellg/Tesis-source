//CWorldGeometry2DPixels.hpp: Define geometry 2d world to elements using pixels.

#include "IWorld.hpp"

class CWorldGeometry2DPixels : public IWorld
{
	public:

		CWorldGeometry2DPixels();
		virtual ~CWorldGeometry2DPixels();

		virtual class CAgent *move(const char *idElement, class CAgent **agentMoved) const;

		void createBlockSprite(const char *id, long x, long y);

		void setDesire(const char *id, long dx, long dy);

		void desactiveAllBlocks(void);

		void deleteBlocksDesactived(void);

		void setSizeFigure(const char *idFigure, unsigned long width, unsigned long heigth);

		bool hasCollisionWithBackground(const char *id);

		bool hasCollisionWithOtherBlock(const char *id, char **idBlockCollisioned_opt);

		void calculateObstableMap(
						const class CImg *map,
						const class IEvaluateMap *evaluateMap,
						unsigned long widthGrid, unsigned long heightGrid);

	private:

		struct prv_dataPrivateGeometry2DPixels_t *m_dataPrivate;
};

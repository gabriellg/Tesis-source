//CWorldSprite.cpp:

#include "CWorldSprite.hpp"

#include "CWorldGeometry2DPixels.hpp"
#include "CCollectionPositionCreation.hpp"
#include "IEvaluateMap.hpp"
#include "ICreatorActors.hpp"

#include "CScene.hpp"
#include "CGestorDisplays.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CGraphicsSprite.hpp"
#include "CImg.hpp"

enum prv_typeMap_t
{
	PRV_MAP_PLANE,
	PRV_MAP_PERSPECTIVA
};

//-----------------------------------------------------------------------

static class CGraphicsSprite *prv_createGraphics(
							enum prv_typeMap_t typeMap,
							unsigned long widthImageMap, unsigned long heightImageMap,
							unsigned long widthGrid, unsigned long heightGrid)
{
	unsigned long widthMap, heightMap;

	switch(typeMap)
	{
		case PRV_MAP_PLANE:
		{
			widthMap = widthGrid * widthImageMap;
			heightMap = heightGrid * heightImageMap;
			break;
		}
		case PRV_MAP_PERSPECTIVA:
		{
			widthMap = heightGrid * widthImageMap + heightGrid * heightImageMap;
			heightMap = widthMap;
			break;
		}
	}

	return new CGraphicsSprite(widthMap, heightMap, 0, 0, 0);
}

//-----------------------------------------------------------------------

static void prv_calculatePosition(
							enum prv_typeMap_t typeMap,
							unsigned long x, unsigned long y,
							unsigned long widthGrid, unsigned long heightGrid,
							unsigned long heightMap,
							unsigned long *xPos, unsigned long *yPos)
{
	unsigned long xPos_loc, yPos_loc;

	assert_no_null(xPos);
	assert_no_null(yPos);

	switch(typeMap)
	{
		case PRV_MAP_PLANE:
		{
			xPos_loc = x * widthGrid;
			yPos_loc = y * heightGrid;
			break;
		}
		case PRV_MAP_PERSPECTIVA:
		{
			xPos_loc = (heightMap - 1 - y + x) * heightGrid;
			yPos_loc = (x + y) * heightGrid / 2;
			break;
		}
	}

	*xPos = xPos_loc;
	*yPos = yPos_loc;
}

//-----------------------------------------------------------------------

static class CImg *prv_createImageMap(
					enum prv_typeMap_t typeMap,
					const class CImg *map,
					const class IEvaluateMap *evaluateMap,
					unsigned long widthGrid, unsigned long heightGrid)
{
	class CImg *imageMap;
	unsigned long width, height;
	class CGraphicsSprite *graphicsSprite;

	assert_no_null(evaluateMap);

	map->size(&width, &height);

	graphicsSprite = prv_createGraphics(typeMap, width, height, widthGrid, heightGrid);

	for (unsigned long y = 0; y < height; y++)
	{
		for (unsigned long x = 0; x < width; x++)
		{
			unsigned char r, g, b;
			class CImg *imageGrid;
			unsigned long widthImageGrid, heightImageGrid;
			unsigned long xPos, yPos;

			prv_calculatePosition(typeMap, x, y, widthGrid, heightGrid, height, &xPos, &yPos);

			map->getPixelRGB(x, y, &r, &g, &b);
			imageGrid = evaluateMap->evaluteImage(r, g, b);

			imageGrid->size(&widthImageGrid, &heightImageGrid);

			assert(widthImageGrid == widthGrid);
			assert(heightImageGrid == heightGrid);

			graphicsSprite->pushTransformation();
			graphicsSprite->translate(xPos, yPos);

			graphicsSprite->drawImage(imageGrid);

			graphicsSprite->popTransformation();

			delete imageGrid;
		}
	}

	imageMap = graphicsSprite->getImage();
	delete graphicsSprite;

	return imageMap;
}

//-----------------------------------------------------------------------

static class CCollectionPositionCreation *prv_calculatePositionCreation(
                        enum prv_typeMap_t typeMap,
                        const class CImg *map,
                        const class IEvaluateMap *evaluateMap,
                        unsigned long widthGrid, unsigned long heightGrid)
{
    unsigned long width, height;
    class CCollectionPositionCreation *collectionPositionCreation;

    assert_no_null(evaluateMap);

    collectionPositionCreation = new CCollectionPositionCreation();

    map->size(&width, &height);

    for (unsigned long y = 0; y < height; y++)
    {
        for (unsigned long x = 0; x < width; x++)
        {
            unsigned char r, g, b;
            unsigned long xPos, yPos;
            char *idPosition;

            map->getPixelRGB(x, y, &r, &g, &b);
            prv_calculatePosition(typeMap, x, y, widthGrid, heightGrid, height, &xPos, &yPos);

            idPosition = NULL;
            if (evaluateMap->isPositionCreateAgent(r, g, b, &idPosition) == true)
                collectionPositionCreation->appendPosition(&idPosition, xPos, yPos);
        }
    }

    return collectionPositionCreation;
}

//-----------------------------------------------------------------------

class CImg *CWorldSprite::imageScenarioPlane(
                        const class CImg *map,
                        const class IEvaluateMap *evaluateMap,
                        unsigned long widthGrid,
                        unsigned long heightGrid)
{
    return prv_createImageMap(PRV_MAP_PLANE, map, evaluateMap, widthGrid, heightGrid);
}

//-----------------------------------------------------------------------

class CImg *CWorldSprite::imageScenarioPerspective(
                        const class CImg *map,
                        const class IEvaluateMap *evaluateMap,
                        unsigned long heightGrid)
{
    unsigned long widthGrid;

    widthGrid = 2 * heightGrid;
    return prv_createImageMap(PRV_MAP_PERSPECTIVA, map, evaluateMap, widthGrid, heightGrid);
}

//-----------------------------------------------------------------------

class CWorldGeometry2DPixels *CWorldSprite::createWorld2d(
                    const class CImg *map,
                    const class IEvaluateMap *evaluateMap,
                    unsigned long widthGrid,
                    unsigned long heightGrid)
{
    class CWorldGeometry2DPixels *worldGeometry2D;

    worldGeometry2D = new CWorldGeometry2DPixels();
    worldGeometry2D->calculateObstableMap(map, evaluateMap, widthGrid, heightGrid);

    return worldGeometry2D;
}

//-----------------------------------------------------------------------

class CCollectionPositionCreation *CWorldSprite::collectionPositionCreationPlane(
                        const class CImg *map,
                        const class IEvaluateMap *evaluateMap,
                        unsigned long widthGrid,
                        unsigned long heightGrid)
{
    return prv_calculatePositionCreation(PRV_MAP_PLANE, map, evaluateMap, widthGrid, heightGrid);
}

//-----------------------------------------------------------------------

class CCollectionPositionCreation *CWorldSprite::collectionPositionCreationPerspective(
                        const class CImg *map,
                        const class IEvaluateMap *evaluateMap,
                        unsigned long heightGrid)
{
    unsigned long widthGrid;

    widthGrid = 2 * heightGrid;
    return prv_calculatePositionCreation(PRV_MAP_PERSPECTIVA, map, evaluateMap, widthGrid, heightGrid);
}

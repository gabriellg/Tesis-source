//CWorldGeometry2DPixels.cpp:

#include "CWorldGeometry2DPixels.hpp"

#include "CTransform2D.hpp"
#include "IEvaluateMap.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "array.h"

#include "CTransform.hpp"
#include "CImg.hpp"

enum prv_typeCollision_t
{
	PRV_NO_COLLISION,
	PRV_COLLISION_WITH_BACKGROUND,
	PRV_COLLISION_WITH_ELEMENT
};

struct SPrvBlockSprite
{
	char *id;

	bool isActived;
	enum prv_typeCollision_t typeCollision;
	char *idElementCollisioned;

	long x, y;
	unsigned long sizeX, sizeY;
};

struct prv_dataPrivateGeometry2DPixels_t
{
	ArrTipo(SPrvBlockSprite) *sprites;

	class CImg *imageCollision;
	unsigned char r_obstacle, g_obstacle, b_obstacle;
};

//-----------------------------------------------------------------------

static struct SPrvBlockSprite *prv_createSprite(
							const char *id,
							bool isActived,
							enum prv_typeCollision_t typeCollision,
							const char *idElementCollisioned,
							long x, long y,
							unsigned long sizeX, unsigned long sizeY)
{
	struct SPrvBlockSprite *blockSprite;

	blockSprite = MALLOC(struct SPrvBlockSprite);

	blockSprite->id = strbas_copyString(id);

	blockSprite->isActived = isActived;
	blockSprite->typeCollision = typeCollision;
	blockSprite->idElementCollisioned = strbas_copyString(idElementCollisioned);

	blockSprite->x = x;
	blockSprite->y = y;

	blockSprite->sizeX = sizeX;
	blockSprite->sizeY = sizeY;

	return blockSprite;
}

//-----------------------------------------------------------------------

static void prv_destroySprite(struct SPrvBlockSprite **blockSprite)
{
	assert_no_null(blockSprite);
	assert_no_null(*blockSprite);

	strbas_destroy(&(*blockSprite)->id);
	strbas_destroy(&(*blockSprite)->idElementCollisioned);

	FREE_T(blockSprite, struct SPrvBlockSprite);
}

//-----------------------------------------------------------------------

static bool prv_isId(const struct SPrvBlockSprite *blockSprite, const char *id)
{
	assert_no_null(blockSprite);

	return strbas_equalsStrings(blockSprite->id, id);
}

//-----------------------------------------------------------------------

static void prv_positionSprite(
							const struct SPrvBlockSprite *blockSprite,
							long *xMin, long *yMin, long *xMax, long *yMax)
{
	assert_no_null(blockSprite);
	assert_no_null(xMin);
	assert_no_null(yMin);
	assert_no_null(xMax);
	assert_no_null(yMax);

	*xMin = blockSprite->x;
	*yMin = blockSprite->y;
	*xMax = blockSprite->x + (long)blockSprite->sizeX;
	*yMax = blockSprite->x + (long)blockSprite->sizeY;
}

//-----------------------------------------------------------------------

static bool prv_isInside(long xMin, long yMin, long xMax, long yMax, long x, long y)
{
	if (xMin <= x && x <= xMax && yMin <= y && y <= yMax)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

static bool prv_hasCollisionTwoBlocks(
						const struct SPrvBlockSprite *blockSprite1,
						const struct SPrvBlockSprite *blockSprite2,
						long xPosDesired2, long yPosDesired2)
{
	long x1Sprite1, y1Sprite1, x2Sprite1, y2Sprite1;

	assert_no_null(blockSprite1);
	assert_no_null(blockSprite2);

	prv_positionSprite(blockSprite1, &x1Sprite1, &y1Sprite1, &x2Sprite1, &y2Sprite1);

	if (prv_isInside(
					x1Sprite1, y1Sprite1,
					x2Sprite1, y2Sprite1,
					xPosDesired2, yPosDesired2) == true
			|| prv_isInside(
					x1Sprite1, y1Sprite1,
					x2Sprite1, y2Sprite1,
					xPosDesired2, yPosDesired2 + blockSprite2->sizeY) == true
			|| prv_isInside(
					x1Sprite1, y1Sprite1,
					x2Sprite1, y2Sprite1,
					xPosDesired2 + blockSprite2->sizeX,
					yPosDesired2 + blockSprite2->sizeY) == true
			|| prv_isInside(
					x1Sprite1, y1Sprite1,
					x2Sprite1, y2Sprite1,
					xPosDesired2 + blockSprite2->sizeX,
					yPosDesired2) == true)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGeometry2DPixels_t *prv_create(
							class CImg **imageCollision,
							unsigned char r_obstacle,
							unsigned char g_obstacle,
							unsigned char b_obstacle,
							ArrTipo(SPrvBlockSprite) **sprites)
{
	struct prv_dataPrivateGeometry2DPixels_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateGeometry2DPixels_t);

	dataPrivate->imageCollision = ASSIGN_PP(imageCollision, class CImg);

	dataPrivate->r_obstacle = r_obstacle;
	dataPrivate->g_obstacle = g_obstacle;
	dataPrivate->b_obstacle = b_obstacle;

	dataPrivate->sprites = ASSIGN_PP_NO_NULL(sprites, ArrTipo(SPrvBlockSprite));

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGeometry2DPixels_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	if ((*dataPrivate)->imageCollision != NULL)
	    delete (*dataPrivate)->imageCollision;

	array_Destruye(SPrvBlockSprite, &(*dataPrivate)->sprites, prv_destroySprite);

	FREE_T(dataPrivate, struct prv_dataPrivateGeometry2DPixels_t);
}

//-----------------------------------------------------------------------

CWorldGeometry2DPixels::CWorldGeometry2DPixels()
{
	ArrTipo(SPrvBlockSprite) *sprites;
	unsigned char r_obstacle, g_obstacle, b_obstacle;
	class CImg *imageCollision;

	imageCollision = NULL;
	r_obstacle = g_obstacle = b_obstacle = 255;
	sprites = array_CreaPuntero(0, SPrvBlockSprite);

	m_dataPrivate = prv_create(&imageCollision, r_obstacle, g_obstacle, b_obstacle, &sprites);
}

//-----------------------------------------------------------------------

CWorldGeometry2DPixels::~CWorldGeometry2DPixels()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::createBlockSprite(const char *id, long x, long y)
{
	struct SPrvBlockSprite *blockSprite;
	bool exist;
	unsigned long sizeX, sizeY;

	assert_no_null(m_dataPrivate);
	exist = array_BuscarUnica(SPrvBlockSprite, m_dataPrivate->sprites, char, id, prv_isId, NULL);
	assert(exist == false);

	sizeX = 0;
	sizeY = 0;

	blockSprite = prv_createSprite(
						id,
						true, //bool isActived,
						PRV_NO_COLLISION,
						"", //idElementCollisioned,
						x, y, sizeX, sizeY);

	array_Anyadir(SPrvBlockSprite, m_dataPrivate->sprites, blockSprite);
}

//-----------------------------------------------------------------------

static bool prv_hasCollisionWithBackground(
					const struct SPrvBlockSprite *blockSprite,
					long xPosDesired, long yPosDesired,
					const class CImg *imageBackground,
					unsigned char r_obstacle,
					unsigned char g_obstacle,
					unsigned char b_obstacle)
{
	unsigned long sizeXImage, sizeYImage;

	assert_no_null(blockSprite);

	imageBackground->size(&sizeXImage, &sizeYImage);

	for (unsigned long x = 0; x < blockSprite->sizeX; x++)
	{
		for (unsigned long y = 0; y < blockSprite->sizeY; y++)
		{
			long xPixel, yPixel;

			xPixel = xPosDesired + (long)x;
			yPixel = yPosDesired + (long)y;

			if (xPixel >= 0 && xPixel < (long)sizeXImage && yPixel >= 0 && yPixel < (long)sizeYImage)
			{
				unsigned char red, green, blue;

				imageBackground->getPixelRGB((unsigned long)xPixel, (unsigned long)yPixel, &red, &green, &blue);

				if (red == r_obstacle && green == g_obstacle && blue == b_obstacle)
					return true;
			}
			else
				return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

static struct SPrvBlockSprite *i_getBlock(
							const ArrTipo(SPrvBlockSprite) *sprites,
							const char *symbol)
{
	unsigned long indPos;
	bool exist;

	exist = array_BuscarUnica(SPrvBlockSprite, sprites, char, symbol, prv_isId, &indPos);
	assert(exist == true);
	return array_Get(SPrvBlockSprite, sprites, indPos);
}

//-----------------------------------------------------------------------

static bool prv_hasCollisionBetweenBlocks(
				const struct SPrvBlockSprite *blockSpriteTest,
				long xPosDesired, long yPosDesired,
				const ArrTipo(SPrvBlockSprite) *sprites,
				char **idBlockCollisioned_opt)
{
	unsigned long i, num;

	num = array_numElementos(SPrvBlockSprite, sprites);

	for (i = 0; i < num; i++)
	{
		const struct SPrvBlockSprite *spriteBlock;

		spriteBlock = array_Get(SPrvBlockSprite, sprites, i);

		if (spriteBlock != blockSpriteTest
				&& prv_hasCollisionTwoBlocks(
						spriteBlock, blockSpriteTest,
						xPosDesired, yPosDesired) == true)

		{
			if (idBlockCollisioned_opt != NULL)
				*idBlockCollisioned_opt = strbas_copyString(spriteBlock->id);

			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::setDesire(const char *id, long dx, long dy)
{
	struct SPrvBlockSprite *blockSprite;
	long xPosDesired, yPosDesired, dxPixel, dyPixel;

	assert_no_null(m_dataPrivate);

	blockSprite = i_getBlock(m_dataPrivate->sprites, id);
	blockSprite->isActived = true;

	dxPixel = blockSprite->sizeX / 4;
	dyPixel = blockSprite->sizeY / 4;

	assert(dxPixel > 0);
	assert(dyPixel > 0);

	xPosDesired = blockSprite->x + dx * dxPixel;
	yPosDesired = blockSprite->y + dy * dyPixel;

	strbas_destroy(&blockSprite->idElementCollisioned);

	if (prv_hasCollisionWithBackground(
						blockSprite, xPosDesired, yPosDesired,
						m_dataPrivate->imageCollision,
						m_dataPrivate->r_obstacle,
						m_dataPrivate->g_obstacle,
						m_dataPrivate->b_obstacle) == true)
	{
		blockSprite->typeCollision = PRV_COLLISION_WITH_BACKGROUND;
		blockSprite->idElementCollisioned = strbas_copyString("COLLISION_BLACKGROUND");
	}
	else if (prv_hasCollisionBetweenBlocks(
						blockSprite, xPosDesired, yPosDesired,
						m_dataPrivate->sprites,
						&blockSprite->idElementCollisioned) == true)
	{
		blockSprite->typeCollision = PRV_COLLISION_WITH_BACKGROUND;
	}
	else
	{
		blockSprite->typeCollision = PRV_NO_COLLISION;
		blockSprite->idElementCollisioned = strbas_copyString("NO_COLLISION");

		blockSprite->x = xPosDesired;
		blockSprite->y = yPosDesired;
	}
}

//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::desactiveAllBlocks(void)
{
	unsigned long i, num;

	assert_no_null(m_dataPrivate);
	num = array_numElementos(SPrvBlockSprite, m_dataPrivate->sprites);

	for (i = 0; i < num; i++)
	{
		struct SPrvBlockSprite *spriteBlock;

		spriteBlock = array_Get(SPrvBlockSprite, m_dataPrivate->sprites, i);
		assert_no_null(spriteBlock);

		spriteBlock->isActived = false;
		spriteBlock->typeCollision = PRV_NO_COLLISION;
		strbas_destroy(&spriteBlock->idElementCollisioned);
		spriteBlock->idElementCollisioned = strbas_copyString("NO_COLLISION");
	}
}

//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::deleteBlocksDesactived(void)
{
	unsigned long i, num, numDeleted;

	assert_no_null(m_dataPrivate);

	num = array_numElementos(SPrvBlockSprite, m_dataPrivate->sprites);
	numDeleted = 0;

	for (i = 0; i < num; i++)
	{
		const struct SPrvBlockSprite *sprite;
		unsigned long ind;

		ind = i - numDeleted;

		sprite = array_Get(SPrvBlockSprite, m_dataPrivate->sprites, ind);
		assert_no_null(sprite);

		if (sprite->isActived == false)
		{
			array_Borrar(SPrvBlockSprite, m_dataPrivate->sprites, ind, prv_destroySprite);
			numDeleted++;
		}
	}
}

//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::setSizeFigure(const char *idFigure, unsigned long width, unsigned long heigth)
{
	struct SPrvBlockSprite *blockSprite;

	assert_no_null(m_dataPrivate);

	blockSprite = i_getBlock(m_dataPrivate->sprites, idFigure);
	blockSprite->isActived = true;
	blockSprite->sizeX = width;
	blockSprite->sizeY = heigth;
}

//-----------------------------------------------------------------------

bool CWorldGeometry2DPixels::hasCollisionWithBackground(const char *id)
{
	bool hasCollision;
	struct SPrvBlockSprite *blockSprite;

	assert_no_null(m_dataPrivate);

	blockSprite = i_getBlock(m_dataPrivate->sprites, id);
	blockSprite->isActived = true;

	switch(blockSprite->typeCollision)
	{
		case PRV_COLLISION_WITH_BACKGROUND:

			hasCollision = true;
			break;

		case PRV_NO_COLLISION:
		case PRV_COLLISION_WITH_ELEMENT:

			hasCollision = false;
			break;

		default_error();
	}

	return hasCollision;
}

//-----------------------------------------------------------------------

bool CWorldGeometry2DPixels::hasCollisionWithOtherBlock(const char *id, char **idBlockCollisioned_opt)
{
	bool hasCollision;
	struct SPrvBlockSprite *blockSprite;

	assert_no_null(m_dataPrivate);

	blockSprite = i_getBlock(m_dataPrivate->sprites, id);
	blockSprite->isActived = true;

	switch(blockSprite->typeCollision)
	{
		case PRV_COLLISION_WITH_ELEMENT:

			hasCollision = true;
			if (idBlockCollisioned_opt != NULL)
				*idBlockCollisioned_opt = strbas_copyString(blockSprite->idElementCollisioned);
			break;

		case PRV_NO_COLLISION:
		case PRV_COLLISION_WITH_BACKGROUND:

			hasCollision = false;
			break;

		default_error();
	}

	return hasCollision;
}

//-----------------------------------------------------------------------

class IObjectDraw *CWorldGeometry2DPixels::move(const char *idElement, class IObjectDraw **agentMoved) const
{
	struct SPrvBlockSprite *blockSprite;

	assert_no_null(m_dataPrivate);

	blockSprite = i_getBlock(m_dataPrivate->sprites, idElement);
	blockSprite->isActived = true;

	return CTransform2D::createTraslate2D(agentMoved, blockSprite->x, blockSprite->y);
}

//-----------------------------------------------------------------------

static void prv_fillTypeObstable(
					unsigned long f, unsigned long c,
					unsigned long widthGrid, unsigned long heightGrid,
					enum IEvaluateMap::ETypeObstable obstacleMap,
					unsigned char r_obstacle, unsigned char g_obstacle, unsigned char b_obstacle,
					class CImg *image)
{
	unsigned long xInicio, yInicio;

	assert_no_null(image);

	xInicio = c * widthGrid;
	yInicio = f * heightGrid;

	for (unsigned long i = 0; i < widthGrid; i++)
	{
		for (unsigned long j = 0; j < heightGrid; j++)
		{
			unsigned long xPos, yPos;

			xPos = xInicio + i;
			yPos = yInicio + j;

			switch(obstacleMap)
			{
				case IEvaluateMap::OBSTACLE:

					image->setValueRGB(xPos, yPos, r_obstacle, g_obstacle, b_obstacle);
					break;

				case IEvaluateMap::FREE:

					break;

				default_error();
			}
		}
	}
}

//-----------------------------------------------------------------------

void CWorldGeometry2DPixels::calculateObstableMap(
					const class CImg *map,
					const class IEvaluateMap *evaluateMap,
					unsigned long widthGrid, unsigned long heightGrid)
{
	unsigned long width, height, widthMap, heightMap;
	class CImg *imageCollision;

	assert_no_null(m_dataPrivate);
	assert_no_null(evaluateMap);

	array_Destruye(SPrvBlockSprite, &m_dataPrivate->sprites, prv_destroySprite);
	m_dataPrivate->sprites = array_CreaPuntero(0, SPrvBlockSprite);

	if (m_dataPrivate->imageCollision != NULL)
	{
	    delete m_dataPrivate->imageCollision;
	    m_dataPrivate->imageCollision = NULL;
	}

	map->size(&width, &height);

	widthMap = widthGrid * width;
	heightMap = heightGrid * height;

	imageCollision = new CImg(widthMap, heightMap);

	for (unsigned long f = 0; f < height; f++)
	{
		for (unsigned long c = 0; c < width; c++)
		{
			unsigned char r, g, b;
			enum IEvaluateMap::ETypeObstable typeObstable;

			map->getPixelRGB(c, f, &r, &g, &b);
			typeObstable = evaluateMap->evaluteObstable(r, g, b);

			prv_fillTypeObstable(
					f, c, widthGrid, heightGrid,
					typeObstable,
					m_dataPrivate->r_obstacle,
					m_dataPrivate->g_obstacle,
					m_dataPrivate->b_obstacle,
					imageCollision);
		}
	}

	m_dataPrivate->imageCollision = imageCollision;
}

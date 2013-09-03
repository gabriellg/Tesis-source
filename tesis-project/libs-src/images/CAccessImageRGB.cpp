//AccessImageRGB.cpp:

#include "CAccessImageRGB.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateAccessImageRGB_t
{
	unsigned char *imageData;
	unsigned long widthStep;
	unsigned long width, height;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateAccessImageRGB_t *prv_create(
				unsigned char *imageData, unsigned long widthStep,
				unsigned long width, unsigned long height)
{
	struct prv_dataPrivateAccessImageRGB_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateAccessImageRGB_t);

	dataPrivate->imageData = imageData;
	dataPrivate->widthStep = widthStep;
	dataPrivate->width = width;
	dataPrivate->height = height;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateAccessImageRGB_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateAccessImageRGB_t);
}

//-----------------------------------------------------------------------

static unsigned char *prv_getPositionRGBConst(
							const struct prv_dataPrivateAccessImageRGB_t *dataPrivate,
							unsigned long f, unsigned long c)
{
	const unsigned char *pos;

	pos = dataPrivate->imageData + f * dataPrivate->widthStep;
	return (unsigned char *)&pos[c * 3];
}

//-----------------------------------------------------------------------

CAccessImageRGB::CAccessImageRGB(
					unsigned char *imageData, unsigned long widthStep,
					unsigned long width, unsigned long height)
{
	m_dataPrivate = prv_create(imageData, widthStep, width, height);
}

//-----------------------------------------------------------------------

CAccessImageRGB::~CAccessImageRGB()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

unsigned long CAccessImageRGB::getWidth(void) const
{
	assert_no_null(m_dataPrivate);
	return m_dataPrivate->width;
}

//-----------------------------------------------------------------------

unsigned long CAccessImageRGB::getHeight(void) const
{
	assert_no_null(m_dataPrivate);
	return m_dataPrivate->height;
}

//-----------------------------------------------------------------------

void CAccessImageRGB::getRGB(
					unsigned long f, unsigned long c,
					unsigned char *r, unsigned char *g, unsigned char *b)  const
{
	unsigned char *ptr;

	assert_no_null(m_dataPrivate);
	assert_no_null(r);
	assert_no_null(g);
	assert_no_null(b);

	ptr = prv_getPositionRGBConst(m_dataPrivate, f, c);

	*r = ptr[2];
	*g = ptr[1];
	*b = ptr[0];
}

//-----------------------------------------------------------------------

void CAccessImageRGB::setRGB(
					unsigned long f, unsigned long c,
					unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char *ptr;

	assert_no_null(m_dataPrivate);

	ptr = prv_getPositionRGBConst(m_dataPrivate, f, c);

	ptr[2] = r;
	ptr[1] = g;
	ptr[0] = b;
}


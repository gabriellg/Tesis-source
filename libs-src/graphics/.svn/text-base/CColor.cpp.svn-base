//CColor.cpp

#include "CColor.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"

struct prv_dataPrivateCColor_t
{
	unsigned char r, g, b;
};

static double prv_EPSILON = 1e-6;

//---------------------------------------------------------

static struct prv_dataPrivateCColor_t *prv_create(unsigned char r, unsigned char g, unsigned char b)
{
	struct prv_dataPrivateCColor_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateCColor_t);
	
	dataPrivate->r = r;
	dataPrivate->g = g;
	dataPrivate->b = b;
	
	return dataPrivate;
}

//---------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateCColor_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateCColor_t);
}

//---------------------------------------------------------

static unsigned char prv_rgb_unsigned_char(double channel)
{
	assert(channel > 0. - prv_EPSILON);
	assert(channel < 1. + prv_EPSILON);

	return (unsigned char)CMath::floor(channel * 255.);
}

//---------------------------------------------------------

CColor::CColor(double r, double g, double b)
{
	unsigned char red, green, blue;

	red = prv_rgb_unsigned_char(r);
	green = prv_rgb_unsigned_char(g);
	blue = prv_rgb_unsigned_char(b);

	m_dataPrivate = prv_create(red, green, blue);
}

//---------------------------------------------------------

CColor::CColor(enum Color_t color)
{
	unsigned char red, green, blue;

	switch(color)
	{
		case Black:

			red = 0; green = 0; blue = 0;
			break;

		case White:

			red = 255; green = 255; blue = 255;
			break;

		default_error();
	}

	m_dataPrivate = prv_create(red, green, blue);
}


//---------------------------------------------------------

CColor::CColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_dataPrivate = prv_create(r, g, b);
}

//---------------------------------------------------------

CColor::~CColor()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------

void CColor::getColorDouble(double *r, double *g, double *b) const
{
	assert_no_null(m_dataPrivate);
	assert_no_null(r);
	assert_no_null(g);
	assert_no_null(b);
	
	*r = m_dataPrivate->r / 255.;
	*g = m_dataPrivate->g / 255.;
	*b = m_dataPrivate->b / 255.;
}

//---------------------------------------------------------

void CColor::getColor(unsigned char *r, unsigned char *g, unsigned char *b) const
{
	assert_no_null(m_dataPrivate);
	assert_no_null(r);
	assert_no_null(g);
	assert_no_null(b);

	*r = m_dataPrivate->r;
	*g = m_dataPrivate->g;
	*b = m_dataPrivate->b;
}

//---------------------------------------------------------

bool CColor::isWhite(void) const
{
	assert_no_null(m_dataPrivate);

	if (m_dataPrivate->r == 255 && m_dataPrivate->g == 255 && m_dataPrivate->b == 255)
		return true;
	else
		return false;
}

//---------------------------------------------------------

bool CColor::isBlack(void) const
{
	assert_no_null(m_dataPrivate);

	if (m_dataPrivate->r == 0 && m_dataPrivate->g == 0 && m_dataPrivate->b == 0)
		return true;
	else
		return false;
}

//---------------------------------------------------------

void CColor::transformColorRGB(
                unsigned char r, unsigned char g, unsigned char b,
                double *fr, double *fg, double *fb)
{
    assert_no_null(fr);
    assert_no_null(fg);
    assert_no_null(fb);

    *fr = r / 255.;
    *fg = g / 255.;
    *fb = b / 255.;
}

//---------------------------------------------------------

void CColor::transformToRGB(
                        double fred, double fgreen, double fblue,
                        unsigned char *red, unsigned char *green, unsigned char *blue)
{
    assert(fred >= 0. && fred <= 1.);
    assert(fgreen >= 0. && fgreen <= 1.);
    assert(fblue >= 0. && fblue <= 1.);

    assert_no_null(red);
    assert_no_null(green);
    assert_no_null(blue);

    *red = (unsigned char)(fred * 255);
    *green = (unsigned char)(fgreen * 255);
    *blue = (unsigned char)(fblue * 255);
}

//---------------------------------------------------------

bool CColor::isSameColorWithTolerance(
                double fr1, double fg1, double fb1,
                double fr2, double fg2, double fb2, double tolerance)
{
    if (CMath::isEqual(fr1, fr2, tolerance) == false)
        return false;

    if (CMath::isEqual(fg1, fg2, tolerance) == false)
        return false;

    if (CMath::isEqual(fb1, fb2, tolerance) == false)
        return false;

    return true;
}

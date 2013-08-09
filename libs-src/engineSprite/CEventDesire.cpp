//CEventDesired.cpp:

#include "CEventDesire.hpp"

#include "CWorldGeometry2DPixels.hpp"

#include "strbas.h"

const char *CEventDesire::ID_EVENT = "CEventDesire";

#include "asrtbas.h"
#include "memory.h"

const char *CEventDesire::DESIRE_GO_LEFT = "Go_Left";
const char *CEventDesire::DESIRE_GO_RIGHT = "Go_Right";
const char *CEventDesire::DESIRE_GO_FORWARD = "Go_forward";
const char *CEventDesire::DESIRE_GO_BACK = "Go_back";

struct prv_dataPrivateEventDesire_t
{
	class CWorldGeometry2DPixels *worldGeometry2D;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateEventDesire_t *prv_create(class CWorldGeometry2DPixels *worldGeometry2D)
{
	struct prv_dataPrivateEventDesire_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateEventDesire_t);

	dataPrivate->worldGeometry2D = worldGeometry2D;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateEventDesire_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateEventDesire_t);
}

//-----------------------------------------------------------------------

CEventDesire::CEventDesire(class CWorldGeometry2DPixels *worldGeometry2D) : CEventSystem(CEventDesire::ID_EVENT)
{
	m_dataPrivate = prv_create(worldGeometry2D);
}

//-----------------------------------------------------------------------

CEventDesire::~CEventDesire()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static void i_calculateDesire(const char *idDesire, long *dx, long *dy)
{
	long dx_loc, dy_loc;

	assert_no_null(dx);
	assert_no_null(dy);

	if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_LEFT) == true)
	{
		dx_loc = -1;
		dy_loc = 0;
	}
	else if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_RIGHT) == true)
	{
		dx_loc = 1;
		dy_loc = 0;
	}
	else if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_FORWARD) == true)
	{
		dx_loc = 0;
		dy_loc = -1;
	}
	else if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_BACK) == true)
	{
		dx_loc = 0;
		dy_loc = 1;
	}
	else
	{
		dx_loc = 0;
		dy_loc = 0;
	}

	*dx = dx_loc;
	*dy = dy_loc;
}

//-----------------------------------------------------------------------

void CEventDesire::setDesire(const char *idBlock, const char *idDesire)
{
	long dx, dy;

	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->worldGeometry2D);

	i_calculateDesire(idDesire, &dx, &dy);

	m_dataPrivate->worldGeometry2D->setDesire(idBlock, dx, dy);
}

//-----------------------------------------------------------------------

bool CEventDesire::isDesire(const char *idDesire)
{
	if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_BACK) == true)
		return true;

	if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_FORWARD) == true)
		return true;

	if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_LEFT) == true)
		return true;

	if (strbas_equalsStrings(idDesire, CEventDesire::DESIRE_GO_RIGHT) == true)
		return true;

	return false;
}

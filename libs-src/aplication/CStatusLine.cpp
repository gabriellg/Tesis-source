//CStatusLine.cpp

#include "CStatusLine.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "etiqueta.h"
#include "panel.h"

struct prv_privateDataStatusLine_t
{
	struct etiqueta_t *labelLeft;
	struct etiqueta_t *labelRight;
};

//---------------------------------------------------------

static struct prv_privateDataStatusLine_t *prv_create(
					struct etiqueta_t **labelLeft,
					struct etiqueta_t **labelRight)
{
	struct prv_privateDataStatusLine_t *dataPrivateStatusLine;
	
	dataPrivateStatusLine = MALLOC(struct prv_privateDataStatusLine_t);
	
	dataPrivateStatusLine->labelLeft = ASSIGN_PP(labelLeft, struct etiqueta_t);
	dataPrivateStatusLine->labelRight = ASSIGN_PP(labelRight, struct etiqueta_t);
	
	return dataPrivateStatusLine;
}

//---------------------------------------------------------

static void prv_destroy(struct prv_privateDataStatusLine_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_privateDataStatusLine_t);
}

//---------------------------------------------------------

CStatusLine::CStatusLine()
{
	struct etiqueta_t *labelLeft, *labelRight;
	
	labelLeft = NULL; 
	labelRight = NULL;
	
	m_dataPrivate = prv_create(&labelLeft, &labelRight);
}

//---------------------------------------------------------

CStatusLine::~CStatusLine()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------

void CStatusLine::setIcon(const struct Image_t *image)
{
	//TODO: Hacer setIcono;
}

//---------------------------------------------------------

void CStatusLine::setTextLeft(const char *text)
{
	assert_no_null(m_dataPrivate);
	etiqueta_set_texto(m_dataPrivate->labelLeft, text);
}

//---------------------------------------------------------

void CStatusLine::setTextRigth(const char *text)
{
	assert_no_null(m_dataPrivate);
	etiqueta_set_texto(m_dataPrivate->labelRight, text);
}

//---------------------------------------------------------

struct panel_t *CStatusLine::panel(void)
{
	struct panel_t *panel, *pnLabelLeft, *pnLabelRight;
	
	assert_no_null(m_dataPrivate);
	assert(m_dataPrivate->labelLeft == NULL);
	assert(m_dataPrivate->labelRight == NULL);
	
	pnLabelLeft = etiqueta_creaJustificacionIzquierda("", &m_dataPrivate->labelLeft);
	panel_set_expand(pnLabelLeft);
	panel_set_fill(pnLabelLeft);
	
	pnLabelRight = etiqueta_creaJustificacionDerecha("", &m_dataPrivate->labelRight);
	
	panel = panel_crea_con_disposicion_horizontal();
	
	panel_append_panel(panel, &pnLabelLeft);
	panel_append_panel(panel, &pnLabelRight);

	return panel;
}

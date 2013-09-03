// Testeamos los arrays 3d.

#include "CArrPoint2d.hpp"

#include "asrtbas.h"
#include "math.h"

#define NUM_PUNTOS 4

static const double i_PUNTOS_TEST_X[NUM_PUNTOS] = {1., 2., 3.};
static const double i_PUNTOS_TEST_Y[NUM_PUNTOS] = {1., 2., 3.};
static const double i_PUNTOS_TEST_Z[NUM_PUNTOS] = {1., 2., 3.};

//-----------------------------------------------------------------------

static void prv_compare(const class CArrPoint2d *points)
{
	unsigned long i, num;
	
	assert_no_null(points);

	num = points->size();
	assert(num == NUM_PUNTOS);
	
	for (i = 0; i < num; i++) 
	{
		double x, y;
		double distx, disty;
		
		points->get(i, &x, &y);
		
		distx = fabs(x - i_PUNTOS_TEST_X[i]);
		disty = fabs(y - i_PUNTOS_TEST_Y[i]);
		
		assert(distx < 0.00001);
		assert(disty < 0.00001);
	}
}

//-----------------------------------------------------------------------

static void prv_test_set(void)
{
	class CArrPoint2d *points;
	unsigned long i;
	
	points = new CArrPoint2d(NUM_PUNTOS);
	
	for (i = 0; i < NUM_PUNTOS; i++)
	    points->set(i, i_PUNTOS_TEST_X[i], i_PUNTOS_TEST_Y[i]);

	prv_compare(points);
	
	delete points;
}

//-----------------------------------------------------------------------

static void prv_test_append(void)
{
	class CArrPoint2d *points;
	unsigned long i;
	
	points = new CArrPoint2d();
	
	for (i = 0; i < NUM_PUNTOS; i++) 
		points->add(i_PUNTOS_TEST_X[i], i_PUNTOS_TEST_Y[i]);

	prv_compare(points);
	
	delete points;
}

//-----------------------------------------------------------------------

static void prv_test_insert(void)
{
	class CArrPoint2d *points;
	unsigned long i;
	
	points = new CArrPoint2d();
	
	for (i = 0; i < NUM_PUNTOS; i++) 
	{
		unsigned long posArray;
		
		posArray = NUM_PUNTOS - i - 1;
		points->insert(0, i_PUNTOS_TEST_X[posArray], i_PUNTOS_TEST_Y[posArray]);
	}

	prv_compare(points);
	
	delete points;
}

//-----------------------------------------------------------------------

static void prv_test_delete(void)
{
	class CArrPoint2d *points;
	unsigned long i;
	
	points = new CArrPoint2d();
	
	for (i = 0; i < NUM_PUNTOS; i++) 
	    points->add(i_PUNTOS_TEST_X[i], i_PUNTOS_TEST_Y[i]);
		
	points->insert(2, 0., 0.);
	points->erase(2);

	prv_compare(points);
	
	delete points;
}


//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
    prv_test_set();
    prv_test_append();
    prv_test_insert();
    prv_test_delete();
	
	return 0;
}

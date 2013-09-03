// Testeamos los arrays 3d.

#include "CArrPoint3d.hpp"

#include "asrtbas.h"
#include "math.h"

#define NUM_PUNTOS 4

static const double i_PUNTOS_TEST_X[NUM_PUNTOS] = {1., 2., 3., 4.};
static const double i_PUNTOS_TEST_Y[NUM_PUNTOS] = {1., 2., 3., 4.};
static const double i_PUNTOS_TEST_Z[NUM_PUNTOS] = {1., 2., 3., 4.};

//-----------------------------------------------------------------------

static void i_compara(const class CArrPoint3d *puntos)
{
	unsigned long i, num;
	
	assert_no_null(puntos);

	num = puntos->size();
	assert(num == NUM_PUNTOS);
	
	for (i = 0; i < num; i++) 
	{
		double x, y, z;
		double distx, disty, distz;
		
		puntos->get(i, &x, &y, &z);
		
		distx = fabs(x - i_PUNTOS_TEST_X[i]);
		disty = fabs(y - i_PUNTOS_TEST_Y[i]);
		distz = fabs(z - i_PUNTOS_TEST_Z[i]);
		
		assert(distx < 0.00001);
		assert(disty < 0.00001);
		assert(distz < 0.00001);
	}
}

//-----------------------------------------------------------------------

static void i_test_set(void)
{
	class CArrPoint3d *puntos;
	unsigned long i, num;
	
	puntos = new CArrPoint3d(NUM_PUNTOS);
	
	num = puntos->size();
	
	for (i = 0; i < num; i++) 
		puntos->set(i, i_PUNTOS_TEST_X[i], i_PUNTOS_TEST_Y[i], i_PUNTOS_TEST_Z[i]);

	i_compara(puntos);
	
	delete puntos;
}

//-----------------------------------------------------------------------

static void i_test_append(void)
{
	class CArrPoint3d *puntos;
	unsigned long i;
	
	puntos = new CArrPoint3d();
	
	for (i = 0; i < NUM_PUNTOS; i++) 
		puntos->add(i_PUNTOS_TEST_X[i], i_PUNTOS_TEST_Y[i], i_PUNTOS_TEST_Z[i]);

	i_compara(puntos);
	
	delete puntos;
}

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	i_test_set();
	i_test_append();
	
	return 0;
}

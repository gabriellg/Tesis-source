#include "CAplicationGTK.hpp"
#include "marcogtk.hpp"

//---------------------------------------------------------

CAplicationGTK::CAplicationGTK(const char *titleAplication) : CAplication(titleAplication)
{
	;
}

//---------------------------------------------------------

struct marcogrf_t *CAplicationGTK::getFrameworkAplication(void)
{
	return gtkmarco_crea_marco_gtk();
}

/* 
 * Testing CImg
 *
 * Created on Sep 3, 2010
 */

#include "stdlib.h"
#include "CImg.hpp"

#include "marcogtk.hpp"
#include "CColor.hpp"

static unsigned long prv_SIZEX_IMAGE_TEST = 512;
static unsigned long prv_SIZEY_IMAGE_TEST = 512;

//------------------------------------------------------------------------------
//
static void prv_testRead(void)
{
    class CImg *imageTest;

    imageTest = new CImg("./img/rojo128x128.png");

    imageTest->show("id_test");

    delete imageTest;
}

//------------------------------------------------------------------------------
//
static void prv_testImageColor(enum CColor::Color_t typeColor)
{
	class CImg *imageTest;
	class CColor *color;

	color = new CColor(typeColor);
	imageTest = new CImg(prv_SIZEX_IMAGE_TEST, prv_SIZEY_IMAGE_TEST, color);

	imageTest->show("id_test");

	delete imageTest;
	delete color;
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    class CImg *image;

	prv_testImageColor(CColor::Black);
    prv_testImageColor(CColor::White);
	prv_testRead();

    marcogtk_inicia_imagenes_gtk(argc, argv);

    image = marcogtk_read_image_from_file("./img/rojo128x128.png");
    image->show("test");


    return EXIT_SUCCESS;
}


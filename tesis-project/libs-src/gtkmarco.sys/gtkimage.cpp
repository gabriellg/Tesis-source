// Lee imagenes de gtk.

#include "gtkimage.inl"

#include "gtkinc.inl"

#include "asrtbas.h"
#include "memory.h"
#include "string.h"

#include "CImg.hpp"

//---------------------------------------------------------

static GdkPixbuf *i_read_image(const char *filename)
{
	GdkPixbuf *gtkpixbuf;
	GError *error;
	
	error = NULL;
	gtkpixbuf = gdk_pixbuf_new_from_file(filename, &error);
	
	if (error != NULL)
	    assert_message(error->message);
	
	return gtkpixbuf;
}

//---------------------------------------------------------

struct ImageGtk_t *gtkimage_createPixbufFromImage(const class CImg *image)
{
	unsigned char *dataImage;
	bool hasAlpha;
	unsigned long width, height, rowstride, nChannels;
	
	assert(sizeof(guchar) == sizeof(unsigned char));
	
	image->getDataRaw(&width, &height, &rowstride, &nChannels, &dataImage);

	if (nChannels == 3)
	    hasAlpha = false;
	else
	{
	    assert(nChannels == 4);
	    hasAlpha = true;
	}

	return (struct ImageGtk_t *)gdk_pixbuf_new_from_data (
					dataImage,
					GDK_COLORSPACE_RGB,
	                hasAlpha,
					8, //bits_per_sample,
	                width, height,
	                rowstride,
	                NULL, //destroy_fn,
	                NULL);

}

//---------------------------------------------------------

class CImg *gtkimage_read_image_from_file(const char *filename)
{
	class CImg *image;
	GdkPixbuf *pixbuf;
	unsigned long width, height, n_channels, rowstride;
	guchar *pixels;
	
	assert(sizeof(guchar) == sizeof(unsigned char));
	
	pixbuf = i_read_image(filename);

	assert(gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
	assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
	
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	n_channels = gdk_pixbuf_get_n_channels(pixbuf);
	pixels = gdk_pixbuf_get_pixels (pixbuf);
	
	image = new CImg(width, height, rowstride, n_channels, pixels);
	
	g_object_unref(pixbuf);
	
	return image;
}


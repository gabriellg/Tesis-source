//CAlgoritmsImage.cpp:

#include "CAlgoritmsImage.hpp"

#include "CImg.hpp"
#include "CColor.hpp"
#include "memory.h"
#include "asrtbas.h"

//-----------------------------------------------------------------------

bool prv_pixelIsWhite(const CImg *image, unsigned long row, unsigned long col)
{
	bool isWhite;
	class CColor *color;

	color = image->getValue(row, col);

	isWhite = color->isWhite();
	if (isWhite == false)
		assert(color->isBlack() == true);

	delete color;

	return isWhite;
}

//-----------------------------------------------------------------------

void prv_setBlack(class CImg *image, unsigned long row, unsigned long col)
{
	class CColor *color;

	color = new CColor(CColor::Black);
	image->setValue(row, col, color);
	delete color;
}

//-----------------------------------------------------------------------
// * Thinning algorithm from the article:
// * "Efficient Binary Image Thinning using Neighborhood Maps"
// * by Joseph M. Cychosz, 3ksnn64@ecn.purdue.edu
// * in "Graphics Gems IV", Academic Press, 1994
void CAlgoritmsImage::skeletonization(class CImg *image)
{
    // Direction masks:
    //              N     S     W     E
    const int masks[] = { 0200, 0002, 0040, 0010 };

    //	True if pixel neighbor map indicates the pixel is 8-simple and
    //	not an end point and thus can be deleted.  The neighborhood
    //	map is defined as an integer of bits abcdefghi with a non-zero
    //	bit representing a non-zero pixel.  The bit assignment for the
    //	neighborhood is:
    //				a b c
    //				d e f
    //				g h i
    const unsigned char todelete[512] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    unsigned long xsize, ysize; // Image resolution
    unsigned long x, y;	// Pixel location
    int	i;		// Pass index
    int	pc = 0;         // Pass count
    int	count = 1;	// Deleted pixel count
    int	p, q;		// Neighborhood maps of adjacent
                        // cells
    int *qb;		// Neighborhood maps of previous
                        // scanline
    int	m;		// Deletion direction mask

    assert_no_null(image);

    image->size(&xsize, &ysize);
    assert(xsize);

    qb = CALLOC(xsize, int);
    qb[xsize - 1] = 0;		// Used for lower-right pixel

    while ( count ) 		// Scan image while deletions
    {
        pc++;
        count = 0;

        for ( i = 0 ; i < 4 ; i++ )
        {
            m = masks[i];

            // Build initial previous scan buffer.

            p = prv_pixelIsWhite(image, 0, 0);
            for ( x = 0 ; x < xsize-1 ; x++ )
                qb[x] = p = ((p<<1)&0006) | prv_pixelIsWhite(image, 0, x+1);

            // Scan image for pixel deletion candidates.

            for ( y = 0 ; y < ysize-1 ; y++ )
            {
                q = qb[0];
                p = ((q<<3)&0110) | prv_pixelIsWhite(image, y+1, 0);

                for ( x = 0 ; x < xsize-1 ; x++ )
                {
                    q = qb[x];
                    p = ((p<<1)&0666) | ((q<<3)&0110) | prv_pixelIsWhite(image, y+1, x+1);
                    qb[x] = p;

                    if  ( ((p&m) == 0) && todelete[p] )
                    {
                        count++;
                        prv_setBlack(image, y, x);
                    }
                }

                // Process right edge pixel.

                p = (p<<1)&0666;
                if ( (p&m) == 0 && todelete[p] )
                {
                    count++;
                    prv_setBlack(image, y, xsize - 1);
                }
            }

            // Process bottom scan line.
            for ( x = 0 ; x < xsize ; x++ )
            {
                q = qb[x];
                p = ((p<<1)&0666) | ((q<<3)&0110);

                if	( (p&m) == 0 && todelete[p] )
                {
                    count++;
                    prv_setBlack(image, ysize-1, x);
                }
            }
        }
    }

    FREE_T(&qb, int);
}

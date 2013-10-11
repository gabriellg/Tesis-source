/* -----------------------------------------------------------------------------
 *  CTextureGL.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CTextureGL.inl"

#include "opengl.inl"

#include "asrtbas.h"
#include "CImg.hpp"

//-----------------------------------------------------------------------

static GLenum prv_getFormatPixel(unsigned long nChannels)
{
    GLenum formatPixel;

    if (nChannels == 4)
        formatPixel = GL_BGRA;
    else
    {
        assert(nChannels == 3);
        formatPixel = GL_BGR;
    }

    return formatPixel;
}

//-----------------------------------------------------------------------

unsigned long CTextureGL::createTexture(const class CImg *image)
{
    GLuint idTexture;
    unsigned long width, height, widthStep, nChannels;
    unsigned char *dataImage;
    GLenum formatPixel;

    image->getDataRaw(&width, &height, &widthStep, &nChannels, &dataImage);

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    formatPixel = prv_getFormatPixel(nChannels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, formatPixel, GL_UNSIGNED_BYTE, dataImage);

    return idTexture;
}

//-----------------------------------------------------------------------

void CTextureGL::textureOn(unsigned long idTexture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glColor3d(.5f, .5f, .5f);
}

//-----------------------------------------------------------------------

void CTextureGL::textureOff(void)
{
    glDisable(GL_TEXTURE_2D);
}

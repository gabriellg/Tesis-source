/* -----------------------------------------------------------------------------
 *  CTextureGL.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CTextureGL
{
    public:

        static unsigned long createTexture(const class CImg *image);
        static void textureOn(unsigned long idTexture);
        static void textureOff(void);
};

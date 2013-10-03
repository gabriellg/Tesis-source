/* -----------------------------------------------------------------------------
 *  CDisplayLimitsSprite.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "ITraslatorDisplay.hpp"

class CDisplayLimitsSprite : public ITraslatorDisplay
{
    public:

        CDisplayLimitsSprite(class CWorldGeometry2DPixels *worldGeometry2D);
        virtual ~CDisplayLimitsSprite();

        virtual void appendDescription(const char *symbol, class IDescription **description);
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene);

        virtual void setInitialPositionCamera(void) {;}
        virtual void positionCamera(class IGraphics *graphics) const {;}
        virtual void makeRotationCamera(class IGraphics *graphics) const {;}
        virtual void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees) {;}
        virtual void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ) {;}
        virtual void frontCamera(struct areaDibujo_t *areaDibujo, double step) {;}
        virtual void backCamera(struct areaDibujo_t *areaDibujo, double step) {;}

        bool isArea3D(void) const {return false;};
        bool withSize(unsigned long *width, unsigned long *height) const { return false;};

    private:

        struct SPrvDataPrivateDisplayLimitsSprite *m_dataPrivate;
};

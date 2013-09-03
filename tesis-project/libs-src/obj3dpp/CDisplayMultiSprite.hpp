/* -----------------------------------------------------------------------------
 *  CDisplayMultiSprite.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDisplay.hpp"

class CDisplayMultiSprite : public IDisplay
{
    public:

        enum ETypeDistribution
        {
            HORIZONTAL, VERTICAL
        };

        CDisplayMultiSprite(enum ETypeDistribution distribution);
        virtual ~CDisplayMultiSprite();

        void appendDisplaySprite(class IDisplay **display);

        virtual void appendDescription(const char *symbol, class IDescription **description);
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene);

        virtual void setInitialPositionCamera(void);
        virtual void positionCamera(class IGraphics *graphics) const;
        virtual void makeRotationCamera(class IGraphics *graphics) const;
        virtual void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees);
        virtual void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ);
        virtual void frontCamera(struct areaDibujo_t *areaDibujo, double step);
        virtual void backCamera(struct areaDibujo_t *areaDibujo, double step);

        bool isArea3D(void) const;
        bool withSize(unsigned long *width, unsigned long *height) const { return false; };

    private:

        struct prv_dataPrivateDisplayMultiSprite_t *m_dataPrivate;
};

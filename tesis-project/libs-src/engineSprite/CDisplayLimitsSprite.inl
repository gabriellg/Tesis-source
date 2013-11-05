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

    private:

        struct SPrvDataPrivateDisplayLimitsSprite *m_dataPrivate;
};

/* -----------------------------------------------------------------------------
 *  CDescriptionsGL.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionGraphics3D.hpp"

class CDescriptionForrestGL : public IDescriptionGraphics3D
{
    public:

        CDescriptionForrestGL(double sizeX, double sizeY);
        virtual ~CDescriptionForrestGL();

        virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}

        static void defineLayer(class IGraphics *graphics);

    private:

        struct prv_dataPrivateDescriptionForrestGL_t *m_dataPrivate;
};

class CDescriptionTreeGL : public IDescriptionGraphics3D
{
    public:

        virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}

        static void defineLayer(class IGraphics *graphics);
};

class CDescriptionCylinderLightningGL : public IDescriptionGraphics3D
{
    public:

        virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}

        static void defineLayer(class IGraphics *graphics);
};

//CLight3d.hpp

#include "IDescriptionGraphics3D.hpp"

class CLight3d: public IDescriptionGraphics3D
{
public:

    CLight3d(class CLight **light);

private:

    virtual ~CLight3d();

    virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}
    virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure);
    virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}

    struct SPrvLight3d *m_dataPrivate;
};

/* -----------------------------------------------------------------------------
 *  CDescriptionModel3d.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionGraphics3D.hpp"

class CDescriptionModel3d : public IDescriptionGraphics3D
{
    public:

        CDescriptionModel3d(class CModel3d **model);
        virtual ~CDescriptionModel3d();

        virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) {;}

    private:

        struct SDataPrivateDescriptionModel3d *m_dataPrivate;
};

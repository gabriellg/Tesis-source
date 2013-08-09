//CFigure3DProcessDraw.hpp

#ifndef IDESCRIPTIONGRAPHICS3D_HPP_
#define IDESCRIPTIONGRAPHICS3D_HPP_

#include "IDescription.hpp"

class IDescriptionGraphics3D : public IDescription
{
	public:
		
        virtual void beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) = 0;
        virtual void drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) = 0;
        virtual void endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure) = 0;
};

#endif /* IDESCRIPTIONGRAPHICS3D_HPP_ */

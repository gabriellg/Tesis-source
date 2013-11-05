//IDisplay.hpp

#pragma once

class ITraslatorDisplay
{
	public:
		
		virtual ~ITraslatorDisplay() {;}
		
        virtual void appendDescription(const char *symbol, class IDescription **description) = 0;
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene) = 0;
};

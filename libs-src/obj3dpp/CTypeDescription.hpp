//CTypeDescription.hpp

#ifndef CTYPEDESCRIPTION_HPP_
#define CTYPEDESCRIPTION_HPP_

#include "CEventSystem.hpp"

class CTypeDescription: public CEventSystem
{
	public:
		
        CTypeDescription(const char *nameEvent) : CEventSystem(nameEvent) {;}
		virtual ~CTypeDescription() {;}
		
        virtual void beginSymbol(const char *symbol, const class IDataSymbol *dataFigure) = 0;
        virtual void drawSymbol(const char *symbol, const class IDataSymbol *dataFigure) = 0;
        virtual void endSymbol(const char *symbol, const class IDataSymbol *dataFigure) = 0;

};

#endif /* CTYPEDESCRIPTION_HPP_ */

/* -----------------------------------------------------------------------------
 *  CTypeDescriptionExecuteRobot.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CTypeDescription.hpp"

class CTypeDescriptionExecuteRobot : public CTypeDescription
{
    public:

        CTypeDescriptionExecuteRobot(class CWorldSimulationRobot *worldRef, const class CDictionaryDescription *dictionaryDescription);
        virtual ~CTypeDescriptionExecuteRobot();

    private:

        virtual void beginSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void endSymbol(const char *symbol, const class IDataSymbol *dataFigure);

        struct SPrvDataPrivateTypeDescriptionExecuteRobot *m_dataPrivate;
};

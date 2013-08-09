/* -----------------------------------------------------------------------------
 *  IDescriptionCommandRobot.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IDESCRIPTIONCOMMANDROBOT_HPP_
#define IDESCRIPTIONCOMMANDROBOT_HPP_

#include "IDescription.hpp"

class IDescriptionCommandRobot: public IDescription
{
    public:

        virtual void beginSymbol(class CWorldSimulationRobot *worldSimulation, const class IDataSymbol *dataSymbol) = 0;
        virtual void drawSymbol(class CWorldSimulationRobot *worldSimulation, const class IDataSymbol *dataSymbol) = 0;
        virtual void endSymbol(class CWorldSimulationRobot *worldSimulation, const class IDataSymbol *dataSymbol) = 0;
};

#endif /* IDESCRIPTIONCOMMANDROBOT_HPP_ */

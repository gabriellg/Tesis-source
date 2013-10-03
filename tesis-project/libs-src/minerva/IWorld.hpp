/* -----------------------------------------------------------------------------
 *  IWorld.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IWORLD_HPP_
#define IWORLD_HPP_

class IWorld
{
    public:

        virtual ~IWorld(){;}
        virtual class IObjectDraw *move(const char *idElement, class IObjectDraw **figure) const = 0;
};

#endif /* IWORLD_HPP_ */

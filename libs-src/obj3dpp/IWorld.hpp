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
        virtual class CAgent *move(const char *idElement, class CAgent **figure) const = 0;
};

#endif /* IWORLD_HPP_ */

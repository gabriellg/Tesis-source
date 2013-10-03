/* -----------------------------------------------------------------------------
 *  CTypeDescrPhysics.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef CTYPEDESCRPHYSICS_HPP_
#define CTYPEDESCRPHYSICS_HPP_

#include "CTypeDescription.hpp"

class CTypeDescrPhysics: public CTypeDescription
{
    public:

        CTypeDescrPhysics(const char *nameEvent) : CTypeDescription(nameEvent) {;};

        virtual class IObjectDraw *move(const char *idElement, class IObjectDraw **figure) = 0;
};

#endif /* CTYPEDESCRPHYSICS_HPP_ */

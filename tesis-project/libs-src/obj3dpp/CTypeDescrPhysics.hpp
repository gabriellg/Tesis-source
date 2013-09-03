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

        virtual class CAgent *move(const char *idElement, class CAgent **figure) = 0;
};

#endif /* CTYPEDESCRPHYSICS_HPP_ */

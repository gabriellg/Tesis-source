/* -----------------------------------------------------------------------------
 *  IOptionOGL.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IOPTIONOGL_HPP_
#define IOPTIONOGL_HPP_

class IOptionOGL
{
    public:

        enum EResult
        {
            NO_CHANGE,
            REDRAW,
            REGENERATE
        };

        virtual ~IOptionOGL() {;};
        virtual enum EResult execute() = 0;
};

#endif /* IOPTION_HPP_ */

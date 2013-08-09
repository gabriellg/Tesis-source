/* -----------------------------------------------------------------------------
 *  IGenerator.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IGENERATOR_HPP_
#define IGENERATOR_HPP_

class IGenerator
{
    public:

        virtual ~IGenerator() {;};

        virtual unsigned short versionGeneration(void) const = 0;
        virtual bool isFileSource(const char *fileSource) const = 0;
        virtual bool isFileGenerated(const char *fileSource) const = 0;
        virtual char *nameFileGenerated(const char *fileSource) const = 0;
        virtual void createSource(const char *fileSource, class CArrayString *codeFile) const = 0;
};

#endif /* IGENERATOR_HPP_ */

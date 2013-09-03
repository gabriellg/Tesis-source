// CGenFileImage.cpp: Genera ficheros fuentes de imagenes.

#include "CGenFileImage.inl"

#include "CGenerators.inl"
#include "IGenerator.hpp"

#include "CGenUtil.inl"

#include "asrtbas.h"
#include "filetxt.hpp"
#include "CImg.hpp"

#include "CFileName.hpp"
#include "CString.hpp"
#include "CArrayString.hpp"

class PrvGeneratorImage : public IGenerator
{
    public:

        virtual unsigned short versionGeneration(void) const;
        virtual bool isFileSource(const char *fileSource) const;
        virtual bool isFileGenerated(const char *fileGenerated) const;
        virtual char *nameFileGenerated(const char *fileSource) const;
        virtual void createSource(const char *fileSource, class CArrayString *codeFile) const;
};

//-----------------------------------------------------------------------

unsigned short PrvGeneratorImage::versionGeneration(void) const
{
    return 1;
}

//-----------------------------------------------------------------------

bool PrvGeneratorImage::isFileSource(const char *fileSource) const
{
	bool isFileImage;
	char *extFile;
	
	extFile = CFileName::getExtension(fileSource);
	
	if (CString::equalWithoutMatchCase(extFile, "png") == true)
		isFileImage = true;
	else if (CString::equalWithoutMatchCase(extFile, "ico") == true)
		isFileImage = true;
	else
	    isFileImage = false;
	
	CString::free(&extFile);
	
	return isFileImage;
}

//-----------------------------------------------------------------------

char *PrvGeneratorImage::nameFileGenerated(const char *fileSource) const
{
	char *nameFileDest;
	char *extFile;
	
	extFile = CFileName::getExtension(fileSource);
	assert(CString::size(extFile) > 2);
	extFile[0] = 'i';
	extFile[1] = 'i';
	
	nameFileDest = CFileName::changeExtension(fileSource, extFile);
	
	CString::free(&extFile);
	
	return nameFileDest;
}

//-----------------------------------------------------------------------

bool PrvGeneratorImage::isFileGenerated(const char *fileGenerated) const
{
    bool isFileGenerated;
    char *extFile;

    extFile = CFileName::getExtension(fileGenerated);

    if (CString::size(extFile) > 2)
    {
        if (extFile[0] == 'i' && extFile[1] == 'i')
            isFileGenerated = true;
        else
            isFileGenerated = false;

    }
    else
        isFileGenerated = false;

    CString::free(&extFile);

    return isFileGenerated;
}

//-----------------------------------------------------------------------

static class CArrayString *prv_imageToStrings(unsigned long numBytes, unsigned long numElementsForLine, const unsigned char *buffer)
{
    class CArrayString *stringsPixels;
    char *stringValuesPixels;

    stringsPixels = new CArrayString;
    stringValuesPixels = NULL;

    for (unsigned long i = 0; i < numBytes; i++)
    {
        char *strPixel;

        strPixel = CString::printf("%3d", buffer[i]);

        if (stringValuesPixels == NULL)
            stringValuesPixels = strPixel;
        else
        {
            CString::concatenate(&stringValuesPixels, ", ");
            CString::concatenate(&stringValuesPixels, strPixel);
        }

        if ((i + 1) % numElementsForLine == 0)
        {
            if (i != numBytes - 1)
                CString::concatenate(&stringValuesPixels, ",");

            stringsPixels->addDestroyingString(&stringValuesPixels);
        }
    }

    if (stringValuesPixels != NULL)
        stringsPixels->addDestroyingString(&stringValuesPixels);

    return stringsPixels;
}

//-----------------------------------------------------------------------

static class CArrayString *prv_bodyImage(const char *fileSource)
{
	class CArrayString *codeBody;
	class CImg *image;
	class CArrayString *stringsValuesPixels;
	unsigned long width, height, widthStep, nChannels, numBytes, numElementsForLine;
    unsigned char *buffer;
	char *strReturn;
	
	image = new CImg(fileSource);

	image->getDataRaw(&width, &height, &widthStep, &nChannels, &buffer);
    numBytes = widthStep * height;
    numElementsForLine = 5 * nChannels;

	stringsValuesPixels = prv_imageToStrings(numBytes, numElementsForLine, buffer);
	
	codeBody = new CArrayString;
	
	CGenUtil::appendDefinitionStaticArrayData(0, "unsigned char", "pixels", numBytes, &stringsValuesPixels, codeBody);
	CGenUtil::appendLineWhite(codeBody);
	
	strReturn = CString::printf("return new CImg(%d, %d, %d, %d, pixels);", width, height, widthStep, nChannels);
	codeBody->addDestroyingString(&strReturn);

	delete image;
	
	return codeBody;
}

//-----------------------------------------------------------------------

void PrvGeneratorImage::createSource(const char *fileSource, class CArrayString *sourceCode) const
{
	class CArrayString *bodyCode;
	
	assert_no_null(sourceCode);

	sourceCode->add("// File type image generated");

	CGenUtil::appendLineWhite(sourceCode);
	CGenUtil::appendLineWhite(sourceCode);
	CGenUtil::appendComentary(sourceCode);
	CGenUtil::appendLineWhite(sourceCode);
	
	{
		char *nameFuncion, *nameFileSource, *nameImage, *extSource;
		
		nameFileSource = CFileName::getFileNameWithoutExtension(fileSource);
		extSource = CFileName::getExtension(fileSource);
		nameImage = CString::endExtractFromChar(nameFileSource, '_');
		
		nameFuncion = NULL;
		CString::concatenate(&nameFuncion, "i_");
		CString::concatenate(&nameFuncion, nameImage);
		CString::concatenate(&nameFuncion, "_");
		CString::concatenate(&nameFuncion, extSource);
				
		CGenUtil::appendStaticFunctionDefinition("class CImg *", nameFuncion, "void", sourceCode);
		
		CString::free(&nameFuncion);
		CString::free(&nameFileSource);
		CString::free(&extSource);
		CString::free(&nameImage);
	}
	
	bodyCode = prv_bodyImage(fileSource);
	
	CGenUtil::appendBodyCode(0, &bodyCode, sourceCode);
	CGenUtil::appendLineWhite(sourceCode);
}

//-----------------------------------------------------------------------

void CGenFileImage::appendGeneratorImage(class CGenerators *generators)
{
    class IGenerator *generator;

    generator = new PrvGeneratorImage();
    generators->add(generator);
}

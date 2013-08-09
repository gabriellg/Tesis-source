//robots.c

#include "CApplicationWorldVirtual.hpp"

#include "CEscalextric.inl"

#include "CString.hpp"

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
    CApplicationWorldVirtual *appEscalextric;
    CEscalextric *escalextric;
    const char *fileNameXML;
    bool is3d;

    is3d = true;
    
    if (argc >= 2)
        fileNameXML = argv[1];
    else
        fileNameXML = "./circuitos/redondo.xml";

    if (argc >= 3)
    {
        if (CString::equal(argv[2], "Sprite") == true)
            is3d = false;
    }

    escalextric = new CEscalextric(is3d, fileNameXML);
	
    appEscalextric = new CApplicationWorldVirtual(escalextric, "Escalextric");
	
    appEscalextric->run(argc, argv);
	
	delete appEscalextric;
		
	return 0;
}


//---------------------------------------------------------------------------
// CColor
//---------------------------------------------------------------------------

#pragma once


#include <iostream>
#include <stdlib.h>

using namespace std;


class CColorImage
{
    public:
        enum TColorName { white, black, gray, red, green, blue };


        CColorImage()                            : R(0), G(0), B(0) {};
        CColorImage( TColorName color )          {  SetColorByName( color ); };
        CColorImage( char _R, char _G, char _B ) : R(_R), G(_G), B(_B) {};

        CColorImage( const CColorImage &_color )          : R(_color.R), G(_color.G), B(_color.B) {};
        void operator=( const CColorImage &_color )  { R=_color.R; G=_color.G; B=_color.B; };


        friend ostream &operator << (ostream &out, const CColorImage &color);



        unsigned char R, G, B;


    private:

        void SetRGB( char _R, char _G, char _B ) { R=_R; G=_G; B=_B; };
        void SetColorByName( TColorName color );
};

/**
*/

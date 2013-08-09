
#include "CColorImage.hpp"


//-----------------------------------------------------------------------
//
void CColorImage::SetColorByName( TColorName color )
{
    switch( color )
    {
        case white: SetRGB(255, 255, 255);
                    break;

        case black: SetRGB(0, 0, 0);
                    break;

        case gray:  SetRGB(125, 125, 125);
                    break;

        case red:   SetRGB(255, 0, 0);
                    break;

        case green: SetRGB(0, 255, 0);
                    break;

        case blue:  SetRGB(0, 0, 255);
                    break;

        default:    cerr << "ERROR(TColor): Invalid color" << endl;
                    exit(-1);
    }
}


//-----------------------------------------------------------------------
//
ostream &operator << (ostream &out, const CColorImage &color)
{
    out << "(" << color.R << "," << color.G << "," << color.B << ")";

    return out;
}

/**
*/

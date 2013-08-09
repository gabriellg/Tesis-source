
#include "CColorOperations.hpp"


//-----------------------------------------------------------------------
// http://es.wikipedia.org/wiki/Modelo_de_color_HSV
//
// H: Tonalidad, el tipo de color (como rojo, azul o amarillo). Se representa
//    como un grado de ángulo cuyos valores posibles van de 0 a 360° (aunque para
//    algunas aplicaciones se normalizan del 0 al 100%). Cada valor corresponde a
//    un color. Ejemplos: 0 es rojo, 60 es amarillo y 120 es verde.
// S: Saturación. Se representa como la distancia al eje de brillo negro-blanco.
//    Los valores posibles van del 0 al 100%. A este parámetro también se le suele
//    llamar "pureza" por la analogía con la pureza de excitación y la pureza
//    colorimétrica de la colorimetría. Cuanto menor sea la saturación de un color,
//    mayor tonalidad grisácea habrá y más decolorado estará. Por eso es útil definir
//    la insaturación como la inversa cualitativa de la saturación.
// V: Valor del color, el brillo del color. Representa la altura en el eje
//    blanco-negro. Los valores posibles van del 0 al 100%. 0 siempre es negro.
//    Dependiendo de la saturación, 100 podría ser blanco o un color más o menos saturado.
void CColorOperations::GetHSV( CColorImage color, float &H, float &S, float &V )
{
	float R01 = (float)color.R / 255.0;
	float G01 = (float)color.G / 255.0;
	float B01 = (float)color.B / 255.0;

	float minC = min( min(R01, G01), B01);
        float maxC = max( max(R01, G01), B01);

	float delta = maxC - minC;

        H = 0;
	S = 0;
	V = maxC;

	if( delta == 0 )
	{
            H = 0;
            S = 0;
	}
	else
	{
            S = delta / maxC;

            double dR01 = (((maxC - R01) / 6) + (delta / 2)) / delta;
            double dG01 = (((maxC - G01) / 6) + (delta / 2)) / delta;
            double dB01 = (((maxC - B01) / 6) + (delta / 2)) / delta;

            if ( R01 == maxC )
                H = dB01 - dG01;
            else if( G01 == maxC )
                H = (1/3) + dR01 - dB01;
            else
                H = (2/3) + dG01 - dR01;

            if (H < 0)
                H += 1;
            if (H > 1)
                H -= 1;
	}
	H = H * 360.0;
	S = S * 100;
	V = V * 100;
}


//-----------------------------------------------------------------------
//
void CColorOperations::SetHSV( CColorImage color, int H, int S, int V )
{
    int f;
    long p, q, t;

    if( S == 0 )
    {
        color.R = color.G = color.B = V * 255;
        return;
    }

    f = ( (H % 43) * 255) / 43;  //f = ( (H % 60) * 255) / 60;
    H /= 43;            //H /= 60;

    p = (V * ( 256 - S)) / 256;
    q = (V * ( 256 - (S * f)/256 )) / 256;
    t = (V * ( 256 - (S * ( 256 - f )) / 256)) / 256;

    switch( H ) {
        case 0:
                color.R = V;
                color.G = t;
                color.B = p;
                break;
        case 1:
                color.R = q;
                color.G = V;
                color.B = p;
                break;
        case 2:
                color.R = p;
                color.G = V;
                color.B = t;
                break;
        case 3:
                color.R = p;
                color.G = q;
                color.B = V;
                break;
        case 4:
                color.R = t;
                color.G = p;
                color.B = V;
                break;
        default:
                color.R = V;
                color.G = p;
                color.B = q;
                break;
    }
    
    color.R *= 255;
    color.G *= 255;
    color.B *= 255;


    /*
    if( S == 0 )
    {
        R = G = B = V * 255;
        return;
    }


    H = H * 6
    if ( H == 6 ) H = 0     //H must be < 1
    i = int( H )            //Or ... var_i = floor( var_h )
    int var1 = V * ( 1 - S )
    int var2 = V * ( 1 - S * ( h - var_i ) )
    int var3 = V * ( 1 - S * ( 1 - ( var_h - var_i ) ) )

    if      ( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1 }
    else if ( var_i == 1 ) { var_r = var_2 ; var_g = V     ; var_b = var_1 }
    else if ( var_i == 2 ) { var_r = var_1 ; var_g = V     ; var_b = var_3 }
    else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = V     }
    else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = V     }
    else                   { var_r = V     ; var_g = var_1 ; var_b = var_2 }

    R = var_r * 255                  //RGB results from 0 to 255
    G = var_g * 255
    B = var_b * 255
    */
}



/*
//-----------------------------------------------------------------------
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void CColorOperations::GetHSV1( float &H, float &S, float &V )
{
    float R01 = (float)R / 255.0;
    float G01 = (float)G / 255.0;
    float B01 = (float)B / 255.0;

    float min = min( min(R01, G01), B01 );
    float max = max( max(R01, G01), B01 );
    
    V = max;				// v
    float delta = max - min;

    if( max != 0 )
        S = delta / max;		// s
    else {
        // r = g = b = 0		// s = 0, v is undefined
        S = 0;
        H = -1;
        return;
    }
    if( R01 == max )
        H = ( g - b ) / delta;		// between yellow & magenta
    else if( g == max )
        H = 2 + ( b - r ) / delta;	// between cyan & yellow
    else
        H = 4 + ( r - g ) / delta;	// between magenta & cyan
    
    H *= 60;				// degrees
    if( H < 0 )
        H += 360;
}*/





/*
// RGB are converted to 0..1,
// H is from 0..360, SV from 0..1
void GetHSV3( float &H, float &S, float &V )
{
    float R01 = (float)R / 255.0;
    float G01 = (float)G / 255.0;
    float B01 = (float)B / 255.0;

    float min = min( min(R01, G01), B01);
    float max = max( max(R01, G01), B01);

    double delta = max - min;

    V = max;
    S = 0;
    H = 0;

    if (delta == 0)
    {
        H = 0;
        S = 0;
    }
    else
    {
        S = delta / max;
        double dR = 60*(max - r)/delta + 180;
        double dG = 60*(max - g)/delta + 180;
        double dB = 60*(max - b)/delta + 180;
        if (R01 == max)
                H = dB - dG;
        else if (g == max)
                H = 120 + dR - dB;
        else
                H = 240 + dG - dR;
    }

    if (H<0)
            H+=360;
    if (H>=360)
            H-=360;
}
*/


/*
var_R = ( R / 255 )                     //RGB from 0 to 255
var_G = ( G / 255 )
var_B = ( B / 255 )

var_Min = min( var_R, var_G, var_B )    //Min. value of RGB
var_Max = max( var_R, var_G, var_B )    //Max. value of RGB
del_Max = var_Max - var_Min             //Delta RGB value

V = var_Max

if ( del_Max == 0 )                     //This is a gray, no chroma...
{
   H = 0                                //HSV results from 0 to 1
   S = 0
}
else                                    //Chromatic data...
{
   S = del_Max / var_Max

   del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
   del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
   del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max

   if      ( var_R == var_Max ) H = del_B - del_G
   else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B
   else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R

   if ( H < 0 ) ; H += 1
   if ( H > 1 ) ; H -= 1
}
*/


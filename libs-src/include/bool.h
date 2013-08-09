// M�dulo:bool.c
// Descripci�n: Definici�n del bool.

#ifndef _BOOL_H
#define _BOOL_H

typedef struct ArrPoint3d_tag *ArrPoint3d;
typedef struct ArrPoint2d_tag *ArrPoint2d;
typedef struct ArrPixel_tag *ArrPixel;
typedef struct ArrString_tag *ArrString;

#define ArrTipo(tipo) struct Arr_##tipo
#define ArrPointer(tipo) struct Arr_pointer_##tipo
#define ArrClass(tipo) struct Arr_Class_##tipo
#define ArrGroup(tipo) struct Arr_Group_##tipo

class CArrPoint3d;
class CArrayDouble;
class CArrayULong;
template <class Type> class CArrayRef;
template <class Type> class CArray;

#endif

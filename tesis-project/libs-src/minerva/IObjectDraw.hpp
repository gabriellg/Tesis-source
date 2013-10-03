// Objeto genérico del Lenguaje L(E).

#pragma once

class IObjectDraw
{
public:

    IObjectDraw() {;}
    virtual ~IObjectDraw() {;}

    virtual void drawRepresentation(class CTypeDescription *evtDescription) = 0;
};

//CTransform3D.cpp

#include "CTransform3D.hpp"

#include "IGraphics.hpp"

#include "CTransform.hpp"
#include "CMatrix4x4.hpp"
#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"
#include "CPositionCamera.hpp"

const char *CTransform3D::ID_SYMBOL_SCALE3D = "Scale3d";
const char *CTransform3D::ID_SYMBOL_TRASLATE3D = "Traslate3d";
const char *CTransform3D::ID_SYMBOL_ROTATE3D = "Rotate3d";
const char *CTransform3D::ID_SYMBOL_EXTRUSION3D = "Extrusion3d";
const char *CTransform3D::ID_SYMBOL_CAMERA3D = "Camera3d";

class CDataScale3D: public IDataTransformation3D
{
public:

    CDataScale3D(double sx, double sy, double sz);

    virtual void applyTransformacion(class IGraphics *graphics) const;

private:

    double m_sx, m_sy, m_sz;
};

class CDataTraslate3D: public IDataTransformation3D
{
public:

    CDataTraslate3D(double tx, double ty, double tz);

    virtual void applyTransformacion(class IGraphics *graphics) const;

private:

    double m_tx, m_ty, m_tz;
};

class CDataRotate3D: public IDataTransformation3D
{
public:

    CDataRotate3D(double angle, double Ux, double Uy, double Uz);

    virtual void applyTransformacion(class IGraphics *graphics) const;

private:

    double m_angle, m_Ux, m_Uy, m_Uz;
};

class CDataExtrusion3D: public IDataTransformation3D
{
public:

    CDataExtrusion3D(double Nx, double Ny, double Nz);

    virtual void applyTransformacion(class IGraphics *graphics) const;

private:

    double m_Nx, m_Ny, m_Nz;
};

class CDataCamera3D: public IDataTransformation3D
{
public:

    CDataCamera3D(class CPositionCamera **positionCamera);
    virtual ~CDataCamera3D();

    virtual void applyTransformacion(class IGraphics *graphics) const;

private:

    class CPositionCamera *m_positionCamera;
};


//---------------------------------------------------------------

CDataScale3D::CDataScale3D(double sx, double sy, double sz)
{
    m_sx = sx;
    m_sy = sy;
    m_sz = sz;
}

//---------------------------------------------------------------

void CDataScale3D::applyTransformacion(class IGraphics *graphics) const
{
    assert_no_null(graphics);
    graphics->scale(m_sx, m_sy, m_sz);
}

//---------------------------------------------------------------

CDataTraslate3D::CDataTraslate3D(double tx, double ty, double tz)
{
    m_tx = tx;
    m_ty = ty;
    m_tz = tz;
}

//---------------------------------------------------------------

void CDataTraslate3D::applyTransformacion(class IGraphics *graphics) const
{
    assert_no_null(graphics);
    graphics->traslation(m_tx, m_ty, m_tz);
}

//---------------------------------------------------------------

CDataRotate3D::CDataRotate3D(double angle, double Ux, double Uy, double Uz)
{
    m_angle = angle;
    m_Ux = Ux;
    m_Uy = Uy;
    m_Uz = Uz;
}

//---------------------------------------------------------------

void CDataRotate3D::applyTransformacion(class IGraphics *graphics) const
{
    assert_no_null(graphics);

    graphics->rotation(m_angle, m_Ux, m_Uy, m_Uz);
}

//-----------------------------------------------------------------------

CDataExtrusion3D::CDataExtrusion3D(double Nx, double Ny, double Nz)
{
    m_Nx = Nx;
    m_Ny = Ny;
    m_Nz = Nz;
}

//---------------------------------------------------------------

void CDataExtrusion3D::applyTransformacion(class IGraphics *graphics) const
{
    class CMatrix4x4 *matrix;

    assert_no_null(graphics);

    matrix = CMatrix4x4::createExtrusion(m_Nx, m_Ny, m_Nz);
    graphics->appendMatrix(matrix);
    delete matrix;
}

//---------------------------------------------------------------

CDataCamera3D::CDataCamera3D(class CPositionCamera **positionCamera)
{
    m_positionCamera = ASSIGN_PP_NO_NULL(positionCamera, class CPositionCamera);
}

//---------------------------------------------------------------

CDataCamera3D::~CDataCamera3D()
{
    assert_no_null(m_positionCamera);
    DELETE_OBJECT(&m_positionCamera, class CPositionCamera);
}

//---------------------------------------------------------------

void CDataCamera3D::applyTransformacion(class IGraphics *graphics) const
{
    assert_no_null(graphics);
    assert_no_null(m_positionCamera);

    m_positionCamera->positionCamera(graphics);
}

//---------------------------------------------------------------

static class CArray<IObjectDraw> *prv_createSons(class IObjectDraw **figure)
{
    class CArray<IObjectDraw> *sons;

    sons = new CArray<IObjectDraw>(1);
    sons->set(0, *figure);
    *figure = NULL;

    return sons;
}

//---------------------------------------------------------------

class CTransform *CTransform3D::createScale3D(class IObjectDraw **figure, double sx, double sy, double sz)
{
    class IDataSymbol *dataTransformation;
    class CArray<IObjectDraw> *sons;

    sons = prv_createSons(figure);

    dataTransformation = new CDataScale3D(sx, sy, sz);
    return new CTransform(ID_SYMBOL_SCALE3D, &dataTransformation, &sons);
}

//---------------------------------------------------------------

class CTransform *CTransform3D::createTraslate3D(class IObjectDraw **figure, double tx, double ty, double tz)
{
    class IDataSymbol *dataTransformation;
    class CArray<IObjectDraw> *sons;

    sons = prv_createSons(figure);

    dataTransformation = new CDataTraslate3D(tx, ty, tz);
    return new CTransform(ID_SYMBOL_TRASLATE3D, &dataTransformation, &sons);
}

//---------------------------------------------------------------

class CTransform *CTransform3D::createRotate3D(class IObjectDraw **figure, double angle, double Ux, double Uy,
        double Uz)
{
    class IDataSymbol *dataTransformation;
    class CArray<IObjectDraw> *sons;

    sons = prv_createSons(figure);

    dataTransformation = new CDataRotate3D(angle, Ux, Uy, Uz);
    return new CTransform(ID_SYMBOL_ROTATE3D, &dataTransformation, &sons);
}

//---------------------------------------------------------------

class CTransform *CTransform3D::createExtrusion3D(class IObjectDraw **figure, double Nx, double Ny, double Nz)
{
    class IDataSymbol *dataTransformation;
    class CArray<IObjectDraw> *sons;

    sons = prv_createSons(figure);

    dataTransformation = new CDataExtrusion3D(Nx, Ny, Nz);
    return new CTransform(ID_SYMBOL_EXTRUSION3D, &dataTransformation, &sons);
}

//---------------------------------------------------------------

class CTransform *CTransform3D::createCamera3d(class CArray<IObjectDraw> **sons, class CPositionCamera **posCamera)
{
    class IDataSymbol *dataTransformation;

    dataTransformation = new CDataCamera3D(posCamera);
    return new CTransform(ID_SYMBOL_CAMERA3D, &dataTransformation, sons);
}

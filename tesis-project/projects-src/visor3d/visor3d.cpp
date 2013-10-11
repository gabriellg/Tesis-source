/* -----------------------------------------------------------------------------
*  visor3d.cpp
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CApplicationScreen.hpp"
#include "options.hpp"

#include "CModel3d.hpp"
#include "CMaterial.hpp"
#include "CMesh.hpp"
#include "CString.hpp"
#include "CMatrix4x4.hpp"
#include "CGeneratorSolid.hpp"
#include "CGeneratorModel.hpp"

#include "asrtbas.h"

class CApplicationVisor : public CApplicationScreen
{
    public:

        CApplicationVisor(const char *nameFile);
        virtual ~CApplicationVisor(void);

    protected:

        virtual void initAplication(void);
        virtual void initMaterialsAndBlocks(class IGraphics *graphics);
        virtual void drawScreen(class IGraphics *graphics);

    private:

        const char *m_nameFile;
        class CModel3d *m_model;
};

//---------------------------------------------------------------

CApplicationVisor::CApplicationVisor(const char *nameFile) : CApplicationScreen("Visor Modelos 3d")
{
    class IOptionOGL *option;

    m_nameFile = CString::copy(nameFile);
    m_model = NULL;

    option = new COptionCreateCube(&m_model);
    appendOption("Cube", 'C', &option);

    option = new COptionCreateGrid3d(&m_model);
    appendOption("Grid 3d", 'G', &option);

    option = new COptionCreatePathOpen(&m_model);
    appendOption("Path Opened", 'O', &option);

    option = new COptionCreatePathClosed(&m_model);
    appendOption("Path Closed", 'P', &option);

    option = new COptionCreatePrism(&m_model);
    appendOption("Prim", 'M', &option);

    option = new COptionCreateSurface(&m_model);
    appendOption("Surface", 'S', &option);

    option = new COptionCreateCylinder(&m_model);
    appendOption("Cylinder", 'Y', &option);

    option = new COptionCreateModelRead(m_nameFile, &m_model);
    appendOption("Model Read", 'R', &option);

    option = new COptionCreateWithImage(&m_model);
    appendOption("Model With Image", 'I', &option);

    option = new COptionCreateSky(&m_model);
    appendOption("Sky", 'k', &option);
}

//---------------------------------------------------------------

CApplicationVisor::~CApplicationVisor(void)
{
    delete m_nameFile;
    delete m_model;
}

//---------------------------------------------------------------

static class CMesh *prv_meshPrism(double sizeX, double sizeY, double sizeZ)
{
    class CMesh *mesh;
    class CMatrix4x4 *matrix;

    matrix = CMatrix4x4::createScale(sizeX, sizeY, sizeZ);
    mesh = CGeneratorSolid::createUnitCube();
    mesh->applyMatrix(matrix);

    delete matrix;

    return mesh;
}

//---------------------------------------------------------------

static class CModel3d *prv_modelPrism(void)
{
    class CModel3d *model;
    class CMaterial *material;
    class CMesh *mesh1, *mesh2;
    const char *PRV_DEFAULT = "default";
    const char *PRV_DEFAULT2 = "default2";

    mesh1 = prv_meshPrism(10., 5., 2.);
    mesh2 = prv_meshPrism(5., 10., 1.);

    model = new CModel3d;

    material = new CMaterial(PRV_DEFAULT, 1., 0., 0., 0.4);
    model->appendMaterial(&material);

    material = new CMaterial(PRV_DEFAULT2, 0., 1., 0., 1.);
    model->appendMaterial(&material);

    model->appendMesh(PRV_DEFAULT, mesh1);
    model->appendMesh(PRV_DEFAULT2, mesh2);

    delete mesh1;
    delete mesh2;

    return model;
}

//---------------------------------------------------------------

void CApplicationVisor::initAplication(void)
{
    m_model = prv_modelPrism();
}

//---------------------------------------------------------------

void CApplicationVisor::initMaterialsAndBlocks(class IGraphics *graphics)
{
    assert_no_null(m_model);
    m_model->defineMaterialsInGraphics(graphics);
    m_model->defineBlock(graphics);
}

//---------------------------------------------------------------

void CApplicationVisor::drawScreen(class IGraphics *graphics)
{
    assert_no_null(m_model);
    m_model->drawIdBlock(graphics);
}

//---------------------------------------------------------------

int main(int argc, char *argv[])
{
    class CApplicationVisor *application;
    const char *nameFile;

    if (argc > 1)
        nameFile = argv[1];
    else
        nameFile = "";

    application = new CApplicationVisor(nameFile);
    application->run(argc, argv);
    delete application;

    return 0;
}

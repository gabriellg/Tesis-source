// Define una aplicación con pantalla.

#include "CApplicationScreen.hpp"
#include "CMenu.inl"
#include "IOptionOGL.hpp"

#include "opengl.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"
#include "CString.hpp"
#include "CColor.hpp"
#include "CLight.hpp"
#include "CGeometry.hpp"
#include "CPositionCamera.hpp"
#include "CGraphicsRect.hpp"
#include "CGraphicsGL.hpp"

static class CApplicationScreen *s_ApplicationCurrent = NULL;

struct SPrvDataPrivateApplicationScreen
{
    char *title;

    unsigned long idNewIdentOption;
    class CMenu *menu;

    bool hasLimits;
    double xmin, ymin, zmin, xmax, ymax, zmax;

    double xWindows, yWindows;

    bool isPressed;
    int xPrev, yPrev;

    class CGraphicsGL *graphics;
    class CPositionCamera *positionCamera;
    class CLight *ligth0;
};

static const double PRV_PRECISION = 1e-5;
static const double PRV_INFINITE = 1e100;
static const double PRV_STEP_CAMERA = .1;

//-----------------------------------------------------------------------

static class CPositionCamera *prv_createPositionCameraDefault(void)
{
    class CPositionCamera *positionCamera;
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;

    eyeX = 0.;
    eyeY = 0.;
    eyeZ = 5.;

    pointReferenceX = 0.;
    pointReferenceY = 0.;
    pointReferenceZ = 0.;

    upX = 0.;
    upY = 1.;
    upZ = 0.;

    positionCamera = new CPositionCamera(eyeX, eyeY, eyeZ, pointReferenceX, pointReferenceY, pointReferenceZ, upX, upY, upZ);

    return positionCamera;
}

//-----------------------------------------------------------------------

static void prv_putPositionCamera(const class CPositionCamera *positionCamera, class IGraphics *graphics)
{
    assert_no_null(positionCamera);

    positionCamera->positionCamera(graphics);
    positionCamera->makeRotationCamera(graphics);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateApplicationScreen *prv_createApplicationScreen(
        const char *title,
        unsigned long idNewIdentOption,
        class CMenu **menu,
        bool hasLimits,
        double xmin, double ymin, double zmin,
        double xmax, double ymax, double zmax,
        double xWindows, double yWindows,
        bool isPressed,
        int xPrev, int yPrev,
        class CGraphicsGL **graphics,
        class CPositionCamera **positionCamera,
        class CLight **ligth0)
{
    struct SPrvDataPrivateApplicationScreen *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateApplicationScreen);

    dataPrivate->title = CString::copy(title);

    dataPrivate->idNewIdentOption = idNewIdentOption;
    dataPrivate->menu = ASSIGN_PP_NO_NULL(menu, class CMenu);

    dataPrivate->hasLimits = hasLimits;
    dataPrivate->xmin = xmin;
    dataPrivate->ymin = ymin;
    dataPrivate->zmin = zmin;

    dataPrivate->xmax = xmax;
    dataPrivate->ymax = ymax;
    dataPrivate->zmax = zmax;

    dataPrivate->xWindows = xWindows;
    dataPrivate->yWindows = yWindows;

    dataPrivate->isPressed = isPressed;
    dataPrivate->xPrev = xPrev;
    dataPrivate->yPrev = yPrev;

    dataPrivate->graphics = ASSIGN_PP(graphics, class CGraphicsGL);
    dataPrivate->positionCamera = ASSIGN_PP_NO_NULL(positionCamera, class CPositionCamera);
    dataPrivate->ligth0 = ASSIGN_PP(ligth0, class CLight);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyApplicationScreen(struct SPrvDataPrivateApplicationScreen **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);
    assert_no_null((*dataPrivate)->positionCamera);
    assert_no_null((*dataPrivate)->ligth0);
    assert_no_null((*dataPrivate)->menu);
    assert_no_null((*dataPrivate)->graphics);

    delete (*dataPrivate)->positionCamera;

    CString::free(&(*dataPrivate)->title);
    delete (*dataPrivate)->menu;
    delete (*dataPrivate)->graphics;
    delete (*dataPrivate)->ligth0;

    FREE_T(dataPrivate, struct SPrvDataPrivateApplicationScreen);
}

//-----------------------------------------------------------------------

CApplicationScreen::CApplicationScreen(const char *title)
{
    class CMenu *menu;
    class CGraphicsGL *graphics;
    double xWindows, yWindows;
    class CPositionCamera *positionCamera;
    class CLight *ligth0;

    positionCamera = prv_createPositionCameraDefault();
    menu = new CMenu();
    graphics = NULL;
    ligth0 = NULL;
    xWindows = 512.;
    yWindows = 512.;

    m_dataPrivate = prv_createApplicationScreen(
                        title,
                        0, //unsigned long idNewIdentOption,
                        &menu,
                        false, //hasLimits,
                        0., 0., 0., //xmin, ymin, zmin,
                        0., 0., 0., //xmax, ymax, zmax,
                        xWindows, yWindows,
                        false, //isPressed,
                        0, 0, //xPrev, yPrev,
                        &graphics, &positionCamera, &ligth0);
}

//-----------------------------------------------------------------------

CApplicationScreen::~CApplicationScreen()
{
    prv_destroyApplicationScreen(&m_dataPrivate);
}

//---------------------------------------------------------------

void CApplicationScreen::appendOption(const char *nameOption, char key, class IOptionOGL **option)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->menu);

    m_dataPrivate->menu->appendOption(nameOption, m_dataPrivate->idNewIdentOption, key, option);
    m_dataPrivate->idNewIdentOption++;
}

//---------------------------------------------------------------

static double prv_factor(double minimum, double maximum)
{
    double factor;
    double size;

    size = maximum - minimum;

    if (CMath::isZero(size, PRV_PRECISION) == true)
        factor = PRV_INFINITE;
    else
    {
        assert(size > 0.);
        factor = 1. / size;
    }

    return factor;
}

//---------------------------------------------------------------

static double prv_calculateFactor(
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    double factorMin, factor;

    factorMin = PRV_INFINITE;

    factor = prv_factor(xmin, xmax);
    factorMin = MIN(factor, factorMin);

    factor = prv_factor(ymin, ymax);
    factorMin = MIN(factor, factorMin);

    factor = prv_factor(zmin, zmax);
    factorMin = MIN(factor, factorMin);

    return factorMin;
}

//---------------------------------------------------------------

void CApplicationScreen::draw(void)
{
    class IGraphics *graphics;
    double ratioXforY;
    double xcenter, ycenter, zcenter;
    double factor;

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->positionCamera);
    assert_no_null(m_dataPrivate->ligth0);
    assert(m_dataPrivate->yWindows > 0);

    ratioXforY = m_dataPrivate->xWindows / m_dataPrivate->yWindows;

    if (m_dataPrivate->hasLimits == true)
    {
        CGeometry::calculateCenter(
                    m_dataPrivate->xmin, m_dataPrivate->ymin, m_dataPrivate->zmin,
                    m_dataPrivate->xmax, m_dataPrivate->ymax, m_dataPrivate->zmax,
                    &xcenter, &ycenter, &zcenter);
    }
    else
    {
        xcenter = 0.; ycenter = 0.; zcenter = 0.;
    }

    factor = prv_calculateFactor(
                    m_dataPrivate->xmin, m_dataPrivate->ymin, m_dataPrivate->zmin,
                    m_dataPrivate->xmax, m_dataPrivate->ymax, m_dataPrivate->zmax);

    graphics = m_dataPrivate->graphics;

    graphics->viewport(0, 0, m_dataPrivate->xWindows, m_dataPrivate->xWindows);

    graphics->clear();

    graphics->definePerpective(45., ratioXforY, 1., 100.);

    m_dataPrivate->ligth0->putLight(graphics, IGraphics::LIGHT0);

    graphics->pushTransformation();

    prv_putPositionCamera(m_dataPrivate->positionCamera, graphics);

    graphics->scale(factor, factor, factor);

    graphics->traslation(-xcenter, -ycenter, -zcenter);

    m_dataPrivate->graphics->setModeOpaque();
    drawScreen(graphics);

    m_dataPrivate->graphics->setModeTransparency();
    drawScreen(graphics);

    m_dataPrivate->graphics->setModeOpaque();

    graphics->popTransformation();
}

//---------------------------------------------------------------

void CApplicationScreen::executeKey(char keyPressed)
{
	class IOptionOGL *optionFound;

	assert_no_null(m_dataPrivate);

	if (m_dataPrivate->menu->searchOptionKeyPressed(keyPressed, &optionFound) == true)
	    executeOption(optionFound);
}

//---------------------------------------------------------------

void CApplicationScreen::executeIdOption(unsigned long idOption)
{
    class IOptionOGL *option;

    assert_no_null(m_dataPrivate);

    option = m_dataPrivate->menu->getOptionId(idOption);
    executeOption(option);
}

//---------------------------------------------------------------

class CLight *CApplicationScreen::createLight0(void)
{
    class CColor *colorAmbient, *colorDiffuse, *colorSpecular;
    double xPosLight, yPosLight, zPosLight;

    xPosLight = 7.;
    yPosLight = 7.;
    zPosLight = 15.;

    colorAmbient = new CColor(0.4, 0.4, 0.4);
    colorDiffuse = new CColor(0.7, 0.7, 0.7);
    colorSpecular = new CColor(0.8, 0.6, 0.6);

    return new CLight(&colorAmbient, &colorDiffuse, &colorSpecular, xPosLight, yPosLight, zPosLight);
}

//---------------------------------------------------------------

void CApplicationScreen::initAplication(void)
{
    ;
}

//---------------------------------------------------------------

void CApplicationScreen::changeWindows(int width, int height)
{
    assert_no_null(m_dataPrivate);
    m_dataPrivate->xWindows = width;
    m_dataPrivate->yWindows = height;
}

//---------------------------------------------------------------

void CApplicationScreen::mouseClick(int button, int state, int x, int y)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->positionCamera);

    if (button == GLUT_LEFT_BUTTON)
    {
        switch(state)
        {
            case GLUT_DOWN:
            {
                m_dataPrivate->isPressed = true;
                m_dataPrivate->xPrev = x;
                m_dataPrivate->yPrev = y;
                break;
            }
            case GLUT_UP:
            {
                m_dataPrivate->isPressed = false;
                m_dataPrivate->xPrev = 0;
                m_dataPrivate->yPrev = 0;
                break;
            }
            default_error();
        }
    }
    else if (button == GLUT_WHEEL_UP)
    {
        m_dataPrivate->positionCamera->frontCamera(PRV_STEP_CAMERA);
    }
    else if (button == GLUT_WHEEL_DOWN)
    {
        m_dataPrivate->positionCamera->backCamera(PRV_STEP_CAMERA);
    }
}

//---------------------------------------------------------------

void CApplicationScreen::moveMouseWithButtonPulsed(int x, int y)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->positionCamera);

    if (m_dataPrivate->isPressed == true)
    {
        double incrX, incrY;

        incrX = y - m_dataPrivate->yPrev;
        incrX *= (360. / m_dataPrivate->yWindows);

        incrY = x - m_dataPrivate->xPrev;
        incrY *= (360. / m_dataPrivate->xWindows);

        m_dataPrivate->positionCamera->incrRotationCamera(incrX, incrY, 0.);

        m_dataPrivate->xPrev = x;
        m_dataPrivate->yPrev = y;

        glutPostRedisplay();
    }
}

//---------------------------------------------------------------

static void prv_setupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//---------------------------------------------------------------

static void prv_renderScene(void)
{
    assert_no_null(s_ApplicationCurrent);
    s_ApplicationCurrent->draw();
    glutSwapBuffers();
}

//---------------------------------------------------------------

static void prv_timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(33, prv_timer, 1);
}

//---------------------------------------------------------------

static void prv_keysApplication(unsigned char keyPressed,int x, int y)
{
    assert_no_null(s_ApplicationCurrent);
    s_ApplicationCurrent->executeKey((char)keyPressed);
}

//---------------------------------------------------------------

static void prv_executeOption(int idOption)
{
    s_ApplicationCurrent->executeIdOption(idOption);
}

//---------------------------------------------------------------

static void prv_createMenuOGL(class CMenu *menu)
{
    unsigned long idMenuOGL;

    assert_no_null(menu);

    if (menu->hasOptions() == true)
    {
        idMenuOGL = glutCreateMenu(prv_executeOption);
        menu->appendOptionsToMenuOGL(idMenuOGL);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

//---------------------------------------------------------------

static void prv_changeSize(int width, int height)
{
    if (height == 0)
        height = 1;

    s_ApplicationCurrent->changeWindows(width, height);
    glutPostRedisplay();
}

//---------------------------------------------------------------

static void prv_mouseClickApplication(int button, int state, int x, int y)
{
    assert_no_null(s_ApplicationCurrent);
    s_ApplicationCurrent->mouseClick(button, state, x, y);
    glutPostRedisplay();
}

//---------------------------------------------------------------

static void prv_moveMouseWithButtonPulsed(int x, int y)
{
    assert_no_null(s_ApplicationCurrent);
    s_ApplicationCurrent->moveMouseWithButtonPulsed(x, y);
}

//---------------------------------------------------------------

void CApplicationScreen::calculateLimits(void)
{
    class CGraphicsRect *graphicsRect;

    graphicsRect = new CGraphicsRect();

    drawScreen(graphicsRect);

    m_dataPrivate->hasLimits = graphicsRect->getLimits(
                    &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                    &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    delete graphicsRect;
}

//---------------------------------------------------------------

void CApplicationScreen::executeOption(class IOptionOGL *option)
{
    enum IOptionOGL::EResult result;

    assert_no_null(option);

    result = option->execute();

    switch (result)
    {
        case IOptionOGL::NO_CHANGE:

            break;

        case IOptionOGL::REDRAW:

            glutPostRedisplay();
            break;

        case IOptionOGL::REGENERATE:
        {
            m_dataPrivate->graphics->removeBlocksRegisters();

            delete m_dataPrivate->positionCamera;
            m_dataPrivate->positionCamera = prv_createPositionCameraDefault();

            calculateLimits();
            initMaterialsAndBlocks(m_dataPrivate->graphics);
            glutPostRedisplay();
            break;
        }
        default_error();
    };
}

//---------------------------------------------------------------

void CApplicationScreen::initialization(void)
{
    assert_no_null(m_dataPrivate);

    glutInitWindowSize(m_dataPrivate->xWindows, m_dataPrivate->xWindows);
    glutCreateWindow(m_dataPrivate->title);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutReshapeFunc(prv_changeSize);
    glutDisplayFunc(prv_renderScene);
    glutKeyboardFunc(prv_keysApplication);
    glutMouseFunc(prv_mouseClickApplication);
    glutMotionFunc(prv_moveMouseWithButtonPulsed);

    prv_setupRC();

    assert(m_dataPrivate->graphics == NULL);
    assert(m_dataPrivate->ligth0 == NULL);

    m_dataPrivate->graphics = new CGraphicsGL();
    m_dataPrivate->graphics->initDevice();
    m_dataPrivate->ligth0 = createLight0();

    initAplication();
    calculateLimits();
    initMaterialsAndBlocks(m_dataPrivate->graphics);

    prv_createMenuOGL(m_dataPrivate->menu);
}

//---------------------------------------------------------------

void CApplicationScreen::run(int argc, char **argv)
{
	assert(s_ApplicationCurrent == NULL);
	assert_no_null(m_dataPrivate);
	
    glutInit(&argc, argv);

	s_ApplicationCurrent = this;
	
	initialization();

	//TODO: Implementar animación.
	//glutTimerFunc(33, prv_timer, 1);

	glutMainLoop();
	
	s_ApplicationCurrent = NULL;
}

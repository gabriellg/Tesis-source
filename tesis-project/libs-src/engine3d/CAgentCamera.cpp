//CAgentCamera.cpp : Agent to handle camera.

#include "CAgentCamera.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CPositionCamera.hpp"
#include "CArray.hpp"
#include "CMath.hpp"
#include "CTransform3D.hpp"
#include "CTransform.hpp"
#include "CCollectionEventsSystem.hpp"
#include "CEventCamera.hpp"
#include "CTypeDescriptionGL.hpp"

struct SPrvAnimationRotation
{
    double t;

    double rotXDegrees0, rotYDegrees0, rotZDegrees0;
    double rotXDegrees1, rotYDegrees1, rotZDegrees1;
};

struct SPrvAgentCamera
{
    class CPositionCamera *positionCameraInitial;
    class CPositionCamera *positionCamera;
    struct SPrvAnimationRotation *currentAnimation;
};

//-----------------------------------------------------------------------

static struct SPrvAnimationRotation *prv_createAnimationRotation(double t, double rotXDegrees0, double rotYDegrees0,
        double rotZDegrees0, double rotXDegrees1, double rotYDegrees1, double rotZDegrees1)
{
    struct SPrvAnimationRotation *animation;

    animation = new struct SPrvAnimationRotation;

    animation->t = t;

    animation->rotXDegrees0 = rotXDegrees0;
    animation->rotYDegrees0 = rotYDegrees0;
    animation->rotZDegrees0 = rotZDegrees0;

    animation->rotXDegrees1 = rotXDegrees1;
    animation->rotYDegrees1 = rotYDegrees1;
    animation->rotZDegrees1 = rotZDegrees1;

    return animation;
}

//---------------------------------------------------------------

static void prv_destroyAnimationRotation(struct SPrvAnimationRotation **animation)
{
    DELETE_OBJECT(animation, struct SPrvAnimationRotation);
}

//-----------------------------------------------------------------------

static struct SPrvAgentCamera *prv_createAgentCamera(class CPositionCamera **positionCameraInitial,
        class CPositionCamera **positionCamera, struct SPrvAnimationRotation **currentAnimation)
{
    struct SPrvAgentCamera *dataPrivate;

    dataPrivate = new struct SPrvAgentCamera;

    dataPrivate->positionCameraInitial = ASSIGN_PP_NO_NULL(positionCameraInitial, class CPositionCamera);
    dataPrivate->positionCamera = ASSIGN_PP_NO_NULL(positionCamera, class CPositionCamera);
    dataPrivate->currentAnimation = ASSIGN_PP(currentAnimation, struct SPrvAnimationRotation);

    return dataPrivate;
}

//---------------------------------------------------------------

CAgentCamera::CAgentCamera(class CPositionCamera **positionCameraInitial)
{
    class CPositionCamera *positionCamera;
    struct SPrvAnimationRotation *currentAnimation;

    assert_no_null(positionCameraInitial);
    assert_no_null(*positionCameraInitial);

    positionCamera = new CPositionCamera(*positionCameraInitial);
    currentAnimation = NULL;

    m_dataPrivate = prv_createAgentCamera(positionCameraInitial, &positionCamera, &currentAnimation);
}

//---------------------------------------------------------------

CAgentCamera::~CAgentCamera()
{
    assert_no_null(m_dataPrivate);

    DELETE_OBJECT(&m_dataPrivate->positionCamera, class CPositionCamera);
    if (m_dataPrivate->currentAnimation != NULL)
        prv_destroyAnimationRotation(&m_dataPrivate->currentAnimation);

    DELETE_OBJECT(&m_dataPrivate, struct SPrvAgentCamera);
}

//---------------------------------------------------------------

void CAgentCamera::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    assert_no_null(allEvents);
}

//---------------------------------------------------------------

static void prv_processEvtCamera(const class CEventCamera *evtCamera,
        const class CPositionCamera *positionCameraInitial, class CPositionCamera **positionCamera,
        struct SPrvAnimationRotation **currentAnimation)
{
    assert_no_null(evtCamera);
    assert_no_null(positionCamera);
    assert_no_null(*positionCamera);
    assert_no_null(currentAnimation);
    assert(*currentAnimation == NULL);

    switch (evtCamera->getType())
    {
        case CEventCamera::INITIAL_CAMERA:

            DELETE_OBJECT(positionCamera, class CPositionCamera);
            *positionCamera = new CPositionCamera(positionCameraInitial);
            break;

        case CEventCamera::TRASLATE:
        {
            double step;

            evtCamera->getTranslate(&step);
            (*positionCamera)->traslate(step);
            break;
        }
        case CEventCamera::INCR_ROTATE:
        {
            double incrXRotate, incrYRotate, incrZRotate;

            evtCamera->getIncrRotate(&incrXRotate, &incrYRotate, &incrZRotate);
            (*positionCamera)->incrRotationCamera(incrXRotate, incrYRotate, incrZRotate);
            break;
        }
        case CEventCamera::ROTATION_ANIMATION:
        {
            double xRotate0, yRotate0, zRotate0;
            double xRotate1, yRotate1, zRotate1;

            (*positionCamera)->getRotation(&xRotate0, &yRotate0, &zRotate0);
            evtCamera->getRotateAnimation(&xRotate1, &yRotate1, &zRotate1);

            *currentAnimation = prv_createAnimationRotation(0., xRotate0, yRotate0, zRotate0, xRotate1, yRotate1,
                    zRotate1);
            break;
            break;
        }
        default_error()
            ;
    }
}

//-----------------------------------------------------------------------

static double prv_velocityFunction(double x)
{
    return CMath::pow(x - 1, 3) + 1;
}

//---------------------------------------------------------------

static void prv_makeAnimation(struct SPrvAnimationRotation *currentAnimation, class CPositionCamera *positionCamera)
{
    double tCalculate;

    assert_no_null(positionCamera);

    tCalculate = prv_velocityFunction(currentAnimation->t);

    positionCamera->setParametricRotation(tCalculate, currentAnimation->rotXDegrees0, currentAnimation->rotYDegrees0,
            currentAnimation->rotZDegrees0, currentAnimation->rotXDegrees1, currentAnimation->rotYDegrees1,
            currentAnimation->rotZDegrees1);

    currentAnimation->t += 0.04;
}

//---------------------------------------------------------------

class CAgent *CAgentCamera::evolution(class CCollectionEventsSystem *allEvents,
        class CArrayRef<CAgent> **nextGenerationSons)
{
    class CEventSystem *event;

    if (m_dataPrivate->currentAnimation != NULL)
    {
        prv_makeAnimation(m_dataPrivate->currentAnimation, m_dataPrivate->positionCamera);

        if (m_dataPrivate->currentAnimation->t > 1.)
            prv_destroyAnimationRotation(&m_dataPrivate->currentAnimation);
    }
    else if (allEvents->existEventSystem(CEventCamera::ID_EVENT, &event) == true)
    {
        class CEventCamera *evtCamera;

        evtCamera = dynamic_cast<class CEventCamera *>(event);
        assert_no_null(evtCamera);
        prv_processEvtCamera(evtCamera, m_dataPrivate->positionCameraInitial, &m_dataPrivate->positionCamera,
                &m_dataPrivate->currentAnimation);
    }

    setSons(nextGenerationSons);

    return this;
}

//---------------------------------------------------------------

class CArray<IObjectDraw> *CAgentCamera::createRepresentation(class CTypeDescription *evtDescription,
        class CArray<IObjectDraw> **childsOpt)
{
    class CArray<IObjectDraw> *objetsDraw;
    class CTypeDescriptionGL *descriptionGL;

    descriptionGL = dynamic_cast<class CTypeDescriptionGL *>(evtDescription);

    if (descriptionGL != NULL)
    {
        class IObjectDraw *objsIntoCamera;
        class CPositionCamera *camera;

        assert_no_null(childsOpt);
        assert_no_null(*childsOpt);

        camera = new CPositionCamera(m_dataPrivate->positionCamera);

        objsIntoCamera = CTransform3D::createCamera3d(childsOpt, &camera);

        objetsDraw = new CArray<IObjectDraw>();
        objetsDraw->add(objsIntoCamera);
    }
    else
        objetsDraw = *childsOpt;

    return objetsDraw;
}

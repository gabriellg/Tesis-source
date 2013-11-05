//CEventCamera.hpp: Evento de cámara.

#include "CEventSystem.hpp"

class CEventCamera: public CEventSystem
{
public:

    static const char *ID_EVENT;

    enum ETypeEventCamera
    {
        INITIAL_CAMERA, TRASLATE, INCR_ROTATE, ROTATION_ANIMATION
    };

    static class CEventCamera *createInitialPositionCamera();
    static class CEventCamera *createTraslateCamera(double step);
    static class CEventCamera *createAnimationRotation(double rotXDegrees, double rotYDegrees, double rotZDegrees);
    static class CEventCamera *createIncrRotateCamera(double incrXRotate, double incrYRotate, double incrZRotate);

    virtual ~CEventCamera();

    enum ETypeEventCamera getType() const;

    void getTranslate(double *step) const;
    void getIncrRotate(double *incrXRotate, double *incrYRotate, double *incrZRotate) const;
    void getRotateAnimation(double *xRotate, double *yRotate, double *zRotate) const;

private:

    struct SPrvEventCamera *m_dataPrivate;

    CEventCamera(struct SPrvEventCamera **dataPrivate);
};

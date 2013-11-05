// Define posición de camara.

class CPositionCamera
{
    public:
    
    	CPositionCamera(
                    double eyeX, double eyeY, double eyeZ,
                    double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                    double upX, double upY, double upZ);

    	CPositionCamera(const class CPositionCamera* positionCamera);
    	
    	virtual ~CPositionCamera();

    	void getRotation(double *rotXDegrees, double *rotYDegrees, double *rotZDegrees) const;
        
        void positionCamera(class IGraphics *graphics) const;
        void makeRotationCamera(class IGraphics *graphics) const;
        
        void incrRotationCamera(double incrRotXDegrees, double incrRotYDegrees, double incrRotZDegrees);
        void traslate(double step);
        void setParametricRotation(
                                double t,
                                double rotXIniDegrees, double rotYIniDegrees, double rotZIniDegrees,
                                double rotXEndDegrees, double rotYEndDegrees, double rotZEndDegrees);

        
    private:
    
        struct i_dataPrivate_t *m_dataPrivate;
};

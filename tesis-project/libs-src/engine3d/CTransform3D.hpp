//CTransformScale.hpp

#include "IDataSymbol.hpp"

class IDataTransformation3D : public IDataSymbol
{
    public:

        virtual void applyTransformacion(class IGraphics *graphics) const = 0;
};

class CTransform3D
{
	public:

        static const char *ID_SYMBOL_SCALE3D;
        static const char *ID_SYMBOL_TRASLATE3D;
        static const char *ID_SYMBOL_ROTATE3D;
        static const char *ID_SYMBOL_EXTRUSION3D;

		static class CTransform *createScale3D(class CAgent **actorToTransform, double sx, double sy, double sz);
		static class CTransform *createTraslate3D(class CAgent **actorToTransform, double tx, double ty, double tz);
		static class CTransform *createRotate3D(class CAgent **actorToTransform, double angle, double Ux, double Uy, double Uz);
		static class CTransform *createExtrusion3D(class CAgent **actorToTransform, double Nx, double Ny, double Nz);
};


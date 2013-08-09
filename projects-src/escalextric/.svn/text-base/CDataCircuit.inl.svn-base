//CDataCircuit.hpp: Definición del circuito del escalextric.

class CDataCircuit
{
    public:

        CDataCircuit(const char *fileNameXML);
        virtual ~CDataCircuit();

        class CImg *image(unsigned long *sizeXTrack, unsigned long *sizeYTrack) const;
        class CModel3d *model3d(void) const;

        void applyVelocity(unsigned long velocity, double radius, unsigned long *indTrack, double *stepInTrack) const;

        void getPositionLane(unsigned long indTrack, double stepInTrack, double traslateLane, double *xPos, double *yPos, double *angleRotation) const;

    private:

        struct SPrvDataPrivateCircuit *m_dataPrivate;
};

//CPlanetFigure.hpp

class CRobotSprites
{
    public:

        static class CImg *createMap(void);
        static class CImg *createImageMap(void);
        static class CModel3d *createModelScenario(void);

        static class CArray<CImg> *createImagesRobot(void);
        static class CModel3d* createModelRobot(void);

        static bool isObstable(class CImg *map, long x, long y);
        static bool cellWithMark(class CImg *map, long x, long y, char **markObjective);

        static class CImg *imageMark1(void);
        static class CImg *imageMark2(void);
        static class CImg *imageMark3(void);
        static class CImg *imageMark4(void);
        static class CImg *imageMarkEmpty(void);

        static class CImg *backgroundStatus(void);
};

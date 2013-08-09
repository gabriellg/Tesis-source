//CWorldSprite.hpp:

class CWorldSprite
{
	public:

        static class CImg *imageScenarioPlane(
                                const class CImg *map,
                                const class IEvaluateMap *evaluateMap,
                                unsigned long widthGrid,
                                unsigned long heightGrid);

        static class CImg *imageScenarioPerspective(
                                const class CImg *map,
                                const class IEvaluateMap *evaluateMap,
                                unsigned long heightGrid);

        static class CWorldGeometry2DPixels *createWorld2d(
                            const class CImg *map,
                            const class IEvaluateMap *evaluateMap,
                            unsigned long widthGrid,
                            unsigned long heightGrid);

        static class CCollectionPositionCreation *collectionPositionCreationPlane(
                                const class CImg *map,
                                const class IEvaluateMap *evaluateMap,
                                unsigned long widthGrid,
                                unsigned long heightGrid);

        static class CCollectionPositionCreation *collectionPositionCreationPerspective(
                                const class CImg *map,
                                const class IEvaluateMap *evaluateMap,
                                unsigned long heightGrid);

};

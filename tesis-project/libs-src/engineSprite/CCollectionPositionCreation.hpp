//CCollectionPositionCreation: positions Collection where it is been able to create Actors.

class CCollectionPositionCreation
{
	public:

		CCollectionPositionCreation();
		virtual ~CCollectionPositionCreation();

		void appendPosition(char **typePosition, long x, long y);

		void appendEventsCreations(
							class CCollectionEventsSystem *events,
							class CWorldGeometry2DPixels *worldGeometry2DPixels,
							class ICreatorActors *createActors);

	private:

		struct prv_dataPrivateCollectionPositionCreation_t *m_dataPrivate;
};

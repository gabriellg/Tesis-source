// Define una coleccion de eventos.

class CCollectionEventsSystem
{
    public:

	   CCollectionEventsSystem(const class CScene *scene);
       
	   virtual ~CCollectionEventsSystem();

       void appendEventSystem(class CEventSystem **eventSystem);
       
       bool existEventSystem(const char *idEvent, class CEventSystem **evtSystem_opt);
       class CArrayRef<CEventSystem> *allTypeEvent(const char *idEvent) const;

       const class CArray<CEventSystem> *getEvents(void) const;

       const class CScene *getScene() const;

    private:
    
        struct SPrvDataPrivateCollectionEventSystem *m_dataPrivate;
};

// Define un objeto del lenguja L(E).

#ifndef COBJECT3D_HPP_
#define COBJECT3D_HPP_

class CObject3d
{
	public:

		virtual ~CObject3d() {;}

        virtual void processDraw(class CTypeDescription *evtDraw) = 0;
        virtual class CObject3d *representation(class CTypeDescription *evtDraw) const = 0;
        virtual class CObject3d *nextGeneration(class CCollectionEventsSystem *allEvents) = 0;

        void eraseOldObjects(class CObject3d **oldObject);

        virtual void appendReferenceNotRepeated(class CArrayRef<CObject3d> *objectsNotRepeated) = 0;

        static void eraseObjectsScene(class CObject3d **object3d);

    protected:

        void appendNotRepeated(class CArrayRef<CObject3d> *objectsNoRepeated, CObject3d *object);
};

#endif /*COBJECT3D_HPP_*/


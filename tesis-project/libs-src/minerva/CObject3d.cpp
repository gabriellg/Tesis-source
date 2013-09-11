//Define object World.

#include "CObject3d.inl"

#include "asrtbas.h"
#include "CArrayRef.hpp"

//-----------------------------------------------------------------------

void CObject3d::appendNotRepeated(class CArrayRef<CObject3d> *objectsNoRepeated, CObject3d *object)
{
    if (objectsNoRepeated->existOnlyOneElement<CObject3d>(object, NULL, NULL) == false)
        objectsNoRepeated->add(object);
}

//-----------------------------------------------------------------------

void CObject3d::eraseOldObjects(class CObject3d **oldObject)
{
    class CArrayRef<CObject3d> *oldObjects, *objectsCurrent;
	unsigned long num;

	assert_no_null(oldObject);
	assert_no_null(*oldObject);

	oldObjects = new CArrayRef<CObject3d>;
	(*oldObject)->appendReferenceNotRepeated(oldObjects);

	objectsCurrent = new CArrayRef<CObject3d>;
	this->appendReferenceNotRepeated(objectsCurrent);

	num = oldObjects->size();
	for (unsigned long i = 0; i < num; i++)
	{
		class CObject3d *objectOld;

		objectOld = oldObjects->get(i);

		if (objectsCurrent->existOnlyOneElement<CObject3d>(objectOld, NULL, NULL) == false)
		    delete objectOld;
	}

	delete oldObjects;
	delete objectsCurrent;

	oldObject = NULL;
}

//---------------------------------------------------------------

void CObject3d::eraseObjectsScene(class CObject3d **object3d)
{
	class CArrayRef<CObject3d> *objectsNotRepeated;
	unsigned long num;

	assert_no_null(object3d);
	assert_no_null(*object3d);

	objectsNotRepeated = new CArrayRef<CObject3d>;
	(*object3d)->appendReferenceNotRepeated(objectsNotRepeated);

    num = objectsNotRepeated->size();
    for (unsigned long i = 0; i < num; i++)
    {
        class CObject3d *objectDeleted;

        objectDeleted = objectsNotRepeated->get(i);
        delete objectDeleted;
    }

    delete objectsNotRepeated;

	*object3d = NULL;
}


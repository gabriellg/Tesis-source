//ICreatorActors.hpp: Interfaz to create actors.

#ifndef ICREATORACTORS_HPP_
#define ICREATORACTORS_HPP_

class ICreatorActors
{
	public:

		ICreatorActors() {;}
		virtual ~ICreatorActors() {;}

		virtual bool isGoingToCreate(const char *typeActor) = 0;
        virtual bool createActor(const char *typeActor, char **idActor, class CAgent **actorCreate) = 0;
};

#endif /* ICREATORACTORS_HPP_ */

//IEvalutateMap.hpp

#ifndef IEVALUATEMAP_HPP_
#define IEVALUATEMAP_HPP_

class IEvaluateMap
{
	public:

		enum ETypeObstable
		{
			OBSTACLE, FREE
		};

		IEvaluateMap() {;};
		virtual ~IEvaluateMap() {;};
		
		virtual class CImg *evaluteImage(unsigned char r, unsigned char g, unsigned char b) const = 0;
		virtual enum ETypeObstable evaluteObstable(unsigned char r, unsigned char g, unsigned char b) const = 0;
		virtual bool isPositionCreateAgent(unsigned char r, unsigned char g, unsigned char b, char **typePosition) const = 0;
};

#endif /* IEVALUTATEMAP_HPP_ */

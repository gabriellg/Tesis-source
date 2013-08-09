#ifndef CRESULTDATA_HPP_
#define CRESULTDATA_HPP_

class CResultData
{
	public:
	
		virtual ~CResultData() {;}
		
		virtual const char *type() const = 0;
		
		bool isType(const char *type) const;
		
		virtual void draw(class IGraphics *graphics) const = 0;
};

#define CRESULTDATA_DECLARE_TYPE(classType) const char *type() const {return #classType; };

#define CRESULTDATA_GET(classType, data)\
(\
	assert_no_null(data),\
	assert(data->isType(#classType) == true),\
	(class classType *)data\
)

class CResultData *cresultdata_searchDataResultPrivate(const ArrClass(CResultData) *results, const char *type);

#define cresultdata_searchDataResult(results, typeSearch)\
(\
	(class typeSearch *)cresultdata_searchDataResultPrivate(results, #typeSearch)\
)

#endif /*CRESULTDATA_HPP_*/

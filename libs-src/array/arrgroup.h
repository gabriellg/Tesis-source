// Agrupaciones de arrays.

typedef void (*arrgroup_FuncLiberaPtr)(void **);
#define ARRGROUP_FUNC_DESTROY(func, tipo) (void)((void (*)(struct tipo **))func == func)

typedef bool (*arrgroup_FuncSearchPtr)(const void *,const void *);
#define ARRGROUP_FUNC_SEACH(func, tipo, tipoElem) (void)((bool (*)(const struct tipo *, tipoElem *))func == func)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct ArrGroup_t *arrgroup_createEmptyPrivate(const char *tipo);

void arrgroup_destroyPrivate(const char *tipo, struct ArrGroup_t **arrGroup, arrgroup_FuncLiberaPtr func_destroyElement);

unsigned long arrgroup_numGroupsPrivate(const char *tipo, const struct ArrGroup_t *arrGroup);

void arrgroup_appendGroupPrivate(const char *tipo, struct ArrGroup_t *arrGroup, struct Array_t **arrayElements);

struct Array_t *arrgroup_getPrivate(const char *tipo, const struct ArrGroup_t *arrGroup, unsigned long ind);

bool arrgroup_seachGroupAndElementPrivate(const char *tipo, const struct ArrGroup_t *arrGroup, const void *elem, unsigned long *indGroup, unsigned long *indElement, arrgroup_FuncSearchPtr funcSearch);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define arrgroup_createEmpty(tipo) ((ArrGroup(tipo) *)arrgroup_createEmptyPrivate(#tipo)) 

#define arrgroup_destroy(tipo, arrGroup, func_destroy)\
(\
		(void)((ArrGroup(tipo) **)arrGroup == arrGroup),\
		ARRGROUP_FUNC_DESTROY(func_destroy, tipo),\
		arrgroup_destroyPrivate(#tipo, (struct ArrGroup_t **)arrGroup, (arrgroup_FuncLiberaPtr)func_destroy)\
)

#define arrgroup_numGroups(tipo, arrGroup)\
(\
		(void)((const ArrGroup(tipo) *)arrGroup == arrGroup),\
		arrgroup_numGroupsPrivate(#tipo, (const struct ArrGroup_t *)arrGroup)\
)


#define arrgroup_appendGroup(tipo, arrGroup, arrayElements)\
(\
		(void)((ArrGroup(tipo) *)arrGroup == arrGroup),\
		(void)((ArrTipo(tipo) **)arrayElements),\
		arrgroup_appendGroupPrivate(#tipo, (struct ArrGroup_t *)arrGroup, (struct Array_t **)arrayElements)\
)

#define arrgroup_Get(tipo, arrGroup, ind)\
(\
		(void)((ArrGroup(tipo) *)arrGroup == arrGroup),\
		(ArrTipo(tipo) *)arrgroup_getPrivate(#tipo, (const struct ArrGroup_t *)arrGroup, ind)\
)

#define arrgroup_serachGroupAndElement(tipo, arrGroup, tipoElem, elem, indGroup, indElement, funcSearch)\
(\
		(void)((const ArrGroup(tipo) *)arrGroup == arrGroup),\
		(void)((const tipoElem *)elem == elem),\
		ARRGROUP_FUNC_SEACH(funcSearch, tipo, tipoElem),\
		arrgroup_seachGroupAndElementPrivate(\
				#tipo, (const struct ArrGroup_t *)arrGroup, (const void *)elem,\
				indGroup, indElement, (arrgroup_FuncSearchPtr)funcSearch)\
)

#ifndef RTTI_H_
#define RTTI_H_

#include <typeinfo>
#include <extension.h>

class IBaseType
{
public:
	/* Offset from the original IType* */
	virtual ptrdiff_t GetOffset() =0;

	/* Gets the basic type info for this type */
	virtual const std::type_info &GetTypeInfo() =0;

	/* Returns the number of types this type inherits from */
	virtual size_t GetNumBaseClasses() =0;

	/* Gets the first base class */
	virtual IBaseType *GetBaseClass(size_t num) =0;
};

class IType
{
public:
	/* Gets the top level class this type represents */
	virtual IBaseType *GetBaseType() =0;

	/* Destroy all memory resources held by this object chain */
	virtual void Destroy() =0;
};

/* Get type information for a class pointer */
IType *GetType(const void *ptr);

/* Returns the classname for a given type - Removes platform specific formatting */
const char *GetTypeName(const std::type_info &type);

inline void PrintSpaces(int indent)
{
	for (int i=0; i<indent; i++)
		META_CONPRINT("  ");
}

inline void DumpType(IBaseType *pType, int indent)
{
	PrintSpaces(indent);
	META_CONPRINTF("%s (%i)\n", GetTypeName(pType->GetTypeInfo()), pType->GetOffset());

	for (size_t i=0; i<pType->GetNumBaseClasses(); i++)
	{
		DumpType(pType->GetBaseClass(i), indent+1);
	}
}


#endif // RTTI_H_
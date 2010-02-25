#ifndef _INCLUDE_MACROS_H_
#define _INCLUDE_MACROS_H_

class IHookTracker
{
public:
	IHookTracker()
	{
		m_Next = m_Head;
		m_Head = this;
	}
	virtual void ReconfigureHook(IGameConfig *pConfig) =0;
	virtual void AddHook(CEntity *pEnt) =0;
	virtual void ClearFlag(CEntity *pEnt) =0;
public:
	static IHookTracker *m_Head;
	IHookTracker *m_Next;
};

#define DECLARE_HOOK(name, cl) \
class name##HookTracker : public IHookTracker \
{ \
public: \
	void ReconfigureHook(IGameConfig *pConfig) \
	{ \
		int offset; \
		pConfig->GetOffset(#name, &offset); \
		SH_MANUALHOOK_RECONFIGURE(name, offset, 0, 0); \
	} \
	void AddHook(CEntity *pEnt) \
	{ \
		cl *pThisType = dynamic_cast<cl *>(pEnt); \
		if (pThisType) \
		{ \
			SH_ADD_MANUALVPHOOK(name, pEnt->BaseEntity(), SH_MEMBER(pThisType, &cl::Internal##name), false); \
		} \
	} \
	void ClearFlag(CEntity *pEnt) \
	{ \
		cl *pThisType = dynamic_cast<cl *>(pEnt); \
		if (pThisType) \
		{ \
			pThisType->m_bIn##name = false; \
		} \
	} \
}; \
name##HookTracker name##cl##HookTracker;

#define PROP_SEND 0
#define PROP_DATA 1

#define DECLARE_SENDPROP(typeof, name) DECLARE_PROP(typeof, name, PROP_SEND)
#define DECLARE_DATAMAP(typeof, name) DECLARE_PROP(typeof, name, PROP_DATA)

#define DEFINE_PROP(name, cl)	cl::name##PropTracker cl::name##PropTrackerObj;

class IPropTracker
{
public:
	IPropTracker()
	{
		m_Next = m_Head;
		m_Head = this;
	}
	virtual void InitProp(CEntity *pEnt) =0;
public:
	static IPropTracker *m_Head;
	IPropTracker *m_Next;
};

#define DECLARE_PROP(typeof, name, search) \
typeof * name; \
friend class name##PropTracker; \
class name##PropTracker : public IPropTracker \
{ \
public: \
	name##PropTracker() \
	{ \
		lookup = false; \
		found = false; \
		typedesc = NULL; \
	} \
	void InitProp(CEntity *pEnt) \
	{ \
		ThisClass *pThisType = dynamic_cast<ThisClass *>(pEnt); \
		if (pThisType) \
		{ \
			if (!lookup) \
			{ \
				if (search == PROP_SEND) \
					found = gamehelpers->FindSendPropInfo(pEnt->edict()->GetNetworkable()->GetServerClass()->GetName(), #name, &info); \
				else \
				{ \
					datamap_t *pMap = gamehelpers->GetDataMap(pEnt->BaseEntity()); \
					if (pMap) \
					{ \
						typedesc = gamehelpers->FindInDataMap(pMap, #name); \
						if (typedesc != NULL) \
							found = true; \
					} \
				} \
				lookup = true; \
			} \
			if (found) \
				if (search == PROP_SEND) \
					pThisType->name = (typeof *)(((uint8_t *)(pEnt->BaseEntity())) + info.actual_offset); \
				else \
					pThisType->name = (typeof *)(((uint8_t *)(pEnt->BaseEntity())) + typedesc->fieldOffset[TD_OFFSET_NORMAL]); \
			else \
				pThisType->name = NULL; \
		} \
	} \
private: \
	sm_sendprop_info_t info; \
	typedescription_t *typedesc; \
	bool lookup; \
	bool found; \
}; \
static name##PropTracker name##PropTrackerObj;



#define DECLARE_DEFAULTHANDLER(type, name, ret, params, paramscall) \
ret type::name params \
{ \
	if (!m_bIn##name) \
		return SH_MCALL(BaseEntity(), name) paramscall; \
	SET_META_RESULT(MRES_IGNORED); \
	SH_GLOB_SHPTR->DoRecall(); \
	SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr()); \
	RETURN_META_VALUE(MRES_SUPERCEDE, (thisptr->*(__SoureceHook_FHM_GetRecallMFP##name(thisptr))) paramscall); \
} \
ret type::Internal##name params \
{ \
	SET_META_RESULT(MRES_SUPERCEDE); \
	type *pEnt = (type *)CEntity::Instance(META_IFACEPTR(CBaseEntity)); \
	if (!pEnt) \
		RETURN_META_VALUE(MRES_IGNORED, (ret)0); \
	int index = pEnt->entindex(); \
	pEnt->m_bIn##name = true; \
	ret retvalue = pEnt->name paramscall; \
	pEnt = (type *)CEntity::Instance(index); \
	if (pEnt) \
		pEnt->m_bIn##name = false; \
	return retvalue; \
}

#define DECLARE_DEFAULTHANDLER_void(type, name, params, paramscall) \
void type::name params \
{ \
	if (!m_bIn##name) \
	{ \
		SH_MCALL(BaseEntity(), name) paramscall; \
		return; \
	} \
	SET_META_RESULT(MRES_IGNORED); \
	SH_GLOB_SHPTR->DoRecall(); \
	SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr()); \
	(thisptr->*(__SoureceHook_FHM_GetRecallMFP##name(thisptr))) paramscall; \
	SET_META_RESULT(MRES_SUPERCEDE); \
} \
void type::Internal##name params \
{ \
	SET_META_RESULT(MRES_SUPERCEDE); \
	type *pEnt = (type *)CEntity::Instance(META_IFACEPTR(CBaseEntity)); \
	if (!pEnt) \
		RETURN_META(MRES_IGNORED); \
	int index = pEnt->entindex(); \
	pEnt->m_bIn##name = true; \
	pEnt->name paramscall; \
	pEnt = (type *)CEntity::Instance(index); \
	if (pEnt) \
		pEnt->m_bIn##name = false; \
}

#endif // _INCLUDE_MACROS_H_

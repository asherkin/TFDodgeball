/**
* vim: set ts=4 :
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2009 Matt Woodrow.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _INCLUDE_IENTITYFACTORY_H_
#define _INCLUDE_IENTITYFACTORY_H_

#include "CEntityManager.h"

#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) \
	static CEntityFactory<DLLClassName> mapClassName( #mapClassName );

class CEntity;

class IEntityFactory
{
public:
	virtual CEntity *Create(edict_t *pEdict, CBaseEntity *pEnt, bool addHooks) = 0;
};

template <class T>
class CEntityFactory : public IEntityFactory
{
public:
	CEntityFactory(const char *pClassName)
	{
		GetEntityManager()->LinkEntityToClass(this, pClassName);
	}

	CEntity *Create(edict_t *pEdict, CBaseEntity *pEnt, bool addHooks)
	{
		if (!pEdict || !pEnt)
		{
			return NULL;
		}

		T* pOurEnt = new T();
		pOurEnt->Init(pEdict, pEnt, addHooks);
		return pOurEnt;
	}
};

class IEntityFactoryDictionary
{
public:
	virtual void InstallFactory( IEntityFactory *pFactory, const char *pClassName ) = 0;
	virtual IServerNetworkable *Create( const char *pClassName ) = 0;
	virtual void Destroy( const char *pClassName, IServerNetworkable *pNetworkable ) = 0;
	virtual IEntityFactory *FindFactory( const char *pClassName ) = 0;
	virtual const char *GetCannonicalName( const char *pClassName ) = 0;
};

#endif // _INCLUDE_IENTITYFACTORY_H_

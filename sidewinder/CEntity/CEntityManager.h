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

#ifndef _INCLUDE_CENTITYMANAGER_H_
#define _INCLUDE_CENTITYMANAGER_H_

#include "sm_trie_tpl.h"
#include "extension.h"

class IEntityFactoryDictionary;
class IEntityFactory;

#define RECONFIGURE_HOOK(name) \
	int name##offset; \
	pConfig->GetOffset(#name, &name##offset); \
	SH_MANUALHOOK_RECONFIGURE(name, name##offset, 0, 0);

class CEntityManager
{
public:
	CEntityManager();
	bool Init(IGameConfig *pConfig);
	void Shutdown();
	void LinkEntityToClass(IEntityFactory *pFactory, const char *className);

	virtual IServerNetworkable *Create(const char *pClassName);
	void RemoveEdict(edict_t *e);
	//virtual IEntityFactory *FindFactory(const char *pClassName);
private:
	KTrie<IEntityFactory *> pFactoryTrie;
	KTrie<bool> pHookedTrie;
	IEntityFactoryDictionary *pDict;
	bool m_bEnabled;
};

CEntityManager *GetEntityManager();

#endif // _INCLUDE_CENTITYMANAGER_H_

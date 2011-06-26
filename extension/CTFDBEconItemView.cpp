/**
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2010 Matt Woodrow.  All rights reserved.
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

#include "CTFDBEconItemView.h"

DECLARE_SIGOFFSET(GetStaticData);
DECLARE_SIGOFFSET(GetLoadoutSlot);

class CTFItemDefinition;

#ifndef WIN32
typedef CTFItemDefinition *(* GetStaticDataFunc)(CEconItemView *);
#else
typedef CTFItemDefinition *(__fastcall * GetStaticDataFunc)(CEconItemView *, void *);
#endif

#ifndef WIN32
typedef int (* GetLoadoutSlotFunc)(CTFItemDefinition *, int);
#else
typedef int (__fastcall * GetLoadoutSlotFunc)(CTFItemDefinition *, void *, int);
#endif

int CTFDBEconItemView::GetLoadoutSlot(int playerClass) {
	if (GET_SIGOFFSET(GetLoadoutSlot) == NULL || GET_SIGOFFSET(GetLoadoutSlot) == NULL)
		return -1;

#ifndef WIN32
	CTFItemDefinition *pItemDefinition = ((GetStaticDataFunc)GET_SIGOFFSET(GetStaticData))(this);
#else
	CTFItemDefinition *pItemDefinition = ((GetStaticDataFunc)GET_SIGOFFSET(GetStaticData))(this, NULL);
#endif

#ifndef WIN32
	return ((GetLoadoutSlotFunc)GET_SIGOFFSET(GetLoadoutSlot))(pItemDefinition, playerClass);
#else
	return ((GetLoadoutSlotFunc)GET_SIGOFFSET(GetLoadoutSlot))(pItemDefinition, NULL, playerClass);
#endif
}

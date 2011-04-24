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

DECLARE_SIGOFFSET(GetLoadoutSlot);

#ifndef WIN32
typedef int (* GetLoadoutSlotFunc)(CEconItemView *, int m_iClass);
#else
typedef int (__fastcall * GetLoadoutSlotFunc)(CEconItemView *, void *, int m_iClass);
#endif

int CTFDBEconItemView::GetLoadoutSlot(int playerClass) {
	if (!GET_SIGOFFSET(GetLoadoutSlot))
		return -1;

#ifndef WIN32
	return ((GetLoadoutSlotFunc)GET_SIGOFFSET(GetLoadoutSlot))(this, playerClass);
#else
	return ((GetLoadoutSlotFunc)GET_SIGOFFSET(GetLoadoutSlot))(this, NULL, playerClass);
#endif
}

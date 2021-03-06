/**
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2011 Matt Woodrow.  All rights reserved.
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

#include "CTFWeaponBase.h"

LINK_ENTITY_TO_INTERNAL_CLASS(CTFWeaponBase, CTFWeaponBase);

SH_DECL_MANUALHOOK5(DeflectPlayer, 0, 0, 0, bool, CBaseEntity *, CBaseEntity *, Vector &, Vector &, Vector &);

DECLARE_HOOK(DeflectPlayer, CTFWeaponBase);

DECLARE_DEFAULTHANDLER(CTFWeaponBase, DeflectPlayer, bool, (CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize), (pTarget, pOwner, vecForward, vecCenter, vecSize));

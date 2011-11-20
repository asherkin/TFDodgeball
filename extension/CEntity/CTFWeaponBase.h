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

#ifndef _INCLUDE_CTFWeaponBase_H_
#define _INCLUDE_CTFWeaponBase_H_

#include "CEntity.h"
#include "CAnimating.h"
#include "CPlayer.h"

class CTFWeaponBase : public CAnimating
{
public:
	DECLARE_CLASS(CTFWeaponBase, CAnimating);

	virtual bool DeflectPlayer(CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize);

public:
	DECLARE_DEFAULTHEADER(DeflectPlayer, bool, (CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize));
};

#endif // _INCLUDE_CTFWeaponBase_H_

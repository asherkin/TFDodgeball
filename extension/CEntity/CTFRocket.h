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

#ifndef _INCLUDE_CTFRocket_H_
#define _INCLUDE_CTFRocket_H_

#include "CEntity.h"
#include "CAnimating.h"
#include "CPlayer.h"

class CTFRocket : public CAnimating
{
public:
	DECLARE_CLASS(CTFRocket, CAnimating);

	virtual float GetRadius();

public:
	DECLARE_DEFAULTHEADER(GetRadius, float, ());
};

#endif // _INCLUDE_CTFRocket_H_

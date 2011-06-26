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

#include "CEntity.h"

#ifndef CScriptCreatedItem_h__
#define CScriptCreatedItem_h__

// Taken from the TF2Items extension by Asher "asherkin" Baker

template< class T, class I = int >
class CUtlMemoryTF2Items : public CUtlMemory< T, I >
{
public:
	CUtlMemoryTF2Items( int nGrowSize = 0, int nInitSize = 0 ) { CUtlMemory< T, I >( nGrowSize, nInitSize ); }
    CUtlMemoryTF2Items( T* pMemory, int numElements ) { CUtlMemory< T, I >( pMemory, numElements ); }
    CUtlMemoryTF2Items( const T* pMemory, int numElements ) { CUtlMemory< T, I >( pMemory, numElements ); }
    
	void Purge()
	{
		if ( !CUtlMemory< T, I >::IsExternallyAllocated() )
		{
			if (CUtlMemory< T, I >::m_pMemory)
			{
				UTLMEMORY_TRACK_FREE();
				//free( (void*)m_pMemory );
				CUtlMemory< T, I >::m_pMemory = 0;
			}
			CUtlMemory< T, I >::m_nAllocationCount = 0;
		}
	}
};

class CEconItemAttribute
{
public:
	CEconItemAttribute() {};

	CEconItemAttribute(int iAttributeDefinitionIndex, float flValue)
	{
		this->m_iAttributeDefinitionIndex = iAttributeDefinitionIndex;
		this->m_flValue = flValue;
	}

public:
	void *m_pVTable;

	uint16 m_iAttributeDefinitionIndex;
	float m_flValue;
	wchar_t m_wszDescription[96];
};

class CEconItemView
{
public:
	void *m_pVTable;

	uint16 m_iItemDefinitionIndex;
	uint16 m_Padding;
	uint32 m_iEntityQuality;
	uint32 m_iEntityLevel;

	uint64 m_iItemID;
	uint32 m_iItemIDHigh;
	uint32 m_iItemIDLow;
	uint32 m_iAccountID;
	uint32 m_iInventoryPosition;

	wchar_t m_wszItemName[128];
	char m_szItemName[128];

	wchar_t m_wszAttributeDescription[1536];
	char m_vecAttributeColors[20];

	void *m_pAlternateItemData;
	void *m_pLocalizationProvider;

	void *m_pVTable_Attributes;
	CUtlVector<CEconItemAttribute, CUtlMemoryTF2Items<CEconItemAttribute> > m_Attributes;

	bool m_bInitialized;
};
#endif // CScriptCreatedItem_h__

#ifndef _INCLUDE_CANIMATING_H_
#define _INCLUDE_CANIMATING_H_

#include "CEntity.h"

class CAnimating : public CEntity
{
public:
	DECLARE_CLASS(CAnimating, CEntity);
	DECLARE_DATADESC();
	//virtual bool IsActivityFinished( void ) { return m_bSequenceFinished; }
	inline bool IsSequenceFinished( void ) { return m_bSequenceFinished; }
	virtual void StudioFrameAdvance();

public:
	DECLARE_DEFAULTHEADER(StudioFrameAdvance, void, ());

protected:
	DECLARE_DATAMAP(bool, m_bSequenceFinished);
};

#endif // _INCLUDE_CANIMATING_H_
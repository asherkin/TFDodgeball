#include "CAnimating.h"

LINK_ENTITY_TO_CLASS(CBaseAnimating, CAnimating);

IMPLEMENT_NULL_DATADESC(CAnimating);

DEFINE_PROP(m_bSequenceFinished, CAnimating);

SH_DECL_MANUALHOOK0_void(StudioFrameAdvance, 0, 0, 0);
DECLARE_HOOK(StudioFrameAdvance, CAnimating);

DECLARE_DEFAULTHANDLER_void(CAnimating, StudioFrameAdvance, (), ());

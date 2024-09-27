
#include "../main.h"

CObjectPool::CObjectPool()
{
	for(WORD wObjectID = 0; wObjectID < MAX_OBJECTS; wObjectID++)
	{
		m_bObjectSlotState[wObjectID] = FALSE;
		m_pObjects[wObjectID] = NULL;
	}
	field_0 = 0;
};

void CObjectPool::UpdatePoolSize()
{
	int iNewSize = 0;
}
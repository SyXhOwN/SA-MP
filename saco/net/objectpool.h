
#pragma once

class CObjectPool
{
private:
	int field_0;
	BOOL		m_bObjectSlotState[MAX_OBJECTS];
	CObject		*m_pObjects[MAX_OBJECTS];

public:
	CObjectPool();

	void UpdatePoolSize();
};

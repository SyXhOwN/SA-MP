
#include "../main.h"

extern CChatWindow *pChatWindow;

//----------------------------------------------------

CActorPool::CActorPool()
{
	for(ACTORID ActorID = 0; ActorID < MAX_ACTORS; ActorID++) {
		m_bActorSlotState[ActorID] = FALSE;
		m_pActors[ActorID] = NULL;
		m_pGTAPed[ActorID] = NULL;
	}
	field_0 = 0;
}

//----------------------------------------------------

CActorPool::~CActorPool()
{
	for(ACTORID ActorID = 0; ActorID < MAX_ACTORS; ActorID++) {
		Delete(ActorID);
	}
}

//----------------------------------------------------

BOOL CActorPool::New(NEW_ACTOR *pNewActor)
{
	if(m_pActors[pNewActor->ActorID] != NULL) {
		pChatWindow->AddDebugMessage("Warning: actor %u was not deleted", pNewActor->ActorID);
		Delete(pNewActor->ActorID);
	}

	m_pActors[pNewActor->ActorID] = new CActorPed(pNewActor->iSkin,
		pNewActor->vecPos.X,pNewActor->vecPos.Y,pNewActor->vecPos.Z,
		pNewActor->fRotation);

	if(m_pActors[pNewActor->ActorID])
	{
		m_pGTAPed[pNewActor->ActorID] = m_pActors[pNewActor->ActorID]->GetGtaActor();
		m_bActorSlotState[pNewActor->ActorID] = TRUE;
		m_pActors[pNewActor->ActorID]->SetHealth(pNewActor->fHealth);
		if(pNewActor->byteImmunity)
			m_pActors[pNewActor->ActorID]->ToggleImmunity(1);
		else
			m_pActors[pNewActor->ActorID]->ToggleImmunity(0);
		field_2EE4[pNewActor->ActorID] = 1;
		field_3E84[pNewActor->ActorID] = 0;

		RecalcPoolSize();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CActorPool::Delete(ACTORID ActorID)
{
	if(!GetSlotState(ActorID) || !m_pActors[ActorID])
	{
		return FALSE;
	}

	m_bActorSlotState[ActorID] = FALSE;
	delete m_pActors[ActorID];
	m_pActors[ActorID] = NULL;
	m_pGTAPed[ActorID] = NULL;

	RecalcPoolSize();

	return TRUE;
}

//----------------------------------------------------

void CActorPool::RecalcPoolSize()
{
	int iNewIndex = 0;
	int i = 0;

	do
	{
		if(m_bActorSlotState[i])
			iNewIndex = i;
		if(m_bActorSlotState[i+1])
			iNewIndex = i+1;
		if(m_bActorSlotState[i+2])
			iNewIndex = i+2;
		if(m_bActorSlotState[i+3])
			iNewIndex = i+3;
		if(m_bActorSlotState[i+4])
			iNewIndex = i+4;
		i += 5;
	}
	while(i < MAX_ACTORS);

	field_0 = iNewIndex;
}

//----------------------------------------------------

ACTORID CActorPool::FindIDFromGtaPtr(PED_TYPE *pGtaPed)
{
	int x=0;

	while(x<=field_0) {
		if(pGtaPed == m_pGTAPed[x]) return x;
		x++;
	}

	return 0xFFFF;
}

//----------------------------------------------------

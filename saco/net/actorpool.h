
#pragma once

#pragma pack(1)
typedef struct _NEW_ACTOR {
	ACTORID ActorID;
	int iSkin;
	VECTOR vecPos;
	float fRotation;
	float fHealth;
	BYTE byteImmunity;
} NEW_ACTOR;

//----------------------------------------------------

class CActorPool
{
private:
	int					field_0;
	CActorPed			*m_pActors[MAX_ACTORS];
	BOOL				m_bActorSlotState[MAX_ACTORS];
	PED_TYPE			*m_pGTAPed[MAX_ACTORS]; // pointers to actual ingame actors.
	int					field_2EE4[MAX_ACTORS];
	int					field_3E84[MAX_ACTORS];

	void RecalcPoolSize();

public:

	CActorPool();
	~CActorPool();

	BOOL New(NEW_ACTOR *pNewActor);
	BOOL Delete(ACTORID ActorID);

	// Find out if the slot is inuse.
	BOOL GetSlotState(ACTORID ActorID) {
		if(ActorID >= MAX_ACTORS) { return FALSE; }
		return m_bActorSlotState[ActorID];
	};

	ACTORID FindIDFromGtaPtr(PED_TYPE *pGtaPed);
};

//----------------------------------------------------

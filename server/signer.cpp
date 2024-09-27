
#include "signer.h"
#include "stdio.h"
#include "memory.h"

//----------------------------------------------

void CSigner::SetPublicKey(unsigned char* pbKey)
{
	memcpy(&m_e, pbKey, sizeof(m_e));
	memcpy(m_n, pbKey+sizeof(m_e), sizeof(m_n));
}

//----------------------------------------------

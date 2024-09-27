
#ifndef _SIGNER_H_INCLUDED
#define _SIGNER_H_INCLUDED

#include "../raknet/RSACrypt.h"

#define SIGNER_RSA_BIT_SIZE big::u1024

class CSigner
{
private:

	big::u32 m_e;
	SIGNER_RSA_BIT_SIZE m_n;

public:
	void SetPublicKey(unsigned char* pbKey);
};

#endif

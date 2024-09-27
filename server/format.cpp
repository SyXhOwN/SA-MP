
#include "main.h"
#include "amx/amx.h"

#include "format.h"

template size_t atcprintf<char, cell>(char *, size_t, const cell *, AMX *, cell *, int *);

void AddInt(int val)
{
	int		signedVal;
	int		neg;

	signedVal = val;
	neg = 0;

	if (val < 0)
	{
		neg = 1;
		signedVal = -val;
	}
}

template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, cell *params, int *param)
{
	int		arg;
	int		args = params[0] / sizeof(cell);
	int		llen = maxlen;

	arg = *param;

	return 0;
}

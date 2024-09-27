
template<typename T>
class SimpleArray
{
public:
	SimpleArray() : m_pData(NULL), m_nSize(0) {}
	~SimpleArray() { SetSize(0); }

	bool SetSize(unsigned int nNewSize);

	int Insert(T value);
	bool Remove(unsigned int nIndex);

	// Member variables
	T*				m_pData;
	unsigned int	m_nSize;
};

template<typename T>
bool SimpleArray<T>::SetSize(unsigned int nNewSize)
{
	if(nNewSize == 0)
	{
		if(m_pData)
		{
			free(m_pData);
			m_pData = NULL;
			m_nSize = 0;
		}
		else
			m_nSize = 0;

		return true;
	}

	T* pData = (T*)realloc(m_pData, sizeof(T) * nNewSize);
	if(pData)
	{
		m_pData = pData;

		if(m_nSize < nNewSize)
		{
			unsigned int nIndex = m_nSize;
			do
			{
				m_pData[nIndex] = NULL;
				nIndex++;
			}
			while(nIndex != nNewSize);
		}

		m_nSize = nNewSize;

		return true;
	}

	return false;
}

template<typename T>
int SimpleArray<T>::Insert(T value)
{
	int nSize = (int)m_nSize;
	int nIndex;

	for(nIndex = 0; nIndex != nSize; nIndex++)
	{
		if(m_pData[nIndex] == NULL)
			break;
	}

	if(nIndex >= 0)
	{
		m_pData[nIndex] = value;
		return nIndex;
	}

	if(SetSize(nSize + 1))
	{
		m_pData[nSize] = value;
		return nSize;
	}
	return -1;
}

template<typename T>
bool SimpleArray<T>::Remove(unsigned int nIndex)
{
	int nCurrentSize = m_nSize;
	int nIndex = 0;
	int nNewSiye = -1;

	if(nCurrentSize == 0)
	{
		SetSize(0);
		return;
	}

	while(nIndex != nCurrentSize)
	{
		if(m_pData[nIndex])
		{
			nNewSize = nIndex
		}
		nIndex++;
	}

	if(nNewSize != -1)
		SetSize(nNewSize + 1);
	else
		SetSize(0);
}


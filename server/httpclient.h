
#define HTTP_GET			1
#define HTTP_POST			2
#define HTTP_HEAD			3

//----------------------------------------------------

#define HTTP_ERROR_BAD_HOST				1
#define HTTP_ERROR_CANT_WRITE			4

#pragma pack(1)
typedef struct{
	unsigned short port;	/* remote port */
	int		rtype;			/* request type */
	char	host[256];		/* hostname */
	char _gap106;
	char	file[4096];		/* GET/POST request file */
	char _gap1107;
	char	data[8192];		/* POST data (if rtype HTTP_POST) */
	char _gap3108;
	char	referer[256];	/* http referer. */
	char _gap3209[16385];
} HTTP_REQUEST;

//----------------------------------------------------

class CHttpClient
{
private:

	int				m_iSocket;
	int				m_iError;
	HTTP_REQUEST	m_Request;

	bool Connect(char *szHost, int iPort);
	void CloseConnection();
	bool Send(char *szData);
	int  Recv(char *szBuffer, int iBufferSize);

	void InitRequest(int iType, char *szURL, char *szPostData, char *szReferer);

	void Process();

public:

	int ProcessURL(int iType, char *szURL, char *szData, char *szReferer);

	CHttpClient(char *a2);
};

//----------------------------------------------------

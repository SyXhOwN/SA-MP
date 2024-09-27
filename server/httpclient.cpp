
#ifdef WIN32
# include <windows.h>
#else
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <netinet/in.h>
# include <netdb.h>
#endif

#include "httpclient.h"

//----------------------------------------------------

CHttpClient::CHttpClient(char *a2)
{
	// TODO: CHttpClient::CHttpClient()

// Winsock init
#ifdef WIN32
	WORD				wVersionRequested;
	WSADATA				wsaData;
	wVersionRequested = MAKEWORD(2,2);
	WSAStartup(wVersionRequested,&wsaData);
#endif
}

//----------------------------------------------------


//----------------------------------------------------

int CHttpClient::ProcessURL(int iType, char *szURL, char *szPostData, char *szReferer)
{
	InitRequest(iType,szURL,szPostData,szReferer);

	Process();

	return m_iError;
}

//----------------------------------------------------


//----------------------------------------------------

bool CHttpClient::Connect(char *szHost, int iPort)
{
	struct sockaddr_in	sa;
	struct hostent		*hp;

	// Hostname translation
	if((hp=(struct hostent *)gethostbyname(szHost)) == NULL ) {
		m_iError=HTTP_ERROR_BAD_HOST;
		return false;
	}

	// Prepare a socket	
	memset(&sa,0,sizeof(sa));

	memcpy(&sa.sin_addr,hp->h_addr,hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((unsigned short)iPort);

	// TODO: CHttpClient::Connect
}

//----------------------------------------------------

void CHttpClient::CloseConnection()
{
#ifdef WIN32
	closesocket(m_iSocket);
#else
	close(m_iSocket);
#endif
}

//----------------------------------------------------

bool CHttpClient::Send(char *szData)
{
	if(send(m_iSocket,szData,strlen(szData),0) < 0) {
		m_iError = HTTP_ERROR_CANT_WRITE;
		return false;
	}
	return true;
}

//----------------------------------------------------

int CHttpClient::Recv(char *szBuffer, int iBufferSize)
{
	return recv(m_iSocket,szBuffer,iBufferSize,0);
}

//----------------------------------------------------

void CHttpClient::InitRequest(int iType, char *szURL, char *szPostData, char *szReferer)
{
	char		 port[129];		// port string
	char		 *port_char;	// position of ':' if any
	unsigned int slash_pos;		// position of first '/' numeric
	char		 *slash_ptr;
	char		 szUseURL[2049]={0}; // incase we have to cat something to it.

	memset(szUseURL,0,sizeof(szUseURL));
	strncpy(szUseURL,szURL,sizeof(szUseURL)-1);

	m_Request.rtype = iType;

	strncpy(m_Request.referer,szReferer,256);

	if(iType==HTTP_POST) {
		strncpy(m_Request.data,szPostData,8192);
	}

	// Copy hostname from URL
	slash_ptr = strchr(szUseURL,'/');

	if(!slash_ptr) {
		strcat(szUseURL,"/");
		slash_ptr = strchr(szUseURL,'/');
	}

	slash_pos=(slash_ptr-szUseURL);
	if(slash_pos > 256) slash_pos=256;
	memcpy(m_Request.host,szUseURL,slash_pos);
	m_Request.host[slash_pos]='\0';

	// Copy the rest of the url to the file string.
	strncpy(m_Request.file,strchr(szUseURL,'/'),4096);

	// Any special port used in the URL?
	if((port_char=strchr(m_Request.host,':'))!=NULL) {
		memset(port,0,sizeof(port));
		strncpy(port,port_char+1,128);
		*port_char='\0';
		m_Request.port = atoi(port);
	}
	else {
		m_Request.port = 80;
	}
}

//----------------------------------------------------

void CHttpClient::Process()
{
	// TODO: CHttpClient::Process
}



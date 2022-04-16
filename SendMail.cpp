#using <mscorlib.dll>
#using <System.dll> 
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Net;
using System::Net::Sockets::NetworkStream;
using System::Net::Sockets::TcpClient;
using System::IO::StreamReader;

#include "sendemail.h"

CSendEmail::CSendEmail(String * Server)
{
	m_sServer = Server;
}

bool CSendEmail::SendEmail( String * From, 
						    String * To, 
							String * Subject,
							String * Body)
{
	NetworkStream  * pNsEmail;
	StreamReader   * RdStrm;
	String		   * Data;
	unsigned char  sendbytes __gc[];
	
		
	TcpClient *pServer = new TcpClient(m_sServer,25);

	try
	{
		pNsEmail = pServer->GetStream();
		RdStrm = new StreamReader(pServer->GetStream());
	}

	catch (Exception  * e )
	{
		Console::WriteLine(e->ToString());
		Console::WriteLine("Socket Close by the server");
					
		pServer->Close();
		sendbytes = 0;
		return false;
	}

	Console::WriteLine(RdStrm->ReadLine());

	Data  = "HELO server \r\n";	
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());
		
	Data = String::Concat(S"MAIL FROM: ", S"<", From, S">", S"\r\n"); 	
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());
	
	Data = String::Concat(S"RCPT TO: ", S"<", To, S">", S"\r\n");	
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());

	Data = "DATA \r\n";
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());

	Data = String::Concat(S"FROM: ", From, S"\r\n", S"SUBJECT: ", Subject, S"\r\n", Body, S"\r\n.\r\n");
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());
	
	Data = "QUIT \r\n";
	sendbytes = ConvertToChar(Data);	
	pNsEmail->Write(sendbytes, 0, sendbytes->get_Length());
	sendbytes = 0;
	Data = 0;
	Console::WriteLine(RdStrm->ReadLine());

	pNsEmail->Close();
	RdStrm->Close();
	pServer->Close();

}

unsigned char CSendEmail::ConvertToChar ( String * str) __gc[]
{
	IntPtr	pPtr;
	int len = 0;
	unsigned char  sendbytes __gc[];

	len = str->get_Length();
	sendbytes = new unsigned char __gc[len];
	pPtr= Marshal::StringToHGlobalAnsi(str).ToPointer();	
	Marshal::Copy(pPtr, sendbytes, 0,len);
	Marshal::FreeHGlobal(pPtr); 

	return (sendbytes);
}
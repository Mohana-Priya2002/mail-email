#pragma once

__gc class CSendEmail
{
public:
	CSendEmail(String * Server);
	bool SendEmail( String * From, 
					String * To, 
					String * Subject,
					String * Body);

private:
	String * m_sServer;

	unsigned char ConvertToChar ( String * str) __gc[];
};


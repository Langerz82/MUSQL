// Query.h: interface for the CQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUERY_H__8A3116E5_E735_4FF9_A61D_5566130C3610__INCLUDED_)
#define AFX_QUERY_H__8A3116E5_E735_4FF9_A61D_5566130C3610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Common.h"
#include "Database/DatabaseEnv.h"

class CQuery  
{
public:

	CQuery();
	virtual ~CQuery();

	BOOL Connect(LPTSTR lpszDNS, LPSTR lpszPort, LPTSTR lpszUser, LPTSTR lpszPassword, LPSTR lpszServerName);
	BOOL ReConnect();
	void Disconnect();

	BOOL ExecQuery(TCHAR* lpszStatement, ...);
	BOOL Execute(TCHAR* lpszStatement);
	QueryResult* Fetch(TCHAR* lpszStatement, ...);
	bool HasFields();

	int GetAsBinary(LPSTR lpszStatement, LPBYTE OUT lpszReturnBuffer, int size);
	void SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, UINT32 BinaryBufferSize);

	int GetResult(int iIndex);
	
	//void GetAsString(LPTSTR ColName, LPTSTR pOutBuffer, int size);
	//DWORD GetAsInteger(LPTSTR ColName);
	//INT64 GetAsInteger64(LPTSTR ColName);
	//float GetAsFloat(LPTSTR ColName);

	//int FindIndex(LPTSTR ColName);
	void GetAsString(int iIndex, LPTSTR pOutBuffer, int size);
	DWORD GetAsInteger(int iIndex);
	INT64 GetAsInteger64(int iIndex);
	float GetAsFloat(int iIndex);

	void Close();
	void Diagnosis(bool &bReconnect);

	TCHAR m_szDNS[64];
	TCHAR m_szPort[8];
	TCHAR m_szUser[64];
	TCHAR m_szPassword[64];
	TCHAR m_szDatabase[64];

	DatabaseType m_Database;
	QueryResult* m_Result;
	Field* m_Fields;

};

#endif // !defined(AFX_QUERY_H__8A3116E5_E735_4FF9_A61D_5566130C3610__INCLUDED_)


////////////////////////////////////////////////////////////////////////////////
//  vnDev.Games - MuServer S12EP2 IGC v12.0.1.0 - Trong.LIVE - DAO VAN TRONG  //
////////////////////////////////////////////////////////////////////////////////


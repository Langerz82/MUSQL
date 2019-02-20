#ifndef _CUSTTYPEDEF_H
#define _CUSTTYPEDEF_H

typedef int BOOL;
typedef wchar_t WCHAR;

typedef unsigned char BYTE, *LPBYTE, UCHAR;
typedef unsigned short USHORT, WORD;
typedef const WCHAR* LPCWSTR;
typedef unsigned long ULONG;
typedef void* HANDLE;
typedef unsigned long long UINT64, ULONGLONG;
typedef char* LPSTR;
typedef char CHAR;

typedef long long LONGLONG;


typedef unsigned long long SOCKET;

typedef unsigned int DWORD;



#ifdef WIN32

typedef long long time_t;


#else

typedef unsigned int COLORREF;
typedef char TCHAR;


#endif


#endif

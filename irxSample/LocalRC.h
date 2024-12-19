#pragma once
// LocalRC.h: interface for the CLocalRC class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
class CLocalRC
{
protected:
	static UINT    m_AppIconResID;
	static HMODULE m_hMyResource;
	static CList<HINSTANCE, HINSTANCE> m_HMSaveStack;
public:
	static void SetRCHandleST(HINSTANCE hInstance);
	static void SetRCHandleST(AFX_EXTENSION_MODULE* pExtModule);
	static void TempResetLocalRC_ST();
	static void TempRestoreLocalRC_ST();
	static HINSTANCE GetRCHandle();
	static void SetAppIconResID(UINT AppIconResID);
	static UINT GetAppIconResID();

	static void PushResHandle();
	static void PopResHandle();

	CLocalRC();
	~CLocalRC();
};
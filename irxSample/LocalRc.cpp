// LocalRC.cpp: implementation of the CLocalRC class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "LocalRC.h"

UINT    CLocalRC::m_AppIconResID = 0;
HMODULE CLocalRC::m_hMyResource = NULL;
CList<HINSTANCE, HINSTANCE>CLocalRC::m_HMSaveStack;

void CLocalRC::SetRCHandleST(HINSTANCE hInstance)
{
	m_hMyResource = hInstance;
}

void CLocalRC::SetRCHandleST(AFX_EXTENSION_MODULE* pExtModule)
{
	m_hMyResource = pExtModule->hResource;

	//--------------------------------------------------------------------
	// MFC�� �������� Resource ���� Chain ������ ���������� ���´�. 
	// Extension Dll�� Resource Chain�� ���� Extension Dll�� Resource
	// �浹 ������ �����Ѵ�. Extension DLL�� �������� Loading�������� 
	// �������� ������ ���´�. 
	pExtModule->hResource = NULL;
}

//----------------------------------------------------------------------
// ���缳���� Resource Handle�� Local Resource ���� �������� �����Ѵ�.
// EX) Local Resource���� ������ Dialog Box�� Modal�� �����ǰ�.
//     Modal dialog���� �ٸ� Resource�� �䱸�ϴ� ó���� �߻��Ҷ�....
//     ���������� Local Resourceó���� ���ϴ� ���¿����� ���ʿ� ..
//     Local Resourceó���� �̷������ �ִ� DLL Module���� 
//     MFC Resource Chain�� ����ϴ� ������ ȣ�⿡�� �߻��ϴ� 
//     Resource �浹 ������ �ؼ��ϱ� ���� ó��..
//     Old Resource Stack�� ������ ���� �ʴ´�. 
void CLocalRC::TempResetLocalRC_ST()
{
	INT_PTR nHM = m_HMSaveStack.GetCount();
	if (nHM > 0)
	{
		//-----------------------------------------------------------
		//AfxSetResourceHandle(m_HMSaveStack.GetHead());
		//Current Resource Handle�� NULL�� �����Ǹ� MFC�� 
		//CDynLinkLibrary Chain�� Search�ؼ� �䱸�� Resource�� 
		//ã�´�.  AfxFindResourceHandle Routine�� �����Ұ�. 
		AfxSetResourceHandle(NULL);
	}
}

void CLocalRC::TempRestoreLocalRC_ST()
{
	AfxSetResourceHandle(m_hMyResource);
}

CLocalRC::CLocalRC()
{
	// �̷��� ���������� ���� ���ٰ� �ǴܵǾ� �����ڿ� �̷��� �س��Ҵ�.
	// Ȥ�� �̷����� ������ �ʿ��ҋ� ��Ʈ�忡�� �����ٰ�!
	//ASSERT(!"�� ������ ����ִ� ���� ��ü�� Resource �� �� irx���� ��Ե�. ��뿡 ����.");
	PushResHandle();
}

CLocalRC::~CLocalRC()
{
	PopResHandle();
}

HINSTANCE CLocalRC::GetRCHandle()
{
	return m_hMyResource;
}

void CLocalRC::SetAppIconResID(UINT AppIconResID)
{
	CLocalRC::m_AppIconResID = AppIconResID;
}

UINT CLocalRC::GetAppIconResID()
{
	return CLocalRC::m_AppIconResID;
}

void CLocalRC::PushResHandle()
{
	if (NULL != m_hMyResource)
	{
		HINSTANCE CurRCH = AfxGetResourceHandle();
		m_HMSaveStack.AddTail(CurRCH);
		AfxSetResourceHandle(m_hMyResource);
	}
}

void CLocalRC::PopResHandle()
{
	INT_PTR nHM = m_HMSaveStack.GetCount();
	if (nHM > 0)
	{
		AfxSetResourceHandle(m_HMSaveStack.GetTail());
		m_HMSaveStack.RemoveTail();
	}
}

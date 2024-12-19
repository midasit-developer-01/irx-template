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
	// MFC의 내부적인 Resource 연결 Chain 생성을 인위적으로 막는다. 
	// Extension Dll의 Resource Chain에 의한 Extension Dll간 Resource
	// 충돌 문제를 방지한다. Extension DLL을 동적으로 Loading했을때의 
	// 잠재적인 문제를 막는다. 
	pExtModule->hResource = NULL;
}

//----------------------------------------------------------------------
// 현재설정된 Resource Handle을 Local Resource 설정 이전으로 복구한다.
// EX) Local Resource에서 생성된 Dialog Box가 Modal로 생성되고.
//     Modal dialog에서 다른 Resource를 요구하는 처리가 발생할때....
//     정상적으로 Local Resource처리를 행하는 상태에서는 불필요 ..
//     Local Resource처리가 이루어지고 있는 DLL Module에서 
//     MFC Resource Chain을 사용하는 모듈로의 호출에서 발생하는 
//     Resource 충돌 문제를 해소하기 위한 처리..
//     Old Resource Stack에 영향을 주지 않는다. 
void CLocalRC::TempResetLocalRC_ST()
{
	INT_PTR nHM = m_HMSaveStack.GetCount();
	if (nHM > 0)
	{
		//-----------------------------------------------------------
		//AfxSetResourceHandle(m_HMSaveStack.GetHead());
		//Current Resource Handle이 NULL로 설정되면 MFC는 
		//CDynLinkLibrary Chain을 Search해서 요구된 Resource를 
		//찾는다.  AfxFindResourceHandle Routine을 참조할것. 
		AfxSetResourceHandle(NULL);
	}
}

void CLocalRC::TempRestoreLocalRC_ST()
{
	AfxSetResourceHandle(m_hMyResource);
}

CLocalRC::CLocalRC()
{
	// 이렇게 생성할일이 거의 없다고 판단되어 생성자에 이렇게 해놓았다.
	// 혹시 이런식의 도구가 필요할떄 파트장에게 말해줄것!
	//ASSERT(!"이 변수가 살아있는 동안 본체도 Resource 를 본 irx에서 얻게됨. 사용에 주의.");
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

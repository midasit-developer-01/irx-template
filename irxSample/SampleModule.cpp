#include "pch.h"
#include "SampleModule.h"

#include "RxObjectImpl.h"
#include "Ed/EdCommandStack.h"

#include "SampleCmd.h"
#include "ipRxToolbar.h"
#include "ipRxMenu.h"
#include "./PNGUtil.h"
#include "Resource.h"
#include "LocalRc.h"
#include "StringArray.h"

#define MENU_DWGPARAMETERIZER 100

ODRX_DEFINE_DYNAMIC_MODULE(CSampleModule);
MapMenu				g_MenuData;							// 쌓아놓은 메뉴 정보
OdStringArray		g_MenuSortIndex[2];	// 메뉴 정렬을 위함.
std::map<int, int>	g_MenuIndex;						// menu to index
ip::ipRxMenu		g_MenuItems;

bool CopyGifToToolTipFolder(OdString& name, int nResourceId)
{
	CString resName = (const TCHAR*)name.c_str();
	name = L"";
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	CString strName;
	GetModuleFileName(hInst, strName.GetBuffer(MAX_PATH), MAX_PATH);
	strName.ReleaseBuffer();
	int nPos = strName.ReverseFind('\\');
	CString strDir = strName.Left(nPos + 1) + L"ToolTips\\";

	if (_waccess_s(strDir, 0) != 0)
	{
		CreateDirectory(strDir, 0);
	}

	resName = strDir + resName;


	if (_waccess_s(resName, 0) == 0)
	{
		if (!DeleteFile(resName)) return false;
	}

	CLocalRC rc;
	HRSRC hRes = ::FindResource(rc.GetRCHandle(), MAKEINTRESOURCE(nResourceId), _T("GIF"));
	if (NULL == hRes) return false;
	DWORD dwResourceSize = ::SizeofResource(rc.GetRCHandle(), hRes);
	if (0 == dwResourceSize) return false;
	LPVOID pData = ::LockResource(::LoadResource(rc.GetRCHandle(), hRes));
	if (NULL == pData) return false;

	try
	{
		CFile file(resName,
			CFile::modeCreate | CFile::modeWrite |
			CFile::shareExclusive);
		file.Write(pData, dwResourceSize);
	}
	catch (CFileException *pEx)
	{
		pEx->Delete();
		return false;
	}
	name = resName;
	return true;
}

void AddExtensionCommand(OdString strMenu, OdString strDesc, OdString strCmd, int nIconIndex, bool bEnable, OdString strGif)
{
	T_MENU_DATA data;
	data.enVersion = 0;
	data.strName = strMenu;
	data.strDesc = strDesc;
	data.strCmd.format(_T("^C^C^C%s"), strCmd.c_str());
	data.nIconIndex = nIconIndex;
	data.bEnable = bEnable;
	data.strGif = strGif;

	if (g_MenuData.find(strMenu) != g_MenuData.end())
	{
		// 이미 등록도니 메뉴일 때.. 실수로 이름을 중복으로 사용하지 않았는지 확인합니다.
		//ASSERT_ONCE(0);
	}
	g_MenuData[strMenu] = data;
}

int AppendMenu(int enMenu, UINT uiFlag, int nImageIndex, /* uiFLag = MF_SEPARATOR, MF_POPUP, MF_STRING */
	OdString strName = L"", OdString strCmd = L"", OdString strDesc = L"", bool bEnable = true, OdString strGif = L"")
{
	UINT flag = bEnable ? uiFlag : uiFlag | MF_DISABLED;

	int nResult = g_MenuItems.append(enMenu, flag, strName, strCmd, strDesc, nImageIndex, strGif);

	return nResult;
}

void CSampleModule::initApp()
{
	HINSTANCE test = AfxGetResourceHandle();
	CLocalRC::SetRCHandleST(test);

	g_MenuIndex.clear();

	OdEdCommandStackPtr pCmd = ::odedRegCmds();

	// 커맨드 등록 및 데이터 세팅
	OdString strMenu;
	strMenu.format(GetProductName());
	AddCommand<CSampleCmd>(pCmd, strMenu);

	// 메뉴 이름 등록
	strMenu.format(GetProductName() + _T(" Menu"));
	g_MenuSortIndex[EN_MENU_TOP].push_back(strMenu);
	g_MenuSortIndex[EN_MENU_TOP].push_back(L"");

	strMenu.format(GetProductName() + _T(" 팝업"));
	g_MenuSortIndex[EN_MENU_TOP].push_back(strMenu);
	g_MenuSortIndex[EN_MENU_TOP].push_back(L"");

	strMenu.format(GetProductName()); // 커맨드 실행 == 같은 이름
	g_MenuSortIndex[EN_MENU_SETTINGS].push_back(strMenu);
	g_MenuSortIndex[EN_MENU_SETTINGS].push_back(L"");
	SetMenuData();
}

void CSampleModule::uninitApp()
{
	OdEdCommandStackPtr pCommands = ::odedRegCmds();
	pCommands->removeGroup(SAMPLE_GROUP);

	if (pCommands->lookupCmd(SAMPLE_GLOBAL, OdEdCommandStack::LookupFlags::kGlobal | OdEdCommandStack::LookupFlags::kLocal, SAMPLE_GROUP).isNull() == false)
		pCommands->removeCmd(SAMPLE_GROUP, SAMPLE_GLOBAL);

	g_MenuItems.unload(GetProductName());
}

void CSampleModule::SetMenuData()
{
	// 초기화
	g_MenuItems.reserve(g_MenuData.size());

	// 매뉴 등록
	int cnt = 0;
	for (int i = 0; i < 2; i++)
	{
		auto nMenu = i == 0 ? EN_MENU_TOP : EN_MENU_SETTINGS;
		OdStringArray& rSortedMenu = g_MenuSortIndex[i];

		for (OdString& strMenu : rSortedMenu)
		{
			if (strMenu.getLength() <= 0)
			{
				AppendMenu(nMenu, MF_SEPARATOR, -1);
				continue;
			}
			auto itrFind = g_MenuData.find(strMenu);
			if (itrFind == g_MenuData.end())
			{
				if (cnt < 1)AppendMenu(nMenu, MF_STRING, -1, strMenu); // 메뉴 바
				else AppendMenu(nMenu, MF_POPUP, -1, strMenu); // 팝업
				cnt++;
				continue;
			}
			const T_MENU_DATA& rData = itrFind->second;
			AppendMenu(nMenu, MF_STRING, rData.nIconIndex, rData.strName, rData.strCmd, rData.strDesc, rData.bEnable, rData.strGif); // 커맨드 실행 버튼
		}
	}

	g_MenuItems.load();
}

void AddCommandImpl(OdEdCommandStack* pCmds, OdEdCommand* pCmd, bool bEnable,
	const TCHAR* pName, const TCHAR* pDesc, int nIcon, int nGif)
{
	pCmds->addCommand(pCmd);
	OdString strCmd;
	strCmd.format((_T("^C^C^C%s"), pCmd->localName().c_str()));

	OdString strGif = L"";
	if (pName != nullptr)
	{
		OdString strDesc = pDesc;
		if (strDesc.getLength() <= 0) strDesc = pName;
		if (nGif != -1) // gif 파일 있을경우
		{
			strGif.format(_T("%s%s"), pCmd->globalName().c_str(), OD_T(".gif"));
			if (!CopyGifToToolTipFolder(strGif, nGif)) strGif = L"";
		}
		AddExtensionCommand(pName, strDesc, pCmd->localName(), nIcon, bEnable, strGif);
	}
}

CString CSampleModule::GetProductName()
{
	return _T("irxSample");
}

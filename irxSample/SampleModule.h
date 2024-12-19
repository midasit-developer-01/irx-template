#pragma once
#include "pch.h"
#include <map>

#include "RxDynamicModule.h"
#include "Ed\EdCommandStack.h"

struct T_MENU_DATA
{
	UINT		enVersion;
	OdString	strName;
	OdString	strDesc;
	OdString	strCmd;
	int			nIconIndex;		// menu icon index. if icon not exist -1, other is index.
	bool		bEnable;
	OdString	strGif;
};
typedef std::map<OdString, T_MENU_DATA> MapMenu;

enum EN_MENU
{
	EN_MENU_TOP,			// 탑 메뉴
	EN_MENU_SETTINGS,		// 설정

	EN_MENU_NUMBER,
};

class CSampleModule : public OdRxModule
{
public:
	// Irx_ExtensionModule
	virtual void initApp() override;
	virtual void uninitApp() override;
public:
	CString GetProductName();
	void SetMenuData();

private:
	CSampleModule* CSampleModule::m_pInstance = 0;
};

template <typename T>
void AddCommand(OdEdCommandStack* pCommands, const TCHAR* pName = nullptr, int nIcon = -1, int nGif = -1)
{
	OdSmartPtr<T> pCmd = OdRxObjectImpl<T>::createObject();
	AddCommandImpl(pCommands, pCmd, true, pName, pName, nIcon, nGif);
}

// irxSample.h : irxSample DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// メイン シンボル


// CirxSampleApp
// このクラスの実装に関しては irxSample.cpp をご覧ください
//

class CirxSampleApp : public CWinApp
{
public:
	CirxSampleApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

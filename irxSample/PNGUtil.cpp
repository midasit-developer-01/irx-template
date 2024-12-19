#include "pch.h"
#include "LocalRc.h"
bool LoadBitmapFromPNG(UINT uResourceID, CBitmap& BitmapOut)
{
	bool bRet = false;

	HINSTANCE hModuleInstance = CLocalRC::GetRCHandle();
	HRSRC hResourceHandle = ::FindResource(hModuleInstance, MAKEINTRESOURCE(uResourceID), _T("BITMAP"));
	if (0 == hResourceHandle)
	{
		return bRet;
	}

	HGLOBAL hResourceInstance = ::LoadResource(hModuleInstance, hResourceHandle);
	if (0 == hResourceInstance)
	{
		return bRet;
	}

	const void* pResourceData = ::LockResource(hResourceInstance);
	if (0 == pResourceData)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	DWORD nImageSize = ::SizeofResource(hModuleInstance, hResourceHandle);
	if (0 == nImageSize)
	{
		return bRet;
	}

	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
	if (0 == hBuffer)
	{
		return bRet;
	}

	void* pBuffer = ::GlobalLock(hBuffer);
	if (0 != pBuffer)
	{
		memcpy(pBuffer, pResourceData, nImageSize);

		::GlobalUnlock(hBuffer);

		IStream* pStream = 0;
		if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream))
		{
			CImage ImageFromResource;
			ImageFromResource.Load(pStream);
			pStream->Release();
			bRet = BitmapOut.Attach(ImageFromResource.Detach());
		}
	}
	::GlobalFree(hBuffer);

	UnlockResource(hResourceInstance);
	FreeResource(hResourceInstance);

	return bRet;
}

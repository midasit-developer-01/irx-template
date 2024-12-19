#pragma once
#include "StaticRxObject.h"
#include "Ed/EdCommandStack.h"

#define SAMPLE_GROUP	 L"SAMPLE_GROUP"
#define SAMPLE_GLOBAL	 L"_GSAMPLE"
#define SAMPLE_LOCAL	 L"GSAMPLE"
class OdGePoint3d;
class CSampleCmd : public OdStaticRxObject<OdEdCommand>
{
public:
	CSampleCmd();
	~CSampleCmd();

	virtual const OdString groupName() const;
	virtual const OdString globalName() const;
	virtual const OdString localName() const;

	virtual void execute(OdEdCommandContext* pCmdCtx);

protected:
	OdString m_groupName;
	OdString m_globalName;
	OdString m_localName;
};

typedef OdSmartPtr<CSampleCmd> CSampleCmdPtr;
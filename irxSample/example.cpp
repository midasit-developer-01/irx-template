#include "pch.h"
#include <DbCommandContext.h>

#include "SampleCmd.h"

#include "DbBlockTableRecord.h"
#include "DbLine.h"

#include <IcApi.h>
#include "ipRxApi.h"

CSampleCmd::CSampleCmd()
{
	m_groupName = SAMPLE_GROUP;
	m_globalName = SAMPLE_GLOBAL;
	m_localName = SAMPLE_LOCAL;
}
CSampleCmd::~CSampleCmd()
{
}

const OdString CSampleCmd::groupName() const
{
	return m_groupName;
}

const OdString CSampleCmd::globalName() const
{
	return m_globalName;
}

const OdString CSampleCmd::localName() const
{
	return m_localName;
}

void CSampleCmd::execute(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbCmdCtx->database();

	icPrintf(L"Hello World\n");

	AfxMessageBox(_T("���ݺ��� ���� �׸��ڽ��ϴ�."));

	OdDbBlockTableRecordPtr pBtr = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
	if (pBtr.isNull()) {
		ASSERT(FALSE);
		//ip::rx::api::abortTransaction();
		return;
	}

	OdGePoint3d sttPt1;
	OdGePoint3d endPt1;
	icedGetPoint(NULL, _T("�������� �Է��ϼ���."), sttPt1);
	icedGetPoint(&sttPt1, _T("�������� �Է��ϼ���."), endPt1);

	OdDbLinePtr pLine1 = OdDbLine::createObject();
	pLine1->setStartPoint(sttPt1);
	pLine1->setEndPoint(endPt1);

	pBtr->appendOdDbEntity(pLine1);
}

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
	//AfxMessageBox(_T("���ڰ��� �׸��ڽ��ϴ�"));

	OdDbCommandContextPtr pDbcmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbcmdCtx->database();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	OdDbBlockTableRecordPtr pBr = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
	if (pBr.isNull()) {
		ASSERT(FALSE);
		return;
	}
	// UI Interaction
	OdGePoint3d start, end;
	icedGetPoint(NULL, _T("�������� �Է��ϼ���\n"), start);
	icedGetPoint(&start, _T("������ �Է��ϼ���\n"), end);

	// Line �����͸� ���� DB�� ����
	OdDbLinePtr pLine1 = OdDbLine::createObject();
	pLine1->setStartPoint(start);
	pLine1->setEndPoint(end);
	pBr->appendOdDbEntity(pLine1);
}

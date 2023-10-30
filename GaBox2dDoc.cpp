// GaBox2dDoc.cpp : implementation of the CGaBox2dDoc class
//

#include "stdafx.h"
#include "GaBox2d.h"

#include "GaBox2dDoc.h"
#include "EditorChaoDlg.h"
#include ".\gabox2ddoc.h"

namespace GUI
{

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGaBox2dDoc
IMPLEMENT_DYNCREATE(CGaBox2dDoc, CDocument)

BEGIN_MESSAGE_MAP(CGaBox2dDoc, CDocument)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_EDITARCH, OnEditEditarch)
END_MESSAGE_MAP()


// CGaBox2dDoc construction/destruction

CGaBox2dDoc::CGaBox2dDoc()
{
	m_pWorld = NULL;
}

CGaBox2dDoc::~CGaBox2dDoc()
{
	if(m_pWorld != NULL)
		delete m_pWorld;
}

b2Vec2 operator*(b2Vec2 left, double mul)
{
	return b2Vec2(left.x*(float)mul, left.y*(float)mul);
}

BOOL CGaBox2dDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	srand((unsigned)time(NULL));

	CEditorChaoDlg dlgChao;
	if(dlgChao.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	m_env = dlgChao.m_World;

	m_pWorld = PHYS::buildWorld(&m_env);
	m_vecGround = m_env.get_vecs();

	m_car.beginSimulate(m_pWorld);

	return TRUE;
}

// CGaBox2dDoc serialization
void CGaBox2dDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CGaBox2dDoc diagnostics

#ifdef _DEBUG
void CGaBox2dDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGaBox2dDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CGaBox2dDoc::OnEditCopy()
{
	CString strGenes;
	m_car.getGenes(strGenes);

	COleDataSource*	pSource = new COleDataSource();
	CSharedFile	sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	sf.Write(strGenes, strGenes.GetLength());

	HGLOBAL hMem = sf.Detach();
	if (!hMem) return;
	pSource->CacheGlobalData(CF_TEXT, hMem);
	pSource->SetClipboard();
}

void CGaBox2dDoc::OnEditEditarch()
{
	CEditorChaoDlg dlg;
	if(dlg.DoModal() != IDOK)
		return;
	
	m_car.Destroy();
	delete m_pWorld;

	m_env = dlg.m_World;
	m_pWorld = PHYS::buildWorld(&m_env);
	m_vecGround = m_env.get_vecs();
	m_car.beginSimulate(m_pWorld);

	POSITION pos = GetFirstViewPosition();
	GetNextView(pos)->Invalidate();
}

void CGaBox2dDoc::BeginSimulation(void)
{
	m_car.beginSimulate(m_pWorld);
}
void CGaBox2dDoc::EndSimulation(void)
{
	m_car.endSimulate();
}

PointF	CGaBox2dDoc::GetCenter(void)
{
	b2Vec2 pos = m_car.getCenter();
	return PointF(pos.x,pos.y);
}

};//namespace GUI
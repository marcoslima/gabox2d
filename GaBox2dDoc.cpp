// GaBox2dDoc.cpp : implementation of the CGaBox2dDoc class
//

#include "stdafx.h"
#include "GaBox2d.h"

#include "GaBox2dDoc.h"

vec_vecs_t	g_vecGroundPoints;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGaBox2dDoc
IMPLEMENT_DYNCREATE(CGaBox2dDoc, CDocument)

BEGIN_MESSAGE_MAP(CGaBox2dDoc, CDocument)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
END_MESSAGE_MAP()


// CGaBox2dDoc construction/destruction

CGaBox2dDoc::CGaBox2dDoc()
{
	m_pWorld = NULL;
	g_vecGroundPoints = CreateGround();
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

	m_pWorld = CreateWorld(g_vecGroundPoints);

	/////////////////////////////////////////////

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

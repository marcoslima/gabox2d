#include "GaBox2dDoc.h"
#include "EditorChaoDlg.h"


namespace GUI
{
// CGaBox2dDoc
// CGaBox2dDoc construction/destruction
CGaBox2dDoc::CGaBox2dDoc()
	: m_GroundId()
	, m_env()
{
	m_WorldId = b2_nullWorldId;
}

CGaBox2dDoc::~CGaBox2dDoc()
{
	if(!b2World_IsValid(m_WorldId))
		b2DestroyWorld(m_WorldId);
}

b2Vec2 operator*(const b2Vec2 left, const double mul)
{
	return b2Vec2(left.x*static_cast<float>(mul), left.y*static_cast<float>(mul));
}

bool CGaBox2dDoc::OnNewDocument(CEnv& env)
{
	srand(static_cast<unsigned>(time(nullptr))); // NOLINT(*-msc51-cpp)

	// CEditorChaoDlg dlgChao;
	// if(dlgChao.DoModal() == IDCANCEL)
	// {
	// 	return FALSE;
	// }

	// m_env = dlgChao.m_World;
	m_env = env;

	_start_world();

	return true;
}

void CGaBox2dDoc::OnEditCopy() const
{
	string strGenes;
	m_car.getGenes(strGenes);

	// Código para copiar para a área de transferência
	// TODO: Implementar para Linux ou genérico
	// ...
}

void CGaBox2dDoc::_start_world() {
	m_WorldId = PHYS::buildWorld(&m_env);
	m_vecGround = m_env.get_vecs();
	m_car.beginSimulate(m_WorldId);
}

void CGaBox2dDoc::OnEditEditarch()
{
	// TODO: chamar editor de chão aqui.
	// CEditorChaoDlg dlg;
	// if(dlg.DoModal() != IDOK)
	// 	return;
	
	// m_car.Destroy();
	// delete m_pWorld;

	// m_env = dlg.m_World;

	_start_world();

	// POSITION pos = GetFirstViewPosition();
	// GetNextView(pos)->Invalidate();
}

void CGaBox2dDoc::BeginSimulation()
{
	m_car.beginSimulate(m_WorldId);
}

void CGaBox2dDoc::EndSimulation()
{
	CCar::endSimulate();
}

PointF	CGaBox2dDoc::GetCenter()
{
	auto [x, y] = m_car.getCenter();
	return PointF(x, y);
}

}

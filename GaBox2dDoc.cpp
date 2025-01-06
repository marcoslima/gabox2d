#include "GaBox2dDoc.h"

#include <iostream>
#include <mutex>
#include "EditorChaoDlg.h"
#include "phys.h"


namespace GUI
{
    // CGaBox2dDoc
    // CGaBox2dDoc construction/destruction
    CGaBox2dDoc::CGaBox2dDoc()
        : m_env() {}

    CGaBox2dDoc::~CGaBox2dDoc()
    {
        if (!b2World_IsValid(m_World.m_WorldId))
            b2DestroyWorld(m_World.m_WorldId);
    }

    b2Vec2 operator*(const b2Vec2 left, const double mul)
    {
        return b2Vec2(left.x * static_cast<float>(mul), left.y * static_cast<float>(mul));
    }

    bool CGaBox2dDoc::OnNewDocument(const CEnv &env)
    {
        const auto seed = static_cast<unsigned>(time(nullptr));
        srand(seed); // NOLINT(*-msc51-cpp)

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

    void CGaBox2dDoc::_start_world()
    {
        buildWorld(m_env, m_World);
        m_vecGround = m_env.get_vecs();
        m_car.beginSimulate(m_World.m_WorldId);
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
        m_IsSimulating = true;
        m_car.beginSimulate(m_World.m_WorldId);
    }

    void CGaBox2dDoc::EndSimulation()
    {
        m_IsSimulating = false;
        m_car.endSimulate();
    }

    PointF CGaBox2dDoc::GetCenter() const
    {
        auto [x, y] = m_car.getCenter();
        return PointF(x, y);
    }

    void CGaBox2dDoc::Quit()
    {
        m_bQuit = true;
    }
}

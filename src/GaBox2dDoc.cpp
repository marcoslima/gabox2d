#include "GaBox2dDoc.h"

#include <iostream>
#include <mutex>
#include "EditorChaoDlg.h"
#include <phys.h>
#include <ga.h>


namespace GUI
{
    // CGaBox2dDoc
    // CGaBox2dDoc construction/destruction
    CGaBox2dDoc::CGaBox2dDoc()
        : m_car(CCarFactory().createRandomCar()) {}

    CGaBox2dDoc::~CGaBox2dDoc()
    {
        if ( m_World.isValid() ) m_World.destroy();
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

    void CGaBox2dDoc::_start_world()
    {
        m_World.create(m_env);
        m_vecGround = m_env.get_vecs();
        m_car->beginSimulate(m_World);
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
        m_car->beginSimulate(m_World);
    }

    void CGaBox2dDoc::EndSimulation()
    {
        m_IsSimulating = false;
    }

    PointF CGaBox2dDoc::GetCenter() const
    {
        auto [x, y] = m_car->getCenter();
        return PointF(x, y);
    }

    void CGaBox2dDoc::Quit()
    {
        m_bQuit = true;
    }

    const icar_ptr_t& CGaBox2dDoc::GetCar()
    {
        return m_car;
    }
}

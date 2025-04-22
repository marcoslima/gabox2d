#include "GaBox2dDoc.h"

#include <iostream>
#include <mutex>
#include <ga.h>


namespace GUI
{
    // CGaBox2dDoc
    // CGaBox2dDoc construction/destruction
    CGaBox2dDoc::CGaBox2dDoc()
        : m_car(CCarFactory().createRandomCar())
    {
        _world = std::make_shared<PHYS::CWorld>();
    }

    void CGaBox2dDoc::createRandomCar()
    {
        _generation = -1;
        m_car->createGaRandomCar();// = CCarFactory().createRandomCar();
        m_car->beginSimulate(_world);
    }

    void CGaBox2dDoc::simulateGaCar(const std::string &genes, const float fitness, const size_t generation)
    {
        m_car->createGaFromGenes(genes);
        _generation = generation;
        _fitness = fitness;
    }

    bool CGaBox2dDoc::isSimulating() const
    {
        return _isSimulating;
    }

    PHYS::IWorldPtr CGaBox2dDoc::GetWorld() const
    {
        return _world;
    }

    bool CGaBox2dDoc::isQuit() const
    {
        return m_bQuit;
    }

    vec_vecs_t CGaBox2dDoc::GetGround() const
    {
        return m_vecGround;
    }

    CEnv CGaBox2dDoc::GetEnv() const
    {
        return m_env;
    }

    size_t CGaBox2dDoc::getGeneration() const
    {
        return _generation;
    }

    float CGaBox2dDoc::getFitness() const
    {
        return _fitness;
    }

    CGaBox2dDoc::~CGaBox2dDoc()
    {
        if ( _world->isValid() )
        {
            _world->destroy();
        }
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
        _world->create(m_env.env_data);
        m_vecGround = m_env.env_data.ground;
        m_car->beginSimulate(_world);
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
        _isSimulating = true;
        m_car->beginSimulate(_world);
    }

    void CGaBox2dDoc::EndSimulation()
    {
        _isSimulating = false;
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

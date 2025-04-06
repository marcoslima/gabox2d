#include <iostream>
#include <thread_params.h>

#include "car.h"
#include "CCronometro.h"
#include "ga.h"
#include "World.h"
#include <ga_ipc.h>
#include <ga_server.h>

using namespace std;


void fnGa(void *pParam)
{
    cout << "fnGa starting..." << endl;
    const auto tp = static_cast<CThreadParams *>(pParam);

    // while (!tp->m_bStopGa.load())
    // {
    //     this_thread::sleep_for(chrono::seconds(3));
    // }

    // cout << "fnGa escaped the waiting." << endl;


    // HWND hWndNotify = tp->m_wndNotify;
    const ga_params_t gaParams = tp->m_Params;
    // CGaInfo *pGaInfo = tp->m_pGaInfo;
    const PHYS::IWorldPtr world = make_shared<PHYS::CWorld>();
    world->create(tp->m_env);

    GaServer gaServer;
    // gaServer.startAccept();

    GA::CGa ga(make_unique<CCarFactory>());
    const float cross = gaParams.m_fCrossover;
    const float mut = gaParams.m_fMutacao;

    ga.setParams(gaParams.m_nPopulacao, // Número de indivíduos
                 gaParams.m_nElitismo, // Tamanho do elitismo
                 cross, // Probabilidade de crossover
                 mut, // Probabilidade de mutação
                 gaParams.m_nAlienismo, // Tamanho do alienismo
                 gaParams.m_nMutInt, // Intensidade da mutação
                 gaParams.m_fMaxT); // Tempo máximo a ser simulado

    cout << "Iniciando evolução..." << endl;
    ga.BeginEvolve();

    CCronometro crInfo, crGa;
    crInfo.Start();

    // Medição da velocidade gerações por segundo:
    double gps = -1;
    size_t nCount = 0;
    constexpr size_t N = 10;
    size_t nLastGeneration = 0;

    crGa.Start();

    cout << "Evolving..." << endl;
    while (!tp->m_bStopGa.load())
    {
        ga.Ordena(world, tp->m_bStopGa);
        if (crInfo.Get() > .250 && !tp->m_bStopGa.load())
        {
            crInfo.Start();

            if (ga.getGeracao() > nLastGeneration)
            {
                nLastGeneration = ga.getGeracao();

                // Create status update and broadcast to GUI
                ipc::GaStatus status(
                    ga.getGeracao(),
                    gps,
                    ga.getBest()->getFitness(),
                    ga.getBest()->getGenes(),
                    ga.getPopulacao(),
                    ga.getMelhores()
                );

                gaServer.broadcastStatus(status);
            }
        }

        if (tp->m_bStopGa.load()) break;

        ga.Step();

        nCount++;
        if (nCount == N)
        {
            gps = static_cast<double>(N) / crGa.Get();
            nCount = 0;
            crGa.Start();
        }
    }
    cout << "fnGa exiting..." << endl;
    // delete pWorld;
    // SetEvent(hGaStopped);
}

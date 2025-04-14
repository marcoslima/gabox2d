#include <iostream>
#include <thread_params.h>

#include "car.h"
#include "CCronometro.h"
#include "ga.h"
#include <ga_status.h>
#include <network/ga_server.h>

using namespace std;


void update_ga_status(GaServer &gaServer, const GA::CGa &ga, const float gps)
{
    // Create status update and broadcast to GUI
    ipc::GaStatus status(
        ga.getGeracao(),
        gps,
        ga.getBest()->getFitness(),
        ga.getBest()->getGenes(),
        ga.getPopulacao(),
        ga.getMelhores()
    );
    const auto data = ipc::GaStatusSerializer::serializeGaStatus(status);
    gaServer.broadcastStatus(data);
}

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
    GaServer gaServer;

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
    crGa.Start();

    // Medição da velocidade gerações por segundo:
    float gps = -1;
    size_t nCount = 0;
    constexpr size_t N = 10;


    cout << "Evolving..." << endl;
    while (!tp->m_bStopGa.load())
    {
        ga.Ordena(tp->m_env_data, tp->m_bStopGa);
        if (crInfo.Get() > .250 && !tp->m_bStopGa.load())
        {
            crInfo.Start();
            update_ga_status(gaServer, ga, gps);
        }
        ga.Step();

        if (tp->m_bStopGa.load()) break;

        nCount++;
        if (nCount == N)
        {
            gps = N / static_cast<float>(crGa.Get());
            nCount = 0;
            crGa.Start();
        }
    }
    cout << "fnGa exiting..." << endl;
    // delete pWorld;
    // SetEvent(hGaStopped);
}

#include "GaBox2dDoc.h"
#include "GaBox2dView.h"
#include "EvolucaoDlg.h"
#include "devutils.h"
#include "Pen.h"
#include "SolidBrush.h"
#include <CDT.hpp>

using namespace DevUtils;

namespace GUI
{
    CGaBox2dView::CGaBox2dView()
        : m_nSimTimer(0)
          , m_nVelocidade(1)
          , m_bGaRunning(false)
          , m_bGaExited(false)
          , m_bShowInfoId(true)
          , m_bShowInfoGaGenes(false)
          , m_bWaitingEvolucao(false)
          , _pDocument(nullptr) {}

    sf::Vector2f WorldToLogical(b2Vec2 worldPoint)
    {
        return {worldPoint.x, worldPoint.y};
    }


    void CGaBox2dView::_draw_sky(sf::RenderWindow &window, const CEnv &env)
    {
        // World na cor de céu
        sf::RectangleShape sky(sf::Vector2f(env._brx - env._tlx, env._tly - env._bry));
        sky.setFillColor(sf::Color(100, 100, 255));
        sky.setPosition(env._tlx, env._bry);
        window.draw(sky);
    }

    void CGaBox2dView::_draw_ground(sf::RenderWindow &window, const CGaBox2dDoc *pDoc)
    {
        const CPen penGround(sf::Color(0, 0, 0), 0.1);
        const CSolidBrush bshTransparent(sf::Color::Transparent);
        const CSolidBrush bshGround(sf::Color(32, 128, 32));

        const vec_vecs_t vecGround = pDoc->m_vecGround;

        const size_t nSize = vecGround.size();
        if (nSize == 0)
            return;

        // Triangularização para preenchimento do polígono:
        // (Delunay Triangulation)
        CDT::Triangulation<float> cdt;
        vector<CDT::V2d<float> > vecVertices;
        for (const auto &v: vecGround)
        {
            vecVertices.emplace_back(v.x, v.y);
        }
        cdt.insertVertices(vecVertices);
        vector<CDT::Edge> vecEdges;
        vecEdges.reserve(vecGround.size());
        for (int i = 0; i < vecGround.size(); i++)
        {
            vecEdges.emplace_back(i, (i + 1) % vecGround.size());
        }
        cdt.insertEdges(vecEdges);
        cdt.eraseOuterTrianglesAndHoles();
        // const CPen penDebug(sf::Color(255, 0, 0), 0.1);
        // ReSharper disable once CppUseStructuredBinding
        for (const auto &triangle: cdt.triangles)
        {
            sf::ConvexShape polygon(3);
            for (size_t i = 0; i < 3; i++)
            {
                polygon.setPoint(i, sf::Vector2f(vecVertices[triangle.vertices[i]].x,
                                                 vecVertices[triangle.vertices[i]].y));
            }
            // penDebug.apply(polygon);
            bshGround.apply(polygon);
            window.draw(polygon);
        }

        // Contorno do ground:
        sf::ConvexShape polygon(nSize);
        for (size_t i = 0; i < nSize; i++)
        {
            polygon.setPoint(i, sf::Vector2f(vecGround[i].x, vecGround[i].y));
        }
        penGround.apply(polygon);
        bshTransparent.apply(polygon);

        window.draw(polygon);
    }

    void CGaBox2dView::_draw_border(sf::RenderWindow &window, const CEnv &env)
    {
        // Pen penBorder(sf::Color(255, 0, 0), 5);
        // gr.DrawRectangle(&penBorder, rcWorld);
        const CPen penBorder(sf::Color(255, 0, 0), 5);
        const CSolidBrush bshBorder(sf::Color::Transparent);
        sf::RectangleShape border(sf::Vector2f(env._brx - env._tlx, env._tly - env._bry));
        border.setPosition(env._tlx, env._bry);
        penBorder.apply(border);
        bshBorder.apply(border);

        window.draw(border);
    }

    void CGaBox2dView::Draw(sf::RenderWindow &window)
    {
        CGaBox2dDoc *pDoc = GetDocument();
        const CEnv env = pDoc->m_env;
        constexpr float move_step = 1.0f;
        constexpr float zoom_step = 1.01f;
        if(m_bZoomOut)   m_ZoomFactor = min(m_ZoomFactor * zoom_step, 8.0f);
        if(m_bZoomIn)  m_ZoomFactor = max(m_ZoomFactor / zoom_step, 0.125f);
        if(m_bMoveLeft) m_view_pos.x -= move_step*m_ZoomFactor;
        if(m_bMoveRight) m_view_pos.x += move_step*m_ZoomFactor;
        if(m_bMoveUp)   m_view_pos.y += move_step*m_ZoomFactor;
        if(m_bMoveDown) m_view_pos.y -= move_step*m_ZoomFactor;

        // Zoom to fit
        sf::View view(sf::Vector2f(10.0f+m_view_pos.x, 20.0f+m_view_pos.y),
                      sf::Vector2f(100*m_ZoomFactor, -70*m_ZoomFactor));
        view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
        window.setView(view);


        _draw_sky(window, env);
        _draw_ground(window, pDoc);
        _draw_border(window, env);
        pDoc->GetCar().Draw(window);
#if 0

        pDoc->GetCar().Draw(&gr);

        m_pdlgIdInfo->set(-1, -1,
                          pDoc->GetCar().getT(),
                          pDoc->GetCar().getGenes());
#endif
    }

    CGaBox2dDoc *CGaBox2dView::GetDocument() const
    {
        return _pDocument;
    }

    void CGaBox2dView::SetDocument(CGaBox2dDoc *pDoc)
    {
        _pDocument = pDoc;
    }

    // CGaBox2dView message handlers
    void CGaBox2dView::OnSimulaPlay()
    {
#if 0
          if (m_nSimTimer == 0)
        {
            // Ligar simula��o:
            GetDocument()->BeginSimulation();
            m_nSimTimer = (UINT) SetTimer((UINT_PTR) 1001, 10,NULL);
        } else
        {
            KillTimer(m_nSimTimer);
            m_nSimTimer = 0;
            GUI::CGaBox2dDoc::EndSimulation();
        }
#endif
    }

    void CGaBox2dView::OnSimulaReset() const
    {
        CGaBox2dDoc *pDoc = GetDocument();
        if (!pDoc)
            return;

        pDoc->GetCar().CreateCar();
        pDoc->GetCar().beginSimulate(pDoc->m_World.m_WorldId);
    }

    void CGaBox2dView::OnVelocidadeMais()
    {
        m_nVelocidade++;
    }

    void CGaBox2dView::OnVelocidadeMenos()
    {
        m_nVelocidade--;
    }

    void CGaBox2dView::OnSimulaRepetir()
    {
#if 0
          CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
            return;

        CString strGenes;
        pDoc->GetCar().getGenes(strGenes);

        if (m_nSimTimer != 0)
            OnSimulaPlay();

        pDoc->GetCar().CreateCar(strGenes);

        if (m_nSimTimer == 0)
            OnSimulaPlay();
#endif
    }

    void CGaBox2dView::OnVelocidade1x()
    {
        m_nVelocidade = 1;
    }

    void CGaBox2dView::OnVelocidade2x()
    {
        m_nVelocidade = 2;
    }

    void CGaBox2dView::OnVelocidade4x()
    {
        m_nVelocidade = 4;
    }

    void CGaBox2dView::OnVelocidade10x()
    {
        m_nVelocidade = 10;
    }

    void CGaBox2dView::OnVelocidade100x()
    {
        m_nVelocidade = 100;
    }

    void fnGa(void *pParam)
    {
#if 0
        CThreadParams *tp = (CThreadParams *) pParam;
        HANDLE hStopGA = tp->m_hStopGa;
        HANDLE hGaStopped = tp->m_hGaStopped;
        HWND hWndNotify = tp->m_wndNotify;
        ga_params_t gaParams = tp->m_Params;
        CGaInfo *pGaInfo = tp->m_pGaInfo;
        b2World *pWorld = PHYS::buildWorld(&tp->m_env);

        CGa ga;
        // Preparamos os par�metros do GA:
        double cross, mut;
        gaParams.m_strMutacao.Replace(",", ".");
        gaParams.m_strCrossover.Replace(",", ".");

        cross = atof(gaParams.m_strCrossover);
        mut = atof(gaParams.m_strMutacao);

        ga.setParams(gaParams.m_nPopulacao, // N�mero de indiv�duos
                     gaParams.m_nElitismo, // Tamanho do elitismo
                     cross, // Probabilidade de crossover
                     mut, // Probabilidade de muta��o
                     gaParams.m_nAlienismo, // Tamanho do alienismo
                     gaParams.m_nMutInt, // Intensidade da muta��o
                     gaParams.m_dMaxT); // Tempo m�ximo � ser simulado

        ga.BeginEvolve();
        size_t nSize = ga.getPopulacaoLen();

        CCronometro crInfo, crGa;
        crInfo.Start();

        // Medi��o da velocidade gera��es por segundo:
        double gps = -1;
        int nCount = 0;
        crGa.Start();

        while (WaitForSingleObject(hStopGA, 0) == WAIT_TIMEOUT)
        {
            ga.Ordena(pWorld, hStopGA);
            if (crInfo.Get() > 250)
            {
                pGaInfo->Lock();
                pGaInfo->m_geracao = ga.getGeracao();
                pGaInfo->m_gps = gps;
                ga.CopyPopulacao(&pGaInfo->m_populacao);

                if (pGaInfo->m_reqMelhores)
                {
                    pGaInfo->m_reqMelhores = false;
                    pGaInfo->m_vecMelhores = ga.m_melhores;
                }
                if (pGaInfo->m_reqExtincao)
                {
                    pGaInfo->m_reqExtincao = false;
                    ga.MassExtinctionEvent();
                }

                pGaInfo->Release();
                crInfo.Start();
                PostMessage(hWndNotify,IDM_GA_INFO, 0, 0);
            }
            ga.Step();

            nCount++;
            if (nCount == 10)
            {
                gps = 10.0 / crGa.Get(CCronometro::uS);
                nCount = 0;
                crGa.Start();
            }

            //		if(gaParams.m_b
        }

        delete pWorld;
        SetEvent(hGaStopped);
#endif
    }

    void CGaBox2dView::OnGaIniciarga()
    {
#if 0
          if (m_bGaRunning)
        {
            // J� est� rodando, ent�o � para parar:
            if (AfxMessageBox("Tem certeza de que quer parar o GA?", MB_YESNO) == IDNO)
                return;

            // Ok, vamos parar:
            CMessageDlg dlgMsg;
            dlgMsg.BeginMessage("Interrompendo GA...", this);
            SetEvent(_thread_params.m_hStopGa);
            WaitForSingleObject(_thread_params.m_hGaStopped, INFINITE);
            dlgMsg.EndMessage();

            m_bGaRunning = false;

            // Liberamos os eventos:
            CloseHandle(_thread_params.m_hStopGa);
            CloseHandle(_thread_params.m_hGaStopped);

            // Pronto!
            return;
        }


        // Obtemos os par�metros do GA:
        CGaParamsDlg dlgParams;
        if (dlgParams.DoModal() != IDOK)
        {
            return;
        }

        _thread_params.m_Params = dlgParams;
        _thread_params.m_hStopGa = CreateEvent(NULL, TRUE, FALSE,NULL);
        _thread_params.m_hGaStopped = CreateEvent(NULL, TRUE, FALSE,NULL);
        _thread_params.m_wndNotify = m_hWnd;
        _thread_params.m_pGaInfo = &m_GaInfo;
        _thread_params.m_env = GetDocument()->m_env;

        m_bGaRunning = true;
        _beginthread(fnGa, 0, (void *) &_thread_params);
#endif
    }

    void CGaBox2dView::OnEditPaste()
    {
#if 0
          CString buffer;
        COleDataObject obj;

        if (obj.AttachClipboard())
        {
            if (obj.IsDataAvailable(CF_TEXT))
            {
                HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
                unsigned char *pBuf;
                size_t nSize;

                pBuf = (unsigned char *) ::GlobalLock(hmem);
                nSize = ::GlobalSize(hmem);
                CMemFile sf(pBuf, (UINT) nSize);

                LPSTR str = buffer.GetBufferSetLength((int) ::GlobalSize(hmem));
                sf.Read(str, (UINT) ::GlobalSize(hmem));
                ::GlobalUnlock(hmem);
                buffer.ReleaseBuffer();
            }
        }

        size_t i, nSize = buffer.GetLength();
        if (nSize != GENES)
            return;


        for (i = 0; i < nSize; i++)
        {
            if (buffer.GetAt((int) i) < 'A' || buffer.GetAt((int) i) > 'Z')
            {
                return;
            }
        }

        // Ok, podemos colar:
        CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
            return;

        m_pdlgIdInfo->set(0, 0, 0, "nenhum");
        if (m_nSimTimer != 0)
            OnSimulaPlay();

        pDoc->GetCar().CreateCar(buffer);

        if (m_nSimTimer == 0)
            OnSimulaPlay();
#endif
    }

    void CGaBox2dView::OnMostrarMelhor()
    {
#if 0
          CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
            return;

        if (m_nSimTimer != 0)
            OnSimulaPlay();

        m_GaInfo.Lock(); {
            if (m_GaInfo.m_populacao.size() == 0)
            {
                m_GaInfo.Release();
                AfxMessageBox("N�o h� ningu�m para mostrar ainda.");
                return;
            }
            pDoc->GetCar().CreateCar(m_GaInfo.m_populacao.begin()->getGenesCString());
            m_pdlgIdInfo->set(m_GaInfo.m_geracao,
                              m_GaInfo.m_populacao.begin()->_pontos,
                              m_GaInfo.m_populacao.begin()->getT(),
                              m_GaInfo.m_populacao.begin()->getGenes());
        }
        m_GaInfo.Release();


        if (m_nSimTimer == 0)
            OnSimulaPlay();
#endif
    }

    void CGaBox2dView::OnMostrarQualquer()
    {
#if 0
          CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
            return;

        if (m_nSimTimer != 0)
            OnSimulaPlay();

        m_GaInfo.Lock(); {
            size_t nQq = rand() % m_GaInfo.m_populacao.size();
            lst_car_t::iterator it;
            for (int i = 0; i < nQq; i++) it++;

            pDoc->GetCar().CreateCar(it->getGenesCString());

            m_pdlgIdInfo->set(m_GaInfo.m_geracao,
                              it->_pontos,
                              it->getT(),
                              it->getGenes());
        }
        m_GaInfo.Release();

        if (m_nSimTimer == 0)
            OnSimulaPlay();
#endif
    }

    void CGaBox2dView::OnInformaGagenes()
    {
        m_bShowInfoGaGenes = !m_bShowInfoGaGenes;
        // Invalidate();
    }

    void CGaBox2dView::OnInformaIndividuoatual()
    {
        m_bShowInfoId = !m_bShowInfoId;
        // Invalidate();
    }

    void CGaBox2dView::OnGaExtins()
    {
        // m_GaInfo.Lock();
        // m_GaInfo.m_reqExtincao = true;
        // m_GaInfo.Release();
    }

    void CGaBox2dView::OnGaColar()
    {
#if 0
          CString buffer;
        COleDataObject obj;

        if (obj.AttachClipboard())
        {
            if (obj.IsDataAvailable(CF_TEXT))
            {
                HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
                unsigned char *pBuf;
                size_t nSize;

                pBuf = (unsigned char *) ::GlobalLock(hmem);
                nSize = ::GlobalSize(hmem);
                CMemFile sf(pBuf, (UINT) nSize);

                LPSTR str = buffer.GetBufferSetLength((int) ::GlobalSize(hmem));
                sf.Read(str, (UINT) ::GlobalSize(hmem));
                ::GlobalUnlock(hmem);
                buffer.ReleaseBuffer();
            }
        }

        size_t i, nSize = buffer.GetLength();
        if (nSize != GENES)
            return;


        for (i = 0; i < nSize; i++)
        {
            if (buffer.GetAt((int) i) < 'A' || buffer.GetAt((int) i) > 'Z')
            {
                return;
            }
        }

        // Ok, podemos colar:
        m_GaInfo.Lock();
        m_GaInfo.m_reqIncludeId = true;
        m_GaInfo.m_strId2Include = buffer;
        m_GaInfo.Release();
#endif
    }

#if 0
  LRESULT CGaBox2dView::OnGaInfo(WPARAM wParma, LPARAM lParam)
    {
        Invalidate();
        CGaBox2dDoc *pDoc = GetDocument();

        if (m_bWaitingEvolucao)
        {
            m_bWaitingEvolucao = false;

            CEvolucaoDlg dlgEvolucao;
            m_GaInfo.Lock();
            dlgEvolucao.m_vecCarros = m_GaInfo.m_vecMelhores;
            m_GaInfo.Release();
            dlgEvolucao.DoModal();
        }

        m_GaInfo.Lock();
        m_pdlgGaInfo->Refresh(&m_GaInfo);
        m_GaInfo.Release();

        return 0L;
    }
#endif


    void CGaBox2dView::OnInformaEvolu()
    {
        // // Marcamos o evento:
        // m_GaInfo.Lock();
        // m_GaInfo.m_reqMelhores = true;
        // m_bWaitingEvolucao = true;
        // m_GaInfo.Release();
    }

#if 0
      void CGaBox2dView::OnUpdateInformaEvolu(CCmdUI *pCmdUI)
    {
    	if(m_bWaitingEvolucao)
    	{
    		pCmdUI->SetText("Evolu��o (em andamento...)");
    		pCmdUI->Enable(FALSE);
    	}
    	else
    	{
    		pCmdUI->SetText("Evolu��o");
    		pCmdUI->Enable(TRUE);
    	}
    }
#endif


#if 0
      LRESULT CGaBox2dView::OnSimularGene(WPARAM wParam, LPARAM lParam)
    {
        //TODO: Reativar fun��o
#if 0
	if(m_nSimTimer != 0)
		OnSimulaPlay();

	GetDocument()->m_car.CreateCar((LPCSTR) lParam);
	m_pdlgIdInfo->set(	m_GaInfo.m_geracao,
						m_GaInfo.m_populacao[0]._pontos,
						m_GaInfo.m_populacao[0]._t,
						m_GaInfo.m_populacao[0].getGenes());

	OnSimulaPlay();

#endif
        return 0L;
    }
#endif


#if 0
      void CGaBox2dView::OnGaMudarpar()
    {
        CGaParamsDlg dlgParams;
        if (dlgParams.DoModal() != IDOK)
        {
            return;
        }
        // TODO: Reativar
        //	_thread_params.m_Params = dlgParams;
        //	_thread_params.bParamsChanged = true;
    }
#endif


    void CGaBox2dView::OnKeyPressed(const sf::Keyboard::Key key)
    {
        const auto pDoc = GetDocument();
        switch (key)
        {
            case sf::Keyboard::Add:
                m_bZoomIn = true;
                break;
            case sf::Keyboard::Subtract:
                m_bZoomOut = true;
                break;
            case sf::Keyboard::Left:
                m_bMoveLeft = true;
                break;
            case sf::Keyboard::Right:
                m_bMoveRight = true;
                break;
            case sf::Keyboard::Up:
                m_bMoveUp = true;
                break;
            case sf::Keyboard::Down:
                m_bMoveDown = true;
                break;
            default:
                break;
        }
    }

    void CGaBox2dView::OnKeyReleased(const sf::Keyboard::Key key)
    {
        const auto pDoc = GetDocument();
        switch (key)
        {
            case sf::Keyboard::R:
                OnSimulaReset();
                break;
            case sf::Keyboard::Q:
                pDoc->Quit();
                break;
            case sf::Keyboard::Add:
                m_bZoomIn = false;
                break;
            case sf::Keyboard::Subtract:
                m_bZoomOut = false;
                break;
            case sf::Keyboard::Left:
                m_bMoveLeft = false;
            break;
            case sf::Keyboard::Right:
                m_bMoveRight = false;
            break;
            case sf::Keyboard::Up:
                m_bMoveUp = false;
            break;
            case sf::Keyboard::Down:
                m_bMoveDown = false;
            break;
            default:
                break;
        }
    }
}

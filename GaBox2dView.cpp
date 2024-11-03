#include "GaBox2d.h"
#include "GaBox2dDoc.h"
#include "GaBox2dView.h"
#include "EditorChaoDlg.h"
#include "EvolucaoDlg.h"
#include "devutils.h"
using namespace DevUtils;

namespace GUI
{
    // CGaBox2dView
    // CGaBox2dView construction/destruction
    CGaBox2dView::CGaBox2dView()
        : m_nSimTimer(0)
          , m_nVelocidade(1)
          , m_bGaRunning(false)
          , m_bGaExited(false)
          , m_bShowInfoId(true)
          , m_bShowInfoGaGenes(false)
          // , _thread_params()
          , m_bWaitingEvolucao(false)
          , m_pdlgIdInfo(nullptr)
          , m_pdlgGaInfo(nullptr) {}

    CGaBox2dView::~CGaBox2dView()
    {
        m_bGaRunning = false;
        // TODO:CVT: SetEvent(_thread_params.m_hStopGa);
        // TODO:CVT: WaitForSingleObject(_thread_params.m_hGaStopped,INFINITE);
        delete m_pdlgGaInfo;
        delete m_pdlgIdInfo;
    }

    sf::Vector2f WorldToLogical(b2Vec2 worldPoint)
    {
        return {worldPoint.x, worldPoint.y};
    }

#if 0
      // CGaBox2dView drawing
    void DrawShape(sf::RenderWindow &window,
                   const b2Fixture *fixture,
                   const b2Transform &xf,
                   b2Vec2 position,
                   sf::Color crFill,
                   sf::Color crCont)
    {
        const b2Shape *shape = fixture->GetShape();
        b2Shape::Type shapeType = shape->GetType();
        switch (shapeType)
        {
            case b2Shape::Type::e_circle:
            {
                const b2CircleShape *circle = (const b2CircleShape *) shape;
                sf::Vector2f center = WorldToLogical(b2Mul(xf, circle->m_p));
                float r = WorldToLogical(b2Vec2(circle->m_radius, 0)).x;
                sf::CircleShape circle_shape(r);
                circle_shape.setPosition(center - WorldToLogical(b2Vec2(r, r)));
                circle_shape.setFillColor(crFill);
                circle_shape.setOutlineColor(crCont);
                circle_shape.setOutlineThickness(0.2f);

                window.draw(circle_shape);

                sf::Vector2f circle_pos = WorldToLogical(b2Mul(xf, circle->m_p + (circle->m_radius * b2Vec2(1.0f, 0))));
                sf::Vertex line[] =
                {
                    sf::Vertex(circle_pos, crCont),
                    sf::Vertex(center, crCont)
                };
                window.draw(line, 2, sf::Lines);
            }
            break;

            case b2Shape::Type::e_polygon:
            {
                const b2PolygonShape *poly = (const b2PolygonShape *) shape;
                size_t vertexCount = poly->m_count;
                sf::ConvexShape convex(vertexCount);
                for (size_t i = 0; i < vertexCount; ++i)
                {
                    b2Vec2 v = b2Mul(xf, poly->m_vertices[i]);
                    convex.setPoint(i, WorldToLogical(v));
                }
                convex.setFillColor(crFill);
                convex.setOutlineColor(crCont);
                convex.setOutlineThickness(0.2f);

                window.draw(convex);
            }
            break;

            default:
                break;
        }
    }
#endif


    void CGaBox2dView::Draw(sf::RenderWindow &window) const
    {
        CGaBox2dDoc *pDoc = GetDocument();
        // ReSharper disable once CppUseStructuredBinding
        CEnv env = pDoc->m_env;

        // Rect rcClient;
        PointF ptScrCenter;
        // {
        // 	CRect rcc;
        // 	CPoint ptc;
        sf::Vector2u rcc;
        sf::Vector2u ptc;
        // 	GetClientRect(rcc);
        rcc = window.getSize();
        // 	ptc = rcc.CenterPoint();
        ptc = sf::Vector2u(rcc.x / 2, rcc.y / 2);

        // 	rcClient = Rect(rcc.left,rcc.top,rcc.right,rcc.bottom);
        // 	ptScrCenter = PointF(ptc.x,ptc.y);
        // }
        // Rect rcWorld(env._tlx,env._bry,env._brx-env._tlx,env._tly-env._bry);

        // gr.SetClip(rcClient,CombineModeReplace);

        // gr.FillRectangle(&SolidBrush(Color(0,0,0)),rcClient);

        // Ajustamos a transforma��o:
        // A transforma��o ser� tal que
        // T(x,y) = (a.x + b, c.y + d)
        // onde (x,y) est� em world coordinates (wc) e
        // T(x,y) est� em device coordinates (dc)
        //
        // sabemos as transforma��es dos seguintes pontos:
        // T(0,0) = (pc.x,pc.y) -> b = pc.x, d = pc.y
        // T(tl.x,tl.y) = (rc.l,rc.t)
        // T(br.x,br.y) = (rc.r,rc.b)
        //
        // a.tl_x + pc_x = rc_l
        // a.br_x + pc_x = rc.r
        //-----------------------
        // a(tl_x-br_x) = (rc_l-rc.r)
        //
        //      (rc_l-rc_r)
        // a = -------------
        //      (tl_x-br_x)
        //
        //      (rc_t-rc_b)
        // b = -------------
        //      (tl_y-br_y)
        ////////////////////////////////////////////////////////////

        //	double dZoom = pow(1.3,(double)m_zoom);
        float dZoom = 10.0;
        float cx, cy;
        PointF pos = pDoc->GetCenter();

        cx = ptScrCenter.x - (pos.x * dZoom);
        cy = ptScrCenter.y + (pos.y * dZoom);

        // Matrix mt( dZoom	,	0		,
        // 	       0		,	-dZoom	,
        // 		   cx		,	cy		);
        // gr.SetTransform(&mt);
        sf::Transform transform;
        transform.translate(cx, cy);
        transform.scale(dZoom, dZoom);

        // World na cor de c�u
        // SolidBrush bshSky(Color(100,100,255));
        // gr.FillRectangle(&bshSky,rcWorld);
        sf::RectangleShape sky(sf::Vector2f(env._brx - env._tlx, env._tly - env._bry));
        sky.setFillColor(sf::Color(100, 100, 255));
        sky.setPosition(env._tlx, env._bry);
        window.draw(sky);

        vec_vecs_t *pVg = &pDoc->m_vecGround;
        size_t i;
        // ReSharper disable once CppTooWideScopeInitStatement
        size_t nSize = pVg->size();
        if (nSize == 0)
            return;
#if 0
        auto *pPoints = new PointF[nSize];
        for (i = 0; i < nSize; i++)
        {
            pPoints[i].x = (*pVg)[i].x;
            pPoints[i].y = (*pVg)[i].y;
        }

        Pen penGround(sf::Color(0, 0, 0), 0.1);
        SolidBrush bshGround(sf::Color(32, 128, 32));
        gr.FillPolygon(&bshGround, pPoints, nSize);
        gr.DrawPolygon(&penGround, pPoints, nSize);
        delete pPoints;

        Pen penBorder(sf::Color(255, 0, 0), 5);
        gr.DrawRectangle(&penBorder, rcWorld);

        pDoc->GetCar().Draw(&gr);

        m_pdlgIdInfo->set(-1, -1,
                          pDoc->GetCar().getT(),
                          pDoc->GetCar().getGenes());
#endif
    }

    CGaBox2dDoc * CGaBox2dView::GetDocument() const
    {
        return _pDocument;
    }

    void CGaBox2dView::SetDocument(CGaBox2dDoc *pDoc)
    {
        _pDocument = pDoc;
    }

    void CGaBox2dView::OnInitialUpdate()
    {
#if 0
        CScrollView::OnInitialUpdate();
        CSize sizeTotal;
        // TODO: calculate the total size of this view
        sizeTotal.cx = sizeTotal.cy = 100;
        SetScrollSizes(MM_TEXT, sizeTotal);

        m_fntSmall.CreateFont(-12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
        m_fntSupersmall.CreateFont(-8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "small fonts");

        m_pdlgIdInfo = new CIdInfoDlg(this);
        m_pdlgIdInfo->Create(CIdInfoDlg::IDD, this);
        m_pdlgIdInfo->ShowWindow(SW_SHOW);

        m_pdlgGaInfo = new CGaInfoDlg(this);
        m_pdlgGaInfo->m_pView = this;
        m_pdlgGaInfo->Create(CGaInfoDlg::IDD, this);
        m_pdlgGaInfo->ShowWindow(SW_SHOW);
#endif
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

#if 0
    void CGaBox2dView::OnTimer(unsigned nIDEvent)
    {
          static bool bWorking = false;
        if (bWorking)
            return;
        bWorking = true;

        CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
        {
            bWorking = false;
            return;
        }

        UINT k;
        for (k = 0; k < m_nVelocidade; k++)
        {
            if (!pDoc->GetCar().doStep())
            {
                //			OnSimulaPlay();
                //			break;
            }
        }
        Invalidate();

        CScrollView::OnTimer(nIDEvent);
        bWorking = false;
    }
#endif

    void CGaBox2dView::OnSimulaReset()
    {
#if 0
          CGaBox2dDoc *pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
            return;

        if (m_nSimTimer != 0)
            OnSimulaPlay();

        pDoc->GetCar().CreateCar();
        m_pdlgIdInfo->set(0, 0, 0, "");

        if (m_nSimTimer == 0)
            OnSimulaPlay();
#endif
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


    void CGaBox2dView::OnKeyPressed(sf::Keyboard::Key key) {}
    void CGaBox2dView::OnKeyReleased(sf::Keyboard::Key key) {}
}

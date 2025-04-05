#include <imgui.h>
#include <iostream>

#include <GaBox2dDoc.h>
#include <GaBox2dView.h>
#include <EvolucaoDlg.h>
#include <Pen.h>
#include <SolidBrush.h>
#include <assets.h>
#include <GaParamsDlg.h>
#include <phys.h>
#include <fn_ga.h>
#include <ga_server.h>

#include "CRandom.h"


namespace GUI
{
    CGaBox2dView::CGaBox2dView()
        : _dlgGaParams([this](const ga_params_t &params)
          {
              this->startGa(params);
          })
          , m_nVelocidade(2)
          , m_bGaRunning(false)
          , m_bGaExited(false)
          , m_bShowInfoId(true)
          , m_bShowInfoGaGenes(false)
          , m_bWaitingEvolucao(false) {}

    CGaBox2dView::~CGaBox2dView()
    {
        // Stop the IO context to cancel any outstanding operations
        if (io_context_)
        {
            io_context_->stop();
        }

        // Close the socket
        if (socket_ && socket_->is_open())
        {
            boost::system::error_code ec;
            socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            socket_->close(ec);
        }

        // Wait for the client thread to finish
        if (client_thread_.joinable())
        {
            client_thread_.join();
        }

        // Clean up the GA thread if still running
        if (isGaRunning())
        {
            _stop_ga();
        }
    }

    void CGaBox2dView::startGa(const ga_params_t params)
    {
        _start_ga(params);
    }

    unsigned CGaBox2dView::getVelocidade() const
    {
        return m_nVelocidade;
    }

    bool CGaBox2dView::isGaRunning() const
    {
        return m_bGaRunning;
    }

    sf::Vector2f WorldToLogical(b2Vec2 worldPoint)
    {
        return {worldPoint.x, worldPoint.y};
    }


    void CGaBox2dView::toggleDrawDebugGround()
    {
        m_bDrawDebugGround = !m_bDrawDebugGround;
    }

    void CGaBox2dView::ShowHelp()
    {
        m_bShowHelp = true;
    }

    bool CGaBox2dView::isFollowCar() const
    {
        return m_bFollowCar;
    }

    void CGaBox2dView::toggleFollowCar()
    {
        m_bFollowCar = !m_bFollowCar;
    }

    bool CGaBox2dView::isDebugGround() const
    {
        return m_bDrawDebugGround;
    }

    bool CGaBox2dView::isShowHelp() const
    {
        return m_bShowHelp;
    }

    void CGaBox2dView::_draw_sky(sf::RenderWindow &window, const CEnv &env)
    {
        // World na cor de céu
        sf::RectangleShape sky(sf::Vector2f(env._brx - env._tlx, env._tly - env._bry));
        sky.setFillColor(sf::Color(100, 100, 255));
        sky.setPosition(env._tlx, env._bry);
        window.draw(sky);
    }

    void CGaBox2dView::_debug_draw_ground(sf::RenderWindow &window, const vec_vecs_t &vecGround)
    {
        /////////////////////////////////////////////
        /// Debug ground: vertices

        // Font for the text:
        auto text_font = sf::Font();
        // load it from linux file:
        text_font.loadFromFile(TTF_FONT_FILE);


        int i = 0;
        for (const auto &v: vecGround)
        {
            sf::CircleShape circle(0.1);
            circle.setPosition(v.x, v.y);
            circle.setFillColor(sf::Color::Red);
            window.draw(circle);

            // Draw the vertice index:
            sf::Text text;
            text.setFont(text_font);
            text.setString(to_string(i));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Black);
            text.setPosition(v.x, v.y);
            // text.setRotation(90);
            text.setScale(0.1f, -0.1f);
            window.draw(text);
            i++;
        }
    }

    void CGaBox2dView::_draw_ground(sf::RenderWindow &window) const
    {
        const CPen penGround(sf::Color(0, 0, 0), 0.08);
        const CSolidBrush bshTransparent(sf::Color::Transparent);
        const CSolidBrush bshGround(sf::Color(32, 128, 32));

        const vec_vecs_t vecGround = _document->GetGround();

        const size_t nSize = vecGround.size();
        if (nSize == 0)
            return;

        // Triangularização para preenchimento do polígono:
        // (Delunay Triangulation)
        auto triangulation = triangularize(vecGround);
        auto &cdt = triangulation.first;
        auto &vecVertices = triangulation.second;

        // const CPen penDebug(sf::Color(255, 0, 0), 0.1);
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

        if (m_bDrawDebugGround) _debug_draw_ground(window, vecGround);
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
        const auto doc = GetDocument();
        const auto &car = doc->GetCar();
        const auto center_mass = car->getCenter();
        const CEnv env = doc->GetEnv();
        constexpr float move_step = 1.0f;
        constexpr float zoom_step = 1.01f;
        if (m_bZoomOut) m_ZoomFactor = min(m_ZoomFactor * zoom_step, 8.0f);
        if (m_bZoomIn) m_ZoomFactor = max(m_ZoomFactor / zoom_step, 0.125f);
        if (m_bMoveLeft) m_view_pos.x -= move_step * m_ZoomFactor;
        if (m_bMoveRight) m_view_pos.x += move_step * m_ZoomFactor;
        if (m_bMoveUp) m_view_pos.y += move_step * m_ZoomFactor;
        if (m_bMoveDown) m_view_pos.y -= move_step * m_ZoomFactor;

        // View configuration
        const float view_x = m_view_pos.x + (m_bFollowCar ? center_mass.x : 0.0f);
        const float view_y = m_view_pos.y + (m_bFollowCar ? center_mass.y : 0.0f);
        sf::View view(sf::Vector2f(view_x, view_y),
                      sf::Vector2f(100 * m_ZoomFactor, -70 * m_ZoomFactor));
        view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
        window.setView(view);


        _draw_sky(window, env);
        _draw_ground(window);
        _draw_border(window, env);
        doc->GetCar()->draw(&window);
    }

    IGaBox2dDocPtr CGaBox2dView::GetDocument() const
    {
        return _document;
    }

    void CGaBox2dView::SetDocument(const IGaBox2dDocPtr doc)
    {
        _document = doc;
    }

    // CGaBox2dView message handlers
    void CGaBox2dView::OnSimulaPlay() const
    {
        const auto pDoc = GetDocument();
        if (pDoc->isSimulating())
        {
            pDoc->EndSimulation();
        }
        else
        {
            pDoc->BeginSimulation();
        }
    }

    void CGaBox2dView::OnSimulaReset() const
    {
        const auto doc = GetDocument();
        if (doc.get() == nullptr)
            return;

        doc->GetCar()->createGaRandomCar();
        doc->GetCar()->beginSimulate(doc->GetWorld());
    }

    void CGaBox2dView::setVelocidade(const unsigned nVelocidade)
    {
        m_nVelocidade = nVelocidade;
    }

    void CGaBox2dView::OnVelocidadeMais()
    {
        m_nVelocidade++;
    }

    void CGaBox2dView::OnVelocidadeMenos()
    {
        m_nVelocidade--;
    }

    void CGaBox2dView::OnSimulaRepetir() const
    {
        const auto doc = GetDocument();

        const string strGenes = doc->GetCar()->getGenes();

        if (doc->isSimulating()) OnSimulaPlay();

        doc->GetCar()->createGaFromGenes(strGenes);

        OnSimulaPlay();
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

    //     void CGaBox2dView::_stop_ga()
    //     {
    // #if 0
    //             // Já está rodando, então é para parar:
    //             if (AfxMessageBox("Tem certeza de que quer parar o GA?", MB_YESNO) == IDNO)
    //                 return;
    //
    //             // Ok, vamos parar:
    //             CMessageDlg dlgMsg;
    //             dlgMsg.BeginMessage("Interrompendo GA...", this);
    //             dlgMsg.EndMessage();
    // #endif
    //
    //         cout << "Commanding GA to stop..." << endl;
    //         _thread_params.m_bStopGa.store(true);
    //         _ga_thread.join();
    //         m_bGaRunning = false;
    //         cout << "GA stopped." << endl;
    //    }
    void CGaBox2dView::_stop_ga()
    {
        static bool showStopGaDialog = false;

        // If this is the first call, show the dialog
        if (!showStopGaDialog)
        {
            showStopGaDialog = true;
            return;
        }

        // Draw the confirmation dialog
        if (showStopGaDialog)
        {
            ImGui::OpenPopup("Stop GA?");

            // Center the popup
            // const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            // ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Stop GA?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Are you sure you want to stop the Genetic Algorithm?");
                ImGui::Separator();

                if (ImGui::Button("Yes", ImVec2(120, 0)))
                {
                    // User confirmed - stop the GA
                    cout << "Commanding GA to stop..." << endl;
                    _thread_params.m_bStopGa.store(true);
                    _ga_thread.join();
                    m_bGaRunning = false;
                    cout << "GA stopped." << endl;

                    showStopGaDialog = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("No", ImVec2(120, 0)))
                {
                    // User canceled
                    showStopGaDialog = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void CGaBox2dView::_start_ga(const ga_params_t &params)
    {
        _thread_params.m_Params = params;
        _thread_params.m_bStopGa = false;
        // _thread_params.m_wndNotify = m_hWnd;
        // _thread_params.m_pGaInfo = &m_GaInfo;
        _thread_params.m_env = GetDocument()->GetEnv();

        m_bGaRunning = true;
        cout << "Iniciando thread do GA..." << endl;
        _ga_thread = thread(fnGa, &_thread_params);
        startClient();
    }

    void CGaBox2dView::_show_start_ga_params()
    {
        _dlgGaParams.Show();
        // Obtemos os parâmetros do GA:
        // CGaParamsDlg dlgParams(getPtr());
        // dlgParams.OnInitDialog();
        // dlgParams.show();
    }

    void CGaBox2dView::OnGaIniciar()
    {
        if (m_bGaRunning)
        {
            // TODO: Show confirmation dialog
            return;
        }

        _show_start_ga_params();
    }

    void CGaBox2dView::OnEditCopy() const
    {
        ImGui::SetClipboardText(GetDocument()->GetCar()->getGenes().c_str());
    }

    void CGaBox2dView::OnEditPaste() const
    {
#ifdef WIN32
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
#endif
#ifdef IMGUI_VERSION
        const char *buffer = ImGui::GetClipboardText();
#endif
        try
        {
            const auto doc = GetDocument();
            doc->GetCar()->createGaFromGenes(buffer);
            doc->GetCar()->beginSimulate(doc->GetWorld());
        }
        catch (const std::length_error& e)
        {
            cerr << "Error pasting genes: " << e.what() << endl;
        }
    }

    void CGaBox2dView::OnMostrarMelhor()
    {
        const auto doc = GetDocument();

        if (current_status_.population.empty())
        {
            cerr << "Population is empty." << endl;
            return;
        }

        doc->GetCar()->createGaFromGenes(current_status_.bestGenes);
        OnSimulaRepetir();
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
        const auto doc = GetDocument();

        if (current_status_.population.empty())
        {
            cerr << "Population is empty." << endl;
            return;
        }

        auto it = std::next(
            current_status_.population.begin(),
            CRandom().discrete_random<long>(0, static_cast<long>(current_status_.population.size()) - 1)
        );

        doc->GetCar()->createGaFromGenes(it->second);
        OnSimulaRepetir();
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


    void CGaBox2dView::OnKeyPressed(void *pParam)
    {
        const auto key = *static_cast<sf::Keyboard::Key *>(pParam);
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
            case sf::Keyboard::F1:
                m_bShowHelp = !m_bShowHelp;
                break;
            case sf::Keyboard::F:
                toggleFollowCar();
                break;
            default:
                break;
        }
    }

    void CGaBox2dView::OnKeyReleased(void *pParam)
    {
        const auto key = *static_cast<sf::Keyboard::Key *>(pParam);
        const auto pDoc = GetDocument();
        switch (key)
        {
            case sf::Keyboard::Num1:
                OnVelocidade1x();
                break;
            case sf::Keyboard::Num2:
                OnVelocidade2x();
                break;
            case sf::Keyboard::Num4:
                OnVelocidade4x();
                break;
            case sf::Keyboard::Num0:
                OnVelocidade10x();
                break;
            case sf::Keyboard::Hyphen:
                OnVelocidadeMenos();
                break;
            case sf::Keyboard::Equal:
                OnVelocidadeMais();
                break;
            case sf::Keyboard::N:
                OnSimulaReset();
                break;
            case sf::Keyboard::R:
                OnSimulaRepetir();
                break;
            case sf::Keyboard::X:
                pDoc->Quit();
                break;
            case sf::Keyboard::Add:
                m_bZoomIn = false;
                break;

            case sf::Keyboard::C:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    OnEditCopy();
                }
                break;
            case sf::Keyboard::V:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    OnEditPaste();
                }
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
            case sf::Keyboard::M:
                OnMostrarMelhor();
                break;
            case sf::Keyboard::Q:
                OnMostrarQualquer();
                break;
            case sf::Keyboard::Space:
                OnSimulaPlay();
                break;
            default:
                break;
        }
    }

    void CGaBox2dView::updateData()
    {
        updateIdInfo();
        updateGaInfo();
    }

    void CGaBox2dView::draw(void *pParam)
    {
        _dlgGaParams.Render();
        _panelIdInfo.render();
        _panelGaInfo.render();

        const auto pWindow = static_cast<sf::RenderWindow *>(pParam);
        Draw(*pWindow);
    }

    string CGaBox2dView::getDeadReason() const
    {
        return GetDocument()->GetCar()->deadReason();
    }

    const ipc::GaStatus &CGaBox2dView::getCurrentStatus() const
    {
        return current_status_;
    }

    bool CGaBox2dView::_confirm_stop_ga()
    {
        return false;
    }

    void CGaBox2dView::startClient()
    {
        io_context_ = std::make_unique<boost::asio::io_context>();
        socket_ = std::make_unique<boost::asio::ip::tcp::socket>(*io_context_);
        connected_ = false;
        // Start a reconnection timer
        const auto timer = std::make_shared<boost::asio::steady_timer>(*io_context_, chrono::seconds(1));
        attemptConnect(timer);

        // Run IO context in separate thread
        client_thread_ = std::thread([this]
        {
            try
            {
                io_context_->run();
            } catch (const std::exception &e)
            {
                std::cerr << "Client error: " << e.what() << std::endl;
            }
        });
    }

    void CGaBox2dView::attemptConnect(const std::shared_ptr<boost::asio::steady_timer>& timer)
    {
        try
        {
            socket_->connect(boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address::from_string("127.0.0.1"), 9876));
            connected_ = true;

            std::cout << "Connected to GA server" << std::endl;

            // Start async read
            receive_buffer_.resize(BUFFER_SIZE);
            socket_->async_read_some(
                boost::asio::buffer(receive_buffer_),
                std::bind(&CGaBox2dView::handleRead, this,
                          std::placeholders::_1, std::placeholders::_2));
        } catch ([[maybe_unused]] const std::exception &e)
        {
            std::cerr << "Connection attempt failed, retrying in 1 second..." << std::endl;

            // Schedule reconnection attempt
            timer->expires_after(chrono::seconds(1));
            timer->async_wait([this, timer](const boost::system::error_code &error)
            {
                if (!error)
                {
                    attemptConnect(timer);
                }
            });
        }
    }

    void CGaBox2dView::handleRead(const boost::system::error_code &error, const size_t bytes_transferred)
    {
        if (error) return;

        const std::string data(receive_buffer_.begin(), receive_buffer_.begin() + bytes_transferred);

        try
        {
            if (status_serializer.deserializeGaStatus(data))
            {
                current_status_ = status_serializer.getStatus();
                // std::cout << "Received status: "
                //         << "Generation: " << current_status_.generation
                //         << ", GPS: " << current_status_.gps
                //         << ", Best Fitness: " << current_status_.bestFitness
                //         << ", History size: " << current_status_.best_history.size()
                //         << ", Best Genes: " << current_status_.bestGenes
                //         << std::endl;
            }

            // Schedule redraw or update your UI
            // In SFML you might want to set a flag that's checked in the main loop
        } catch ([[maybe_unused]] const std::exception &e)
        {
            cerr << e.what() << std::endl;
            cerr << "Error deserializing data" << std::endl;
            cerr << data << std::endl;
        }
        // Continue reading
        const auto buffers = boost::asio::buffer(receive_buffer_);
        auto handler = std::bind(
            &CGaBox2dView::handleRead,
            this,
            std::placeholders::_1,
            std::placeholders::_2);
        socket_->async_read_some(buffers, handler);
    }

    void CGaBox2dView::updateIdInfo()
    {
        const auto doc = GetDocument();

        constexpr int generation = -1;

        _panelIdInfo.set(generation,
                         doc->GetCar()->getFitness(),
                         doc->GetCar()->getT(),
                         doc->GetCar()->getGenes(),
                         doc->GetCar()->deadReason());

    }

    void CGaBox2dView::updateGaInfo()
    {
        _panelGaInfo.set(current_status_);
    }

    void CGaBox2dView::OnEditEnvironment()
    {
        // TODO: Code to show environment editor.
    }
}

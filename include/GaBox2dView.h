#pragma once

#include <thread>

#include <IGaBox2dView.h>
#include "GaBox2dDoc.h"
#undef CDT_USE_AS_COMPILED_LIBRARY

// ReSharper disable once CppUnusedIncludeDirective
#include <CDT.hpp>

#include <GaInfo.h>
#include <ga_ipc.h>
#include <thread_params.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "GaInfoDlg.h"
#include "GaParamsDlg.h"
#include "IdInfoDlg.h"

using namespace GA;

namespace GUI
{
    class CGaBox2dView final : public IGaBox2dView, public enable_shared_from_this<CGaBox2dView>
    {
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
        std::vector<char> receive_buffer_;
        bool connected_ = false;
        ipc::GaStatus current_status_{};
        std::thread client_thread_;
        const size_t BUFFER_SIZE = 40960;
        ipc::GaStatusSerializer status_serializer;

        CGaParamsDlg _dlgGaParams;
        CIdInfoDlg _panelIdInfo;
        CGaInfoDlg _panelGaInfo;

        void startClient();
        void attemptConnect(std::shared_ptr<boost::asio::steady_timer> timer);
        void handleRead(const boost::system::error_code& error, size_t bytes_transferred);

        void updateIdInfo();
        void updateGaInfo();
    public:
        CGaBox2dView();
        ~CGaBox2dView() override;

        void startGa(ga_params_t params) override;
        void SetDocument(IGaBox2dDocPtr doc) override;
        void OnEditCopy() const override;
        void OnEditPaste() const override;
        void OnSimulaPlay() const override;
        void OnSimulaReset() const override;
        void setVelocidade(unsigned nVelocidade) override;
        void OnSimulaRepetir() const override;
        void toggleDrawDebugGround() override;
        void ShowHelp() override;
        void toggleFollowCar() override;
        void OnGaIniciar() override;
        void OnKeyPressed(void *pParam) override;
        void OnKeyReleased(void *pParam) override;
        void updateData() override;
        void draw(void *pParam) override;
        void OnEditEnvironment() override;

        [[nodiscard]] unsigned getVelocidade() const override;
        [[nodiscard]] IGaBox2dDocPtr GetDocument() const override;
        [[nodiscard]] bool isGaRunning() const override;
        [[nodiscard]] bool isFollowCar() const override;
        [[nodiscard]] bool isDebugGround() const override;
        [[nodiscard]] bool isShowHelp() const override;
        [[nodiscard]] string getDeadReason() const override;
        [[nodiscard]] const ipc::GaStatus& getCurrentStatus() const override;

        bool _confirm_stop_ga();

        IGaBox2dViewPtr getPtr();
        // Attributes
    private:
        unsigned m_nVelocidade;
        bool m_bGaRunning;
        bool m_bGaExited;

        bool m_bShowInfoId;
        bool m_bShowInfoGaGenes = true;

        CThreadParams _thread_params;
        bool m_bWaitingEvolucao;
        thread _ga_thread;
        CGaInfo _ga_info;

        // UI:
        sf::Vector2f m_view_pos = {0.0f, 0.0f};
        float m_ZoomFactor = 1.0f;
        bool m_bZoomIn = false;
        bool m_bZoomOut = false;
        bool m_bMoveLeft = false;
        bool m_bMoveRight = false;
        bool m_bMoveUp = false;
        bool m_bMoveDown = false;
        bool m_bShowHelp = false;
        bool m_bDrawDebugGround = false;
        bool m_bFollowCar = true;

        IGaBox2dDocPtr _document;

    public:
        // Implementation
        static void _draw_sky(sf::RenderWindow &window, const CEnv &env);

        static void _debug_draw_ground(sf::RenderWindow &window, const vec_vecs_t &vecGround);

        void _draw_ground(sf::RenderWindow &window) const;

        static void _draw_border(sf::RenderWindow &window, const CEnv &env);

        void Draw(sf::RenderWindow &window);

        void _stop_ga();

        void _start_ga(const ga_params_t &params);

        void _show_start_ga_params();

        void OnVelocidadeMais();

        void OnVelocidadeMenos();

        void OnVelocidade1x();

        void OnVelocidade2x();

        void OnVelocidade4x();

        void OnVelocidade10x();

        void OnVelocidade100x();

        void OnMostrarMelhor();

        void OnMostrarQualquer();

        void OnInformaGagenes();

        void OnInformaIndividuoatual();

        void OnGaExtins();

        void OnGaColar();

        void OnInformaEvolu();


    };
}

#pragma once

#include <thread>

#include <IGaBox2dView.h>
#include "GaBox2dDoc.h"
#include <SFML/Window/Keyboard.hpp>
#undef CDT_USE_AS_COMPILED_LIBRARY
#include <CDT.hpp>

#include <GaInfoDlg.h>
#include <thread_params.h>

using namespace GA;

namespace GUI
{
    class CGaBox2dView final : public IGaBox2dView
    {
    public:
        CGaBox2dView();

        void startGa(ga_params_t params) override;

        [[nodiscard]] unsigned getVelocidade() const;

        void setVelocidade(const unsigned nVelocidade) // NOLINT(*-convert-member-functions-to-static)
        {
            m_nVelocidade = nVelocidade;
        }

        [[nodiscard]] bool isGaRunning() const;

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
        // CGaInfo			_ga_info;

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
        bool m_bFollowCar = false;

        CGaBox2dDoc *_pDocument;

    public:
        [[nodiscard]] CGaBox2dDoc *GetDocument() const;

        void SetDocument(CGaBox2dDoc *pDoc);

        [[nodiscard]] bool isShowHelp() const
        {
            return m_bShowHelp;
        }

        [[nodiscard]] bool isDebugGround() const
        {
            return m_bDrawDebugGround;
        }

        void toggleDrawDebugGround()
        {
            m_bDrawDebugGround = !m_bDrawDebugGround;
        }

        void ShowHelp();

        [[nodiscard]] bool isFollowCar() const;

        void toggleFollowCar();

        // Implementation
        static void _draw_sky(sf::RenderWindow &window, const CEnv &env);

        static void _debug_draw_ground(sf::RenderWindow &window, const vec_vecs_t &vecGround);

        void _draw_ground(sf::RenderWindow &window) const;

        static void _draw_border(sf::RenderWindow &window, const MODEL::CEnv &env);

        void Draw(sf::RenderWindow &window);

        void _stop_ga();

        void _start_ga(ga_params_t params);

        void _show_start_ga_params();

        void OnSimulaPlay() const;

        void OnSimulaReset() const;

        void OnVelocidadeMais();

        void OnVelocidadeMenos();

        void OnSimulaRepetir() const;

        void OnVelocidade1x();

        void OnVelocidade2x();

        void OnVelocidade4x();

        void OnVelocidade10x();

        void OnVelocidade100x();

        void OnGaIniciarga();

        void OnEditCopy() const;

        void OnEditPaste() const;

        void OnMostrarMelhor();

        void OnMostrarQualquer();

        void OnInformaGagenes();

        void OnInformaIndividuoatual();

        void OnGaExtins();

        void OnGaColar();

        void OnInformaEvolu();

        void OnKeyPressed(sf::Keyboard::Key key);

        void OnKeyReleased(sf::Keyboard::Key key);

        [[nodiscard]] string getDeadReason() const;
    };
}

#pragma once
#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <GaParamsDlg.h>
#include <EditorChaoDlg.h>
#include <IGaBox2dView.h>

#include "GaInfoDlg.h"


namespace GUI
{
    class CMainMenu
    {
        IGaBox2dViewPtr _view;

        void DrawMenu(CGaParamsDlg &dlgParams, CEditorChaoDlg &dlgEditorChao);
        void _show_info();
        void _show_ga_info(CGaInfoDlg &ga_info_dlg);
        static void _show_help();

    public:
        explicit CMainMenu(IGaBox2dViewPtr &view);
        void draw(
            CGaParamsDlg &dlgParams,
            CEditorChaoDlg &dlgEditorChao,
            CGaInfoDlg &dlgGaInfoDlg);
    };

    using CMainMenuPtr = std::shared_ptr<CMainMenu>;
}

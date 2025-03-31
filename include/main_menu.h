#pragma once
#include <GaParamsDlg.h>
#include <EditorChaoDlg.h>
#include <IGaBox2dView.h>
#include "GaInfoDlg.h"


namespace GUI
{
    class CMainMenu
    {
        IGaBox2dViewPtr _view;

        void _render_menu_file();
        void _render_menu_edit();
        void _render_menu_simulacao();
        void _render_menu_ga();
        void _render_menu_view();
        void _render_menu_help();

        void DrawMenu();

        void DrawDialogs();

        void _show_info();
        void _show_ga_info(CGaInfoDlg &ga_info_dlg);
        static void _show_help();

    public:
        explicit CMainMenu(IGaBox2dViewPtr &view);
        void draw(
        );
    };

    using CMainMenuPtr = std::shared_ptr<CMainMenu>;
}

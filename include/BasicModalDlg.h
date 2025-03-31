#pragma once

#include <IModalDialog.h>
#include <imgui.h>

class BasicModalDlg : public IModalDialog
{
    const char* _name = nullptr;
    bool _open = false;

protected:
    virtual void RenderContent() = 0;

public:

    explicit BasicModalDlg(const char* name)
        : _name(name) {}

    void Show() override
    {
        _open = true;
    }

    void Hide() override
    {
        _open = false;
        ImGui::CloseCurrentPopup();
    }

    [[nodiscard]] bool IsVisible() const override
    {
        return ImGui::IsPopupOpen(_name);
    }

    void Render() override
    {
        if (ImGui::BeginPopupModal(_name, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            RenderContent();
            ImGui::EndPopup();
        }
        if (_open) ImGui::OpenPopup(_name);
    }

    ~BasicModalDlg() override
    {
        _open = false;
    }

    void OnClose() override
    {
        _open = false;
    }
};

#pragma once

class IModalDialog
{
public:
    virtual ~IModalDialog() = default;

    virtual void Show() = 0;
    virtual void Hide() = 0;
    [[nodiscard]] virtual bool IsVisible() const = 0;
    virtual void Render() = 0;
    virtual void OnClose() = 0;
};
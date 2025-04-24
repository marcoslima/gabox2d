#pragma once

#include <BasicModalDlg.h>
#include <CarDef.h>
#include <functional>

class CCarEditorDlg final : public BasicModalDlg
{
public:
    using callback_t = std::function<void(const MODEL::CCarDef&)>;

private:
    MODEL::CCarDef _carDef;
    callback_t _callback;
    bool _autoRun{false};


public:
    explicit CCarEditorDlg(const callback_t &fn_callback)
        : BasicModalDlg("Car Editor")
        , _callback(fn_callback)
    {
    }

    void RenderContent() override;
};
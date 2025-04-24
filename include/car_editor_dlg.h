#pragma once

#include <BasicModalDlg.h>
#include <CarDef.h>
#include <functional>
#include <genoma_image.h>

class CCarEditorDlg final : public BasicModalDlg
{
public:
    using callback_t = std::function<void(const MODEL::CCarDef&)>;

private:
    MODEL::CCarDef _carDef;
    callback_t _callback;
    bool _autoRun{false};
    CGenomaImage _genomaImage{static_cast<unsigned int>(MODEL::CCarDef::bits::len())};

public:
    explicit CCarEditorDlg(const callback_t &fn_callback)
        : BasicModalDlg("Car Editor")
        , _callback(fn_callback)
    {
    }

    void setFromGenes(const std::string &genes)
    {
        _carDef = MODEL::CCarDef(genes);
    }

    void RenderContent() override;
};
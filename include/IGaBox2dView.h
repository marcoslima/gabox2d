#pragma once

#include <ga_params.h>
#include <memory>
#include <IGaBox2dDoc.h>
#include <ga_status.h>

namespace GUI
{
    class IGaBox2dView
    {
    public:
        virtual ~IGaBox2dView() = default;
        virtual void SetDocument(IGaBox2dDocPtr) = 0;

        // Message handlers
        virtual void OnEditCopy() const = 0;
        virtual void OnEditPaste() const = 0;
        virtual void OnSimulaPlay() const = 0;
        virtual void OnSimulaReset() const = 0;
        virtual void OnSimulaRepetir() const = 0;
        virtual void OnGaIniciar() = 0;
        virtual void OnMostrarMelhor() = 0;
        virtual void OnKeyPressed(void *pParam) = 0;
        virtual void OnKeyReleased(void *pParam) = 0;
        virtual void OnEditEnvironment() = 0;
        virtual void OnMostrarQualquer() = 0;
        virtual void OnToggleShowParams() = 0;
        virtual void OnGenericLabs() = 0;
        virtual void OnShowDemo() = 0;
        virtual void OnShowPlot() = 0;
        virtual void OnShowPlotDemo() = 0;

        virtual void startGa(ga_params_t) = 0;
        virtual void setVelocidade(unsigned) = 0;
        virtual void toggleDrawDebugGround() = 0;
        virtual void ShowHelp() = 0;
        virtual void toggleFollowCar() = 0;
        virtual void updateData() = 0;
        virtual void draw(void *pParam) = 0;

        [[nodiscard]] virtual IGaBox2dDocPtr GetDocument() const = 0;
        [[nodiscard]] virtual unsigned getVelocidade() const = 0;
        [[nodiscard]] virtual bool isGaRunning() const = 0;
        [[nodiscard]] virtual bool isFollowCar() const = 0;
        [[nodiscard]] virtual bool isDebugGround() const = 0;
        [[nodiscard]] virtual bool isShowHelp() const = 0;
        [[nodiscard]] virtual string getDeadReason() const = 0;
        [[nodiscard]] virtual const ipc::GaStatus& getCurrentStatus() const = 0;
        [[nodiscard]] virtual bool isShowDemo() const = 0;
        [[nodiscard]] virtual bool isShowPlot() const = 0;
        [[nodiscard]] virtual bool isShowPlotDemo() const = 0;
    };

    using IGaBox2dViewPtr = std::shared_ptr<IGaBox2dView>;
}
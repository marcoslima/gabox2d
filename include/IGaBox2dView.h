#pragma once

#include <ga_params.h>
#include <memory>
#include <IGaBox2dDoc.h>
#include <ga_ipc.h>

namespace GUI
{
    class IGaBox2dView
    {
    public:
        virtual ~IGaBox2dView() = default;
        virtual void SetDocument(IGaBox2dDocPtr) = 0;;
        virtual void startGa(ga_params_t) = 0;
        virtual void OnEditCopy() const = 0;
        virtual void OnEditPaste() const = 0;
        virtual void OnSimulaPlay() const = 0;
        virtual void OnSimulaReset() const = 0;
        virtual void setVelocidade(unsigned) = 0;
        virtual void OnSimulaRepetir() const = 0;
        virtual void toggleDrawDebugGround() = 0;
        virtual void ShowHelp() = 0;
        virtual void toggleFollowCar() = 0;
        virtual void OnGaIniciar() = 0;
        virtual void OnKeyPressed(void *pParam) = 0;
        virtual void OnKeyReleased(void *pParam) = 0;
        virtual void updateData() = 0;
        virtual void draw(void *pParam) = 0;
        virtual void OnEditEnvironment() = 0;

        [[nodiscard]] virtual IGaBox2dDocPtr GetDocument() const = 0;
        [[nodiscard]] virtual unsigned getVelocidade() const = 0;
        [[nodiscard]] virtual bool isGaRunning() const = 0;
        [[nodiscard]] virtual bool isFollowCar() const = 0;
        [[nodiscard]] virtual bool isDebugGround() const = 0;
        [[nodiscard]] virtual bool isShowHelp() const = 0;
        [[nodiscard]] virtual string getDeadReason() const = 0;
        [[nodiscard]] virtual const ipc::GaStatus& getCurrentStatus() const = 0;
    };

    using IGaBox2dViewPtr = std::shared_ptr<IGaBox2dView>;
}
#pragma once
#include "systems/system.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

using namespace trafficSimulation::events;

namespace trafficSimulation::systems {
    class CameraSystem : public System {
      protected:
        entt::entity cameraEntity;        

        float yaw = -90.0f;
        float pitch = 0.0f;

        int inputX = 0, inputZ = 0;

        virtual void init() override;

      public:
        CameraSystem(Application* app);

        virtual void update(int dt) override;

        void onMouseMove(const MouseMoveEvent& e);
        void onKey(const KeyEvent& e);
        void onFramebufferSize(const FramebufferSizeEvent& e);
    };
} // namespace trafficSimulation::systems
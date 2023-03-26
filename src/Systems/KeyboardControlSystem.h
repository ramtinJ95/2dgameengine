#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include <memory>
#include <string>

class KeyboardControlSystem: public System {
public:
    KeyboardControlSystem() {
	RequireComponent<SpriteComponent>();
	RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventbus) {
	eventbus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event) {
	std::string keyCode = std::to_string(event.symbol);
	std::string keySymbol(1, event.symbol);
	Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
    }
    void Update() {
    }
};


#endif

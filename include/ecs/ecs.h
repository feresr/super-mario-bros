#pragma once

#include <vector>
#include <bitset>
#include <typeindex>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <SDL_events.h>

class System;

class World;

struct Component {
    virtual ~Component() = default;
};

class Entity {
public:

    Entity() : components{new std::unordered_map<std::type_index, Component*>()} {};

    Entity(const Entity& other) = delete;

    Entity(Entity&& other) noexcept {
        this->components = other.components;
        other.components = nullptr;
    }

    Entity& operator=(Entity&& other) noexcept {
        if (this == &other) return *this;
        this->components = other.components;
        other.components = nullptr;
        return *this;
    }

    template<typename ComponentType, typename... Args>
    void assign(Args&& ... arguments) {
        // std::forward will detect whether arguments is an lvalue or rvalue and perform a copy or move, respectively.
        auto* component = new ComponentType(std::forward<Args&&>(arguments)...);
        auto typeIndex = std::type_index(typeid(ComponentType));
        components->insert_or_assign(typeIndex, component);
    }

    template<typename ComponentType>
    bool remove() {
        auto index = std::type_index(typeid(ComponentType));
        return components->erase(index);
    }

    bool clearComponents() {
        components->clear();
        return true;
    }

    template<typename C>
    C* get() {
        return static_cast<C*>((*components)[std::type_index(typeid(C))]);
    }

    template<typename C>
    [[nodiscard]] bool has() const {
        if (components->empty()) return false;
        auto index = std::type_index(typeid(C));
        return components->find(index) != components->end();
    }

    template<typename A, typename B, typename... OTHERS>
    [[nodiscard]] bool has() const {
        return has<A>() && has<B, OTHERS...>();
    }

    template<typename A, typename B, typename... OTHERS>
    [[nodiscard]] bool hasAny() const {
        return has<A>() || has<B, OTHERS...>();
    }

    ~Entity() {
        for (auto& component : *components) delete component.second;
        components->clear();
    }

private:
    std::unordered_map<std::type_index, Component*>* components;
};

class System {

    friend class World;

public:
    virtual void onAddedToWorld(World* world) {}

    virtual void tick(World* world) = 0;

    virtual void handleEvent(SDL_Event& event) {}

    virtual void onRemovedFromWorld(World* world) {}

    virtual ~System() = default;


};

class World {
public:
    World() = default;

    World(const World& other) = delete;

    Entity* create() {
        auto* e = new Entity();
        entities.push_back(e);
        return e;
    }

    void destroy(Entity* entity) {
        if (!entity) return;
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        delete entity;
    }

    template <typename S, typename... Args>
    S* registerSystem(Args&& ... arguments) {
        auto system = new S(std::forward<Args&&>(arguments)...);
        systems.push_back(system);
        system->onAddedToWorld(this);
        return system;
    }

    void unregisterSystem(System* system) {
        if (!system) return;
        systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
        system->onRemovedFromWorld(this);
        delete system;
    }

    ~World() {
        for (auto system : systems) {
            system->onRemovedFromWorld(this);
            delete system;
        }
        systems.clear();

        for (auto entity : entities) delete entity;
        entities.clear();
    }

    template<typename... Components>
    Entity* findFirst() {
        auto found = std::find_if(
                entities.begin(),
                entities.end(),
                [&](const Entity* s) { return s->has<Components...>(); }
        );
        return found != entities.end() ? *found : nullptr;
    }

    template<typename... Components>
    [[deprecated("Replaced by find(std::function<>), which has better performance")]]
    std::vector<Entity*> find() {
        std::vector<Entity*> result;
        std::copy_if(
                entities.begin(),
                entities.end(),
                std::back_inserter(result),
                [&](const Entity* s) { return s->has<Components...>(); }
        );
        return result;
    }

    template<typename... Components>
    void find(const std::function<void(Entity*)>& callback) {
        for (auto entity : entities) if (entity->has<Components...>()) callback(entity);
    }

    template<typename... Components>
    void findAny(const std::function<void(Entity*)>& callback) {
        for (auto entity : entities) if (entity->hasAny<Components...>()) callback(entity);
    }

    template<typename... Components>
    std::vector<Entity*> findAny() {
        //todo: optimize, this should return an iterator and not a new vector
        std::vector<Entity*> result;
        std::copy_if(
                entities.begin(),
                entities.end(),
                std::back_inserter(result),
                [&](const Entity* s) { return s->hasAny<Components...>(); }
        );
        return result;
    }

    void tick() {
        for (auto system : systems) system->tick(this);
    }

    void handleEvent(SDL_Event& event) {
        for (auto system : systems) system->handleEvent(event);
    }

private:
    std::vector<Entity*> entities;
    std::vector<System*> systems;
};
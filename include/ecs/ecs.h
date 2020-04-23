#pragma once

#include <vector>
#include <bitset>
#include <typeindex>
#include <algorithm>
#include <unordered_map>
#include <iostream>

class System;

class World;

class Component {

public:
    virtual ~Component() = default;
};

class Entity {
public:
    template<typename ComponentType, typename... Args>
    void assign(Args&& ... arguments) {
        auto* component = new ComponentType(arguments...);
        auto typeIndex = std::type_index(typeid(ComponentType));
        components.insert_or_assign(typeIndex, component);
    }

    template<typename ComponentType>
    bool remove() {
        auto index = std::type_index(typeid(ComponentType));
        auto found = components.find(index);
        if (found != components.end()) {
            delete found->second;
            components.erase(found);
            return true;
        }

        return false;
    }

    template<typename C>
    C* get() {
        return static_cast<C*>(components[std::type_index(typeid(C))]);
    }

    template<typename C>
    [[nodiscard]] bool has() const {
        auto index = std::type_index(typeid(C));
        return components.find(index) != components.end();
    }

    template<typename A, typename B, typename... OTHERS>
    [[nodiscard]] bool has() const {
        return has<A>() && has<B, OTHERS...>();
    }

    ~Entity();

private:
    std::unordered_map<std::type_index, Component*> components;
};

class System {
public:
    virtual void onAddedToWorld(World* world) {}

    virtual void tick(World* world) = 0;

    virtual void onRemovedFromWorld(World* world) {}

    virtual ~System() = default;
};

class World {
public:
    World() = default;

    World(const World& other) = delete;

    Entity* create();

    void registerSystem(System* system);

    void unregisterSystem(System* system);

    ~World();

    template<typename... Components>
    std::vector<Entity*> find() {
        // todo: Optimize, avoid creating a new vector an returning it by copy
        std::vector<Entity*> result;
        std::copy_if(
                entities.begin(),
                entities.end(),
                std::back_inserter(result),
                [&](const Entity* s) { return s->has<Components...>(); }
        );
        return result;
    }

    void tick();

private:
    std::vector<Entity*> entities;
    std::vector<System*> systems;
};
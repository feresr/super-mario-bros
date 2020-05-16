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

    template<typename ComponentType, typename... Args>
    void assign(Args&& ... arguments) {
        // std::forward will detect whether arguments is an lvalue or rvalue and perform a copy or move, respectively.
        auto* component = new ComponentType(std::forward<Args&&>(arguments)...);
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

    bool clearComponents() {
        components.clear();
        return true;
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

    template<typename A, typename B, typename... OTHERS>
    [[nodiscard]] bool hasAny() const {
        return has<A>() || has<B, OTHERS...>();
    }

    ~Entity();

private:
    std::unordered_map<std::type_index, Component*> components;
};

class System {
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

    Entity* create();

    void destroy(Entity* entity);

    void registerSystem(System* system);

    void unregisterSystem(System* system);

    ~World();

    template<typename... Components>
    //TODO this should return the component? to avoid world->findFirst<TileComponent>()->get<TileComponent>()
    Entity* findFirst() {
        auto found = std::find_if(
                entities.begin(),
                entities.end(),
                [&](const Entity* s) { return s->has<Components...>(); }
        );
        return found != entities.end() ? *found : nullptr;
    }

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

    template<typename... Components>
    std::vector<Entity*> findAny() {
        // todo: Optimize, avoid creating a new vector an returning it by copy
        std::vector<Entity*> result;
        std::copy_if(
                entities.begin(),
                entities.end(),
                std::back_inserter(result),
                [&](const Entity* s) { return s->hasAny<Components...>(); }
        );
        return result;
    }

    void tick();

    void handleEvent(SDL_Event& event);

private:
    std::vector<Entity*> entities;
    std::vector<System*> systems;
    // TODO: use std::vector<std::unique_ptr<System>> systems; to
    // make the fact that World owns the systems more explicit
    // https://stackoverflow.com/questions/45087944/how-to-indicate-c-ownership-of-pointer
};
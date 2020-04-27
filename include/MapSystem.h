#pragma once

#include "ecs/ecs.h"
#include <queue>
#include "Components.h"
#include <fstream>
#include <exception>

struct Tile {
    int x, y, w, h;
    const std::string texture;
};

class Map {

public:
    Map() = delete;

    explicit Map(std::string&& mapPath) {
        std::cout << "Parsing XML" << std::endl;
        std::ifstream infile(mapPath);
        if (!infile) throw std::invalid_argument("Map path not found: " + mapPath);

        char a;
        int x = 0, y = 0;
        int w = 16;
        int h = 16;

        auto marioTexture = std::string("assets/mario.jpg");

        while (infile.get(a)) {
            if (a == '\n') {
                y += h;
                x = 0;
                continue;
            }
            if (a == '1') tiles.push(new Tile{x, y, w, h, marioTexture});
            x += w;
        }
        infile.close();
    }

    std::queue<Tile*> tiles;
};

class MapSystem : public System {
public:

    MapSystem();

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~MapSystem() override;

private:
    Map map;
};
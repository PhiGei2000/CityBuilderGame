#include <iostream>

#include "streetBuilder.hpp"
#include "world.hpp"
#include "rendering/renderer.hpp"

using namespace trafficSimulation;

int main(int, char**) {    
    Renderer::init();

    Renderer renderer;
    renderer.createWindow(800, 600, "Traffic Simulation");    

    World world;
    world.init();

    StreetBuilder builder(&world);
    Path p;
    p.emplace_back(0, 31);
    p.emplace_back(63, 31);
    p.emplace_back(63, 0);
    p.emplace_back(31, 0);
    p.emplace_back(31, 63);

    builder.build(p);

    renderer.render(&world);

    world.destroy();
}

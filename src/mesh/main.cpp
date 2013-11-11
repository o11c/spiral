#include <iostream>

#include "../yaml/dumb.hpp"

int main()
{
    YamlMesh ym = silly_parse(std::cin);
    std::cout << "textures:\n";
    for (auto kv : ym.textures)
        std::cout << "  " << kv.first << ": " << kv.second.filename << '\n';
    std::cout << "\nvertices:\n";
    for (auto v : ym.vertices)
        std::cout
            << "  - position: [" << v.position.x << ", " << v.position.y << ", " << v.position.z << ", " << v.position.w << "]\n"
            << "    texture: ["  << v.texture.x << ", " << v.texture.y << ", " << v.texture.z << ", " << v.texture.w << "]\n"
            << "    normal: ["   << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << ", " << v.normal.w << "]\n";
    std::cout << "\nfaces:\n";
    for (auto f : ym.faces)
        std::cout
            << "  - vertices: [" << f.vertices.x << ", " << f.vertices.y << ", " << f.vertices.z << "]\n"
            << "    ambient: "  << f.ambient  << '\n'
            << "    diffuse: "  << f.diffuse  << '\n'
            << "    specular: " << f.specular << '\n';
    std::cout << std::flush;
}

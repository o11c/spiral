#ifndef YAML_DUMB_HPP
#define YAML_DUMB_HPP

#include <istream>
#include <map>
#include <string>
#include <vector>

#include "../math/vector.hpp"

// this is a quick and dirty parser for a subset of YAML files

struct YamlTexture
{
    std::string filename;
};

struct YamlVertex
{
    vec4 position;
    vec4 texture;
    vec4 normal;
};

struct YamlFace
{
    ivec3 vertices;
    std::string ambient;
    std::string diffuse;
    std::string specular;
};

struct YamlMesh
{
    std::map<std::string, YamlTexture> textures;
    std::vector<YamlVertex> vertices;
    std::vector<YamlFace> faces;
};

YamlMesh silly_parse(std::istream& in);

#endif //YAML_DUMB_HPP

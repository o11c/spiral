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
};

struct YamlMesh
{
    std::map<std::string, YamlTexture> textures;
    // TODO add shininess
    std::vector<YamlVertex> vertices;
    std::vector<YamlFace> faces;
};

struct YamlOrientedMesh
{
    YamlMesh mesh;
    vec3 position;
    float scale;
    vec3 orient_axis;
    float orient_angle;
};

struct YamlLight
{
    vec3 position;
    vec3 color;
};

struct YamlMulti
{
    std::vector<YamlOrientedMesh> meshes;
};

struct YamlScene
{
    YamlMulti multi;
    YamlLight light;
    vec3 camera, look;
};

YamlMesh parse_mesh(std::istream& in);
inline
YamlMesh parse_mesh(std::istream&& in) { return parse_mesh(in); }

YamlMulti parse_multi(std::istream& in);
inline
YamlMulti parse_multi(std::istream&& in) { return parse_multi(in); }

YamlScene parse_scene(std::istream& in);
inline
YamlScene parse_scene(std::istream&& in) { return parse_scene(in); }

#endif //YAML_DUMB_HPP

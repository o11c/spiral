#include "dumb.hpp"

#include <fstream>

static __attribute__((noreturn))
void fail(const char *msg)
{
    fprintf(stderr, "Fatal: %s\n", msg);
    exit(1);
}

static
float parse_float(std::string line)
{
    const char *start = line.c_str();
    char *end;
    float out = strtof(start, &end);
    if (start == end)
        fail("float");
    if (*end)
        fail("sink");
    return out;
}

static
vec3 parse_vec3(std::string line)
{
    vec3 out;
    if (line.front() != '[' || line.back() != ']')
        fail("expect direct list");
    line.back() = ',';
    const char *start = line.c_str() + 1;
    char *end;
    for (int i = 0; i < 3; ++i)
    {
        out.data[i] = strtof(start, &end);
        if (start == end)
            fail("expect float");
        if (*end != ',')
            fail("expect sep");
        start = end + 1;
    }
    if (end != &line.back())
        fail("expect no more");
    return out;
}

static
vec4 parse_vec4(std::string line)
{
    vec4 out;
    if (line.front() != '[' || line.back() != ']')
        fail("expect direct list");
    line.back() = ',';
    const char *start = line.c_str() + 1;
    char *end;
    for (int i = 0; i < 4; ++i)
    {
        out.data[i] = strtof(start, &end);
        if (start == end)
            fail("expect float");
        if (*end != ',')
            fail("expect sep");
        start = end + 1;
    }
    if (end != &line.back())
        fail("expect no more");
    return out;
}

static
ivec3 parse_ivec3(std::string line)
{
    ivec3 out;
    if (line.front() != '[' || line.back() != ']')
        fail("expect direct list");
    line.back() = ',';
    const char *start = line.c_str() + 1;
    char *end;
    for (int i = 0; i < 3; ++i)
    {
        out.data[i] = strtoul(start, &end, 10);
        if (start == end)
            fail("expect int");
        if (*end != ',')
            fail("expect sep");
        start = end + 1;
    }
    if (end != &line.back())
        fail("expect no more");
    return out;
}

YamlMesh parse_mesh(std::istream& in)
{
    enum class Mode
    {
        TEXTURES,
        VERTICES,
        FACES,
        INIT
    } mode = Mode::INIT;
    YamlMesh out;
    std::string line;
    while (std::getline(in, line))
    {
        line = line.substr(0, line.find('#'));
        size_t first = line.find_first_not_of(" ");
        if (first == std::string::npos)
            continue;
        if (line[first] == '-')
        {
            line[first] = ' ';
            switch (mode)
            {
            case Mode::VERTICES: out.vertices.emplace_back(); break;
            case Mode::FACES: out.faces.emplace_back(); break;
            default: fail("unexpected list");
            }
            first = line.find_first_not_of(" ");
            if (first == std::string::npos)
                continue;
        }
        line = line.substr(first);
        if (first == 0)
        {
            size_t colon = line.find(':');
            if (colon == std::string::npos)
                fail("expect map");
            if (line.find_first_not_of(" ", colon + 1) != std::string::npos)
                fail("expect indirect");
            line = line.substr(0, colon);
            if (line == "textures")
                mode = Mode::TEXTURES;
            else if (line == "vertices")
                mode = Mode::VERTICES;
            else if (line == "faces")
                mode = Mode::FACES;
            else
                fail("bad mode");
        }
        else
        {
            if (mode == Mode::FACES)
            {
                if (line.find(':') != std::string::npos)
                    fail("unexpect submap");
                if (out.vertices.empty())
                    fail("expect list");
                out.faces.back().vertices = parse_ivec3(line);
                continue;
            }
            size_t colon = line.find(':');
            if (colon == std::string::npos)
                fail("expect submap");
            size_t nspc = line.find_first_not_of(" ", colon + 1);
            if (nspc == std::string::npos)
                fail("expect direct");
            std::string key = line.substr(0, colon);
            std::string value = line.substr(nspc);
            switch (mode)
            {
            case Mode::TEXTURES:
                if (!out.textures.insert({key, YamlTexture{value}}).second)
                    fail("dup tex");
                break;
            case Mode::VERTICES:
                if (out.vertices.empty())
                    fail("expect list");
                if (key == "position")
                    out.vertices.back().position = parse_vec4(value);
                else if (key == "texture")
                    out.vertices.back().texture = parse_vec4(value);
                else if (key == "normal")
                    out.vertices.back().normal = parse_vec4(value);
                else
                    fail("bad subkey");
                break;
            default:
                fail("unexpected indent");
            }
        }
    }
    return out;
}

YamlMulti parse_multi(std::istream& in)
{
    YamlMulti out;
    std::string line;
    while (std::getline(in, line))
    {
        line = line.substr(0, line.find('#'));
        size_t first = line.find_first_not_of(" ");
        if (first == std::string::npos)
            continue;
        if (line[first] == '-')
        {
            line[first] = ' ';
            if (first)
                fail("indented list");
            out.meshes.emplace_back();
            first = line.find_first_not_of(" ");
            if (first == std::string::npos)
                continue;
        }
        if (out.meshes.empty())
            fail("untangled");
        line = line.substr(first);
        if (!first)
            fail("unindented key");
        size_t colon = line.find(':');
        if (colon == std::string::npos)
            fail("I'm lost");
        size_t nspc = line.find_first_not_of(" ", colon + 1);
        if (nspc == std::string::npos)
            fail("not an entity");
        std::string key = line.substr(0, colon);
        std::string value = line.substr(nspc);
        if (key == "mesh")
            out.meshes.back().mesh = parse_mesh(std::ifstream(value));
        else if (key == "position")
            out.meshes.back().position = parse_vec3(value);
        else if (key == "scale")
            out.meshes.back().scale = parse_float(value);
        else if (key == "about")
            out.meshes.back().orient_axis = parse_vec3(value);
        else if (key == "angle")
            out.meshes.back().orient_angle = parse_float(value);
        else
            fail("locked");
    }
    return out;
}

YamlMirror parse_mirror(std::istream& in)
{
    YamlMirror out;
    std::string line;
    while (std::getline(in, line))
    {
        line = line.substr(0, line.find('#'));
        size_t first = line.find_first_not_of(" ");
        if (first == std::string::npos)
            continue;
        if (line[first] == '-')
            fail("shatter");
        line = line.substr(first);
        if (first)
            fail("shard");

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            fail("fuzzy");
        size_t nspc = line.find_first_not_of(" ", colon + 1);
        if (nspc == std::string::npos)
            fail("foggy");
        std::string key = line.substr(0, colon);
        std::string value = line.substr(nspc);
        if (key == "surface")
            out.onto = parse_mesh(std::ifstream(value));
        else if (key == "multi")
            out.multi = parse_multi(std::ifstream(value));
        else
            fail("danger");
    }
    return out;
}

YamlScene parse_scene(std::istream& in)
{
    YamlScene out;
    std::string line;
    while (std::getline(in, line))
    {
        line = line.substr(0, line.find('#'));
        size_t first = line.find_first_not_of(" ");
        if (first == std::string::npos)
            continue;
        if (line[first] == '-')
            fail("discharge");
        line = line.substr(first);
        if (first)
            fail("datcharge");

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            fail("boring");
        size_t nspc = line.find_first_not_of(" ", colon + 1);
        if (nspc == std::string::npos)
            fail("moron");
        std::string key = line.substr(0, colon);
        std::string value = line.substr(nspc);
        if (key == "mirror")
            out.mirror = parse_mirror(std::ifstream(value));
        else if (key == "camera")
            out.camera = parse_vec3(value);
        else if (key == "look")
            out.look = parse_vec3(value);
        else if (key == "light_position")
            out.light.position = parse_vec3(value);
        else if (key == "light_color")
            out.light.color = parse_vec3(value);
        else
            fail("safe");
    }
    return out;
}

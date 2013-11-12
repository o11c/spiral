#include "dumb.hpp"

static __attribute__((noreturn))
void fail(const char *msg)
{
    fprintf(stderr, "Fatal: %s\n", msg);
    exit(1);
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

YamlMesh silly_parse(std::istream& in)
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

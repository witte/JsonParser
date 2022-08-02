#include "JsonParser.h"
#include <charconv>


namespace
{

std::string_view parseQuotedToken(const std::string_view sv, size_t& index)
{
    if (sv[index] == '"')
        ++index;

    size_t from = index;
    size_t to = index;

    for (; index < sv.size(); ++index)
    {
        char c = sv[index];

        if (c == '"')
        {
            if (sv[index - 1] == '\\')
                continue;

            to = index;
            ++index;
            break;
        }
    }

    return sv.substr(from, (to - from));
}

std::string_view parseUnquotedToken(const std::string_view sv, size_t& index)
{
    size_t from = index;
    index = sv.find_first_of(",\n", index);

    return sv.substr(from);
}

std::string_view parseToken(const std::string_view sv, size_t &index)
{
    index = sv.find_first_not_of(" \n", index);

    return (sv[index] == '"')? parseQuotedToken(sv, index) : parseUnquotedToken(sv, index);
}

std::string_view parseUnquotedName(const std::string_view sv, size_t& index)
{
    size_t from = index;
    index = sv.find_first_of(": ", index);

    return sv.substr(from, (index - from));
}

std::string_view parseName(const std::string_view sv, size_t& index)
{
    index = sv.find_first_not_of(" ,\n", index);

    if (index >= sv.size())
        return {};

    return (sv[index] == '"')? parseQuotedToken(sv, index) : parseUnquotedName(sv, index);
}

}


namespace Parser
{


template <>
void parse<bool>(std::string_view sv, size_t& index, bool& member)
{
    index = sv.find_first_not_of(" ", index);
    if (sv[index] == '"')
        ++index;

    size_t from = index;
    index = sv.find_first_not_of("truefalseonffyTRUEFALSONFFY", index);

    auto token = sv.substr(from, (index - from));

    if (   (token == "true")
        || (token == "TRUE")
        || (token == "yes")
        || (token == "YES")
        || (token == "on")
        || (token == "ON"))
    {
        member = true;
    }
    else if (   (token == "false")
             || (token == "FALSE")
             || (token == "no")
             || (token == "NO")
             || (token == "off")
             || (token == "OFF"))
    {
        member = false;
    }

    if (sv[index] == '"')
        ++index;
}

template <>
void parse<int>(std::string_view sv, size_t& index, int& member)
{
    auto token = parseToken(sv, index);

    auto result = std::from_chars(token.data(), token.data() + token.size(), member);

    if (result.ec == std::errc::invalid_argument)
    {
        std::string err = "could not convert '";
        err += sv;
        err += "' to int";

        throw std::runtime_error{err};
    }
}

template <>
void parse<std::string>(std::string_view sv, size_t& index, std::string& member)
{
    auto token = parseToken(sv, index);

    member = std::string{token};
}

void parseMember(const std::string_view name, const std::string_view, size_t&, void*)
{
    std::cout << "member not found: " << name << std::endl;
}


void parseMessage(const std::string_view sv, size_t& index, void* instance,
                  void (*func)(const std::string_view, const std::string_view, size_t&, void*))
{
    size_t end = sv.size();

    index = sv.find_first_of("{", index);
    if (index >= end)
        return;

    ++index;

    for (; index < end; ++index)
    {
        auto currentName = parseName(sv, index);
        // ñapa
        if (currentName[0] == '}')
            return;

        if (index >= end)
            return;


        index = sv.find_first_of(':', index);
        if (index >= end)
            return;

        ++index;

        func(currentName, sv, index, instance);
    }
}


}

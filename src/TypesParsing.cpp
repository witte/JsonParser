#include "TypesParsing.h"
#include <charconv>
#include "Tokens.h"


namespace Parser
{


template <>
void parse<bool>(const std::string_view sv, size_t& index, bool& member)
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
void parse<int>(const std::string_view sv, size_t& index, int& member)
{
    auto token = getTokenBounds(sv, index);

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
void parse<std::string>(const std::string_view sv, size_t& index, std::string& member)
{
    auto token = getTokenBounds(sv, index);

    member = std::string{token};
}


}

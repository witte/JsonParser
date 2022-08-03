#include "Tokens.h"


namespace Parser
{


std::string_view getQuotedTokenBounds(const std::string_view sv, size_t &index)
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

std::string_view getUnquotedTokenBounds(const std::string_view sv, size_t &index)
{
    size_t from = index;
    index = sv.find_first_of(",\n", index);

    return sv.substr(from);
}

std::string_view getTokenBounds(const std::string_view sv, size_t &index)
{
    index = sv.find_first_not_of(" \n", index);

    return (sv[index] == '"')? getQuotedTokenBounds(sv, index) : getUnquotedTokenBounds(sv, index);
}

std::string_view getUnquotedNameBounds(const std::string_view sv, size_t &index)
{
    size_t from = index;
    index = sv.find_first_of(": ", index);

    return sv.substr(from, (index - from));
}

std::string_view getNameBounds(const std::string_view sv, size_t &index)
{
    index = sv.find_first_not_of(" ,\n", index);

    if (index >= sv.size())
        return {};

    return (sv[index] == '"')? getQuotedTokenBounds(sv, index) : getUnquotedNameBounds(sv, index);
}


}

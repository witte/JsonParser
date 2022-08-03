#include "JsonParser.h"
#include <iostream>
#include "Tokens.h"


namespace Parser
{


void printPadding(int padding)
{
    for (int p = 0; p < padding; ++p)
    {
        std::cout << " ";
    }
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
        auto currentName = getNameBounds(sv, index);
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


template <>
void toString(const std::string& value, int)
{
    std::cout << "\"" << value << "\"," << std::endl;
}


}

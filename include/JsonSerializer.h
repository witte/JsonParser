#pragma once
#include <iostream>


namespace Parser
{


template <typename T>
void toString(const T& value, int padding = 0)
{
    (void)padding;
    std::cout << value << std::endl;
}

template <typename T>
void instanceToString(const T&, int)
{
}

template <typename T, typename MemberType, typename... Args>
void instanceToString(const T& member, int padding, const std::string_view memberName, MemberType T::*memberPointer, Args... args)
{
    for (int p = 0; p < padding; ++p)
    {
        std::cout << " ";
    }

    std::cout << memberName << ": ";
    toString(member.*memberPointer, padding);

    instanceToString(member, padding, args...);
}


}


#define JSON_SERIALIZER(Class, ...)                             \
    template <>                                                 \
    void Parser::toString(const Class& member, int padding)     \
    {                                                           \
        std::cout << std::endl;                                 \
        instanceToString(member, padding + 4, __VA_ARGS__);     \
    }

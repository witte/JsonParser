#pragma once
#include <iostream>
#include <string_view>
#include "TypesParsing.h"


namespace Parser
{


void printPadding(int padding = 0);


// From string
void parseMember(const std::string_view name, const std::string_view msg, size_t& index, void* instance);

template <typename T, typename MemberType, typename... Args>
void parseMember(const std::string_view name, const std::string_view msg, size_t& index, void* instance,
                 const std::string_view memberName, MemberType T::*memberPointer,
                 Args... args)
{
    if (name == memberName)
        return parse(msg, index, static_cast<T*>(instance)->*memberPointer);

    parseMember(name, msg, index, instance, args...);
}

void parseMessage(const std::string_view msg, size_t& index, void* instance,
                  void (*func)(const std::string_view, const std::string_view, size_t&, void*));


// To string
template <typename T>
void toString(const T& value, int padding = 0)
{
    (void)padding;
    std::cout << value << "," << std::endl;
}

template <>
void toString(const std::string& value, int);

template <typename T>
void instanceToString(const T&, int)
{
}

template <typename T, typename MemberType, typename... Args>
void instanceToString(const T& member, int padding, const std::string_view memberName, MemberType T::*memberPointer, Args... args)
{
    printPadding(padding);

    std::cout << memberName << ": ";
    toString(member.*memberPointer, padding);

    instanceToString(member, padding, args...);
}


}


// TODO: How to do this with templates instead of macros?!?
#define JSON_PARSER(Class, ...)                                    \
    template <>                                                    \
    void Parser::parse(std::string_view sv, size_t& index,         \
                       Class& member)                              \
    {                                                              \
        static constexpr auto setMemberValue =                     \
            [](const std::string_view name,                        \
               const std::string_view sv_,                         \
               size_t& index_,                                     \
               void* instance)                                     \
        {                                                          \
            parseMember(name, sv_, index_, instance, __VA_ARGS__); \
        };                                                         \
                                                                   \
        parseMessage(sv, index, &member, setMemberValue);          \
    }                                                              \
                                                                   \
    template <>                                                    \
    void Parser::toString(const Class& member, int padding)        \
    {                                                              \
        std::cout << "{" << std::endl;                             \
        instanceToString(member, padding + 4, __VA_ARGS__);        \
        printPadding(padding);                                     \
        std::cout << "}" << std::endl;                             \
    }

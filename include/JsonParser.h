#pragma once
#include <string_view>
#include <iostream>
#include "JsonSerializer.h"


namespace Parser
{


template <typename Type>
void parse(std::string_view, size_t&, Type&)
{
    throw std::runtime_error{"If you're getting an error here it is because you need to "
                             "use the JSON_PARSER macro in one of your classes!"};
}

template <typename Type>
void parse(std::string_view sv, Type& instance)
{
    size_t index = 0;
    parse(sv, index, instance);
}

void parseMember(const std::string_view name, const std::string_view msg, size_t& index, void* instance);

template <typename T, typename MemberType, typename... Args>
void parseMember(const std::string_view name, const std::string_view msg, size_t& index, void* instance,
                 const std::string_view memberName, MemberType T::*memberPointer,
                 Args... args)
{
    if (name == memberName)
    {
//        std::cout << "found: " << name << std::endl;

        return parse(msg, index, static_cast<T*>(instance)->*memberPointer);
    }

    parseMember(name, msg, index, instance, args...);
}

void parseMessage(const std::string_view msg, size_t& index, void* instance,
                  void (*func)(const std::string_view, const std::string_view, size_t&, void*));


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
    JSON_SERIALIZER(Class, __VA_ARGS__)

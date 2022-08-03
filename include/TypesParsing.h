#pragma once
#include <string>
#include <string_view>


namespace Parser
{


template <typename Type>
void parse(const std::string_view, size_t&, Type&)
{
    throw std::runtime_error{"If you're getting an error here it is because you need to "
                             "use the JSON_PARSER macro in one of your classes!"};
}

template <typename Type>
void parse(const std::string_view sv, Type& instance)
{
    size_t index = 0;
    parse(sv, index, instance);
}

template <>
void parse<bool>(const std::string_view sv, size_t& index, bool& member);

template <>
void parse<int>(const std::string_view sv, size_t& index, int& member);

template <>
void parse<std::string>(const std::string_view sv, size_t& index, std::string& member);


}

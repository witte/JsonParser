#pragma once
#include <string_view>


namespace Parser
{


std::string_view getQuotedTokenBounds(const std::string_view sv, size_t& index);

std::string_view getUnquotedTokenBounds(const std::string_view sv, size_t& index);

std::string_view getTokenBounds(const std::string_view sv, size_t &index);

std::string_view getUnquotedNameBounds(const std::string_view sv, size_t& index);

std::string_view getNameBounds(const std::string_view sv, size_t& index);


}

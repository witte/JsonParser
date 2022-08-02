#pragma once
#include <string>


struct Object
{
    std::string type = "teste";
    bool someBool = true;
};

struct Message
{
    std::string type;
    std::string id;
    int sourceType = 0;
    bool on = false;

    Object object;
};

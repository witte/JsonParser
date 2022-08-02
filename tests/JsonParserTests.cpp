#include <iostream>
#include <JsonParser.h>
#include "Structs.h"


JSON_PARSER(Object,  "type",       &Object::type,
                     "someBool",   &Object::someBool)

JSON_PARSER(Message, "type",       &Message::type,
                     "id",         &Message::id,
                     "sourceType", &Message::sourceType,
                     "on",         &Message::on,
                     "object",     &Message::object)


static constexpr std::string_view msg1 =
R"({
    "id": "376a26a5-c012-\"4464-8259-ab02517b85e8",
    object   : {
        type: "getUserSettings",
        "someBool": "false"         ,
    }  ,

    sourceType: 1234,
    "type": "getUpdate",
})";


int main()
{
    Message msg;
    Parser::toString(msg);

    Parser::parse(msg1, msg);
    Parser::toString(msg);

    return 0;
}

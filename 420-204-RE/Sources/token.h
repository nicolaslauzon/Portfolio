#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class c_Token
{
public:
    c_Token();
    c_Token(const c_Token& token);

    enum c_TokenType {
        kUnknownTokenType=0,
        kMultiplyTokenType=1,
        kDivideTokenType=2,
        kModuloTokenType=3,
        kMinusTokenType=4,
        kPlusTokenType=5,
        kOpenBraceTokenType=6,
        kCloseBraceTokenType=7,
        kIntTokenType=8
    };

    c_Token & operator=(const c_Token& token);

    static c_Token BuildFromString(const std::string& string,bool *is_ok=nullptr);

    c_TokenType Type() const { return token_type_;}
    const std::string& ValueString() const { return value_string_;}
private:
    c_TokenType token_type_;
    std::string value_string_;
};

#endif // TOKEN_H

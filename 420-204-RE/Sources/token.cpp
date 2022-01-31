#include "token.h"

c_Token::c_Token()
    : token_type_(kUnknownTokenType)
{

}

c_Token::c_Token(const c_Token &token)
    : token_type_(token.token_type_),
      value_string_(token.value_string_)
{

}

c_Token &c_Token::operator=(const c_Token &token)
{
    token_type_=token.token_type_;
    value_string_=token.value_string_;
    return *this;
}

c_Token c_Token::BuildFromString(const std::string &string, bool *is_ok)
{
    c_Token token;
    token.value_string_ = string;
    switch (string.at(0)) {
        case '*':
            token.token_type_ = kMultiplyTokenType;
            break;
        case '/':
            token.token_type_ = kDivideTokenType;
        break;
        case '%':
            token.token_type_ = kModuloTokenType;
            break;
        case '-':
            if ((string.size() > 1) && (string.at(1) >= '0' && string.at(1) <= '9')) {
                token.token_type_ = kIntTokenType;
            }
            else {
                token.token_type_ = kMinusTokenType;
            }
            break;
        case '+':
            token.token_type_ = kPlusTokenType;
            break;
        case '(':

            token.token_type_ = kOpenBraceTokenType;
            break;
        case ')':
            token.token_type_ = kCloseBraceTokenType;
            break;
        default:
            token.token_type_ = kUnknownTokenType;
            break;
    }
    if (token.token_type_ == kUnknownTokenType) {
        if (string[0] >= '0' && string[0] <= '9')
            token.token_type_ = kIntTokenType;
    }
    if(!is_ok) {
        if(token.token_type_ == kUnknownTokenType)
            is_ok = new bool(false);
        else
            is_ok = new bool(true);
    }
    return token;
}

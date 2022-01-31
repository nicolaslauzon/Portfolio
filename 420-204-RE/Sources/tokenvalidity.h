#ifndef TOKENVALIDITY_H
#define TOKENVALIDITY_H

#include "token.h"

#include <list>


class TokenValidity
{
public:
    TokenValidity(const std::list<c_Token>& token_list);
    bool isValid() const { return is_valid_;}
    const std::string& ErrorMessage() const {return error_message_;}
private:
    bool testBrace();
    bool testOperator();

    bool is_valid_;
    std::string error_message_;
    std::list<c_Token> token_list_;
};

#endif // TOKENVALIDITY_H

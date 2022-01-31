#ifndef TOKENISER_H
#define TOKENISER_H

#include "token.h"

#include <string>
#include <list>

class Tokenizer
{
public:
    Tokenizer(const std::string& input_equation);

    bool IsValid() const { return is_valid_;}
    const std::string& ErrorMessage() const { return error_message_;}
    const std::list<c_Token>& Result() const { return token_list_;}
    void FillList();
private:

    bool isValid(std::size_t i);
    void isBraceOrOperator(std::size_t i);
    std::size_t isNegativeInteger(std::size_t i);
    std::size_t isPositiveInteger(std::size_t i);

    std::string equation_;
    std::list<c_Token> token_list_;
    bool is_valid_;
    std::string error_message_;
};

#endif

#ifndef TOKENSIMPLIFIER_H
#define TOKENSIMPLIFIER_H

#include "token.h"

#include <list>



class TokenSimplifier
{
public:
    TokenSimplifier(const std::list<c_Token>& token_list);

    const std::list<c_Token>& TokenList() const {return token_list_;};

private:
    void Simplify();
    std::list<c_Token> token_list_;
    std::list<c_Token> RemoveRepetition(const std::list<c_Token>& token_list);
    void AddMultiplication();
};

#endif // TOKENSIMPLIFIER_H

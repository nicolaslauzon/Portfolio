#include "tokensimplifier.h"

TokenSimplifier::TokenSimplifier(const std::list<c_Token>& token_list)
{
    token_list_ = token_list;
    Simplify();
}

void TokenSimplifier::Simplify()
{
    token_list_ = RemoveRepetition(token_list_);
    AddMultiplication();
}

std::list<c_Token> TokenSimplifier::RemoveRepetition(const std::list<c_Token>& token_list)
{
    std::list<c_Token> simplified_list;
    std::list<c_Token>::const_iterator i = token_list.begin();
    std::list<c_Token>::const_iterator j = token_list.begin();
    ++j;
    while (i != token_list.end()) {
        // case of '-' and '-': convert to '+'
        if (i->Type() == c_Token::c_TokenType::kMinusTokenType && j->Type() == c_Token::c_TokenType::kMinusTokenType) {
            simplified_list.push_back(c_Token::BuildFromString("+"));
            ++i;
            ++j;
        }
        // case of '+' and '+': convert to '+'
        else if (i->Type() == c_Token::c_TokenType::kPlusTokenType && j->Type() == c_Token::c_TokenType::kPlusTokenType) {
            simplified_list.push_back(c_Token::BuildFromString("+"));
            ++i;
            ++j;
        }
        // case of '+' and '-', or '-' and '+': convert to '-'
        else if ((i->Type() == c_Token::c_TokenType::kMinusTokenType && j->Type() == c_Token::c_TokenType::kPlusTokenType) ||
                 (i->Type() == c_Token::c_TokenType::kPlusTokenType && j->Type() == c_Token::c_TokenType::kMinusTokenType)) {
            simplified_list.push_back(c_Token::BuildFromString("-"));
            ++i;
            ++j;
        }
        // something else, just keep it
        else {
            simplified_list.push_back(*i);
        }
        if (i != token_list.end())
            ++i;
         if (j != token_list.end())
            ++j;
    }
    if (simplified_list.size() != token_list.size())
        simplified_list = RemoveRepetition(simplified_list);
    return simplified_list;
}

void TokenSimplifier::AddMultiplication()
{
    std::list<c_Token>::const_iterator i = token_list_.begin(), j = token_list_.begin();
    ++j;
    for (; j != token_list_.end(); ++i , ++j) {
        if (i->Type() == c_Token::c_TokenType::kIntTokenType) {
            if (j->Type() == c_Token::kOpenBraceTokenType) {
                token_list_.insert(++i, c_Token::BuildFromString("*"));
                ++j;
            }
        }
        if (i->Type() == c_Token::c_TokenType::kCloseBraceTokenType) {
            if (j->Type() == c_Token::c_TokenType::kIntTokenType) {
                token_list_.insert( ++i, c_Token::BuildFromString("*"));
                ++j;
            }
        }
    }
}

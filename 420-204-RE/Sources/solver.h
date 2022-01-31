#ifndef SOLVER_H
#define SOLVER_H

#include "token.h"
#include "Queue.h"
#include "Stack.h"

#include <list>
#include <cstdio>

int PriorityFromTokenType(c_Token::c_TokenTyoe token_type);

class Solver
{
public:
    Solver(const std::list<c_Token> &list);
    int Answer() { return answer; }
    bool IsValid() { return is_valid_; }
    std::string ErrorMessage() { return error_message_; }
    std::string PostFix() { return post_fix_; }

private:
    std::list<c_Token> list_;
    Queue<c_Token> file_infix_;
    Queue<c_Token> file_postfix_;
    Stack<c_Token> pile_operateur_;
    std::string error_message_;
    std::string post_fix_;
    bool is_valid_;
    int answer;

    void PushListInQueue();
    void InfixOpenBrace();
    void InfixCloseBrace();
    void InfixOperator();
    void InfixInteger();
    void InfixEmptyStack();
    void InfixGetPostFix();
    void InfixGetString(Queue<c_Token> &queue);

    void PostfixStackOperandes();
    void PostFixDoOperation();
    void SolutionFinder();
};

#endif // SOLVER_H

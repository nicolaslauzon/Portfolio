#include "solver.h"

Solver::Solver(const std::list<c_Token>& list)
{
  list_ = list;
  is_valid_ = true;
  error_message_ = "No error computed.";
  answer = 0;
  SolutionFinder();
}

void Solver::PushListInQueue()
{
    std::list<c_Token>::const_iterator i = list_.begin();
    while (i != list_.end()) {
        file_infix_.push(*i);
        ++i;
    }
}

void Solver::InfixOpenBrace()
{
    pile_operateur_.push(file_infix_.front());
    file_infix_.pop();
}

void Solver::InfixCloseBrace()
{
    while (pile_operateur_.size() && (pile_operateur_.top().Type() != c_Token::c_TokenType::kOpenBraceTokenType)) {
        file_postfix_.push(pile_operateur_.top());
        pile_operateur_.pop();
    }
    pile_operateur_.pop();
    file_infix_.pop();
}

void Solver::InfixOperator()
{
    while (file_infix_.size() && pile_operateur_.size() &&  (PriorityFromTokenType(pile_operateur_.top().Type()) >= PriorityFromTokenType(file_infix_.front().Type()))) {
        file_postfix_.push(pile_operateur_.top());
        pile_operateur_.pop();
    }
    if (file_infix_.size()) {
        pile_operateur_.push(file_infix_.front());
        file_infix_.pop();
    }
}

void Solver::InfixInteger()
{
    file_postfix_.push(file_infix_.front());
    file_infix_.pop();
}

void Solver::InfixEmptyStack()
{
    while (pile_operateur_.size()) {
        file_postfix_.push(pile_operateur_.top());
        pile_operateur_.pop();
    }
}

void Solver::InfixGetPostFix()
{
    c_Token var;
    while (file_infix_.size()) {
        var = file_infix_.front();
        if (var.Type() == c_Token::c_TokenType::kOpenBraceTokenType){
            InfixOpenBrace();
        }
        else if (var.Type() == c_Token::c_TokenType::kCloseBraceTokenType){
            InfixCloseBrace();
        }
        else if (var.Type()==c_Token::c_TokenType::kMultiplyTokenType||var.Type()==c_Token::c_TokenType::kDivideTokenType||var.Type()==c_Token::c_TokenType::kModuloTokenType||var.Type()==c_Token::c_TokenType::kPlusTokenType||var.Type()==c_Token::c_TokenType::kMinusTokenType){
            InfixOperator();
        }
        else
            InfixInteger();
    }
    InfixEmptyStack();
}

void Solver::InfixGetString(Queue<c_Token>& queue)
{
    SlIterator<c_Token> *iterator;
    iterator = queue.begin();
    std::string temp;
    temp+= iterator->Data().ValueString();
    iterator->Iterate();
    while (iterator->End()) {
        temp+= iterator->Data().ValueString();
        iterator->Iterate();
    }
    post_fix_ = temp;
    delete iterator;
}

void Solver::PostfixStackOperandes()
{
    if (file_postfix_.size()) {
        pile_operateur_.push(file_postfix_.front());
        file_postfix_.pop();
    }
}

void Solver::PostFixDoOperation()
{
    std::string tmp;
    int operande1, operande2;
    c_Token::c_TokenType operateur = file_postfix_.front().Type();
    file_postfix_.pop();
    operande1 = atoi(pile_operateur_.top().ValueString().c_str());
    pile_operateur_.pop();
    operande2 = atoi(pile_operateur_.top().ValueString().c_str());
    pile_operateur_.pop();
    if (operateur == c_Token::c_TokenType::kPlusTokenType){
        operande1 = operande2 + operande1;
    }
    else if (operateur == c_Token::c_TokenType::kMinusTokenType){
        operande1 = operande2 - operande1;
    }
    else if (operateur == c_Token::c_TokenType::kDivideTokenType){
        if (!operande1) {
            error_message_ = "The equation you  entered is invalid. Division by 0.";
            is_valid_ = false;
        }
        else {
            operande1 = operande2 / operande1;
            is_valid_ = true;
        }
    }
    else if (operateur == c_Token::c_TokenType::kMultiplyTokenType){
        operande1 = operande2 * operande1;
    }
    else if (operateur == c_Token::c_TokenType::kModuloTokenType){
        if (!operande1) {
            error_message_ = "The equation you  entered is invalid. Modulo by 0.";
            is_valid_ = false;
        }
        else {
        operande1 = operande2 % operande1;
        is_valid_ = true;
        }
    }
    pile_operateur_.push(c_Token::BuildFromString(std::to_string(operande1)));
    answer = operande1;
}

void Solver::SolutionFinder()
{
    PushListInQueue();
    InfixGetPostFix();
    InfixGetString(file_postfix_);
    for(int i =0; i<2;i++){
        PostfixStackOperandes();
    }
    while (pile_operateur_.size() != 1 || file_postfix_.size()) {
        if (file_postfix_.front().Type() == c_Token::c_TokenType::kPlusTokenType || file_postfix_.front().Type() == c_Token::c_TokenType::kMinusTokenType || file_postfix_.front().Type() == c_Token::c_TokenType::kModuloTokenType || file_postfix_.front().Type() == c_Token::c_TokenType::kDivideTokenType || file_postfix_.front().Type() == c_Token::c_TokenType::kMultiplyTokenType){
            PostFixDoOperation();
        }
        else {
            PostfixStackOperandes();
        }
    }
    //answer = stoi(pile_operateur_.top().ValueString());
}

int PriorityFromTokenType(c_Token::c_TokenType token_type)
{
    switch (token_type){
    case c_Token::c_TokenType::kMultiplyTokenType:
    case c_Token::c_TokenType::kDivideTokenType:
    case c_Token::c_TokenType::kModuloTokenType:
        return 3;
    case c_Token::c_TokenType::kMinusTokenType:
    case c_Token::c_TokenType::kPlusTokenType:
        return 2;
    case c_Token::c_TokenType::kOpenBraceTokenType:
    case c_Token::c_TokenType::kCloseBraceTokenType:
        return 1;
    case c_Token::c_TokenType::kIntTokenType:
    case c_Token::c_TokenType::kUnknownTokenType:
        break;
    }
    return -1;
}

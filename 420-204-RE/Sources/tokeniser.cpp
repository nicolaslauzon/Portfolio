#include "tokeniser.h"


Tokenizer::Tokenizer(const std::string& input_equation)
{
    is_valid_ = false;
    error_message_="no result computed";
    equation_ = input_equation;
}

void Tokenizer::FillList()
{
    is_valid_ = true;
    for (std::size_t i = 0; i < equation_.size(); i++){
        if(!isValid(i)) {
            is_valid_ = false;
        }
    }
    if(is_valid_) {
        for (std::size_t i = 0; i < equation_.size();){
            if(equation_.at(i)==' ')
                i++;
            if (equation_.at(i) == '(' || equation_.at(i) == ')' || equation_.at(i) == '*' || equation_.at(i) == '/' || equation_.at(i) == '%' || equation_.at(i) == '+') {
                isBraceOrOperator(i);
                i++;
            }
            if (i < equation_.size()) {
                if (equation_.at(i) == '-') {
                    if (equation_.at(i-1) == '*' || equation_.at(i-1) == '/' || equation_.at(i-1) == '%') {
                        i = isNegativeInteger(i);
                    }
                    else {
                        isBraceOrOperator(i);
                        i++;
                    }
                }
            }
            if (i < equation_.size()) {
                if(equation_.at(i) >= '0' && equation_.at(i) <= '9') {
                    i = isPositiveInteger(i);
                }
            }
        }
    }
}

bool Tokenizer::isValid(std::size_t i)
{
    if ((equation_.at(i) >= '0'
        && equation_.at(i) <= '9')||
           equation_.at(i) == '%' ||
           equation_.at(i) == ' ' ||
           equation_.at(i) == '-' ||
           equation_.at(i) == '/' ||
          (equation_.at(i) >= '(' &&
           equation_.at(i) <= '+')) {
        return true;
    }
    error_message_ = "The equation is invalid. Unknown character.\n";
    return false;
}

void Tokenizer::isBraceOrOperator(std::size_t i)
{
    std::string tmpStr(1, equation_.at(i));
    c_Token braceOrOperator;
    braceOrOperator = braceOrOperator.BuildFromString(tmpStr);
    token_list_.push_back(braceOrOperator);
}

std::size_t Tokenizer::isNegativeInteger(std::size_t i)
{
    std::string str(1,'-');
    while ((++i < equation_.size()) && equation_.at(i) >= '0' && equation_.at(i) <= '9') {
        str += equation_.at(i);
    }
    c_Token negativeInteger;
    negativeInteger = negativeInteger.BuildFromString(str);
    token_list_.push_back(negativeInteger);
    return i;
}

std::size_t Tokenizer::isPositiveInteger(std::size_t i)
{
    std::string str(1, equation_.at(i));
        while ((++i < equation_.size()) && (equation_.at(i) <= '0' && equation_.at(i) >= '9')) {
            str += equation_.at(i);
        }
    c_Token positiveInteger;
    positiveInteger = positiveInteger.BuildFromString(str);
    token_list_.push_back(positiveInteger);
    return i;
}

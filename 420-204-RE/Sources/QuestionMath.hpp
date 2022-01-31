///\file QuestionMath.hpp
#ifndef QUESTIONMATH_HPP
#define QUESTIONMATH_HPP


#include <tuple>
#include <string>
#include <random>

#include "solver.h"
#include "tokeniser.h"
#include "tokenvalidity.h"
#include "tokensimplifier.h"
#include "token.h"

using namespace std;

typedef tuple<string, int> Question;

///\class QuestionMath
///\brief Générateur de calcul mental
class QuestionMath
{
private:
    static unsigned int operandeQuantity_; ///< Quantité de chiffres dans l'équation

    ///\brief default constructor
    QuestionMath() {  }

        ///\brief Ajoute une operation a la fin de l'équation
        ///\param currEquation equation
        ///\return nouvelle equation
        static string
        AddOperation(string currEquation)
    {
        int newNumber = GenerateRandomInt(10);
        ++operandeQuantity_;
        string newOperator = GenerateRandomOperator();
        if (currEquation != "")
        {
            currEquation.append(newOperator + to_string(newNumber));
            return currEquation;
        }
        ++operandeQuantity_;
        int newNumber2 = GenerateRandomInt(10);
        currEquation = to_string(newNumber) + newOperator + to_string(newNumber2);
        return currEquation;
    }

    ///\brief Ajoute une paire de parenthèses dans l'équation
    ///\param currEquation equation
    ///\return nouvelle equation
    static string AddBracePair(const string &currEquation)
    {
        int currNumber = 0;
        int whereToPlaceFirstBrace = GenerateRandomInt(operandeQuantity_ - 1, 1);
        int whereToPlaceSecondBrace = GenerateRandomInt(operandeQuantity_ - 1, 2);
        if (whereToPlaceSecondBrace < whereToPlaceFirstBrace)
            swap(whereToPlaceFirstBrace, whereToPlaceSecondBrace);

        string returnString = "";
        bool isNumber = false;

        for (string::const_iterator it = currEquation.begin(); it != currEquation.end(); ++it)
        {
            // is it a number
            if (*it >= '0' && *it <= '9')
            {
                ++currNumber;
                isNumber = true;
            }
            if (currNumber == whereToPlaceFirstBrace && isNumber)
            {
                returnString.push_back('(');
            }
            returnString.push_back(*it);
            if (currNumber == whereToPlaceSecondBrace && isNumber)
            {
                returnString.push_back(')');
            }
            isNumber = false;
        }

        return returnString;
    }

public:
    ///\brief génère un calcul mental
    ///\param complexity complexité de l'équation
    ///\return Le calcul mental et la réponse
    static const Question GenerateQuestion(unsigned int complexity)
    {
        string equation = "";
        bool nextIsOperation = false;
        for (int i = 0; i < complexity; ++i)
        {
            int rand = GenerateRandomInt(2);
            if (rand || i == 0 || nextIsOperation)
            {
                equation = AddOperation(equation);
                nextIsOperation = false;
            }
            else
            {
                string returnEquation = AddBracePair(equation);
                equation = returnEquation;
                nextIsOperation = true;
            }
        }
        operandeQuantity_ = 0;
        // get a list of token from the equation
        Tokenizer decode_string(equation);
        decode_string.FillList();
#ifdef DEBUG
        if (decode_string.IsValid() == false)
        {
            std::cerr << decode_string.ErrorMessage() << std::endl;
        }
#endif
        std::list<c_Token> decoded_list = decode_string.Result();

        // Simplify
        TokenSimplifier simplify(decoded_list);

        // Test validity

        TokenValidity validate_equation(simplify.TokenList());
#ifdef DEBUG
        if (validate_equation.isValid() == false)
        {
            std::cerr << validate_equation.ErrorMessage() << std::endl;
        }
#endif

        // Solve the equation with the list as an imput
        Solver solver(simplify.TokenList());
#ifdef DEBUG
        if (solver.IsValid() == false)
        {
            std::cerr << solver.ErrorMessage() << std::endl;
        }
#endif

        int answer = solver.Answer();

        return Question{equation, answer};
    }

    ///\brief génère un int random
    ///\param range range
    ///\param minimum point 0 du range
    ///\return int random
    static int GenerateRandomInt(int range, int minimum = 0)
    {
        return rand() % range + minimum;
    }

    ///\brief génère un opérateur random
    ///\return opérateur
    static string GenerateRandomOperator()
    {
        int r = GenerateRandomInt(5);
        switch (r)
        {
        case 0:
            return "+";
            break;
        case 1:
            return "-";
            break;
        case 2:
            return "*";
            break;
        case 3:
            return "/";
            break;
        case 4:
            return "%";
            break;
        default:
            break;
        }
        return "\0";
    }
};

unsigned int QuestionMath::operandeQuantity_ = 0;

#endif
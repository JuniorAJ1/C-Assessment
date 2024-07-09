//
//  main.cpp
//  SnsSystems
//
//  Created by Junior Ajala on 09/07/2024.
//

#include <iostream>
#include <string>
#include <stack>
#include <cctype>

// Function prototypes
bool evaluate(const char *expression, int &result);
bool parseExpression(const char *&expr, int &result);
bool parseTerm(const char *&expr, int &result);
bool parseFactor(const char *&expr, int &result);
bool parseNumber(const char *&expr, int &result);
void skipWhitespace(const char *&expr);

// Main evaluation function
bool evaluate(const char *expression, int &result) {
    const char *expr = expression;
    if (!parseExpression(expr, result)) {
        return false;
    }
    // Skip trailing whitespace
    skipWhitespace(expr);
    // If there are remaining characters, the expression is invalid
    return *expr == '\0';
}

// Parse an expression: term { ('+'|'-') term }*
bool parseExpression(const char *&expr, int &result) {
    if (!parseTerm(expr, result)) {
        return false;
    }
    while (true) {
        skipWhitespace(expr);
        char op = *expr;
        if (op != '+' && op != '-') {
            break;
        }
        ++expr;
        int nextTerm;
        if (!parseTerm(expr, nextTerm)) {
            return false;
        }
        if (op == '+') {
            result += nextTerm;
        } else {
            result -= nextTerm;
        }
    }
    return true;
}

bool parseTerm(const char *&expr, int &result) {
    if (!parseFactor(expr, result)) {
        return false;
    }
    while (true) {
        skipWhitespace(expr);
        char op = *expr;
        if (op != '*' && op != '/') {
            break;
        }
        ++expr;  // Consume the operator
        int nextFactor;
        if (!parseFactor(expr, nextFactor)) {
            return false;
        }
        if (op == '*') {
            result *= nextFactor;
        } else {
            if (nextFactor == 0) {
                return false;  // Division by zero error
            }
            result /= nextFactor;
        }
    }
    return true;
}

bool parseFactor(const char *&expr, int &result) {
    skipWhitespace(expr);
    if (*expr == '(') {
        ++expr;
        if (!parseExpression(expr, result)) {
            return false;
        }
        skipWhitespace(expr);
        if (*expr != ')') {
            return false;  // Missing closing parenthesis
        }
        ++expr;
    } else {
        if (!parseNumber(expr, result)) {
            return false;
        }
    }
    return true;
}

// Parse a number
bool parseNumber(const char *&expr, int &result) {
    skipWhitespace(expr);
    bool negative = false;
    if (*expr == '-') {
        negative = true;
        ++expr;  // Consume '-'
    }
    if (!isdigit(*expr)) {
        return false;  // Not a valid number
    }
    result = 0;
    while (isdigit(*expr)) {
        result = result * 10 + (*expr - '0');
        ++expr;
    }
    if (negative) {
        result = -result;
    }
    return true;
}

// Skip whitespace characters
void skipWhitespace(const char *&expr) {
    while (isspace(*expr)) {
        ++expr;
    }
}

// Example usage
int main() {
    const char *expr1 = "1 + 3";
    int result1;
    if (evaluate(expr1, result1)) {
        std::cout << "Result: " << result1 << std::endl;
    } else {
        std::cout << "Invalid expression: " << expr1 << std::endl;
    }

    const char *expr2 = "(1 + (12 * 2)";
    int result2;
    if (evaluate(expr2, result2)) {
        std::cout << "Result: " << result2 << std::endl;
    } else {
        std::cout << "Invalid expression: " << expr2 << std::endl;
    }

    return 0;
}


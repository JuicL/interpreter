#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <cstring>
#include <stdexcept>

struct Expression {
    Expression(std::string token) : token(token) {}
    Expression(std::string token, Expression a) : token(token), args{ a } {}
    Expression(std::string token, Expression a, Expression b) : token(token), args{ a, b } {}

    std::string token;
    std::vector<Expression> args;
};

class ArifParser {
public:
     ArifParser(){
     }
    
    Expression parse();
    double Result(const char* input);

private:
    std::string parse_token();
    Expression parse_simple_expression();
    Expression parse_binary_expression(int min_priority);
    double eval(const Expression& e);
    const char* input;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <iostream>
//
//int errors;
//
//void test(const char* input, double expected) {
//    try {
//        Parser p(input);
//        auto result = eval(p.parse());
//        if (result == expected) return;
//        std::cout << input << " = " << expected << " : error, got " << result << '\n';
//    }
//    catch (std::exception& e) {
//        std::cout << input << " : exception: " << e.what() << '\n';
//    }
//    ++errors;
//}

//int main() {
//    setlocale(LC_ALL, "ru");
//    test("a+2", 2);
//    test("1", 1);
//    test("9", 9);
//    test("10", 10);
//    test("+1", 1);
//    test("-1", -1);
//    test("(1)", 1);
//    test("(-1)", -1);
//
//    /*test("1+20", 21);
//    test("1 + 20", 21);
//    test("1+20+300", 321);
//    test("1+20+300+4000", 4321);
//    test("-1+20", 19);
//    test("--1+20", 21);
//    test("---1+20", 19);
//    test("(1+20)", 21);
//    test("-2*3", -6);
//    test("2*-3", -6);
//    test("1++2", 3);
//    test("1+10*2", 21);
//    test("10*2+1", 21);
//    test("(1+20)*2", 42);
//    test("2*(1+20)", 42);
//    test("(1+2)*(3+4)", 21);
//    test("2*3+4*5", 26);
//    test("100+2*10+3", 123);
//    test("2**3", 8);
//    test("2**3*5+2", 42);
//    test("5*2**3+2", 42);
//    test("2+5*2**3", 42);
//    test("1+2**3*10", 81);
//    test("2**3+2*10", 28);
//    test("5 * 4 + 3 * 2 + 1", 27);*/
//    std::cout << "Done with " << errors << " errors.\n";
//}



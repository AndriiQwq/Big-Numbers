#include "Big_Numbers.hpp"
#include <iostream>
#include <vector>
#include <string>


void test_divisions() {
    struct DivisionTestCase {
        std::string description;
        BigInteger dividend;
        BigInteger divisor;
        BigInteger expected_quotient;
    };
    std::vector<DivisionTestCase> tests = {
        {"Test 1: +123456789 / +12345", BigInteger("123456789"), BigInteger(12345), BigInteger(10000)},
        {"Test 2: +987654321 / +123456789", BigInteger("987654321"), BigInteger(123456789), BigInteger(8)},
        {"Test 3: -987654321 / +123456789", BigInteger("-987654321"), BigInteger(123456789), BigInteger(-8)},
        {"Test 4: +987654321 / -123456789", BigInteger("987654321"), BigInteger(-123456789), BigInteger(-8)},
        {"Test 5: -987654321 / -123456789", BigInteger("-987654321"), BigInteger(-123456789), BigInteger(8)},
        {"Test 6: +1000000000 / +3", BigInteger("1000000000"), BigInteger(3), BigInteger(333333333)},
        {"Test 7: -1000000000 / +3", BigInteger("-1000000000"), BigInteger(3), BigInteger(-333333333)},
        {"Test 8: +1000000000 / -3", BigInteger("1000000000"), BigInteger(-3), BigInteger(-333333333)},
        {"Test 9: -1000000000 / -3", BigInteger("-1000000000"), BigInteger(-3), BigInteger(333333333)},
        {"Test 10: +0 / +123456789", BigInteger(0), BigInteger("123456789"), BigInteger(0)}
    };

    for (const auto& test : tests) {
        try {
            BigInteger quotient = test.dividend; // Создаём копию делимого
            quotient /= test.divisor; // Выполняем деление

            bool pass = (quotient == test.expected_quotient);
            std::string status = pass ? "PASS" : "FAIL";

            std::cout << test.description << " = " << quotient << " | " << status << "\n";
        }
        catch (const std::exception& e) {
            // Обработка деления на ноль или других исключений
            bool pass = (test.divisor == BigInteger(0));
            std::string status = pass ? "PASS (Exception Caught)" : "FAIL (Unexpected Exception)";
            std::cout << test.description << " threw exception: " << e.what() << " | " << status << "\n";
        }
    }
}

int main() {
    try {
        test_divisions();
        //test_division();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#include "Big_Numbers.hpp"
#include <iostream>
#include <vector>
#include <string>

void test_operations() {
    struct TestCase {
        std::string description;
        BigInteger lhs;
        BigInteger rhs;
        BigInteger expected_result;
        char operation;
    };

std::vector<TestCase> tests = {
    {"Test 1: +000000000123456789 + +000000000000012345", BigInteger("000000000123456789"), BigInteger("000000000000012345"), BigInteger("000000000123469134"), '+'},
    {"Test 2: +000000000987654321 - +000000000000012345", BigInteger("000000000987654321"), BigInteger("000000000000012345"), BigInteger("000000000987641976"), '-'},
    {"Test 3: -000000000987654321 * +000000000000012345", BigInteger("-000000000987654321"), BigInteger("000000000000012345"), BigInteger("-1219326311126352695"), '*'},
    {"Test 4: +000000000987654321 / -000000000000012345", BigInteger("000000000987654321"), BigInteger("-000000000000012345"), BigInteger("-80000"), '/'},
    {"Test 5: -000000000987654321 % -000000000000012345", BigInteger("-000000000987654321"), BigInteger("-000000000000012345"), BigInteger("-9876"), '%'},
    {"Test 6: +0000000001000000000 += +000000000000000003", BigInteger("0000000001000000000"), BigInteger("000000000000000003"), BigInteger("0000000001000000003"), 'a'},
    {"Test 7: -0000000001000000000 -= +000000000000000003", BigInteger("-0000000001000000000"), BigInteger("000000000000000003"), BigInteger("-0000000001000000003"), 's'},
    {"Test 8: +0000000001000000000 *= -000000000000000003", BigInteger("0000000001000000000"), BigInteger("-000000000000000003"), BigInteger("-0000000003000000000"), 'm'},
    {"Test 9: -0000000001000000000 /= -000000000000000003", BigInteger("-0000000001000000000"), BigInteger("-000000000000000003"), BigInteger("333333333"), 'd'},
    {"Test 10: +0000000000000000000 %= +000000000123456789", BigInteger("0000000000000000000"), BigInteger("000000000123456789"), BigInteger("0000000000000000000"), 'r'},
    {"Test 11: +000000000123456789 < +000000000000000001", BigInteger("000000000123456789"), BigInteger("000000000000000001"), BigInteger(0), '<'},
    {"Test 12: +000000000123456789 > +000000000123456789", BigInteger("000000000123456789"), BigInteger("000000000123456789"), BigInteger(0), '>'},
    {"Test 13: +000000000123456789 <= +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(0), 'l'},
    {"Test 14: +000000000123456789 >= +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(1), 'g'},
    {"Test 15: +000000000123456789 == +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(0), '='},
    {"Test 16: +000000000123456789 != +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(1), '!'}
};

    for (const auto& test : tests) {
        try {
            BigInteger result;
            bool comparison_result = false;
            switch (test.operation) {
                case '+':
                    result = test.lhs + test.rhs;
                    break;
                case '-':
                    result = test.lhs - test.rhs;
                    break;
                case '*':
                    result = test.lhs * test.rhs;
                    break;
                case '/':
                    result = test.lhs / test.rhs;
                    break;
                case '%':
                    result = test.lhs % test.rhs;
                    break;
                case 'a':
                    result = test.lhs;
                    result += test.rhs;
                    break;
                case 's':
                    result = test.lhs;
                    result -= test.rhs;
                    break;
                case 'm':
                    result = test.lhs;
                    result *= test.rhs;
                    break;
                case 'd':
                    result = test.lhs;
                    result /= test.rhs;
                    break;
                case 'r':
                    result = test.lhs;
                    result %= test.rhs;
                    break;
                case '<':
                    comparison_result = test.lhs < test.rhs;
                    break;
                case '>':
                    comparison_result = test.lhs > test.rhs;
                    break;
                case 'l':
                    comparison_result = test.lhs <= test.rhs;
                    break;
                case 'g':
                    comparison_result = test.lhs >= test.rhs;
                    break;
                case '=':
                    comparison_result = test.lhs == test.rhs;
                    break;
                case '!':
                    comparison_result = test.lhs != test.rhs;
                    break;
                default:
                    throw std::invalid_argument("Unknown operation");
            }

            bool pass = (test.operation == '<' || test.operation == '>' || test.operation == 'l' || test.operation == 'g' || test.operation == '=' || test.operation == '!') ? (comparison_result == (test.expected_result != BigInteger(0))) : (result == test.expected_result);
            std::string status = pass ? "PASS" : "FAIL";

            std::cout << test.description << " | Expected: " << test.expected_result << " | Got: " << result << " | " << status << "\n";
        }
        catch (const std::exception& e) {
            // Обработка деления на ноль или других исключений
            bool pass = (test.operation == '/' || test.operation == 'd' || test.operation == '%') && (test.rhs == BigInteger(0));
            std::string status = pass ? "PASS (Exception Caught)" : "FAIL (Unexpected Exception)";
            std::cout << test.description << " threw exception: " << e.what() << " | " << status << "\n";
        }
    }
}

int main() {
    try {
        test_operations();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
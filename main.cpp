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

void test_rational_operations() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
        char operation;
    };

    std::vector<TestCase> tests = {
        // Тесты для конструктора с int параметрами
        {"Test 1: 1/2 + 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(5, 6), '+'},
        {"Test 2: 1/2 - 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(1, 6), '-'},
        {"Test 3: 1/2 += 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(5, 6), 'a'},
        {"Test 4: 1/2 -= 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(1, 6), 's'},

        // Тесты для конструктора со строковыми параметрами
        {"Test 5: -1/2 + 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-1", "6"), '+'},
        {"Test 6: -1/2 - 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-5", "6"), '-'},
        {"Test 7: -1/2 += 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-1", "6"), 'a'},
        {"Test 8: -1/2 -= 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-5", "6"), 's'},

        // Тесты для конструктора по умолчанию
        {"Test 9: 0/1 + 1/3", BigRational(), BigRational("1", "3"), BigRational("1", "3"), '+'},
        {"Test 10: 0/1 - 1/3", BigRational(), BigRational("1", "3"), BigRational("-1", "3"), '-'},
        {"Test 11: 0/1 += 1/3", BigRational(), BigRational("1", "3"), BigRational("1", "3"), 'a'},
        {"Test 12: 0/1 -= 1/3", BigRational(), BigRational("1", "3"), BigRational("-1", "3"), 's'},

        // Дополнительные простые тесты
        {"Test 13: 2/3 + 1/3", BigRational(2, 3), BigRational(1, 3), BigRational(1, 1), '+'},
        {"Test 14: 2/3 - 1/3", BigRational(2, 3), BigRational(1, 3), BigRational(1, 3), '-'},
        {"Test 15: 2/3 += 1/3", BigRational(2, 3), BigRational(1, 3), BigRational(1, 1), 'a'},
        {"Test 16: 2/3 -= 1/3", BigRational(2, 3), BigRational(1, 3), BigRational(1, 3), 's'}
    };

    for (const auto& test : tests) {
        try {
            BigRational result;
            switch (test.operation) {
                case '+':
                    result = test.lhs + test.rhs;
                    break;
                case '-':
                    result = test.lhs - test.rhs;
                    break;
                case 'a':
                    result = test.lhs;
                    result += test.rhs;
                    break;
                case 's':
                    result = test.lhs;
                    result -= test.rhs;
                    break;
                default:
                    throw std::invalid_argument("Unknown operation");
            }

            bool pass = (result == test.expected_result);
            std::string status = pass ? "PASS" : "FAIL";

            std::cout << test.description << " | Expected: " << test.expected_result << " | Got: " << result << " | " << status << "\n";
        }
        catch (const std::exception& e) {
            std::string status = "FAIL (Exception Caught)";
            std::cout << test.description << " threw exception: " << e.what() << " | " << status << "\n";
        }
    }
}
int main() {
    try {
        //test_operations();
        test_rational_operations();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    BigRational c("-5", "3");
    BigRational d(-2, 7);
    BigRational e("-1", "21");

    BigRational g("-2", "3");
    BigRational h(-2, 3);
    std::cout << "c + d: " << g + h  << std::endl;
}
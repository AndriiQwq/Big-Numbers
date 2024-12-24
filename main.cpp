#include "Big_Numbers.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cassert>
#include <sstream>

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <limits>

// #include "header.h"


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


void test_big_integer_multiplication() {
    struct TestCase {
        std::string description;
        BigInteger lhs;
        BigInteger rhs;
        BigInteger expected_result;
    };

    std::vector<TestCase> tests = {
        {"Test 1: 123 * 456", BigInteger("123"), BigInteger("456"), BigInteger("56088")},
        {"Test 2: -123 * 456", BigInteger("-123"), BigInteger("456"), BigInteger("-56088")},
        {"Test 3: 123 * -456", BigInteger("123"), BigInteger("-456"), BigInteger("-56088")},
        {"Test 4: -123 * -456", BigInteger("-123"), BigInteger("-456"), BigInteger("56088")},
        {"Test 5: 0 * 12345", BigInteger("0"), BigInteger("12345"), BigInteger("0")},
        {"Test 6: 12345 * 0", BigInteger("12345"), BigInteger("0"), BigInteger("0")},
        {"Test 7: 1 * -1", BigInteger("1"), BigInteger("-1"), BigInteger("-1")},
        {"Test 8: -1 * -1", BigInteger("-1"), BigInteger("-1"), BigInteger("1")},
        {"Test 9: Большие числа", BigInteger("12345678901234567890"), BigInteger("98765432109876543210"), BigInteger("1219326311370217952237463801111263526900")}
    };

    for (const auto& test : tests) {
        BigInteger result = test.lhs * test.rhs;
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description << " | Expected: " << test.expected_result << " | Got: " << result << " | " << status << "\n";
    }
}

void test_rational_operations() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        bool expected_result;
        char operation;
    };

    std::vector<TestCase> tests = {
        // Тесты для логических операций
        {"Test 1: 1/2 < 1/3", BigRational(3, 1), BigRational(1, 5), false, '<'},
        {"Test 2: 1/2 > 1/3", BigRational(1, 2), BigRational(1, 3), true, '>'},
        {"Test 3: 1/2 <= 1/2", BigRational(1, 2), BigRational(1, 2), true, 'l'},
        {"Test 4: 1/2 >= 1/3", BigRational(1, 2), BigRational(1, 3), true, 'g'},
        {"Test 5: 1/2 == 1/2", BigRational(1, 2), BigRational(1, 2), true, '='},
        {"Test 6: 1/2 != 1/3", BigRational(1, 2), BigRational(1, 3), true, '!'},

        // Тесты для конструктора со строковыми параметрами
        {"Test 7: -1/2 < 1/3", BigRational("-1", "2"), BigRational("1", "3"), true, '<'},
        {"Test 8: -1/2 > 1/3", BigRational("-1", "2"), BigRational("1", "3"), false, '>'},
        {"Test 9: -1/2 <= -1/2", BigRational("-1", "2"), BigRational("-1", "2"), true, 'l'},
        {"Test 10: -1/2 >= 1/3", BigRational("-1", "2"), BigRational("1", "3"), false, 'g'},
        {"Test 11: -1/2 == -1/2", BigRational("-1", "2"), BigRational("-1", "2"), true, '='},
        {"Test 12: -1/2 != 1/3", BigRational("-1", "2"), BigRational("1", "3"), true, '!'},

        // Тесты для конструктора по умолчанию
        {"Test 13: 0/1 < 1/3", BigRational(), BigRational("1", "3"), true, '<'},
        {"Test 14: 0/1 > 1/3", BigRational(), BigRational("1", "3"), false, '>'},
        {"Test 15: 0/1 <= 0/1", BigRational(), BigRational(), true, 'l'},
        {"Test 16: 0/1 >= 1/3", BigRational(), BigRational("1", "3"), false, 'g'},
        {"Test 17: 0/1 == 0/1", BigRational(), BigRational(), true, '='},
        {"Test 18: 0/1 != 1/3", BigRational(), BigRational("1", "3"), true, '!'},

        // Дополнительные простые тесты
        {"Test 19: 2/3 < 1/3", BigRational(2, 3), BigRational(1, 3), false, '<'},
        {"Test 20: 2/3 > 1/3", BigRational(2, 3), BigRational(1, 3), true, '>'},
        {"Test 21: 2/3 <= 2/3", BigRational(2, 3), BigRational(2, 3), true, 'l'},
        {"Test 22: 2/3 >= 1/3", BigRational(2, 3), BigRational(1, 3), true, 'g'},
        {"Test 23: 2/3 == 2/3", BigRational(2, 3), BigRational(2, 3), true, '='},
        {"Test 24: 2/3 != 1/3", BigRational(2, 3), BigRational(1, 3), true, '!'}
    };

    for (const auto& test : tests) {
        try {
            bool result;
            switch (test.operation) {
                case '<':
                    result = test.lhs < test.rhs;
                    break;
                case '>':
                    result = test.lhs > test.rhs;
                    break;
                case 'l':
                    result = test.lhs <= test.rhs;
                    break;
                case 'g':
                    result = test.lhs >= test.rhs;
                    break;
                case '=':
                    result = test.lhs == test.rhs;
                    break;
                case '!':
                    result = test.lhs != test.rhs;
                    break;
                default:
                    throw std::invalid_argument("Unknown operation");
            }

            bool pass = (result == test.expected_result);
            std::string status = pass ? "PASS" : "FAIL";

            std::cout << test.description << " | Expected: " << (test.expected_result ? "true" : "false") << " | Got: " << (result ? "true" : "false") << " | " << status << "\n";
        }
        catch (const std::exception& e) {
            std::string status = "FAIL (Exception Caught)";
            std::cout << test.description << " threw exception: " << e.what() << " | " << status << "\n";
        }
    }
}

void test_big_rational_comparisons() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        bool expected_less;
        bool expected_equal;
    };

 std::vector<TestCase> tests = {
        {"Test 1: 1/2 < 2/3", BigRational(1, 2), BigRational(2, 3), true, false},
        {"Test 2: 3/4 == 3/4", BigRational(3, 4), BigRational(3, 4), false, true},
        {"Test 3: -1/2 > -2/3", BigRational(-1, 2), BigRational(-2, 3), false, false},
        {"Test 4: 1/2 != 2/3", BigRational(1, 2), BigRational(2, 3), true, false},
        {"Test 5: 1/2 <= 2/3", BigRational(1, 2), BigRational(2, 3), true, false},
        {"Test 6: 3/4 >= 3/4", BigRational(3, 4), BigRational(3, 4), false, true},
        {"Test 7: -1/2 <= -2/3", BigRational(-1, 2), BigRational(-2, 3), false, false},
        {"Test 8: 1/2 >= 2/3", BigRational(1, 2), BigRational(2, 3), true, false}
    };

    for (const auto& test : tests) {
        bool less = test.lhs < test.rhs;
        bool equal = test.lhs == test.rhs;
        bool pass = (less == test.expected_less) && (equal == test.expected_equal);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description << " | Result: " << status << "\n";
    }
}

void test_big_rational_add_subtract() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
        char operation; // 'a' - сложение, 's' - вычитание
    };

    std::vector<TestCase> tests = {
        {"Test 1: 1/2 + 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(5, 6), 'a'},
        {"Test 2: -1/2 + 1/3", BigRational(-1, 2), BigRational(1, 3), BigRational(-1, 6), 'a'},
        {"Test 3: 1/2 - 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(1, 6), 's'},
        {"Test 4: -1/2 - (-1/3)", BigRational(-1, 2), BigRational(-1, 3), BigRational(-1, 6), 's'}
    };

    for (const auto& test : tests) {
        BigRational result;
        if (test.operation == 'a') {
            result = test.lhs + test.rhs;
        } else if (test.operation == 's') {
            result = test.lhs - test.rhs;
        }
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
    }
}

void test_big_rational_multiplication() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
    };

    std::vector<TestCase> tests = {
        // Тесты для умножения с различными конструкторами
        {"Test 1: 1/2 * 2/3", BigRational(1, 2), BigRational(2, 3), BigRational(1, 3)},
        {"Test 2: -1/2 * 2/3", BigRational(-1, 2), BigRational(2, 3), BigRational(-1, 3)},
        {"Test 3: 1/2 * -2/3", BigRational(1, 2), BigRational(-2, 3), BigRational(-1, 3)},
        {"Test 4: -1/2 * -2/3", BigRational(-1, 2), BigRational(-2, 3), BigRational(1, 3)},
        {"Test 5: 1/2 * 3/1 (int + string)", BigRational(1, 2), BigRational("3", "1"), BigRational(3, 2)},
        {"Test 6: -1/2 * 3/1 (int + string)", BigRational(-1, 2), BigRational("3", "1"), BigRational(-3, 2)},
        {"Test 7: 1/2 * -3/1 (int + string)", BigRational(1, 2), BigRational("-3", "1"), BigRational(-3, 2)},
        {"Test 8: -1/2 * -3/1 (int + string)", BigRational(-1, 2), BigRational("-3", "1"), BigRational(3, 2)},
        {"Test 9: 2/3 * 3/4 (string + int)", BigRational("2", "3"), BigRational(3, 4), BigRational(1, 2)},
        {"Test 10: -2/3 * 3/4 (string + int)", BigRational("-2", "3"), BigRational(3, 4), BigRational(-1, 2)},
        {"Test 11: 2/3 * -3/4 (string + int)", BigRational("2", "3"), BigRational(-3, 4), BigRational(-1, 2)},
        {"Test 12: -2/3 * -3/4 (string + int)", BigRational("-2", "3"), BigRational(-3, 4), BigRational(1, 2)}
    };

    for (const auto& test : tests) {
        BigRational result = test.lhs * test.rhs;
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
    }
}

void test_big_rational_division() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
    };

    std::vector<TestCase> tests = {
        // Тесты для деления с различными конструкторами и большими значениями
        {"Test 1: 123456789/987654321 / 2/3", BigRational("123456789", "987654321"), BigRational(2, 3), BigRational("370370367", "1975308642")},
        {"Test 2: -123456789/987654321 / 2/3", BigRational("-123456789", "987654321"), BigRational(2, 3), BigRational("-370370367", "1975308642")},
        {"Test 3: 123456789/987654321 / -2/3", BigRational("123456789", "987654321"), BigRational(-2, 3), BigRational("-370370367", "1975308642")},
        {"Test 4: -123456789/987654321 / -2/3", BigRational("-123456789", "987654321"), BigRational(-2, 3), BigRational("370370367", "1975308642")},
        {"Test 5: 987654321/123456789 / 3/1 (int + string)", BigRational("987654321", "123456789"), BigRational("3", "1"), BigRational("987654321", "370370367")},
        {"Test 6: -987654321/123456789 / 3/1 (int + string)", BigRational("-987654321", "123456789"), BigRational("3", "1"), BigRational("-987654321", "370370367")}
    };

    for (const auto& test : tests) {
        BigRational result = test.lhs / test.rhs;
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
    }
}

BigInteger generate_large_number(size_t num_digits) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string number;
    number.reserve(num_digits);

    // Первую цифру делаем ненулевой
    number += std::to_string(dis(gen) % 9 + 1);

    // Остальные цифры
    for (size_t i = 1; i < num_digits; ++i) {
        number += std::to_string(dis(gen));
    }

    return BigInteger(number);
}

void test_big_integer_operations() {
    struct TestCase {
        std::string description;
        BigInteger lhs;
        BigInteger rhs;
        BigInteger expected_result;
        char operation; // 'a' - сложение, 's' - вычитание, 'm' - умножение
    };

    // Генерация больших чисел для тестов
    BigInteger big_num1 = generate_large_number(10000); // 10000 цифр
    BigInteger big_num2 = generate_large_number(10000); // 10000 цифр

    std::vector<TestCase> tests = {
        {"Test 1: big_num1 + big_num2", big_num1, big_num2, big_num1 + big_num2, 'a'},
        {"Test 2: big_num1 - big_num2", big_num1, big_num2, big_num1 - big_num2, 's'},
        {"Test 3: big_num1 * big_num2", big_num1, big_num2, big_num1 * big_num2, 'm'}
    };

    for (const auto& test : tests) {
        BigInteger result;
        if (test.operation == 'a') {
            result = test.lhs + test.rhs;
        } else if (test.operation == 's') {
            result = test.lhs - test.rhs;
        } else if (test.operation == 'm') {
            result = test.lhs * test.rhs;
        }
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
    }
}

void test_big_integer_sqrt() {
    struct TestCase {
        std::string description;
        BigInteger input;
        double expected_result;
    };

    std::vector<TestCase> tests = {
        {"Test 1: sqrt(4)", BigInteger(4), 2.0},
        {"Test 2: sqrt(9)", BigInteger(9), 3.0},
        {"Test 3: sqrt(16)", BigInteger(16), 4.0},
        {"Test 4: sqrt(25)", BigInteger(25), 5.0},
        {"Test 5: sqrt(100)", BigInteger(100), 10.0},
        {"Test 6: sqrt(1)", BigInteger(1), 1.0},
        {"Test 7: sqrt(0)", BigInteger(0), 0.0},
        {"Test 8: sqrt(2)", BigInteger(2), 1.4142135624},
        {"Test 9: sqrt(3)", BigInteger(3), 1.7320508076},
        {"Test 10: sqrt(123456789)", BigInteger(123456789), 11111.111060555},
        {"Test 11: sqrt(1000000000000)", BigInteger(1000000000000), 1000000.0},
        {"Test 12: sqrt(999999999999999999)", BigInteger(999999999999999999), 999999999.9999999995},
        {"Test 13: sqrt(1000000000000000000)", BigInteger("1000000000000000000"), 1000000000.0},
        {"Test 14: sqrt(1000000000000000000000000000000)", BigInteger("1000000000000000000000000000000"), 1000000000000000.0},
        {"Test 15: sqrt(1e308)", BigInteger("1" + std::string(308, '0')), std::sqrt(1e308)},
        {"Test 16: sqrt(1e309)", BigInteger("1" + std::string(309, '0')), std::numeric_limits<double>::infinity()}         
        };

    for (const auto& test : tests) {
        try {
            double result = test.input.sqrt();
            bool pass = (std::abs(result - test.expected_result) < 1e-9);
            std::string status = pass ? "PASS" : "FAIL";
            std::cout << test.description
                      << " | Expected: " << test.expected_result
                      << " | Got: " << result
                      << " | " << status << "\n";
        } catch (const std::exception& e) {
            std::cout << test.description << " threw exception: " << e.what() << " | FAIL\n";
        }
    }
}


void test_big_rational_sqrt() {
    struct TestCase {
        std::string description;
        BigRational input;
        double expected_result;
    };

    std::vector<TestCase> tests = {
        {"Test 1: sqrt(4/1)", BigRational("4", "1"), 2.0},
        {"Test 2: sqrt(9/1)", BigRational("9", "1"), 3.0},
        {"Test 3: sqrt(16/1)", BigRational("16", "1"), 4.0},
        {"Test 4: sqrt(25/1)", BigRational("25", "1"), 5.0},
        {"Test 5: sqrt(100/1)", BigRational("100", "1"), 10.0},
        {"Test 6: sqrt(1/1)", BigRational("1", "1"), 1.0},
        {"Test 7: sqrt(0/1)", BigRational("0", "1"), 0.0},
        {"Test 8: sqrt(2/1)", BigRational("2", "1"), 1.4142135624},
        {"Test 9: sqrt(3/1)", BigRational("3", "1"), 1.7320508076},
        {"Test 10: sqrt(123456789/1)", BigRational("123456789", "1"), 11111.111060555},
        {"Test 11: sqrt(1000000000000/1)", BigRational("1000000000000", "1"), 1000000.0},
        {"Test 12: sqrt(999999999999999999/1)", BigRational("999999999999999999", "1"), 999999999.9999999995},
        {"Test 13: sqrt(1000000000000000000/1)", BigRational("1000000000000000000", "1"), 1000000000.0},
        {"Test 14: sqrt(1000000000000000000000000000000/1)", BigRational("1000000000000000000000000000000", "1"), 1000000000000000.0},
        {"Test 15: sqrt(1e308/1)", BigRational("1" + std::string(308, '0'), "1"), std::sqrt(1e308)},
        {"Test 16: sqrt(1e309/1)", BigRational("1" + std::string(309, '0'), "1"), std::numeric_limits<double>::infinity()}
    };

    for (const auto& test : tests) {
        try {
            double result = test.input.sqrt();
            bool pass = (std::abs(result - test.expected_result) < 1e-9);
            std::string status = pass ? "PASS" : "FAIL";
            std::cout << test.description
                      << " | Expected: " << test.expected_result
                      << " | Got: " << result
                      << " | Difference: " << std::abs(result - test.expected_result)
                      << " | " << status << "\n";
        } catch (const std::exception& e) {
            std::cout << test.description << " threw exception: " << e.what() << " | FAIL\n";
        }
    }
}
void test_big_integer_isqrt() {
    struct TestCase {
        std::string description;
        BigInteger input;
        BigInteger expected_result;
    };

std::vector<TestCase> tests = {
    {"Test 1: isqrt(7)", BigInteger(7), BigInteger(2)},
    {"Test 2: isqrt(17)", BigInteger(17), BigInteger(4)},
    {"Test 3: isqrt(37)", BigInteger(37), BigInteger(6)},
    {"Test 4: isqrt(57)", BigInteger(57), BigInteger(7)},
    {"Test 5: isqrt(77)", BigInteger(77), BigInteger(8)},
    {"Test 6: isqrt(97)", BigInteger(97), BigInteger(9)},
    {"Test 7: isqrt(117)", BigInteger(117), BigInteger(10)},
    {"Test 8: isqrt(137)", BigInteger(137), BigInteger(11)},
    {"Test 9: isqrt(157)", BigInteger("157"), BigInteger(12)},
    {"Test 10: isqrt(177)", BigInteger(177), BigInteger(13)},
    {"Test 11: isqrt(197)", BigInteger(197), BigInteger("14")},
    {"Test 12: isqrt(217)", BigInteger(217), BigInteger(14)},
    {"Test 13: isqrt(237)", BigInteger(237), BigInteger(15)},
    {"Test 14: isqrt(257)", BigInteger(257), BigInteger(16)},
    {"Test 15: isqrt(277)", BigInteger(277), BigInteger("16")},
    {"Test 16: isqrt(297)", BigInteger(297), BigInteger(17)},
    {"Test 17: isqrt(317)", BigInteger(317), BigInteger(17)},
    {"Test 18: isqrt(337)", BigInteger(337), BigInteger(18)},
    {"Test 19: isqrt(357)", BigInteger(357), BigInteger(18)},
    {"Test 20: isqrt(377)", BigInteger(377), BigInteger(19)},
    {"Test 21: isqrt(12345678901234567890)", BigInteger("12345678901234567890"), BigInteger("3513641828")},
    {"Test 22: isqrt(98765432109876543210)", BigInteger("98765432109876543210"), BigInteger("9938079904")},
    {"Test 23: isqrt(11223344556677889900)", BigInteger("11223344556677889900"), BigInteger("1059762314")},
    {"Test 24: isqrt(99887766554433221100)", BigInteger("99887766554433221100"), BigInteger("9994388245")},
    {"Test 25: isqrt(10203040506070809000)", BigInteger("10203040506070809000"), BigInteger("3194218148")}

};

    for (const auto& test : tests) {
        try {
            BigInteger result = test.input.isqrt();
            bool pass = (result == test.expected_result);
            std::string status = pass ? "PASS" : "FAIL";
            std::cout << test.description
                      << " | Expected: " << test.expected_result
                      << " | Got: " << result
                      << " | " << status << "\n";
        } catch (const std::exception& e) {
            std::cout << test.description << " threw exception: " << e.what() << " | FAIL\n";
        }
    }
}


// void test_big_integer_is_prime() {
//     struct TestCase {
//         std::string description;
//         BigInteger input;
//         bool expected_result;
//     };

//     std::vector<TestCase> tests = {
//         {"Test 1: is_prime(2)", BigInteger(2), true},
//         {"Test 2: is_prime(3)", BigInteger(3), true},
//         {"Test 3: is_prime(4)", BigInteger(4), false},
//         {"Test 4: is_prime(5)", BigInteger(5), true},
//         {"Test 5: is_prime(17)", BigInteger(17), true},
//         {"Test 6: is_prime(18)", BigInteger(18), false},
//         {"Test 7: is_prime(19)", BigInteger(19), true},
//         {"Test 8: is_prime(20)", BigInteger(20), false},
//         {"Test 9: is_prime(23)", BigInteger(23), true},
//         {"Test 10: is_prime(24)", BigInteger(24), false},
//         {"Test 11: is_prime(29)", BigInteger(29), true},
//         {"Test 12: is_prime(97)", BigInteger(97), true},
//         {"Test 13: is_prime(100)", BigInteger(100), false},
//         {"Test 14: is_prime(101)", BigInteger(101), true},
//         {"Test 15: is_prime(102)", BigInteger(102), false},
//         {"Test 16: is_prime(103)", BigInteger(103), true},
//         {"Test 17: is_prime(104)", BigInteger(104), false},
//         {"Test 18: is_prime(1009)", BigInteger(1009), true},
//         {"Test 19: is_prime(1024)", BigInteger(1024), false},
//         {"Test 20: is_prime(104729)", BigInteger(104729), true}, // 10000-е простое число
//         {"Test 21: is_prime(12345678901234567890)", BigInteger("12345678901234567890"), false},
//         {"Test 22: is_prime(9876543210987654321)", BigInteger("9876543210987654321"), false},
//         {"Test 23: is_prime(112233445566778899)", BigInteger("112233445566778899"), false},
//         {"Test 24: is_prime(998877665544332211)", BigInteger("998877665544332211"), false},
//         {"Test 25: is_prime(102030405060708090)", BigInteger("102030405060708090"), false}
//     };

//     for (const auto& test : tests) {
//         try {
//             bool result = test.input.is_prime(5); // 5 раундов теста Миллера-Рабина
//             bool pass = (result == test.expected_result);
//             std::string status = pass ? "PASS" : "FAIL";
//             std::cout << test.description
//                       << " | Expected: " << test.expected_result
//                       << " | Got: " << result
//                       << " | " << status << "\n";
//         } catch (const std::exception& e) {
//             std::cout << test.description << " threw exception: " << e.what() << " | FAIL\n";
//         }
//     }
// }

void test_rational_operations_recovery() {
    struct TestCase {
        std::string description;
        BigRational input1;
        BigRational input2;
        BigRational expected_result;
        char operation; // 'u' for unary minus, '+' for addition, '-' for subtraction
    };

    std::vector<TestCase> tests = {
        // UnaryMinus tests
        {"UnaryMinus: -1/2", BigRational("-1", "2"), BigRational("0", "1"), BigRational("1", "2"), 'u'},
        {"UnaryMinus: 3/4", BigRational("3", "4"), BigRational("0", "1"), BigRational("-3", "4"), 'u'},
        {"UnaryMinus: -123456789/987654321", BigRational("-123456789", "987654321"), BigRational("0", "1"), BigRational("123456789", "987654321"), 'u'},
        {"UnaryMinus: 123456789/987654321", BigRational("123456789", "987654321"), BigRational("0", "1"), BigRational("-123456789", "987654321"), 'u'},

        // BinaryAddition tests
        {"BinaryAddition: 1/2 + 1/3", BigRational("1", "2"), BigRational("1", "3"), BigRational("5", "6"), '+'},
        {"BinaryAddition: -1/2 + 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-1", "6"), '+'},
        {"BinaryAddition: 1/2 + -1/3", BigRational("1", "2"), BigRational("-1", "3"), BigRational("1", "6"), '+'},
        {"BinaryAddition: -1/2 + -1/3", BigRational("-1", "2"), BigRational("-1", "3"), BigRational("-5", "6"), '+'},
        {"BinaryAddition: 123456789/987654321 + 987654321/123456789", BigRational("123456789", "987654321"), BigRational("987654321", "123456789"), BigRational("990702636540161562", "121932631112635269"), '+'},

        // BinarySubtraction tests
        {"BinarySubtraction: 1/2 - 1/3", BigRational("1", "2"), BigRational("1", "3"), BigRational("1", "6"), '-'},
        {"BinarySubtraction: -1/2 - 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-5", "6"), '-'},
        {"BinarySubtraction: 1/2 - -1/3", BigRational("1", "2"), BigRational("-1", "3"), BigRational("5", "6"), '-'},
        {"BinarySubtraction: -1/2 - -1/3", BigRational("-1", "2"), BigRational("-1", "3"), BigRational("-1", "6"), '-'},
        {"BinarySubtraction: 123456789/987654321 - 987654321/123456789", BigRational("123456789", "987654321"), BigRational("987654321", "123456789"), BigRational("-990702636540161562", "121932631112635269"), '-'},

        // Tests with int64_t constructor
        {"BinaryAddition: 1/2 + 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(5, 6), '+'},
        {"BinaryAddition: -1/2 + 1/3", BigRational(-1, 2), BigRational(1, 3), BigRational(-1, 6), '+'},
        {"BinaryAddition: 1/2 + -1/3", BigRational(1, 2), BigRational(-1, 3), BigRational(1, 6), '+'},
        {"BinaryAddition: -1/2 + -1/3", BigRational(-1, 2), BigRational(-1, 3), BigRational(-5, 6), '+'},
        {"BinaryAddition: 123456789/987654321 + 987654321/123456789", BigRational(123456789, 987654321), BigRational(987654321, 123456789), BigRational("990702636540161562", "121932631112635269"), '+'},

        {"BinarySubtraction: 1/2 - 1/3", BigRational(1, 2), BigRational(1, 3), BigRational(1, 6), '-'},
        {"BinarySubtraction: -1/2 - 1/3", BigRational(-1, 2), BigRational(1, 3), BigRational(-5, 6), '-'},
        {"BinarySubtraction: 1/2 - -1/3", BigRational(1, 2), BigRational(-1, 3), BigRational(5, 6), '-'},
        {"BinarySubtraction: -1/2 - -1/3", BigRational(-1, 2), BigRational(-1, 3), BigRational(-1, 6), '-'},
        {"BinarySubtraction: 123456789/987654321 - 987654321/123456789", BigRational(123456789, 987654321), BigRational(987654321, 123456789), BigRational("-990702636540161562", "121932631112635269"), '-'}
    };

    for (const auto& test : tests) {
        try {
            BigRational result;
            switch (test.operation) {
                case 'u':
                    result = -test.input1;
                    break;
                case '+':
                    result = test.input1 + test.input2;
                    break;
                case '-':
                    result = test.input1 - test.input2;
                    break;
                default:
                    throw std::invalid_argument("Invalid operation");
            }
            bool pass = (result == test.expected_result);
            std::string status = pass ? "PASS" : "FAIL";
            std::cout << test.description
                      << " | Expected: " << test.expected_result
                      << " | Got: " << result
                      << " | " << status << "\n";
        } catch (const std::exception& e) {
            std::cout << test.description << " threw exception: " << e.what() << " | FAIL\n";
        }
    }
}

#include <limits>
#include "Big_Numbers.hpp"

// void run_test(const std::string& description, bool condition) {
//     std::cout << description << " | " << (condition ? "PASS" : "FAIL") << std::endl;
// }

// void test_big_rational_operations() {
//     // Basic Addition
//     {
//         BigRational a(1, 2);         // Represents 1/2
//         BigRational b(1, 3);         // Represents 1/3
//         BigRational expected(5, 6);  // Expected result is 5/6
//         run_test("BigRational Basic Addition", a + b == expected);
//     }

//     // Basic Subtraction
//     {
//         BigRational a(3, 4);         // Represents 3/4
//         BigRational b(1, 4);         // Represents 1/4
//         BigRational expected(1, 2);  // Expected result is 1/2
//         // output declaration
//         BigRational output = a - b;
//         run_test("BigRational Basic Subtraction", output == expected);
//         std::cout << output << std::endl;
//         std::cout << expected << std::endl;
//     }

//     // Multiplication with Negative Numbers
//     {
//         BigRational a(-3, 7);  // Represents -3/7
//         BigRational b(2, 5);   // Represents 2/5
//         BigRational expected(-6, 35);  // Expected result is -6/35
//         run_test("BigRational Multiplication with Negative Numbers", a * b == expected);
//     }

//     // Division and Simplification
//     {
//         BigRational a(4, 6);  // Represents 4/6
//         BigRational b(2, 3);  // Represents 2/3
//         BigRational expected(1, 1);  // Expected result is 1/1 or 1
//         run_test("BigRational Division and Simplification", a / b == expected);
//     }

//     // Equality Check
//     {
//         BigRational a(8, 12);  // Represents 8/12
//         BigRational b(2, 3);   // Represents 2/3
//         run_test("BigRational Equality Check", a == b);  // They are equal when simplified
//     }

//     // Inequality Check
//     {
//         BigRational a(1, 3);  // Represents 1/3
//         BigRational b(1, 2);  // Represents 1/2
//         run_test("BigRational Inequality Check", a < b);  // 1/3 is less than 1/2
//     }

//     // Zero Numerator Check
//     {
//         try {
//             BigRational a(1, 0);  // Should throw an exception
//             run_test("BigRational Zero Numerator Check", false);  // Should not reach here
//         } catch (const std::exception&) {
//             run_test("BigRational Zero Numerator Check", true);  // Exception thrown as expected
//         }
//     }

//     // Negative Numerator Check
//     {
//         BigRational a(-1, -2);  // Represents -1/-2
//         BigRational expected(1, 2);  // Expected result is 1/2
//         run_test("BigRational Negative Numerator Check", a == expected);
//     }

//     // Large Number Addition
//     {
//         BigRational a("123456789123456789", "987654321987654321");
//         BigRational b("987654321987654321", "123456789123456789");
//         BigRational expected("12230896747409402", "1505341124847349");  // Calculated result
//         run_test("BigRational Large Number Addition", a + b == expected);
//     }

//     // Mixed Operations
//     {
//         BigRational a("1", "2"), b("1", "3"), c("1", "4");
//         run_test("BigRational Mixed Operations", a + b - c == BigRational("7", "12"));
//     }

//     // Division by Zero
//     {
//         BigRational a("1", "2");
//         try {
//             a / BigRational("0", "1");
//             run_test("BigRational Division by Zero", false);  // Should not reach here
//         } catch (const std::exception&) {
//             run_test("BigRational Division by Zero", true);  // Exception thrown as expected
//         }
//     }

//     // Zero Comparison Equal
//     {
//         run_test("BigRational Zero Comparison Equal", BigRational("0", "1") == BigRational("0", "2"));
//     }

//     // Zero Comparison Not Equal
//     {
//         run_test("BigRational Zero Comparison Not Equal", BigRational("0", "1") != BigRational("1", "2"));
//     }

//     // One Comparison Equal
//     {
//         run_test("BigRational One Comparison Equal", BigRational("1", "1") == BigRational("2", "2"));
//     }

//     // One Comparison Not Equal
//     {
//         run_test("BigRational One Comparison Not Equal", BigRational("1", "1") != BigRational("1", "2"));
//     }

//     // Negative One Comparison Equal
//     {
//         run_test("BigRational Negative One Comparison Equal", BigRational("-1", "1") == BigRational("2", "-2"));
//     }

//     // Negative One Comparison Not Equal
//     {
//         run_test("BigRational Negative One Comparison Not Equal", BigRational("-1", "1") != BigRational("1", "1"));
//     }

//     // Fraction Comparison Equal
//     {
//         run_test("BigRational Fraction Comparison Equal", BigRational("1", "2") == BigRational("2", "4"));
//     }

//     // Fraction Comparison Not Equal
//     {
//         run_test("BigRational Fraction Comparison Not Equal", BigRational("1", "2") != BigRational("3", "4"));
//     }

//     // Large Number Comparison Equal
//     {
//         run_test("BigRational Large Number Comparison Equal", BigRational("123456789", "123456789") == BigRational("246913578", "246913578"));
//     }

//     // Mixed Sign Comparison Equal
//     {
//         run_test("BigRational Mixed Sign Comparison Equal", BigRational("-1", "2") == BigRational("1", "-2"));
//     }

//     // Mixed Sign Comparison Not Equal
//     {
//         run_test("BigRational Mixed Sign Comparison Not Equal", BigRational("-1", "2") != BigRational("1", "2"));
//     }

//     // Zero Numerator Comparison Equal
//     {
//         run_test("BigRational Zero Numerator Comparison Equal", BigRational("0", "123456789") == BigRational("0", "987654321"));
//     }

//     // Zero Numerator Comparison Not Equal
//     {
//         run_test("BigRational Zero Numerator Comparison Not Equal", BigRational("0", "123456789") != BigRational("1", "123456789"));
//     }

//     // One Numerator Comparison Equal
//     {
//         run_test("BigRational One Numerator Comparison Equal", BigRational("123456789", "1") == BigRational("123456789", "1"));
//     }

//     // One Numerator Comparison Not Equal
//     {
//         run_test("BigRational One Numerator Comparison Not Equal", BigRational("123456789", "1") != BigRational("987654321", "1"));
//     }

//     // Negative Numerator Comparison Equal
//     {
//         run_test("BigRational Negative Numerator Comparison Equal", BigRational("1", "-1") == BigRational("-1", "1"));
//     }

//     // Negative Numerator Comparison Not Equal
//     {
//         run_test("BigRational Negative Numerator Comparison Not Equal", BigRational("1", "-1") != BigRational("1", "1"));
//     }

//     // Complex Fraction Comparison Equal
//     {
//         run_test("BigRational Complex Fraction Comparison Equal", BigRational("1", "3") == BigRational("2", "6"));
//     }

//     // Complex Fraction Comparison Not Equal
//     {
//         run_test("BigRational Complex Fraction Comparison Not Equal", BigRational("1", "3") != BigRational("1", "6"));
//     }
// }


// std::string BigIntegerToString(const BigInteger& value) {
//     std::ostringstream oss;
//     oss << value;
//     return oss.str();
// }

// void test_big_integer_operations_assert() {
//     // FromInt64Min
//     {
//         run_test("FromInt64Min", BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())) == BigIntegerToString(BigInteger("-9223372036854775808")));
//         std::cout << BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())) << std::endl;
//         std::cout << BigInteger("-9223372036854775808") << std::endl;
        
//         run_test("FromInt Zero", BigInteger("0") == BigInteger(0));
//     }
// }


// void test_case_int64() {
//     struct TestCase {
//         std::string description;
//         BigInteger lhs;
//         BigInteger rhs;
//         BigInteger expected_result;
//         char operation;
//     };

//     std::vector<TestCase> tests = {
//         {"Test 1: +000000000123456789 + +000000000000012345", BigInteger("000000000123456789"), BigInteger("000000000000012345"), BigInteger("000000000123469134"), '+'},
//         {"Test 2: +000000000987654321 - +000000000000012345", BigInteger("000000000987654321"), BigInteger("000000000000012345"), BigInteger("000000000987641976"), '-'},
//         {"Test 3: -000000000987654321 * +000000000000012345", BigInteger("-000000000987654321"), BigInteger("000000000000012345"), BigInteger("-1219326311126352695"), '*'},
//         {"Test 4: +000000000987654321 / -000000000000012345", BigInteger("000000000987654321"), BigInteger("-000000000000012345"), BigInteger("-80000"), '/'},
//         {"Test 5: -000000000987654321 % -000000000000012345", BigInteger("-000000000987654321"), BigInteger("-000000000000012345"), BigInteger("-9876"), '%'},
//         {"Test 6: +0000000001000000000 += +000000000000000003", BigInteger("0000000001000000000"), BigInteger("000000000000000003"), BigInteger("0000000001000000003"), 'a'},
//         {"Test 7: -0000000001000000000 -= +000000000000000003", BigInteger("-0000000001000000000"), BigInteger("000000000000000003"), BigInteger("-0000000001000000003"), 's'},
//         {"Test 8: +0000000001000000000 *= -000000000000000003", BigInteger("0000000001000000000"), BigInteger("-000000000000000003"), BigInteger("-0000000003000000000"), 'm'},
//         {"Test 9: -0000000001000000000 /= -000000000000000003", BigInteger("-0000000001000000000"), BigInteger("-000000000000000003"), BigInteger("333333333"), 'd'},
//         {"Test 10: +0000000000000000000 %= +000000000123456789", BigInteger("0000000000000000000"), BigInteger("000000000123456789"), BigInteger("0000000000000000000"), 'r'},
//         {"Test 11: +000000000123456789 < +000000000000000001", BigInteger("000000000123456789"), BigInteger("000000000000000001"), BigInteger(0), '<'},
//         {"Test 12: +000000000123456789 > +000000000123456789", BigInteger("000000000123456789"), BigInteger("000000000123456789"), BigInteger(0), '>'},
//         {"Test 13: +000000000123456789 <= +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(0), 'l'},
//         {"Test 14: +000000000123456789 >= +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(1), 'g'},
//         {"Test 15: +000000000123456789 == +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(0), '='},
//         {"Test 16: +000000000123456789 != +000000000000000000", BigInteger("000000000123456789"), BigInteger("000000000000000000"), BigInteger(1), '!'}
//     };

//     for (const auto& test : tests) {
//         try {
//             BigInteger result;
//             bool comparison_result = false;
//             switch (test.operation) {
//                 case '+':
//                     result = test.lhs + test.rhs;
//                     break;
//                 case '-':
//                     result = test.lhs - test.rhs;
//                     break;
//                 case '*':
//                     result = test.lhs * test.rhs;
//                     break;
//                 case '/':
//                     result = test.lhs / test.rhs;
//                     break;
//                 case '%':
//                     result = test.lhs % test.rhs;
//                     break;
//                 case 'a':
//                     result = test.lhs;
//                     result += test.rhs;
//                     break;
//                 case 's':
//                     result = test.lhs;
//                     result -= test.rhs;
//                     break;
//                 case 'm':
//                     result = test.lhs;
//                     result *= test.rhs;
//                     break;
//                 case 'd':
//                     result = test.lhs;
//                     result /= test.rhs;
//                     break;
//                 case 'r':
//                     result = test.lhs;
//                     result %= test.rhs;
//                     break;
//                 case '<':
//                     comparison_result = test.lhs < test.rhs;
//                     break;
//                 case '>':
//                     comparison_result = test.lhs > test.rhs;
//                     break;
//                 case 'l':
//                     comparison_result = test.lhs <= test.rhs;
//                     break;
//                 case 'g':
//                     comparison_result = test.lhs >= test.rhs;
//                     break;
//                 case '=':
//                     comparison_result = test.lhs == test.rhs;
//                     break;
//                 case '!':
//                     comparison_result = test.lhs != test.rhs;
//                     break;
//                 default:
//                     throw std::invalid_argument("Unknown operation");
//             }

//             bool pass = (test.operation == '<' || test.operation == '>' || test.operation == 'l' || test.operation == 'g' || test.operation == '=' || test.operation == '!') ? (comparison_result == (test.expected_result != BigInteger(0))) : (result == test.expected_result);
//             std::string status = pass ? "PASS" : "FAIL";

//             std::cout << test.description << " | Expected: " << test.expected_result << " | Got: " << result << " | " << status << "\n";
//         }
//         catch (const std::exception& e) {
//             bool pass = (test.operation == '/' || test.operation == 'd' || test.operation == '%') && (test.rhs == BigInteger(0));
//             std::string status = pass ? "PASS (Exception Caught)" : "FAIL (Unexpected Exception)";
//             std::cout << test.description << " threw exception: " << e.what() << " | " << status << "\n";
//         }
//     }
// }

// TEST_CASE("FromInt ❌FromInt64Min") {
//   CHECK(BigIntegerToString(BigInteger(std::numeric_limits<int64_t>::min())) ==
//         BigIntegerToString(BigInteger("-9223372036854775808")));
//   CHECK(BigInteger("0") == BigInteger(0));
// }






















































































































































void run_test(const std::string& description, bool condition, const std::string& expected = "", const std::string& actual = "") {
    std::cout << description << " | " << (condition ? "PASS" : "FAIL") << "\n";
    if (!expected.empty() && !actual.empty()) {
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Actual: " << actual << "\n";
    }
}

std::string BigIntegerToString(const BigInteger& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string BigRationalToString(const BigRational& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void test_big_integer_operations2() {
    // FromInt64Min
    {
        BigInteger expected = BigInteger("-9223372036854775808");
        BigInteger actual = BigInteger(std::numeric_limits<int64_t>::min());
        run_test("FromInt64Min", BigIntegerToString(actual) == BigIntegerToString(expected), BigIntegerToString(expected), BigIntegerToString(actual));
        run_test("FromInt Zero", BigInteger("0") == BigInteger(0), "0", BigIntegerToString(BigInteger("0")));
    }

    // FromString
    {
        BigInteger expected = BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        BigInteger actual = BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        run_test("FromString - big one", actual == expected, BigIntegerToString(expected), BigIntegerToString(actual));

        expected = BigInteger("-123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        actual = BigInteger("-123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        run_test("FromString - negative big one", actual == expected, BigIntegerToString(expected), BigIntegerToString(actual));
    }

    // BigIntegerBinaryAddition
    {
        BigInteger a("123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        BigInteger b("123456789123456789123456789123456789123456789123456789123456789123456789123456789");
        BigInteger expected = BigInteger("246913578246913578246913578246913578246913578246913578246913578246913578246913578");
        run_test("BigIntegerBinaryAddition", a + b == expected, BigIntegerToString(expected), BigIntegerToString(a + b));

        expected = BigInteger("123456789123456789123456789123456789123456789123456789123456789123456789123456789") * BigInteger("2");
        run_test("BigIntegerBinaryAddition with multiplication", a + b == expected, BigIntegerToString(expected), BigIntegerToString(a + b));
    }

    // Division
    {
        BigInteger a("12345678912345678916789123456789123456789");
        BigInteger b("1234567891233456786789123456789123456782");
        run_test("Division by zero throws", false, "", "");  // Placeholder for exception test
        try {
            BigInteger("+1a");
        } catch (const std::exception& e) {
            run_test("Division by zero throws", true, "Exception thrown", e.what());
        }

        run_test("Division test 1", ((a * b) / b) == a, BigIntegerToString(a), BigIntegerToString((a * b) / b));
        run_test("Division test 2", (a * b) / a == b, BigIntegerToString(b), BigIntegerToString((a * b) / a));
        run_test("Addition test 1", BigInteger(0) + BigInteger(0) == BigInteger(0), "0", BigIntegerToString(BigInteger(0) + BigInteger(0)));
        run_test("Addition test 2", BigInteger(0) + BigInteger(1) == BigInteger("1"), "1", BigIntegerToString(BigInteger(0) + BigInteger(1)));
    }

    // Other tests
    {
        run_test("TenDivTwoEqualsFive", BigInteger(10) / BigInteger(2) == BigInteger(5), "5", BigIntegerToString(BigInteger(10) / BigInteger(2)));
        run_test("TenDivThreeEqualsThree", BigInteger(10) / BigInteger(3) == BigInteger(3), "3", BigIntegerToString(BigInteger(10) / BigInteger(3)));
        run_test("TenModThreeEqualsOne", BigInteger(10) % BigInteger(3) == BigInteger(1), "1", BigIntegerToString(BigInteger(10) % BigInteger(3)));
        run_test("_255DivOneEquals255", BigInteger(255) / BigInteger(1) == BigInteger(255), "255", BigIntegerToString(BigInteger(255) / BigInteger(1)));
        run_test("_255DivMinusOneEqualsM255", BigInteger(255) / BigInteger(-1) == BigInteger(-255), "-255", BigIntegerToString(BigInteger(255) / BigInteger(-1)));
        run_test("Minus10IsMinus10", -BigInteger(10) == BigInteger(-10), "-10", BigIntegerToString(-BigInteger(10)));
        run_test("OneGreaterThanZero", BigInteger(1) > BigInteger(0), "true", "true");
        run_test("NotZeroGreaterThanOne", !(BigInteger(0) > BigInteger(1)), "true", "true");
        run_test("ZeroLeqOne", BigInteger(0) <= BigInteger(1), "true", "true");
        run_test("NotZeroGeqOne", !(BigInteger(0) >= BigInteger(1)), "true", "true");
        run_test("SpecialDiv2", BigInteger("-11001000000") / BigInteger(86400) == BigInteger(-127326), "-127326", BigIntegerToString(BigInteger("-11001000000") / BigInteger(86400)));
        run_test("SpecialMod", BigInteger("20195283520469175757") % BigInteger("1048576") == BigInteger("888269"), "888269", BigIntegerToString(BigInteger("20195283520469175757") % BigInteger("1048576")));
        run_test("SpecialModNeg", BigInteger(860593) % BigInteger(-201) == BigInteger(112), "112", BigIntegerToString(BigInteger(860593) % BigInteger(-201)));
        run_test("SpecialModNeg2", BigInteger("-18224909727634776050312394179610579601844989529623334093909233530432892596607") / BigInteger("14954691977398614017") == BigInteger("-1218675032235937780712688130619504358062280950643422399658"), "-1218675032235937780712688130619504358062280950643422399658", BigIntegerToString(BigInteger("-18224909727634776050312394179610579601844989529623334093909233530432892596607") / BigInteger("14954691977398614017")));
        run_test("Minus5Divides2EqualsMinus2", BigInteger(-5) / BigInteger(2) == BigInteger(-2), "-2", BigIntegerToString(BigInteger(-5) / BigInteger(2)));
        run_test("FiveDividesMinus2EqualsMinus2", BigInteger(5) / BigInteger(-2) == BigInteger(-2), "-2", BigIntegerToString(BigInteger(5) / BigInteger(-2)));
        run_test("FiveDivides2Equals2", BigInteger(5) / BigInteger(2) == BigInteger(2), "2", BigIntegerToString(BigInteger(5) / BigInteger(2)));
        run_test("MinusFiveDividesMinus2Equals2", BigInteger(-5) / BigInteger(-2) == BigInteger(2), "2", BigIntegerToString(BigInteger(-5) / BigInteger(-2)));
        run_test("Minus10Mod3EqualsMinus1", BigInteger(-10) % BigInteger(3) == BigInteger(-1), "-1", BigIntegerToString(BigInteger(-10) % BigInteger(3)));
        run_test("TenMod3Equals1", BigInteger(10) % BigInteger(3) == BigInteger(1), "1", BigIntegerToString(BigInteger(10) % BigInteger(3)));
        run_test("TenModMinus3Equals1", BigInteger(10) % BigInteger(-3) == BigInteger(1), "1", BigIntegerToString(BigInteger(10) % BigInteger(-3)));
        run_test("MinusTenModMinus3EqualsMinus1", BigInteger(-10) % BigInteger(-3) == BigInteger(-1), "-1", BigIntegerToString(BigInteger(-10) % BigInteger(-3)));
        run_test("TenModMinus5Equals0", BigInteger(10) % BigInteger(-5) == BigInteger(0), "0", BigIntegerToString(BigInteger(10) % BigInteger(-5)));
        run_test("TenModTwoEqualsZero", BigInteger(10) % BigInteger(2) == BigInteger(0), "0", BigIntegerToString(BigInteger(10) % BigInteger(2)));
    }

    // BigInteger Division2
    {
        run_test("BigInteger Division2 - test 1", BigInteger("123456789123456789123456789") / BigInteger("123456789") == BigInteger("1000000001000000001"), "1000000001000000001", BigIntegerToString(BigInteger("123456789123456789123456789") / BigInteger("123456789")));
        run_test("BigInteger Division2 - test 2", BigInteger("1000000000000") / BigInteger("1000") == BigInteger("1000000000"), "1000000000", BigIntegerToString(BigInteger("1000000000000") / BigInteger("1000")));
        run_test("BigInteger Division2 - test 3", BigInteger("1000") / BigInteger("1000000000000") == BigInteger("0"), "0", BigIntegerToString(BigInteger("1000") / BigInteger("1000000000000")));
        run_test("BigInteger Division2 - test 4", BigInteger("-123456789123456789123456789") / BigInteger("123456789") == BigInteger("-1000000001000000001"), "-1000000001000000001", BigIntegerToString(BigInteger("-123456789123456789123456789") / BigInteger("123456789")));
        run_test("BigInteger Division2 - test 5", false, "", "");  // Placeholder for exception test
        try {
            BigInteger("123456789") / BigInteger("0");
        } catch (const std::exception& e) {
            run_test("BigInteger Division2 - test 5", true, "Exception thrown", e.what());
        }
    }
}









void test_big_rational_operations2() {
    // Basic Addition
    {
        BigRational a(1, 2);         // Represents 1/2
        BigRational b(1, 3);         // Represents 1/3
        BigRational expected(5, 6);  // Expected result is 5/6
        run_test("BigRational Basic Addition", a + b == expected, BigRationalToString(expected), BigRationalToString(a + b));
    }

    // Basic Subtraction
    {
        BigRational a(3, 4);         // Represents 3/4
        BigRational b(1, 4);         // Represents 1/4
        BigRational expected(1, 2);  // Expected result is 1/2
        run_test("BigRational Basic Subtraction", a - b == expected, BigRationalToString(expected), BigRationalToString(a - b));
    }

    // Multiplication with Negative Numbers
    {
        BigRational a(-3, 7);  // Represents -3/7
        BigRational b(2, 5);   // Represents 2/5
        BigRational expected(-6, 35);  // Expected result is -6/35
        run_test("BigRational Multiplication with Negative Numbers", a * b == expected, BigRationalToString(expected), BigRationalToString(a * b));
    }

    // Division and Simplification
    {
        BigRational a(4, 6);  // Represents 4/6
        BigRational b(2, 3);  // Represents 2/3
        BigRational expected(1, 1);  // Expected result is 1/1 or 1
        run_test("BigRational Division and Simplification", a / b == expected, BigRationalToString(expected), BigRationalToString(a / b));
    }

    // Equality Check
    {
        BigRational a(8, 12);  // Represents 8/12
        BigRational b(2, 3);   // Represents 2/3
        run_test("BigRational Equality Check", a == b, BigRationalToString(b), BigRationalToString(a));  // They are equal when simplified
    }

    // Inequality Check
    {
        BigRational a(1, 3);  // Represents 1/3
        BigRational b(1, 2);  // Represents 1/2
        run_test("BigRational Inequality Check", a < b, BigRationalToString(b), BigRationalToString(a));  // 1/3 is less than 1/2
    }

    // Zero Numerator Check
    {
        try {
            BigRational a(1, 0);  // Should throw an exception
            run_test("BigRational Zero Numerator Check", false, "", "");  // Should not reach here
        } catch (const std::exception& e) {
            run_test("BigRational Zero Numerator Check", true, "Exception thrown", e.what());  // Exception thrown as expected
        }
    }

    // Negative Numerator Check
    {
        BigRational a(-1, -2);  // Represents -1/-2
        BigRational expected(1, 2);  // Expected result is 1/2
        run_test("BigRational Negative Numerator Check", a == expected, BigRationalToString(expected), BigRationalToString(a));
    }

    // Large Number Addition
    {
        BigRational a("123456789123456789", "987654321987654321");
        BigRational b("987654321987654321", "123456789123456789");
        BigRational expected("12230896747409402", "1505341124847349");  // Calculated result
        run_test("BigRational Large Number Addition", a + b == expected, BigRationalToString(expected), BigRationalToString(a + b));
    }

    // Mixed Operations
    {
        BigRational a("1", "2"), b("1", "3"), c("1", "4");
        run_test("BigRational Mixed Operations", a + b - c == BigRational("7", "12"), BigRationalToString(BigRational("7", "12")), BigRationalToString(a + b - c));
    }

    // Division by Zero
    {
        BigRational a("1", "2");
        try {
            a / BigRational("0", "1");
            run_test("BigRational Division by Zero", false, "", "");  // Should not reach here
        } catch (const std::exception& e) {
            run_test("BigRational Division by Zero", true, "Exception thrown", e.what());  // Exception thrown as expected
        }
    }

    // Zero Comparison Equal
    {
        run_test("BigRational Zero Comparison Equal", BigRational("0", "1") == BigRational("0", "2"), BigRationalToString(BigRational("0", "2")), BigRationalToString(BigRational("0", "1")));
    }

    // Zero Comparison Not Equal
    {
        run_test("BigRational Zero Comparison Not Equal", BigRational("0", "1") != BigRational("1", "2"), BigRationalToString(BigRational("1", "2")), BigRationalToString(BigRational("0", "1")));
    }

    // One Comparison Equal
    {
        run_test("BigRational One Comparison Equal", BigRational("1", "1") == BigRational("2", "2"), BigRationalToString(BigRational("2", "2")), BigRationalToString(BigRational("1", "1")));
    }

    // One Comparison Not Equal
    {
        run_test("BigRational One Comparison Not Equal", BigRational("1", "1") != BigRational("1", "2"), BigRationalToString(BigRational("1", "2")), BigRationalToString(BigRational("1", "1")));
    }

    // Negative One Comparison Equal
    {
        run_test("BigRational Negative One Comparison Equal", BigRational("-1", "1") == BigRational("2", "-2"), BigRationalToString(BigRational("2", "-2")), BigRationalToString(BigRational("-1", "1")));
    }

    // Negative One Comparison Not Equal
    {
        run_test("BigRational Negative One Comparison Not Equal", BigRational("-1", "1") != BigRational("1", "1"), BigRationalToString(BigRational("1", "1")), BigRationalToString(BigRational("-1", "1")));
    }

    // Fraction Comparison Equal
    {
        run_test("BigRational Fraction Comparison Equal", BigRational("1", "2") == BigRational("2", "4"), BigRationalToString(BigRational("2", "4")), BigRationalToString(BigRational("1", "2")));
    }

    // Fraction Comparison Not Equal
    {
        run_test("BigRational Fraction Comparison Not Equal", BigRational("1", "2") != BigRational("3", "4"), BigRationalToString(BigRational("3", "4")), BigRationalToString(BigRational("1", "2")));
    }

    // Large Number Comparison Equal
    {
        run_test("BigRational Large Number Comparison Equal", BigRational("123456789", "123456789") == BigRational("246913578", "246913578"), BigRationalToString(BigRational("246913578", "246913578")), BigRationalToString(BigRational("123456789", "123456789")));
    }

    // Mixed Sign Comparison Equal
    {
        run_test("BigRational Mixed Sign Comparison Equal", BigRational("-1", "2") == BigRational("1", "-2"), BigRationalToString(BigRational("1", "-2")), BigRationalToString(BigRational("-1", "2")));
    }

    // Mixed Sign Comparison Not Equal
    {
        run_test("BigRational Mixed Sign Comparison Not Equal", BigRational("-1", "2") != BigRational("1", "2"), BigRationalToString(BigRational("1", "2")), BigRationalToString(BigRational("-1", "2")));
    }

    // Zero Numerator Comparison Equal
    {
        run_test("BigRational Zero Numerator Comparison Equal", BigRational("0", "123456789") == BigRational("0", "987654321"), BigRationalToString(BigRational("0", "987654321")), BigRationalToString(BigRational("0", "123456789")));
    }

    // Zero Numerator Comparison Not Equal
    {
        run_test("BigRational Zero Numerator Comparison Not Equal", BigRational("0", "123456789") != BigRational("1", "123456789"), BigRationalToString(BigRational("1", "123456789")), BigRationalToString(BigRational("0", "123456789")));
    }

    // One Numerator Comparison Equal
    {
        run_test("BigRational One Numerator Comparison Equal", BigRational("123456789", "1") == BigRational("123456789", "1"), BigRationalToString(BigRational("123456789", "1")), BigRationalToString(BigRational("123456789", "1")));
    }

    // One Numerator Comparison Not Equal
    {
        run_test("BigRational One Numerator Comparison Not Equal", BigRational("123456789", "1") != BigRational("987654321", "1"), BigRationalToString(BigRational("987654321", "1")), BigRationalToString(BigRational("123456789", "1")));
    }

    // Negative Numerator Comparison Equal
    {
        run_test("BigRational Negative Numerator Comparison Equal", BigRational("1", "-1") == BigRational("-1", "1"), BigRationalToString(BigRational("-1", "1")), BigRationalToString(BigRational("1", "-1")));
    }

    // Negative Numerator Comparison Not Equal
    {
        run_test("BigRational Negative Numerator Comparison Not Equal", BigRational("1", "-1") != BigRational("1", "1"), BigRationalToString(BigRational("1", "1")), BigRationalToString(BigRational("1", "-1")));
    }

    // Complex Fraction Comparison Equal
    {
        run_test("BigRational Complex Fraction Comparison Equal", BigRational("1", "3") == BigRational("2", "6"), BigRationalToString(BigRational("2", "6")), BigRationalToString(BigRational("1", "3")));
    }

    // Complex Fraction Comparison Not Equal
    {
        run_test("BigRational Complex Fraction Comparison Not Equal", BigRational("1", "3") != BigRational("1", "6"), BigRationalToString(BigRational("1", "6")), BigRationalToString(BigRational("1", "3")));
    }
}


void test_unarny_op_and_sub_add_op() {
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
        char operation; // 'u' for unary minus, '+' for addition, '-' for subtraction
    };

std::vector<TestCase> tests = {
    // Unary Minus tests
    {"UnaryMinus: -1/2", BigRational("1", "2"), BigRational(), BigRational("-1", "2"), 'u'},
    {"UnaryMinus: -3/4", BigRational("3", "4"), BigRational(), BigRational("-3", "4"), 'u'},
    {"UnaryMinus: -0/1", BigRational("0", "1"), BigRational(), BigRational("0", "1"), 'u'},
    {"UnaryMinus: -(-1/2)", BigRational("-1", "2"), BigRational(), BigRational("1", "2"), 'u'},
    {"UnaryMinus: -(-3/-4)", BigRational("-3", "-4"), BigRational(), BigRational("3", "4"), 'u'},

    // Addition tests
    {"Addition: 1/2 + 1/3", BigRational("1", "2"), BigRational("1", "3"), BigRational("5", "6"), '+'},
    {"Addition: -1/2 + 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-1", "6"), '+'},
    {"Addition: 1/2 + -1/3", BigRational("1", "2"), BigRational("-1", "3"), BigRational("1", "6"), '+'},
    {"Addition: -1/2 + -1/3", BigRational("-1", "2"), BigRational("-1", "3"), BigRational("-5", "6"), '+'},
    {"Addition: 0/1 + 1/3", BigRational("0", "1"), BigRational("1", "3"), BigRational("1", "3"), '+'},
    {"Addition: -1/-2 + 1/3", BigRational("-1", "-2"), BigRational("1", "3"), BigRational("5", "6"), '+'},
    {"Addition: 1/-2 + -1/3", BigRational("1", "-2"), BigRational("-1", "3"), BigRational("-1", "6"), '+'},

    // Subtraction tests
    {"Subtraction: 1/2 - 1/3", BigRational("1", "2"), BigRational("1", "3"), BigRational("1", "6"), '-'},
    {"Subtraction: -1/2 - 1/3", BigRational("-1", "2"), BigRational("1", "3"), BigRational("-5", "6"), '-'},
    {"Subtraction: 1/2 - -1/3", BigRational("1", "2"), BigRational("-1", "3"), BigRational("5", "6"), '-'},
    {"Subtraction: -1/2 - -1/3", BigRational("-1", "2"), BigRational("-1", "3"), BigRational("-1", "6"), '-'},
    {"Subtraction: 0/1 - 1/3", BigRational("0", "1"), BigRational("1", "3"), BigRational("-1", "3"), '-'},
    {"Subtraction: -1/-2 - 1/3", BigRational("-1", "-2"), BigRational("1", "3"), BigRational("1", "6"), '-'},
    {"Subtraction: 1/-2 - -1/3", BigRational("1", "-2"), BigRational("-1", "3"), BigRational("-1", "6"), '-'}
};

    for (const auto& test : tests) {
        BigRational result;
        if (test.operation == 'u') {
            result = -test.lhs;
        } else if (test.operation == '+') {
            result = test.lhs + test.rhs;
        } else if (test.operation == '-') {
            result = test.lhs - test.rhs;
        }
        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
    }
}


void test_input_operator() {
    struct TestCase {
        std::string input;
        std::string expected_output;
        bool should_fail;
    };

    std::vector<TestCase> tests = {
        {"12345", "12345", false},
        {"  12345  ", "12345", false},
        {"-12345", "-12345", false},
        {"  -12345  ", "-12345", false},
        {"+12345", "12345", false},
        {"  +12345  ", "12345", false},
        {"0", "0", false},
        {"  0  ", "0", false},
        {"-0", "0", false},
        {"  -0  ", "0", false},
        {"xyz", "", true},
        {"-xyz", "", true},
        {"", "", true},
        {"-", "", true},
        {"  -  ", "", true}
    };

    for (const auto& test : tests) {
        std::istringstream iss(test.input);
        BigInteger value;
        iss >> value;

        std::cout << "Testing input: \"" << test.input << "\"\n";

        if (test.should_fail) {
            if (iss.fail()) {
                std::cout << "Test passed for input: \"" << test.input << "\" (expected fail)\n";
            } else {
                std::cout << "Test failed for input: \"" << test.input << "\" (expected fail)\n";
            }
        } else {
            if (iss.fail()) {
                std::cout << "Test failed for input: \"" << test.input << "\" (unexpected fail)\n";
            } else {
                std::ostringstream oss;
                oss << value;
                if (oss.str() == test.expected_output) {
                    std::cout << "Test passed for input: \"" << test.input << "\"\n";
                } else {
                    std::cout << "Test failed for input: \"" << test.input << "\" (expected: \"" << test.expected_output << "\", got: \"" << oss.str() << "\")\n";
                }
            }
        }
    }
}



void test_input_operator_bigrational() {
    struct TestCase {
        std::string input;
        std::string expected_output;
        bool should_fail;
    };

std::vector<TestCase> tests = {
        {"123/456", "41/152", false}, // Expected output is the reduced form
        {"  123  /  456  ", "41/152", false}, // Expected output is the reduced form
        {"-123/456", "-41/152", false}, // Expected output is the reduced form
        {"  -123  /  456  ", "-41/152", false}, // Expected output is the reduced form
        {"123", "123", false},
        {"  123  ", "123", false},
        {"0/1", "0", false},
        {"  0  /  1  ", "0", false},
        {"-0/1", "0", false},
        {"  -0  /  1  ", "0", false},
        {"123abc", "123", false},
        {"-123abc", "-123", false},
        {"xyz", "", true},
        {"-xyz", "", true},
        {"", "", true},
        {"-", "", true},
        {"  -  ", "", true}
    };

    for (const auto& test : tests) {
        std::istringstream iss(test.input);
        BigRational value;
        iss >> value;

        //std::cout << "Testing input: \"" << test.input << "\"\n";

        if (test.should_fail) {
            if (iss.fail()) {
                std::cout << "Test passed for input: \"" << test.input << "\" (expected fail)\n";
            } else {
                std::cout << "Test failed for input: \"" << test.input << "\" (expected fail)\n";
            }
        } else {
            if (iss.fail()) {
                std::cout << "Test failed for input: \"" << test.input << "\" (unexpected fail)\n";
            } else {
                std::ostringstream oss;
                oss << value;
                if (oss.str() == test.expected_output) {
                    std::cout << "Test passed for input: \"" << test.input << "\"\n";
                } else {
                    std::cout << "Test failed for input: \"" << test.input << "\" (expected: \"" << test.expected_output << "\", got: \"" << oss.str() << "\")\n";
                }
            }
        }
    }
}


void test_big_integer_arithmetics() {
    // Test cases
    struct TestCase {
        std::string description;
        BigInteger lhs;
        BigInteger rhs;
        BigInteger expected_result;
        std::string operation; // '%' for modulus, '/' for division, 'u' for unary minus, '>' for greater than, '<=' for less than or equal, '>=' for greater than or equal
    };

    std::vector<TestCase> tests = {
        {"TenModThreeEqualsOne", BigInteger(10), BigInteger(3), BigInteger(1), "%"},
        {"_255DivOneEquals255", BigInteger(255), BigInteger(1), BigInteger(255), "/"},
        {"_255DivMinusOneEqualsM255", BigInteger(255), BigInteger(-1), BigInteger(-255), "/"},
        {"Minus10IsMinus10", BigInteger(10), BigInteger(), BigInteger(-10), "u"},
        {"OneGreaterThanZero", BigInteger(1), BigInteger(0), BigInteger(1), ">"},
        {"NotZeroGreaterThanOne", BigInteger(0), BigInteger(1), BigInteger(0), ">"},
        {"ZeroLeqOne", BigInteger(0), BigInteger(1), BigInteger(1), "<="},
        {"NotZeroGeqOne", BigInteger(0), BigInteger(1), BigInteger(0), ">="},
        {"SpecialDiv2", BigInteger("-11001000000"), BigInteger(86400), BigInteger(-127326), "/"},
        {"SpecialMod", BigInteger("20195283520469175757"), BigInteger("1048576"), BigInteger("888269"), "%"},
        {"SpecialModNeg", BigInteger(860593), BigInteger(-201), BigInteger(112), "%"},
        {"SpecialModNeg2", BigInteger("-18224909727634776050312394179610579601844989529623334093909233530432892596607"), BigInteger("14954691977398614017"), BigInteger("-1218675032235937780712688130619504358062280950643422399658"), "/"},
        {"Minus5Divides2EqualsMinus2", BigInteger(-5), BigInteger(2), BigInteger(-2), "/"},
        {"FiveDividesMinus2EqualsMinus2", BigInteger(5), BigInteger(-2), BigInteger(-2), "/"},
        {"FiveDivides2Equals2", BigInteger(5), BigInteger(2), BigInteger(2), "/"},
        {"MinusFiveDividesMinus2Equals2", BigInteger(-5), BigInteger(-2), BigInteger(2), "/"},
        {"Minus10Mod3EqualsMinus1", BigInteger(-10), BigInteger(3), BigInteger(-1), "%"},
        {"TenMod3Equals1", BigInteger(10), BigInteger(3), BigInteger(1), "%"},
        {"TenModMinus3Equals1", BigInteger(10), BigInteger(-3), BigInteger(1), "%"},
        {"MinusTenModMinus3EqualsMinus1", BigInteger(-10), BigInteger(-3), BigInteger(-1), "%"},
        {"TenModMinus5Equals0", BigInteger(10), BigInteger(-5), BigInteger(0), "%"}
    };

    for (const auto& test : tests) {
        BigInteger result;
        if (test.operation == "%") {
            result = test.lhs % test.rhs;
        } else if (test.operation == "/") {
            result = test.lhs / test.rhs;
        } else if (test.operation == "u") {
            result = -test.lhs;
        } else if (test.operation == ">") {
            assert((test.lhs > test.rhs) == (test.expected_result == BigInteger(1)));
            continue;
        } else if (test.operation == "<=") {
            assert((test.lhs <= test.rhs) == (test.expected_result == BigInteger(1)));
            continue;
        } else if (test.operation == ">=") {
            assert((test.lhs >= test.rhs) == (test.expected_result == BigInteger(1)));
            continue;
        }

        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
        assert(pass);
    }
}

void test_big_rational_arithmetics_SJJSJS() {
    // Test cases
    struct TestCase {
        std::string description;
        BigRational lhs;
        BigRational rhs;
        BigRational expected_result;
        std::string operation; // 'i' for integer, 'r' for rational, 'u' for unary minus
    };

    std::vector<TestCase> tests = {
        {"FromInt - integer", BigRational(10, 1), BigRational(), BigRational(10, 1), "i"},
        {"FromInt - rational", BigRational(10, 2), BigRational(), BigRational(5, 1), "r"},
        {"UnaryMinus", BigRational(10, 1), BigRational(), BigRational(-10, 1), "u"},
    };

    for (const auto& test : tests) {
        BigRational result;
        if (test.operation == "i") {
            result = test.lhs;
        } else if (test.operation == "r") {
            result = test.lhs;
        } else if (test.operation == "u") {
            result = -test.lhs;
        }

        bool pass = (result == test.expected_result);
        std::string status = pass ? "PASS" : "FAIL";
        std::cout << test.description
                  << " | Expected: " << test.expected_result
                  << " | Got: " << result
                  << " | " << status << "\n";
        assert(pass);
    }
}

// void test_unary_minus() {
//     // Test cases
//     struct TestCase {
//         std::string description;
//         BigRational input;
//         BigRational expected_result;
//     };

//     std::vector<TestCase> tests = {
//         {"UnaryMinus - positive", BigRational(10, 1), BigRational(-10, 1)},
//         {"UnaryMinus - negative", BigRational(-10, 1), BigRational(10, 1)},
//         {"UnaryMinus - zero", BigRational(0, 1), BigRational(0, 1)},
//         {"UnaryMinus - fraction", BigRational(10, 2), BigRational(-10, 2)},
//         {"UnaryMinus - large positive", BigRational(123456789, 1), BigRational(-123456789, 1)},
//         {"UnaryMinus - large negative", BigRational(-123456789, 1), BigRational(123456789, 1)},
//         {"UnaryMinus - mixed fraction positive", BigRational(123456789, 987654321), BigRational(-123456789, 987654321)},
//         {"UnaryMinus - mixed fraction negative", BigRational(-123456789, 987654321), BigRational(123456789, 987654321)},
//         {"UnaryMinus - small fraction positive", BigRational(1, 1000000), BigRational(-1, 1000000)},
//         {"UnaryMinus - small fraction negative", BigRational(-1, 1000000), BigRational(1, 1000000)},
//         {"UnaryMinus - equal Numerator and Numerator positive", BigRational(1000000, 1000000), BigRational(-1000000, 1000000)},
//         {"UnaryMinus - equal Numerator and Numerator negative", BigRational(-1000000, 1000000), BigRational(1000000, 1000000)},
//         {"UnaryMinus - large fraction positive", BigRational(1234567890123456789, 9876543210987654321), BigRational(-1234567890123456789, 9876543210987654321)},
//         {"UnaryMinus - large fraction negative", BigRational(-1234567890123456789, 9876543210987654321), BigRational(1234567890123456789, 9876543210987654321)},
//         {"UnaryMinus - small fraction positive", BigRational(1, 999999999999999999), BigRational(-1, 999999999999999999)},
//         {"UnaryMinus - small fraction negative", BigRational(-1, 999999999999999999), BigRational(1, 999999999999999999)},
//     };

//     for (const auto& test : tests) {
//         BigRational result = -test.input;

//         bool pass = (result == test.expected_result);
//         std::string status = pass ? "PASS" : "FAIL";
//         std::cout << test.description
//                   << " | Expected: " << test.expected_result
//                   << " | Got: " << result
//                   << " | " << status << "\n";
//         assert(pass);
//     }
// }














































































































































// void testBigRationalConstructors() {
//     // Test default constructor
//     BigRational defaultRational;
//     std::cout << "defaultRational: " << defaultRational << ", expected: 0/1" << std::endl;

//     // Test constructor with two integers
//     BigRational intRational(2, 3);
//     std::cout << "intRational: " << intRational << ", expected: 2/3" << std::endl;

//     // Test constructor with two strings
//     BigRational stringRational("5", "7");
//     std::cout << "stringRational: " << stringRational << ", expected: 5/7" << std::endl;

//     // Test constructor with two BigIntegers
//     BigInteger num1("8");
//     BigInteger den1("10");
//     BigRational biRational(num1, den1);
//     std::cout << "biRational: " << biRational << ", expected: 8/10" << std::endl;
// }

// // Test normalization for BigRational
// void testBigRationalNormalization() {
//     BigRational rational(6, 2);
//     std::cout << "rational: " << rational << ", expected: 3/1" << std::endl;

//     BigRational rational2(24, 9);
//     std::cout << "rational2: " << rational2 << ", expected: 8/3" << std::endl;

//     BigRational rational3(13, 11);
//     std::cout << "rational3: " << rational3 << ", expected: 13/11" << std::endl;
// }
// Test unary operators for BigRational
void testBigRationalUnaryOperators() {
    BigRational rational(3, 4);

    // Test unary plus operator
    BigRational unaryPlus = +rational;
    std::cout << "unaryPlus: " << unaryPlus << ", expected: " << rational << std::endl;

    // Test unary minus operator
    BigRational unaryMinus = -rational;
    std::cout << "unaryMinus: " << unaryMinus << ", expected: " << BigRational(-3, 4) << std::endl;
}

// Test binary addition operator for BigRational
void testBigRationalAdditionOperator() {
    BigRational rational1(1, 2);
    BigRational rational2(1, 3);

    BigRational result = rational1 + rational2;
    std::cout << "result: " << result << ", expected: " << BigRational(5, 6) << std::endl;
}

// Test binary subtraction operator for BigRational
void testBigRationalSubtractionOperator() {
    BigRational rational1(3, 4);
    BigRational rational2(1, 4);

    BigRational result = rational1 - rational2;
    std::cout << "result: " << result << ", expected: " << BigRational(1, 2) << std::endl;
}

// Test binary multiplication operator for BigRational
void testBigRationalMultiplicationOperator() {
    BigRational rational1(2, 3);
    BigRational rational2(3, 5);

    BigRational result = rational1 * rational2;
    std::cout << "result: " << result << ", expected: " << BigRational(2, 5) << std::endl;
}

// Test binary division operator for BigRational
void testBigRationalDivisionOperator() {
    BigRational rational1(3, 4);
    BigRational rational2(1, 2);

    BigRational result = rational1 / rational2;
    std::cout << "result: " << result << ", expected: " << BigRational(3, 2) << std::endl;
}

// Test compound addition assignment operator for BigRational
void testBigRationalAdditionAssignmentOperator() {
    BigRational rational1(1, 3);
    BigRational rational2(2, 3);

    rational1 += rational2;
    std::cout << "rational1: " << rational1 << ", expected: " << BigRational(1, 1) << std::endl;
}

// Test compound subtraction assignment operator for BigRational
void testBigRationalSubtractionAssignmentOperator() {
    BigRational rational1(3, 4);
    BigRational rational2(1, 4);

    rational1 -= rational2;
    std::cout << "rational1: " << rational1 << ", expected: " << BigRational(1, 2) << std::endl;
}

// Test compound multiplication assignment operator for BigRational
void testBigRationalMultiplicationAssignmentOperator() {
    BigRational rational1(2, 3);
    BigRational rational2(3, 5);

    rational1 *= rational2;
    std::cout << "rational1: " << rational1 << ", expected: " << BigRational(2, 5) << std::endl;
}

// Test compound division assignment operator for BigRational
void testBigRationalDivisionAssignmentOperator() {
    BigRational rational1(3, 4);
    BigRational rational2(1, 2);

    rational1 /= rational2;
    std::cout << "rational1: " << rational1 << ", expected: " << BigRational(3, 2) << std::endl;
}

// Test equality operator for BigRational
void testBigRationalEqualityOperator() {
    BigRational rational1(1, 2);
    BigRational rational2(2, 4);

    std::cout << "rational1 == rational2: " << (rational1 == rational2) << ", expected: true" << std::endl;
}

// Test inequality operator for BigRational
void testBigRationalInequalityOperator() {
    BigRational rational1(1, 2);
    BigRational rational2(3, 4);

    std::cout << "rational1 != rational2: " << (rational1 != rational2) << ", expected: true" << std::endl;
}

// Test less than operator for BigRational
void testBigRationalLessThanOperator() {
    BigRational rational1(1, 3);
    BigRational rational2(1, 2);

    std::cout << "rational1 < rational2: " << (rational1 < rational2) << ", expected: true" << std::endl;
}

// Test greater than operator for BigRational
void testBigRationalGreaterThanOperator() {
    BigRational rational1(2, 3);
    BigRational rational2(1, 2);

    std::cout << "rational1 > rational2: " << (rational1 > rational2) << ", expected: true" << std::endl;
}

// Test less than or equal to operator for BigRational
void testBigRationalLessThanOrEqualOperator() {
    BigRational rational1(1, 2);
    BigRational rational2(2, 4);

    std::cout << "rational1 <= rational2: " << (rational1 <= rational2) << ", expected: true" << std::endl;
}

// Test greater than or equal to operator for BigRational
void testBigRationalGreaterThanOrEqualOperator() {
    BigRational rational1(3, 4);
    BigRational rational2(1, 2);

    std::cout << "rational1 >= rational2: " << (rational1 >= rational2) << ", expected: true" << std::endl;
}

// Test output operator for BigRational
void testBigRationalOutputOperator() {
    BigRational rational(1, 2);
    std::stringstream ss;
    ss << rational;
    std::cout << "ss.str(): " << ss.str() << ", expected: 1/2" << std::endl;
}

// Test input operator for BigRational
void testBigRationalInputOperator() {
    std::stringstream ss("3/4");
    BigRational rational;
    ss >> rational;
    std::cout << "rational: " << rational << ", expected: " << BigRational(3, 4) << std::endl;
}

// Test sqrt method for BigRational
void testBigRationalSqrt() {
    BigRational rational2(-1, 4);
    // Attempting to calculate the square root of a negative rational should throw an exception.
    bool exceptionThrown = false;
    try {
        rational2.sqrt();
    } catch (const std::runtime_error& e) {
        exceptionThrown = true;
    }
    std::cout << "exceptionThrown: " << exceptionThrown << ", expected: true" << std::endl;
}

void run_all_BigRational_tests() {
    // testBigRationalConstructors();
    // testBigRationalUnaryOperators();
    // testBigRationalNormalization();

    testBigRationalAdditionOperator();
    testBigRationalSubtractionOperator();
    testBigRationalMultiplicationOperator();
    testBigRationalDivisionOperator();

    testBigRationalAdditionAssignmentOperator();
    testBigRationalSubtractionAssignmentOperator();
    testBigRationalMultiplicationAssignmentOperator();
    testBigRationalDivisionAssignmentOperator();

    std::cout <<"control point 1" << std::endl;

    testBigRationalEqualityOperator();
    testBigRationalInequalityOperator();
    testBigRationalLessThanOperator();
    testBigRationalGreaterThanOperator();
    testBigRationalLessThanOrEqualOperator();
    testBigRationalGreaterThanOrEqualOperator();
    std::cout <<"control point 1" << std::endl;

    testBigRationalOutputOperator();
    testBigRationalInputOperator();

    //testBigRationalSqrt();

    std::cout << "All BigRational tests passed!" << std::endl;
}














void testBigRationalIsqrt() {
    BigRational rational1(9, 16);
    assert(rational1.isqrt() == BigInteger(1));

    BigRational rational2(5, 1);
    // Attempting to calculate the integer square root of a non-integer rational should throw an exception.
    bool exceptionThrown = false;
    try {
        rational2.isqrt();
    } catch (const std::runtime_error& e) {
        exceptionThrown = true;
    }
    assert(!exceptionThrown);

    exceptionThrown = false;
    BigRational rational3(-5, 1);
    try {
        rational3.isqrt();
    } catch (const std::runtime_error& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}


void testBigRationalConstructors() {
    std::cout << "Testing BigRational constructors with additional edge cases...\n";

    // Тест конструктора с большими значениями int64_t
    try {
        BigRational rationalLarge(INT64_MAX, INT64_MAX - 1);
        std::cout << "rationalLarge: " << rationalLarge << ", expected: " << INT64_MAX << "/" << (INT64_MAX -1) << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalLarge: " << e.what() << "\n";
    }

    // Тест конструктора с нулевым числителем и отрицательным знаменателем
    try {
        BigRational rationalZeroNumNegDen(0, -5);
        std::cout << "rationalZeroNumNegDen: " << rationalZeroNumNegDen << ", expected: 0/1\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalZeroNumNegDen: " << e.what() << "\n";
    }

    // Тест конструктора с максимальными и минимальными значениями int64_t
    try {
        BigRational rationalMaxMin(INT64_MIN, INT64_MAX);
        std::cout << "rationalMaxMin: " << rationalMaxMin << ", expected: " << INT64_MIN << "/" << INT64_MAX << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalMaxMin: " << e.what() << "\n";
    }

    // Тест конструктора с отрицательным нулевым числителем
    try {
        BigRational rationalNegZero(-0, 3);
        std::cout << "rationalNegZero: " << rationalNegZero << ", expected: 0/1\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalNegZero: " << e.what() << "\n";
    }

    // Тест конструктора с недопустимой строкой числителя
    try {
        BigRational rationalInvalidNum("abc", "5");
        std::cout << "rationalInvalidNum: " << rationalInvalidNum << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalInvalidNum: " << e.what() << "\n";
    }

    // Тест конструктора с недопустимой строкой знаменателя
    try {
        BigRational rationalInvalidDen("5", "xyz");
        std::cout << "rationalInvalidDen: " << rationalInvalidDen << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalInvalidDen: " << e.what() << "\n";
    }

    // Тест конструктора с десятичными строками (некорректные входные данные)
    try {
        BigRational rationalDecimal("3.5", "2.1");
        std::cout << "rationalDecimal: " << rationalDecimal << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalDecimal: " << e.what() << "\n";
    }

    // Тест конструктора со строками с ведущими нулями
    try {
        BigRational rationalLeadingZeros("0005", "00010");
        std::cout << "rationalLeadingZeros: " << rationalLeadingZeros << ", expected: 1/2\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalLeadingZeros: " << e.what() << "\n";
    }

    // Тест конструктора с очень большими числами в виде строк
    try {
        std::string largeNum = "123456789012345678901234567890";
        std::string largeDen = "987654321098765432109876543210";
        BigRational rationalVeryLarge(largeNum, largeDen);
        std::cout << "rationalVeryLarge: " << rationalVeryLarge << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalVeryLarge: " << e.what() << "\n";
    }

    // Тест конструктора с отрицательным числителем и знаменателем
    try {
        BigRational rationalNegNumDen(-7, -8);
        std::cout << "rationalNegNumDen: " << rationalNegNumDen << ", expected: 7/8\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalNegNumDen: " << e.what() << "\n";
    }

    // Тест конструктора с обоими нулевыми числителем и знаменателем
    try {
        BigRational rationalZeroNumZeroDen(0, 0);
        std::cout << "rationalZeroNumZeroDen: " << rationalZeroNumZeroDen << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalZeroNumZeroDen: " << e.what() << "\n";
    }

    // Тест конструктора с пробелами в строках
    try {
        BigRational rationalSpaces(" 15 ", " 3 ");
        std::cout << "rationalSpaces: " << rationalSpaces << ", expected: 5/1\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalSpaces: " << e.what() << "\n";
    }

    // Тест конструктора с пустыми строками
    try {
        BigRational rationalEmpty("", "");
        std::cout << "rationalEmpty: " << rationalEmpty << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception in rationalEmpty: " << e.what() << "\n";
    }

    std::cout << "Additional BigRational constructor tests completed.\n";
}

// Test the constructor with an int
void testIntConstructor() {
    // Тестирование конструктора с максимальным значением int64_t
    BigInteger num3(std::numeric_limits<int64_t>::max());
    BigInteger expectedNum3(std::to_string(std::numeric_limits<int64_t>::max()));
    assert(num3 == BigInteger(expectedNum3) && "Int constructor failed for int64_t max");

    // // Тестирование конструктора с минимальным значением int64_t
    // BigInteger num4(std::numeric_limits<int64_t>::min());
    // BigInteger expectedNum4("-9223372036854775808");
    // assert(num4 == expectedNum4 && "Int constructor failed for int64_t min");
}
// Test the constructor with a string
void testStringConstructor() {
    BigInteger num1("789");
    BigInteger expectedNum1(789);
    assert(num1 == expectedNum1 && "String constructor failed for positive number");
    std::cout << "num1: " << num1 << std::endl;

    BigInteger num2("-987");
    BigInteger expectedNum2(-987);
    assert(num2 == expectedNum2 && "String constructor failed for negative number");
    std::cout << "num2: " << num2<< std::endl;

    bool exceptionThrown = false;

    // Тест с ведущими и хвостовыми пробелами
    try {
        BigInteger num3("  123  ");
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown && "String constructor should throw exception for string with spaces");
    std::cout << "num3: exception thrown" << std::endl;

    // Тест с нецифровыми символами
    exceptionThrown = false;
    try {
        BigInteger num4("abc");
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown && "String constructor should throw exception for non-digit string");
    std::cout << "num4: exception thrown" << std::endl;

    // Тест с очень большим числом, превышающим пределы int64_t
    exceptionThrown = false;
    try {
        BigInteger num5("1234567890123456789012345678901234567890");
        std::cout << "num5: " << num5 << std::endl;
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(!exceptionThrown && "String constructor should handle very large numbers");
    

    // Тест с недопустимыми символами в строке
    exceptionThrown = false;
    try {
        BigInteger num6("1234567890X1234567890");
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown && "String constructor should handle non-digit characters");
    std::cout << "num6: exception thrown" << std::endl;
}
// Test constructor and assignment operator error handling
void testErrorHandling() {
    // Тест конструктора с некорректной строкой
    try {
        BigInteger num("invalid123");
        assert(false && "Constructor should have thrown an exception for invalid input");
    } catch (const std::exception&) {
        // Ожидается выброс исключения
        std::cout << "num: exception thrown for invalid123" << std::endl;
        assert(true);
    }

    // Тест оператора присваивания с некорректной строкой
    BigInteger num;
    try {
        num = BigInteger("invalid456");
        assert(false && "Assignment operator should have thrown an exception for invalid input");
    } catch (const std::exception&) {
        // Ожидается выброс исключения
        std::cout << "num: exception thrown for invalid456" << std::endl;
        assert(true);
    }
}

void testBigIntegerInputOperator() {
    std::istringstream iss;

    BigInteger num1;
    iss.str("12345");
    iss.clear(); // Очистка состояния потока
    iss >> num1;
    assert(num1 == BigInteger(12345));
    std::cout << "num1: " << num1 << std::endl;

    BigInteger num2;
    iss.str("-98765");
    iss.clear(); // Очистка состояния потока
    iss >> num2;
    assert(num2 == BigInteger(-98765));
    std::cout << "num2: " << num2 << std::endl;

    BigInteger num3;
    iss.str("  123456789012345678901234567890  ");
    iss.clear(); // Очистка состояния потока
    iss >> num3;
    assert(num3 == BigInteger("123456789012345678901234567890"));
    std::cout << "num3: " << num3 << std::endl;

    // Test invalid input
    BigInteger num4;
    iss.str("abc");
    iss.clear(); // Очистка состояния потока
    iss >> num4;
    assert(iss.fail() && "Input should fail for non-digit characters");
    std::cout << "num4: invalid input test passed" << std::endl;

    // Test input with only a negative sign
    BigInteger num5;
    iss.str("-");
    iss.clear(); // Очистка состояния потока
    iss >> num5;
    assert(iss.fail() && "Input should fail for only a negative sign");
    std::cout << "num5: invalid input test passed" << std::endl;

    BigInteger anum;
    iss.str("-0");
    iss.clear(); // Очистка состояния потока
    iss >> anum;
    assert(anum == BigInteger(0));
    std::cout << "anum: " << anum << std::endl;
}

void testEval() {
    std::string json_str = R"({
        {
            "op":"-",
            "left": -123,
            "right": {
                "op":"*",
                "left": "-12345678901234567890",
                "right": {
                    "op":"/",
                    "left":"-34",
                    "right":-1
                }
            }
        }
    })";

    BigInteger result = eval(json_str);
    std::cout << "Result: " << result << std::endl;
}

void testFromInt64Min() {
    try {
        BigInteger num(INT64_MIN);
        std::string expected = "-9223372036854775808";
        std::cout << "FromInt64Min: Expected: " << expected << ", Got: " << num << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FromInt64Min: Failed - " << e.what() << std::endl;
    }
}

void testFromStringInvalid() {
    try {
        BigInteger num("");
        std::cout << "FromStringInvalid (empty string): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (empty string): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("-");
        std::cout << "FromStringInvalid (single '-'): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (single '-'): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("abc");
        std::cout << "FromStringInvalid (non-numeric): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (non-numeric): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("12-34");
        std::cout << "FromStringInvalid (random '-'): Failed" << num <<std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (random '-'): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("12.34");
        std::cout << "FromStringInvalid (decimal point): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (decimal point): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("12+34");
        std::cout << "FromStringInvalid (random '+'): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (random '+'): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("12*34");
        std::cout << "FromStringInvalid (random '*'): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (random '*'): Passed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("12/34");
        std::cout << "FromStringInvalid (random '/'): Failed" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "FromStringInvalid (random '/'): Passed - " << e.what() << std::endl;
    }
}

void testFromStringValid() {
    try {
        BigInteger num("1234567890");
        std::string expected = "1234567890";
        std::cout << "FromStringValid (positive): Expected: " << expected << ", Got: " << num << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FromStringValid (positive): Failed - " << e.what() << std::endl;
    }

    try {
        BigInteger num("-9876543210");
        std::string expected = "-9876543210";
        std::cout << "FromStringValid (negative): Expected: " << expected << ", Got: " << num << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FromStringValid (negative): Failed - " << e.what() << std::endl;
    }
}






































bool areAlmostEqual(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) < epsilon;
}




#include <iostream>
#include <string>
#include <stdexcept>
#include "Big_Numbers.hpp"

// Функция для сравнения двух BigInteger объектов
bool areEqual(const BigInteger& a, const BigInteger& b) {
    return a == b;
}

// ------------------- Тесты для BigRational::isqrt() -------------------

// Тест 1: Проверка isqrt для 0/1
void testRationalIsqrtZero() {
    try {
        BigRational num("0", "1");
        BigInteger result = num.isqrt();
        BigInteger expected("0");
        if (areEqual(result, expected)) {
            std::cout << "testRationalIsqrtZero: Passed" << std::endl;
        } else {
            std::cout << "testRationalIsqrtZero: Failed - Expected: " << expected << ", Got: " << result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "testRationalIsqrtZero: Failed - " << e.what() << std::endl;
    }
}

// Тест 2: Проверка isqrt для 1/1
void testRationalIsqrtOne() {
    try {
        BigRational num("1", "1");
        BigInteger result = num.isqrt();
        BigInteger expected("1");
        if (areEqual(result, expected)) {
            std::cout << "testRationalIsqrtOne: Passed" << std::endl;
        } else {
            std::cout << "testRationalIsqrtOne: Failed - Expected: " << expected << ", Got: " << result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "testRationalIsqrtOne: Failed - " << e.what() << std::endl;
    }
}

// Тест 3: Проверка isqrt для совершенных квадратов дробей
void testRationalIsqrtPerfectSquares() {
    struct TestCase {
        std::string numerator;
        std::string denominator;
        std::string expected;
    };

    TestCase testCases[] = {
        {"4", "1", "2"},
        {"9", "1", "3"},
        {"16", "1", "4"},
        {"25", "1", "5"},
        {"1", "4", "0"},
        {"9", "4", "1"},   // sqrt(9/4) = 1 (floor(1.5))
        {"100", "25", "2"}, // sqrt(100/25) = 2
        {"225", "25", "3"},
        {"10000", "100", "10"},
        {"49", "9", "2"},   // sqrt(49/9) = 2.333... -> floor(2.333...) = 2
    };

    for (const auto& test : testCases) {
        try {
            BigRational num(test.numerator, test.denominator);
            BigInteger result = num.isqrt();
            BigInteger expected(test.expected);
            if (areEqual(result, expected)) {
                std::cout << "testRationalIsqrtPerfectSquares (" << test.numerator << "/" << test.denominator << "): Passed" << std::endl;
            } else {
                std::cout << "testRationalIsqrtPerfectSquares (" << test.numerator << "/" << test.denominator << "): Failed - Expected: " 
                          << expected << ", Got: " << result << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "testRationalIsqrtPerfectSquares (" << test.numerator << "/" << test.denominator << "): Failed - " << e.what() << std::endl;
        }
    }
}

// Тест 4: Проверка isqrt для несовершенных квадратов дробей
void testRationalIsqrtNonPerfectSquares() {
    struct TestCase {
        std::string numerator;
        std::string denominator;
        std::string expected;
    };

    TestCase testCases[] = {
        {"2", "3", "0"},   // floor(sqrt(2/3)) ≈ 0.816 -> 0
        {"3", "2", "1"},   // floor(sqrt(1.5)) ≈ 1.224 -> 1
        {"5", "2", "1"},   // floor(sqrt(2.5)) ≈ 1.581 -> 1
        {"10", "4", "1"},  // floor(sqrt(2.5)) ≈ 1.581 -> 1
        {"7", "4", "1"},   // floor(sqrt(1.75)) ≈ 1.322 -> 1
        {"8", "9", "0"},   // floor(sqrt(0.888...)) ≈ 0.942 ->0
        {"50", "9", "2"},  // floor(sqrt(5.555...)) ≈ 2.357 ->2
        {"99", "25", "1"}, // floor(sqrt(3.96)) ≈1.989 ->1
    };

    for (const auto& test : testCases) {
        try {
            BigRational num(test.numerator, test.denominator);
            BigInteger result = num.isqrt();
            BigInteger expected(test.expected);
            if (areEqual(result, expected)) {
                std::cout << "testRationalIsqrtNonPerfectSquares (" << test.numerator << "/" << test.denominator << "): Passed" << std::endl;
            } else {
                std::cout << "testRationalIsqrtNonPerfectSquares (" << test.numerator << "/" << test.denominator << "): Failed - Expected: " 
                          << expected << ", Got: " << result << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "testRationalIsqrtNonPerfectSquares (" << test.numerator << "/" << test.denominator << "): Failed - " << e.what() << std::endl;
        }
    }
}

// Тест 5: Проверка isqrt для отрицательных дробей (должны выбрасывать исключение)
void testRationalIsqrtNegativeNumbers() {
    struct TestCase {
        std::string numerator;
        std::string denominator;
    };

    TestCase testCases[] = {
        {"-1", "1"},
        {"-4", "1"},
        {"-9", "4"},
        {"-16", "25"},
        {"-100", "9"}
    };

    for (const auto& test : testCases) {
        try {
            BigRational num(test.numerator, test.denominator);
            BigInteger result = num.isqrt();
            std::cout << "testRationalIsqrtNegativeNumbers (" << test.numerator << "/" << test.denominator << "): Failed - Expected exception, Got: " 
                      << result << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "testRationalIsqrtNegativeNumbers (" << test.numerator << "/" << test.denominator << "): Passed - " << e.what() << std::endl;
        } catch (...) {
            std::cout << "testRationalIsqrtNegativeNumbers (" << test.numerator << "/" << test.denominator << "): Failed - Unexpected exception type" << std::endl;
        }
    }
}






struct TestCase {
    std::string name;
    BigInteger number;
    size_t k;
    bool expected;
};

// Функция для сравнения результатов
bool areEqual(bool a, bool b) {
    return a == b;
}

// Функция для запуска тестов деления на простоту
void testIsPrime() {
std::vector<TestCase> tests = {
    // Небольшие простые числа
    {"Test 1: is_prime(2)", BigInteger("2"), 5, true},
    {"Test 2: is_prime(3)", BigInteger("3"), 5, true},
    {"Test 11: is_prime(5)", BigInteger("5"), 5, true},
    {"Test 12: is_prime(7)", BigInteger("7"), 5, true},
    {"Test 13: is_prime(11)", BigInteger("11"), 5, true},
    {"Test 14: is_prime(13)", BigInteger("13"), 5, true},
    {"Test 15: is_prime(17)", BigInteger("17"), 5, true},
    {"Test 16: is_prime(19)", BigInteger("19"), 5, true},
    {"Test 17: is_prime(23)", BigInteger("23"), 5, true},
    {"Test 18: is_prime(29)", BigInteger("29"), 5, true},
    {"Test 19: is_prime(31)", BigInteger("31"), 5, true},
    {"Test 20: is_prime(37)", BigInteger("37"), 5, true},
    {"Test 21: is_prime(41)", BigInteger("41"), 5, true},
    {"Test 22: is_prime(43)", BigInteger("43"), 5, true},
    {"Test 23: is_prime(47)", BigInteger("47"), 5, true},
    {"Test 24: is_prime(53)", BigInteger("53"), 5, true},
    {"Test 25: is_prime(59)", BigInteger("59"), 5, true},
    {"Test 26: is_prime(61)", BigInteger("61"), 5, true},
    {"Test 27: is_prime(67)", BigInteger("67"), 5, true},
    {"Test 28: is_prime(71)", BigInteger("71"), 5, true},
    {"Test 29: is_prime(73)", BigInteger("73"), 5, true},
    {"Test 30: is_prime(79)", BigInteger("79"), 5, true},
    {"Test 31: is_prime(83)", BigInteger("83"), 5, true},
    {"Test 32: is_prime(89)", BigInteger("89"), 5, true},
    {"Test 33: is_prime(97)", BigInteger("97"), 5, true},
    {"Test 34: is_prime(101)", BigInteger("101"), 5, true},
    {"Test 35: is_prime(103)", BigInteger("103"), 5, true},
    {"Test 36: is_prime(107)", BigInteger("107"), 5, true},
    {"Test 37: is_prime(109)", BigInteger("109"), 5, true},
    {"Test 38: is_prime(113)", BigInteger("113"), 5, true},
    {"Test 39: is_prime(127)", BigInteger("127"), 5, true},
    {"Test 40: is_prime(131)", BigInteger("131"), 5, true},
    {"Test 41: is_prime(137)", BigInteger("137"), 5, true},
    {"Test 42: is_prime(139)", BigInteger("139"), 5, true},
    {"Test 43: is_prime(149)", BigInteger("149"), 5, true},
    {"Test 44: is_prime(151)", BigInteger("151"), 5, true},
    {"Test 45: is_prime(157)", BigInteger("157"), 5, true},
    {"Test 46: is_prime(163)", BigInteger("163"), 5, true},
    {"Test 47: is_prime(167)", BigInteger("167"), 5, true},
    {"Test 48: is_prime(173)", BigInteger("173"), 5, true},
    {"Test 49: is_prime(179)", BigInteger("179"), 5, true},
    {"Test 50: is_prime(181)", BigInteger("181"), 5, true},
    {"Test 51: is_prime(191)", BigInteger("191"), 5, true},
    {"Test 52: is_prime(193)", BigInteger("193"), 5, true},
    {"Test 53: is_prime(197)", BigInteger("197"), 5, true},
    {"Test 54: is_prime(199)", BigInteger("199"), 5, true},
    {"Test 55: is_prime(211)", BigInteger("211"), 5, true},
    {"Test 56: is_prime(223)", BigInteger("223"), 5, true},
    {"Test 57: is_prime(227)", BigInteger("227"), 5, true},
    {"Test 58: is_prime(229)", BigInteger("229"), 5, true},
    {"Test 59: is_prime(233)", BigInteger("233"), 5, true},
    {"Test 60: is_prime(239)", BigInteger("239"), 5, true},
    {"Test 61: is_prime(241)", BigInteger("241"), 5, true},
    {"Test 62: is_prime(251)", BigInteger("251"), 5, true},
    {"Test 63: is_prime(257)", BigInteger("257"), 5, true},
    {"Test 64: is_prime(263)", BigInteger("263"), 5, true},
    {"Test 65: is_prime(269)", BigInteger("269"), 5, true},
    {"Test 66: is_prime(271)", BigInteger("271"), 5, true},
    {"Test 67: is_prime(277)", BigInteger("277"), 5, true},
    {"Test 68: is_prime(281)", BigInteger("281"), 5, true},
    {"Test 69: is_prime(283)", BigInteger("283"), 5, true},
    {"Test 70: is_prime(293)", BigInteger("293"), 5, true},
    {"Test 71: is_prime(307)", BigInteger("307"), 5, true},
    {"Test 72: is_prime(311)", BigInteger("311"), 5, true},
    {"Test 73: is_prime(313)", BigInteger("313"), 5, true},
    {"Test 74: is_prime(317)", BigInteger("317"), 5, true},
    {"Test 75: is_prime(331)", BigInteger("331"), 5, true},
    {"Test 76: is_prime(337)", BigInteger("337"), 5, true},
    {"Test 77: is_prime(347)", BigInteger("347"), 5, true},
    {"Test 78: is_prime(349)", BigInteger("349"), 5, true},
    {"Test 79: is_prime(353)", BigInteger("353"), 5, true},
    {"Test 80: is_prime(359)", BigInteger("359"), 5, true},
    {"Test 81: is_prime(367)", BigInteger("367"), 5, true},
    {"Test 82: is_prime(373)", BigInteger("373"), 5, true},
    {"Test 83: is_prime(379)", BigInteger("379"), 5, true},
    {"Test 84: is_prime(383)", BigInteger("383"), 5, true},
    {"Test 85: is_prime(389)", BigInteger("389"), 5, true},
    {"Test 86: is_prime(397)", BigInteger("397"), 5, true},
    {"Test 87: is_prime(401)", BigInteger("401"), 5, true},
    {"Test 88: is_prime(409)", BigInteger("409"), 5, true},
    {"Test 89: is_prime(419)", BigInteger("419"), 5, true},
    {"Test 90: is_prime(421)", BigInteger("421"), 5, true},
    {"Test 91: is_prime(431)", BigInteger("431"), 5, true},
    {"Test 92: is_prime(433)", BigInteger("433"), 5, true},
    {"Test 93: is_prime(439)", BigInteger("439"), 5, true},
    {"Test 94: is_prime(443)", BigInteger("443"), 5, true},
    {"Test 95: is_prime(449)", BigInteger("449"), 5, true},
    {"Test 96: is_prime(457)", BigInteger("457"), 5, true},
    {"Test 97: is_prime(461)", BigInteger("461"), 5, true},
    {"Test 98: is_prime(463)", BigInteger("463"), 5, true},
    {"Test 99: is_prime(467)", BigInteger("467"), 5, true},
    {"Test 100: is_prime(479)", BigInteger("479"), 5, true},
    {"Test 101: is_prime(487)", BigInteger("487"), 5, true},
    {"Test 102: is_prime(491)", BigInteger("491"), 5, true},
    {"Test 103: is_prime(499)", BigInteger("499"), 5, true},
    {"Test 104: is_prime(503)", BigInteger("503"), 5, true},
    {"Test 105: is_prime(509)", BigInteger("509"), 5, true},
    {"Test 106: is_prime(521)", BigInteger("521"), 5, true},
    {"Test 107: is_prime(523)", BigInteger("523"), 5, true},
    {"Test 108: is_prime(541)", BigInteger("541"), 5, true},
    
    // Небольшие составные числа
    {"Test 3: is_prime(4)", BigInteger("4"), 5, false},
    {"Test 5: is_prime(6)", BigInteger("6"), 5, false},
    {"Test 6: is_prime(8)", BigInteger("8"), 5, false},
    {"Test 7: is_prime(9)", BigInteger("9"), 5, false},
    {"Test 8: is_prime(10)", BigInteger("10"), 5, false},
    {"Test 9: is_prime(12)", BigInteger("12"), 5, false},
    {"Test 10: is_prime(14)", BigInteger("14"), 5, false},
    {"Test 11: is_prime(15)", BigInteger("15"), 5, false},
    {"Test 12: is_prime(16)", BigInteger("16"), 5, false},
    {"Test 13: is_prime(18)", BigInteger("18"), 5, false},
    {"Test 14: is_prime(20)", BigInteger("20"), 5, false},
    {"Test 15: is_prime(21)", BigInteger("21"), 5, false},
    {"Test 16: is_prime(22)", BigInteger("22"), 5, false},
    {"Test 17: is_prime(24)", BigInteger("24"), 5, false},
    {"Test 18: is_prime(25)", BigInteger("25"), 5, false},
    {"Test 19: is_prime(26)", BigInteger("26"), 5, false},
    {"Test 20: is_prime(27)", BigInteger("27"), 5, false},
    {"Test 21: is_prime(28)", BigInteger("28"), 5, false},
    {"Test 22: is_prime(30)", BigInteger("30"), 5, false},
    {"Test 23: is_prime(32)", BigInteger("32"), 5, false},
    {"Test 24: is_prime(33)", BigInteger("33"), 5, false},
    {"Test 25: is_prime(34)", BigInteger("34"), 5, false},
    {"Test 26: is_prime(35)", BigInteger("35"), 5, false},
    {"Test 27: is_prime(36)", BigInteger("36"), 5, false},
    {"Test 28: is_prime(38)", BigInteger("38"), 5, false},
    {"Test 29: is_prime(39)", BigInteger("39"), 5, false},
    {"Test 30: is_prime(40)", BigInteger("40"), 5, false},
    {"Test 31: is_prime(42)", BigInteger("42"), 5, false},
    {"Test 32: is_prime(44)", BigInteger("44"), 5, false},
    {"Test 33: is_prime(45)", BigInteger("45"), 5, false},
    {"Test 34: is_prime(46)", BigInteger("46"), 5, false},
    {"Test 35: is_prime(48)", BigInteger("48"), 5, false},
    {"Test 36: is_prime(49)", BigInteger("49"), 5, false},
    {"Test 37: is_prime(50)", BigInteger("50"), 5, false},
    {"Test 38: is_prime(51)", BigInteger("51"), 5, false},
    {"Test 39: is_prime(52)", BigInteger("52"), 5, false},
    {"Test 40: is_prime(54)", BigInteger("54"), 5, false},
    {"Test 41: is_prime(55)", BigInteger("55"), 5, false},
    {"Test 42: is_prime(56)", BigInteger("56"), 5, false},
    {"Test 43: is_prime(57)", BigInteger("57"), 5, false},
    {"Test 44: is_prime(58)", BigInteger("58"), 5, false},
    {"Test 45: is_prime(60)", BigInteger("60"), 5, false},
    {"Test 46: is_prime(62)", BigInteger("62"), 5, false},
    {"Test 47: is_prime(63)", BigInteger("63"), 5, false},
    {"Test 48: is_prime(64)", BigInteger("64"), 5, false},
    {"Test 49: is_prime(65)", BigInteger("65"), 5, false},
    {"Test 50: is_prime(66)", BigInteger("66"), 5, false},
    {"Test 51: is_prime(68)", BigInteger("68"), 5, false},
    {"Test 52: is_prime(69)", BigInteger("69"), 5, false},
    {"Test 53: is_prime(70)", BigInteger("70"), 5, false},
    {"Test 54: is_prime(72)", BigInteger("72"), 5, false},
    {"Test 55: is_prime(74)", BigInteger("74"), 5, false},
    {"Test 56: is_prime(75)", BigInteger("75"), 5, false},
    {"Test 57: is_prime(76)", BigInteger("76"), 5, false},
    {"Test 58: is_prime(77)", BigInteger("77"), 5, false},
    {"Test 59: is_prime(78)", BigInteger("78"), 5, false},
    {"Test 60: is_prime(80)", BigInteger("80"), 5, false},
    {"Test 61: is_prime(81)", BigInteger("81"), 5, false},
    {"Test 62: is_prime(82)", BigInteger("82"), 5, false},
    {"Test 63: is_prime(84)", BigInteger("84"), 5, false},
    {"Test 64: is_prime(85)", BigInteger("85"), 5, false},
    {"Test 65: is_prime(86)", BigInteger("86"), 5, false},
    {"Test 66: is_prime(87)", BigInteger("87"), 5, false},
    {"Test 67: is_prime(88)", BigInteger("88"), 5, false},
    {"Test 68: is_prime(90)", BigInteger("90"), 5, false},
    {"Test 69: is_prime(91)", BigInteger("91"), 5, false},
    {"Test 70: is_prime(92)", BigInteger("92"), 5, false},
    {"Test 71: is_prime(93)", BigInteger("93"), 5, false},
    {"Test 72: is_prime(94)", BigInteger("94"), 5, false},
    {"Test 73: is_prime(95)", BigInteger("95"), 5, false},
    {"Test 74: is_prime(96)", BigInteger("96"), 5, false},
    {"Test 75: is_prime(98)", BigInteger("98"), 5, false},
    {"Test 76: is_prime(99)", BigInteger("99"), 5, false},
    {"Test 77: is_prime(100)", BigInteger("100"), 5, false},
    {"Test 78: is_prime(102)", BigInteger("102"), 5, false},
    {"Test 79: is_prime(104)", BigInteger("104"), 5, false},
    
    // Известные большие простые числа
    {"Test 106: is_prime(1009)", BigInteger("1009"), 5, true},
    {"Test 107: is_prime(10007)", BigInteger("10007"), 5, true},
    {"Test 108: is_prime(10009)", BigInteger("10009"), 5, true},
    {"Test 109: is_prime(10037)", BigInteger("10037"), 5, true},
    {"Test 110: is_prime(10039)", BigInteger("10039"), 5, true},
    {"Test 111: is_prime(10061)", BigInteger("10061"), 5, true},
    {"Test 112: is_prime(10067)", BigInteger("10067"), 5, true},
    {"Test 113: is_prime(10069)", BigInteger("10069"), 5, true},
    {"Test 114: is_prime(10073)", BigInteger("10073"), 5, true},
    {"Test 115: is_prime(10079)", BigInteger("10079"), 5, true},
    {"Test 116: is_prime(10091)", BigInteger("10091"), 5, true},
    {"Test 117: is_prime(10093)", BigInteger("10093"), 5, true},
    {"Test 118: is_prime(10099)", BigInteger("10099"), 5, true},
    
    // Известные составные числа (включая числа-кармичей)
    {"Test 119: is_prime(561)", BigInteger("561"), 5, false},    // Carmichael number
    {"Test 120: is_prime(1105)", BigInteger("1105"), 5, false},  // Carmichael number
    {"Test 121: is_prime(1729)", BigInteger("1729"), 5, false},  // Carmichael number
    {"Test 122: is_prime(2465)", BigInteger("2465"), 5, false},  // Carmichael number
    {"Test 123: is_prime(2821)", BigInteger("2821"), 5, false},  // Carmichael number
    {"Test 124: is_prime(6601)", BigInteger("6601"), 5, false},  // Carmichael number
    {"Test 125: is_prime(8911)", BigInteger("8911"), 5, false},  // Carmichael number
    {"Test 126: is_prime(10585)", BigInteger("10585"), 5, false},// Carmichael number
    {"Test 127: is_prime(15841)", BigInteger("15841"), 5, false},// Carmichael number
    {"Test 128: is_prime(29341)", BigInteger("29341"), 5, false},// Carmichael number
    {"Test 129: is_prime(41041)", BigInteger("41041"), 5, false},// Carmichael number
    {"Test 130: is_prime(46657)", BigInteger("46657"), 5, false},// Carmichael number
    {"Test 131: is_prime(52633)", BigInteger("52633"), 5, false},// Carmichael number
    {"Test 132: is_prime(62745)", BigInteger("62745"), 5, false},// Carmichael number
    {"Test 133: is_prime(63973)", BigInteger("63973"), 5, false},// Carmichael number
    {"Test 134: is_prime(75361)", BigInteger("75361"), 5, false},// Carmichael number
    {"Test 135: is_prime(101101)", BigInteger("101101"), 5, false},// Carmichael number
    {"Test 136: is_prime(115921)", BigInteger("115921"), 5, false},// Carmichael number
    {"Test 137: is_prime(126217)", BigInteger("126217"), 5, false},// Carmichael number
    {"Test 138: is_prime(162401)", BigInteger("162401"), 5, false},// Carmichael number
    {"Test 139: is_prime(172081)", BigInteger("172081"), 5, false},// Carmichael number
    {"Test 140: is_prime(188461)", BigInteger("188461"), 5, false},// Carmichael number
    {"Test 141: is_prime(252601)", BigInteger("252601"), 5, false},// Carmichael number
    {"Test 142: is_prime(278545)", BigInteger("278545"), 5, false},// Carmichael number
    {"Test 143: is_prime(294409)", BigInteger("294409"), 5, false},// Carmichael number
    {"Test 144: is_prime(314821)", BigInteger("314821"), 5, false},// Carmichael number
    {"Test 145: is_prime(334153)", BigInteger("334153"), 5, false},// Carmichael number
    {"Test 146: is_prime(340561)", BigInteger("340561"), 5, false},// Carmichael number
    {"Test 147: is_prime(399001)", BigInteger("399001"), 5, false},// Carmichael number
    {"Test 148: is_prime(410041)", BigInteger("410041"), 5, false},// Carmichael number
    {"Test 149: is_prime(449065)", BigInteger("449065"), 5, false},// Carmichael number
    {"Test 150: is_prime(488884324432434234341)", BigInteger("488884324432434234341"), 5, false} // Carmichael number
};

    for (const auto& test : tests) {
        bool result = test.number.is_prime(test.k);
        if (areEqual(result, test.expected)) {
            std::cout << test.name << ": Passed" << std::endl;
        } else {
            std::cout << test.name << ": Failed - Expected: " << std::boolalpha 
                      << test.expected << ", Got: " << result << std::endl;
        }
    }
}






int main() {
        testIsPrime();



    std::cout << "--------------------------------" << std::endl;
    std::cout << "Running BigRational isqrt() tests..." << std::endl;
    std::cout << "--------------------------------" << std::endl;

    testRationalIsqrtZero();
    testRationalIsqrtOne();
    testRationalIsqrtPerfectSquares();
    testRationalIsqrtNonPerfectSquares();
    testRationalIsqrtNegativeNumbers();

    std::cout << "--------------------------------" << std::endl;
    std::cout << "All tests completed." << std::endl;


    testFromInt64Min();
    testFromStringInvalid();
    testFromStringValid();
        // testEval();
    // testIntConstructor();

    // BigInteger cvc("-1");
    // BigInteger wded("1");

    // std::cout << " 1/-1: " << wded / cvc << std::endl;

    // testIntConstructor();
    // testStringConstructor();
    // testErrorHandling();
    // testBigIntegerInputOperator();

    std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||!\n";


    // testBigIntegerInputOperator();
        // testBigRationalConstructors();

    // testBigRationalIsqrt();
    // run_all_BigRational_tests();









    try {
        //test_big_rational_arithmetics_SJJSJS();

       // test_big_integer_arithmetics();


       //// test_input_operator();
        //test_input_operator_bigrational();

        // test_big_integer_operations_assert();
        // test_big_rational_operations();

        //test_case_int64();
        //////////////test_unarny_op_and_sub_add_op();

        // test_big_integer_operations2();
        //  test_big_rational_operations2();

        // test_unarny_op_and_sub_add_op();
        // test_unary_minus();





    //     test_operations();
    //     test_rational_operations();
    // //    test_big_integer_multiplication();

    //     test_big_rational_comparisons();
    //     test_big_rational_add_subtract();
    //     test_big_rational_multiplication();
    //     test_big_rational_division();
        //test_big_integer_operations();

        //test_big_integer_sqrt();
        //test_big_rational_sqrt();

        // test_big_rational_sqrt();
                //test_big_integer_isqrt();
        //test_big_integer_is_prime();

       // test_rational_operations_recovery();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // //test         {"Test 2: -1/2 + 1/3", BigRational(-1, 2), BigRational(1, 3), BigRational(-1, 6), 'a'},

    // BigRational a("-1", "2");
    // BigRational b("1", "3");
    // std::cout << "a + b: " << a + b << std::endl;

    // // // Test += 3 cases
    // // BigInteger a("-123456789");
    // // BigInteger b("-987654321");
    // // std::cout << "a += b: " << (a -= b) << std::endl;
    // // BigInteger q("987654321");
    // // BigInteger w("-123456789");
    // // std::cout << "q += w: " << (q += w) << std::endl;


    // BigRational c("-5", "3");
    // BigRational d(-2, 7);
    // BigRational e("-1", "21");

    // BigRational g("-2", "3");
    // BigRational h(-2, 3);
    // std::cout << "c * d: " << g * h  << std::endl;

    // //Write test fore BN nnad operator - 
    // BigRational i("-1", "2");
    // BigRational j(1, 3);
    // std::cout << "i - j: " << i + j  << std::endl;
    //     std::cout << "i - j: " << -i - (-j)  << std::endl;


    // BigRational k("-2", "3");
    // BigRational l(-2, 3);
    // std::cout << "k / l: " << k - l  << std::endl;

    // BigRational m("-2", "3");
    // BigRational n(-2, 3);
    // std::cout << "m % n: " << m - n  << std::endl;


}
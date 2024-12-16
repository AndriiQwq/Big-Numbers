#include "Big_Numbers.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>

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

int main() {
    try {
        //test_operations();
        // test_rational_operations();
       // test_big_integer_multiplication();
        //test_big_rational_comparisons();
        test_big_rational_add_subtract();
        test_big_rational_multiplication();
        test_big_rational_division();
        //test_big_integer_operations();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    BigRational c("-5", "3");
    BigRational d(-2, 7);
    BigRational e("-1", "21");

    BigRational g("-2", "3");
    BigRational h(-2, 3);
    std::cout << "c * d: " << g * h  << std::endl;
}
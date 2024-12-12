#include <iostream>
#include "Big_Numbers.hpp"

void test_multiplication() {
    std::cout << "\n=== Basic Multiplication Tests ===\n";
    
    // Test 1: Simple positive numbers
    BigInteger a("12");
    BigInteger b("34");
    std::cout << "Simple multiplication: " << (a * b == BigInteger("408") ? "PASS" : "FAIL")
              << " (12 * 34 = " << a * b << ")\n";

    // Test 2: Multiplication by zero
    BigInteger c("123");
    BigInteger zero("0");
    std::cout << "Multiply by zero: " << (c * zero == zero ? "PASS" : "FAIL")
              << " (123 * 0 = " << c * zero << ")\n";

    // Test 3: Negative numbers
    BigInteger d("-5");
    BigInteger e("7");
    std::cout << "Negative multiplication: " << (d * e == BigInteger("-35") ? "PASS" : "FAIL")
              << " (-5 * 7 = " << d * e << ")\n";

    // Test 4: Two negative numbers
    BigInteger f("-3");
    BigInteger g("-4");
    std::cout << "Negative * negative: " << (f * g == BigInteger("12") ? "PASS" : "FAIL")
              << " (-3 * -4 = " << f * g << ")\n";
}

int main() {
    try {
        test_multiplication();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
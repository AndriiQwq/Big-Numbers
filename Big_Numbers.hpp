#pragma once
// here you can include whatever you want :)

#include <iostream>   // For std::istream, std::ostream
#include <vector>     // For std::vector
#include <string>     // For std::string
#include <cstdint>    // For int64_t, uint8_t
#include <stdexcept>  // For std::invalid_argument
#include <cctype>     // For std::isdigit

// here you can include whatever you want :)
#include <string>
#include <stdint.h>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
#define SUPPORT_IFSTREAM 0
#define SUPPORT_MORE_OPS 0
#define SUPPORT_EVAL 0 // special bonus

class BigInteger
{
    friend std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs);

    friend BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);
    // friend BigInteger operator+(const BigInteger &lhs, const int rhs);

public:
    // constructors
    // BigInteger();
    // BigInteger(int64_t n);
    // explicit BigInteger(const std::string& str);
    /**
        Constructors implementation
    */

    // Single constructor without parameters 
    BigInteger(){
        digits_of_BN.push_back(0);
        first_sign_is_negative = false;
    }

    // Constructor with int64_t parameter
    BigInteger(int64_t num){
        if (num == 0) {
            digits_of_BN.push_back(0);
        } else if (num == -0) {
            // -0 is equal to 0, example: -000 -> 0
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
        } else {

            if (num < 0)
                first_sign_is_negative = true;
            else
                first_sign_is_negative = false;

            int64_t temp;

            if (first_sign_is_negative)
                temp = -num;
            else
                temp = num;

            while (temp > 0) {
                digits_of_BN.push_back(temp % 10);
                temp /= 10;
            }
        }
    }

    // Constructor with strings parameter
    explicit BigInteger(const std::string& str) {
        if (str.empty()) 
            throw std::invalid_argument("Empty string");
        

        if (str[0] == '-') {
            if (str.size() == 1)
                throw std::invalid_argument("Invalid string");

            first_sign_is_negative = true;
        } else if (str[0] == '+'){
            if (str.size() == 1)
                throw std::invalid_argument("Invalid string");

            first_sign_is_negative = false;
        } else {
            first_sign_is_negative = false;
        }

        // Save the number string to the vector
        for (int i = str.size() - 1; i >= 0; i--) {
            if (str[i] == '-' || str[i] == '+')
                continue;

            if (!std::isdigit(str[i]))
                throw std::invalid_argument("Invalid string");

            // Convers chat to int 
            digits_of_BN.push_back(str[i] - '0');
        }

        // Remove leading zeros, example: 000123 -> 123, 000 -> 0
        
        while (digits_of_BN.size() > 1 && digits_of_BN.back() == 0) {
            digits_of_BN.pop_back();
        }

        // example: -000 -> 0   
        if  (digits_of_BN.size() == 1 && digits_of_BN[0] == 0)
            first_sign_is_negative = false;
    }









    // copy
    //BigInteger(const BigInteger& other);
    // copy constructor implementation
    //BigInteger& operator=(const BigInteger& rhs);

    BigInteger(const BigInteger& other) = default;
    BigInteger& operator=(const BigInteger& rhs) = default;



    // unary operators, modificated 9.12.2024
    const BigInteger& operator+() const { return *this; }
    //BigInteger operator-() const;

    BigInteger operator-() const {
        BigInteger result = *this;
        //if (result.digits_of_BN.size() > 1 || result.digits_of_BN[0] != 0) 
        
        result.first_sign_is_negative = !result.first_sign_is_negative;
        
        return result;
    }




    // binary arithmetics operators
    
    BigInteger& operator+=(const BigInteger& rhs) {
        // Если числа имеют одинаковые знаки
        if (first_sign_is_negative == rhs.first_sign_is_negative) {
            // Сложение абсолютных значений
            size_t max_size = std::max(digits_of_BN.size(), rhs.digits_of_BN.size());
            digits_of_BN.resize(max_size, 0);
            
            int carry = 0;
            for (size_t i = 0; i < max_size || carry; ++i) {
                if (i == digits_of_BN.size()) {
                    digits_of_BN.push_back(0);
                }
                int sum = carry + digits_of_BN[i];
                if (i < rhs.digits_of_BN.size()) {
                    sum += rhs.digits_of_BN[i];
                }
                digits_of_BN[i] = sum % 10;
                carry = sum / 10;
            }
        } else {
            // Вычитание для разных знаков
            BigInteger abs_lhs = *this;
            BigInteger abs_rhs = rhs;
            abs_lhs.first_sign_is_negative = false;
            abs_rhs.first_sign_is_negative = false;
            
            // Определяем большее по модулю число
            bool this_is_larger = compare_abs(abs_lhs, abs_rhs) >= 0;
            const BigInteger& larger = this_is_larger ? abs_lhs : abs_rhs;
            const BigInteger& smaller = this_is_larger ? abs_rhs : abs_lhs;
            
            // Результат будет иметь знак большего по модулю числа
            first_sign_is_negative = this_is_larger ? this->first_sign_is_negative : rhs.first_sign_is_negative;
            
            // Вычитание
            digits_of_BN = larger.digits_of_BN;
            int borrow = 0;
            for (size_t i = 0; i < digits_of_BN.size(); ++i) {
                int diff = digits_of_BN[i] - borrow - 
                        (i < smaller.digits_of_BN.size() ? smaller.digits_of_BN[i] : 0);
                if (diff < 0) {
                    diff += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                digits_of_BN[i] = diff;
            }
        }

        // Нормализация результата
        while (digits_of_BN.size() > 1 && digits_of_BN.back() == 0) {
            digits_of_BN.pop_back();
        }
        if (digits_of_BN.size() == 1 && digits_of_BN[0] == 0) {
            first_sign_is_negative = false;
        }

        return *this;
    }

    BigInteger& operator-=(const BigInteger& rhs);
    BigInteger& operator*=(const BigInteger& rhs);
    BigInteger& operator/=(const BigInteger& rhs);
    BigInteger& operator%=(const BigInteger& rhs);

    // friend BigInteger operator+(BigInteger lhs, const BigInteger& rhs) {
    //     lhs += rhs;
    //     return lhs;
    // }
    // friend BigInteger op

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
    bool is_prime(size_t k) const; // use rabbin-miller test with k rounds
#endif
private:
    // here you can add private data and members, but do not add stuff to 
    // public interface, also you can declare friends here if you want
    std::vector<uint8_t> digits_of_BN;
    bool first_sign_is_negative;

    static int compare_abs(const BigInteger& lhs, const BigInteger& rhs) {
        if (lhs.digits_of_BN.size() != rhs.digits_of_BN.size()) {
            return lhs.digits_of_BN.size() < rhs.digits_of_BN.size() ? -1 : 1;
        }
        for (int i = lhs.digits_of_BN.size() - 1; i >= 0; --i) {
            if (lhs.digits_of_BN[i] != rhs.digits_of_BN[i]) {
                return lhs.digits_of_BN[i] < rhs.digits_of_BN[i] ? -1 : 1;
            }
        }
        return 0;
    }
};

inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs) {
    if (rhs.first_sign_is_negative) {
        lhs << '-';
    } else {
        lhs << '+';
    }

    for (auto it = rhs.digits_of_BN.rbegin(); it != rhs.digits_of_BN.rend(); ++it) {
        lhs << static_cast<char>(*it + '0');
    }

  //lhs << rhs.to_string();
  return lhs;
}

inline BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    std::cout << "operator+(const BigInteger& lhs, const BigInteger& rhs)" << std::endl;
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    BigInteger result = lhs;    // Make a copy
    result += rhs;              // Add using operator+=
    return result;
}

inline BigInteger operator-(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator*(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator/(BigInteger lhs, const BigInteger& rhs);
inline BigInteger operator%(BigInteger lhs, const BigInteger& rhs);

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigInteger& lhs, const BigInteger& rhs);
// idea is, that all comparison should work, it is not important how you do it
inline bool operator==(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator<(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator>(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs);


#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to 
// whitespace, consumed characters etc...
inline std::istream& operator>>(std::istream& lhs, BigInteger& rhs); // bonus
#endif

/**
    Rational part
 */

class BigRational
{
public:
    // constructors
    BigRational();
    BigRational(int64_t a, int64_t b);
    BigRational(const std::string& a, const std::string& b);
    // copy
    BigRational(const BigRational& other);
    BigRational& operator=(const BigRational& rhs);
    // unary operators
    const BigRational& operator+() const;
    BigRational operator-() const;
    // binary arithmetics operators
    BigRational& operator+=(const BigRational& rhs);
    BigRational& operator-=(const BigRational& rhs);
    BigRational& operator*=(const BigRational& rhs);
    BigRational& operator/=(const BigRational& rhs);

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
#endif
private:
    // here you can add private data and members, but do not add stuff to 
    // public interface, also you can declare friends here if you want
};

inline BigRational operator+(BigRational lhs, const BigRational& rhs);
inline BigRational operator-(BigRational lhs, const BigRational& rhs);
inline BigRational operator*(BigRational lhs, const BigRational& rhs);
inline BigRational operator/(BigRational lhs, const BigRational& rhs);

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigRational& lhs, const BigRational& rhs);
// idea is, that all comparison should work, it is not important how you do it
inline bool operator==(const BigRational& lhs, const BigRational& rhs);
inline bool operator!=(const BigRational& lhs, const BigRational& rhs);
inline bool operator<(const BigRational& lhs, const BigRational& rhs);
inline bool operator>(const BigRational& lhs, const BigRational& rhs);
inline bool operator<=(const BigRational& lhs, const BigRational& rhs);
inline bool operator>=(const BigRational& lhs, const BigRational& rhs);

inline std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs);

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to 
// whitespace, consumed characters etc...
inline std::istream& operator>>(std::istream& lhs, BigRational& rhs); // bonus
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string&);
#endif
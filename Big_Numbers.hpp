#pragma once

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cctype>
#include <limits> 
#include <random>
#include <sstream>
#include <algorithm>

#define SUPPORT_IFSTREAM 1
#define SUPPORT_MORE_OPS 1

class BigInteger
{
    friend std::ostream& operator<<(std::ostream &lhs, const BigInteger &rhs);

    friend BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);
    friend BigInteger operator-(const BigInteger &lhs, const BigInteger &rhs);
    friend BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs);
    friend BigInteger operator/(const BigInteger &lhs, const BigInteger &rhs);
    friend BigInteger operator%(const BigInteger &lhs, const BigInteger &rhs);

    friend bool operator==(const BigInteger &lhs, const BigInteger &rhs);
    friend bool operator!=(const BigInteger &lhs, const BigInteger &rhs);
    friend bool operator<(const BigInteger &lhs, const BigInteger &rhs);
    friend bool operator>(const BigInteger &lhs, const BigInteger &rhs);
    friend bool operator<=(const BigInteger &lhs, const BigInteger &rhs);
    friend bool operator>=(const BigInteger &lhs, const BigInteger &rhs);

    friend BigInteger power(const BigInteger& base, const BigInteger& pow);

public:

    std::string to_string() const {
        if (digits_of_BN.empty()) return "0";
        
        std::string result;
        if (first_sign_is_negative) result += '-';
        for (auto it = digits_of_BN.rbegin(); it != digits_of_BN.rend(); ++it)
            result += ('0' + *it);
        
        return result;
    }
    /**Constructors implementation*/

    // Single constructor without parameters is 0 
    BigInteger(){
        digits_of_BN.push_back(0); 
        first_sign_is_negative = false;
    }

    // Constructor with int64_t parameter, check int min and int max values
    BigInteger(int64_t num){
        if (num == INT64_MIN) {
            std::string min_val = "-9223372036854775808";
            *this = BigInteger(min_val);
            return;
        }

        if (num == 0) {
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
        } else if (num == -0) {
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
        } else {
            if (num < 0) first_sign_is_negative = true;
            else first_sign_is_negative = false;

            int64_t temp;
            if (first_sign_is_negative) temp = -num;
            else temp = num;
            int base = 10;
            while (temp > 0) {
                digits_of_BN.push_back(temp % base);
                temp /= base;
            }
        }
    }

    // Constructor with strings parameter
    explicit BigInteger(const std::string& str) {
        if (str.empty()) 
            throw std::invalid_argument("Empty string");

        if (str[0] == '-') {
            if (str.size() == 1)
                throw std::invalid_argument("Invalid num");

            first_sign_is_negative = true;
        } else if (str[0] == '+'){
            if (str.size() == 1)
                throw std::invalid_argument("Invalid num");

            first_sign_is_negative = false;
        } else { // without first sign + 
            first_sign_is_negative = false;
        }

        // Save the number in string to the vector
        for (size_t i = str.size(); i-- > 0; ) {
            if ((str[i] == '-' || str[i] == '+') && i == 0)
                continue;
            if (!std::isdigit(str[i]))
                throw std::invalid_argument("Invalid string, it's not a number or incorrect formated number");
            digits_of_BN.push_back(str[i] - '0');
        }

        normalize();
    }

    // copy, default copy constructor
    BigInteger(const BigInteger& other) = default;
    BigInteger& operator=(const BigInteger& rhs) = default;
    // unary operators, modificated 9.12.2024
    const BigInteger& operator+() const { return *this; }
    //BigInteger operator-() const;

    BigInteger operator-() const {
        // change the sign of the num
        BigInteger result = *this;        
        result.first_sign_is_negative = !result.first_sign_is_negative;
        
        return result;
    }
    BigInteger operator*() const {return *this;}

    // binary arithmetics operators
    BigInteger& operator+=(const BigInteger& rhs) {
        // it has 4 cases: 
        // 1. + + + 
        // 2. + + - -> sub + - + 
        // 3. - + + -> sub +(2) - +(1)
        // 4. - + -

        if (first_sign_is_negative && !rhs.first_sign_is_negative) {
            // case 3
            first_sign_is_negative = false;
            *this = rhs - *this;
            return *this;
        } else if (!first_sign_is_negative && rhs.first_sign_is_negative) {
            // case 2
            *this -= (-rhs);
            return *this;
        }

        // num have the same signs, + + + or - + - -> addition
        if (first_sign_is_negative == rhs.first_sign_is_negative) {
            // adding 
            size_t max_size = std::max(digits_of_BN.size(), rhs.digits_of_BN.size());
            digits_of_BN.resize(max_size, 0);
            
            int base = 10 ;
            int carry = 0;
            for (size_t i = 0; i < max_size || carry; ++i) {
                if (i == digits_of_BN.size()) digits_of_BN.push_back(0);        
                int sum = carry + digits_of_BN[i];
                if (i < rhs.digits_of_BN.size()) sum += rhs.digits_of_BN[i];
                digits_of_BN[i] = sum % base;
                carry = sum / base;
            }
        }

        normalize();
        return *this;
    }

    BigInteger& operator-=(const BigInteger& rhs) {
        // it has 4 cases: 
        // 1. + - + 
        // 2. + - - -> add + + + 
        // 3. - - + -> add - + - 
        // 4. - - - -> sub +(2) - +(1)

        if (!first_sign_is_negative && rhs.first_sign_is_negative) {
            // case 2
            *this += -rhs;
            return *this;
        } else if (first_sign_is_negative && !rhs.first_sign_is_negative) {
            // case 3
            *this += -rhs;
            return *this;
        } else if ( first_sign_is_negative && rhs.first_sign_is_negative) {
            // case 4
            first_sign_is_negative = false;
            *this = -rhs - *this;
            return *this;
        }

        // num have the same signs, + - + -> substraction
        BigInteger lhs_num = *this;
        BigInteger rhs_num = rhs;

        // To return thre result with the larger number sign
        bool this_is_larger = compare_abs(lhs_num, rhs_num) >= 0;
        first_sign_is_negative = this_is_larger ? first_sign_is_negative : !first_sign_is_negative;
        const BigInteger& larger = this_is_larger ? lhs_num : rhs_num;
        digits_of_BN = larger.digits_of_BN;
        const BigInteger& smaller = this_is_larger ? rhs_num : lhs_num;

        int base = 10, borrow = 0;
        for (size_t i = 0; i < digits_of_BN.size(); ++i) {
            int s = digits_of_BN[i] - borrow;
            if (i < smaller.digits_of_BN.size())  s -= smaller.digits_of_BN[i];
            
            if (s < 0) {
                s += base;
                borrow = 1;
            } else {borrow = 0;}
            digits_of_BN[i] = s;
        }
        
        normalize();
        return *this;
    }

    BigInteger& operator*=(const BigInteger& rhs){
        if (digits_of_BN.empty() || rhs.digits_of_BN.empty()) throw std::invalid_argument("Invalid BigInteger");

        if (rhs.digits_of_BN[0] == 0 && rhs.digits_of_BN.size() == 1) {
            digits_of_BN.clear();
            digits_of_BN.push_back(0);

            first_sign_is_negative = false;
            return *this;
        } else if (rhs.digits_of_BN[0] == 1 && rhs.digits_of_BN.size() == 1) {
            // if multiplyt by -1 change the sign
            if (rhs.first_sign_is_negative) first_sign_is_negative = !first_sign_is_negative;
            return *this;
        }

        if (!digits_of_BN.empty() && digits_of_BN[0] == 0 && digits_of_BN.size() == 1) {
            first_sign_is_negative = false;
            return *this;
        } else if (digits_of_BN[0] == 1 && digits_of_BN.size() == 1) {
            // if multiplyt by -1 change the sign
            first_sign_is_negative = (first_sign_is_negative != rhs.first_sign_is_negative);
            digits_of_BN = rhs.digits_of_BN;
            return *this;
        }
        
        // Determine the first sign of the result of the multiplication
        bool first_sign_of_multiplication = !(first_sign_is_negative == rhs.first_sign_is_negative);
        
        // Standart base, number of digits in number system 0,1,2...9
        const int base = 10;
        std::vector<uint8_t> product(digits_of_BN.size() + rhs.digits_of_BN.size(), 0);

        for (size_t i = 0; i < rhs.digits_of_BN.size(); i++) {
            int carry = 0;
            for ( size_t j = 0; j < digits_of_BN.size(); j++) {
                int current_product = product[i + j] + carry + digits_of_BN[j] * rhs.digits_of_BN[i];
                carry = current_product / base;
                product[i + j] = current_product % base;
            }

            if (carry > 0)
                product[i + digits_of_BN.size()] = carry;
        }

        first_sign_is_negative = first_sign_of_multiplication;
        digits_of_BN = product;

        normalize();
        return *this;
    }
    BigInteger& operator/=(const BigInteger& rhs){
        // check normalization 

        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 1 && rhs.first_sign_is_negative == true) {
            first_sign_is_negative = !first_sign_is_negative;
            return *this;
        }

        //check division by zer o
        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 0)
            throw std::invalid_argument("Division by zero");

        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 1) {
            return *this;
        }


        if (digits_of_BN.size() < rhs.digits_of_BN.size()) {
            digits_of_BN.clear();
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
            return *this;
        } 

        bool first_sign_after_dividion = !(first_sign_is_negative == rhs.first_sign_is_negative);

        BigInteger dividend = *this;
        BigInteger divisor = rhs;

        BigInteger product;
        product.digits_of_BN.resize(dividend.digits_of_BN.size(), 0);

        BigInteger current; 
        // disable the first sign due to, after compparing the numbers we don't nedd to consider the first sitn 
        dividend.first_sign_is_negative = false;
        divisor.first_sign_is_negative = false;
        for (int i = dividend.digits_of_BN.size() - 1; i >= 0; --i) {
            if (current.digits_of_BN.size() == 1 && current.digits_of_BN[0] == 0) current.digits_of_BN.clear();
            current.digits_of_BN.insert(current.digits_of_BN.begin(), dividend.digits_of_BN[i]);

            int x = 0;
            while (divisor * (x + 1) <= current)x++;
            current = current - divisor * x;
            product.digits_of_BN[i] = x;
        }

        product.normalize();
        product.first_sign_is_negative = first_sign_after_dividion;

        *this = product;
        return *this;
    }
 
    BigInteger& operator%=(const BigInteger& rhs) {
        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 0)
            throw std::invalid_argument("Division by zero");

        if (digits_of_BN.size() < rhs.digits_of_BN.size())
            return *this;
    
        bool first_sign_after_dividion = first_sign_is_negative;

        BigInteger dividend = *this;
        BigInteger divisor = rhs;
        BigInteger current; 
        
        // disable the first sign due to, after compparing the numbers we don't nedd to consider the first sitn 
        dividend.first_sign_is_negative = false;
        divisor.first_sign_is_negative = false;

        for (int i = dividend.digits_of_BN.size() - 1; i >= 0; --i) {
            if (current.digits_of_BN.size() == 1 && current.digits_of_BN[0] == 0)current.digits_of_BN.clear();//get_last = true;
            current.digits_of_BN.insert(current.digits_of_BN.begin(), dividend.digits_of_BN[i]);
            int x = 0;
            while (divisor * (x + 1) <= current)x++;
            current = current - divisor * x;
        }

        current.first_sign_is_negative = first_sign_after_dividion;
        *this = current; // remainder
        return *this;
    }

    // to get the first sign of the number
    bool get_first_sign() const {
        return first_sign_is_negative;
    }

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    // bonus
    BigInteger isqrt() const;
    bool is_prime(size_t k) const;
#endif
private:
    // here you can add private data and members, but do not add stuff to 
    // public interface, also you can declare friends here if you want
    std::vector<uint8_t> digits_of_BN;
    bool first_sign_is_negative;

    static int compare_abs(const BigInteger& lhs, const BigInteger& rhs) {
        // compare length 
        if (lhs.digits_of_BN.size() != rhs.digits_of_BN.size()) return lhs.digits_of_BN.size() < rhs.digits_of_BN.size() ? -1 : 1;
        // compare number signes
        for (int i = lhs.digits_of_BN.size() - 1; i >= 0; --i) 
            if (lhs.digits_of_BN[i] != rhs.digits_of_BN[i]) return lhs.digits_of_BN[i] < rhs.digits_of_BN[i] ? -1 : 1;
        return 0;// equal numbers
    }

    void normalize() {
        // Remove leading zeros, example: 000123 -> 123, 000 -> 0
        while (digits_of_BN.size() > 1 && digits_of_BN.back() == 0)
            digits_of_BN.pop_back();

        // example: -000 -> 0   
        if (digits_of_BN.size() == 1 && digits_of_BN[0] == 0)
            first_sign_is_negative = false;
    }

};

double BigInteger::sqrt() const {
    if (first_sign_is_negative)
        throw std::invalid_argument("Negative number");

    if (digits_of_BN.size() == 1 && digits_of_BN[0] == 0) {
        return 0;
    } else if (digits_of_BN.size() == 1 && digits_of_BN[0] == 1) {
        return 1;
    }

    double n = 0;
    double decade = 1;
    for (size_t i = 0; i < digits_of_BN.size(); ++i) {
        n += digits_of_BN[i] * decade;
        decade *= 10;
    }

    if (n > std::numeric_limits<double>::max()) throw std::runtime_error("numeric_limits: double max reached");
    double out = n / 2;
    const double accuracy = 1e-15; // accuracy == 0.000000000001

    for (;;) {
        double next_output = (out + n / out) / 2;
        if ((next_output - out < 0 ? -(next_output - out) : next_output - out) < accuracy) return out;
        out = next_output;
    }

    return out;
}

BigInteger BigInteger::isqrt() const {
    if (first_sign_is_negative)
        throw std::invalid_argument("Negative number");

    if (digits_of_BN.size() == 1 && digits_of_BN[0] == 0) {
        return BigInteger(0);
    } else if (digits_of_BN.size() == 1 && digits_of_BN[0] == 1) {
        return BigInteger(1);
    }

    // Binary search

    BigInteger out = 0;
    BigInteger min = 0;
    BigInteger max = *this;

    while (min <= max) {
        BigInteger mid = min + (max - min) / 2;

        if (mid * mid == *this) return mid;
        else if (mid * mid < *this) {
            min = mid + 1;
            out = mid;
        }
        else max = mid - 1;
    }

    return out;
}


BigInteger power(BigInteger x, BigInteger y, BigInteger p) {
    BigInteger res = 1;
    x = x % p;
    while (y > 0) {
        if (y % 2 == 1) res = (res * x) % p;
        y = y / 2;
        x = (x * x) % p;
    }
    return res;
}

bool BigInteger::is_prime(size_t k) const {
    if (*this < 2) return false;
    if (*this == 2 || *this == 3) return true;
    if (digits_of_BN.size() == 1 && digits_of_BN[0] % 2 == 0) return false;
        
    size_t s = 0;

    BigInteger n = *this - 1;
    BigInteger d = n;

    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    for (size_t i = 0; i < k; i++){
        BigInteger a; 
        if (2 * BigInteger(i + 1) < n - 1 ) a = 2 * (i + 1);
        else a = 2;
        BigInteger x = power(a, d, *this);
        BigInteger y;
           for (size_t j = 0; j < s; j++) {
               y = (x * x) % *this;
               if (y == 1 && x != 1 && x != n) return false;

               x = y;
           }
            
        if (y != 1) return false;
    }

    return true;
}

inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs) {
    if (rhs.first_sign_is_negative && rhs != 0) lhs << '-';

    for (auto it = rhs.digits_of_BN.rbegin(); it != rhs.digits_of_BN.rend(); ++it)
        lhs << static_cast<char>(*it + '0');

  return lhs;
}

inline BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result += rhs;
    return result;
}

inline BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result -= rhs;
    return result;
}

inline BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs){
    BigInteger result = lhs;
    result *= rhs;
    return result;
}

inline BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result /= rhs;
    return result;
}

inline BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result %= rhs;
    return result;
}
// alternatively you can implement 
// std::strong_ordering operator<=>(const BigInteger& lhs, const BigInteger& rhs);
// idea is, that all comparison should work, it is not important how you do it
inline bool operator==(const BigInteger& lhs, const BigInteger& rhs){
    if (rhs.digits_of_BN.size() == 1 && lhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 0 && lhs.digits_of_BN[0] == 0)
        return true;

    if (lhs.first_sign_is_negative != rhs.first_sign_is_negative) 
        return false;

    if (lhs.digits_of_BN.size() != rhs.digits_of_BN.size())
        return false;

    return lhs.digits_of_BN == rhs.digits_of_BN;
}

inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs) { 
    return !(lhs == rhs); 
}

inline bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.first_sign_is_negative && !rhs.first_sign_is_negative)
        return true;
    else if (!lhs.first_sign_is_negative && rhs.first_sign_is_negative)
        return false;

    // check the first sign of the numbers to their length, 4 cases: 

    if ((lhs.first_sign_is_negative && rhs.first_sign_is_negative) && (lhs.digits_of_BN.size() > rhs.digits_of_BN.size()))
        return true;
    else if ((!lhs.first_sign_is_negative && !rhs.first_sign_is_negative) && (lhs.digits_of_BN.size() < rhs.digits_of_BN.size()))
        return true;
    else if ((lhs.first_sign_is_negative && rhs.first_sign_is_negative) && (lhs.digits_of_BN.size() < rhs.digits_of_BN.size()))
        return false;
    else if ((!lhs.first_sign_is_negative && !rhs.first_sign_is_negative) && (lhs.digits_of_BN.size() > rhs.digits_of_BN.size()))
        return false;
    
    for (int i = lhs.digits_of_BN.size() - 1; i >= 0; --i)
        if (lhs.digits_of_BN[i] != rhs.digits_of_BN[i]) {
            if (lhs.first_sign_is_negative) {// cheke by the first sign 
                return lhs.digits_of_BN[i] > rhs.digits_of_BN[i];
            }else {
                return lhs.digits_of_BN[i] < rhs.digits_of_BN[i];
            }
        }

    return false;
}

inline bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
    if ((lhs < rhs) || lhs == rhs) return false;
    else return true;
}

inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs < rhs || lhs == rhs) return true;
    else return false;
}
inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs > rhs || lhs == rhs) return true;
    else return false;
}


#if SUPPORT_IFSTREAM == 1
// bonus
inline std::istream& operator>>(std::istream& lhs, BigInteger& rhs) {
    std::string input;

    lhs >> input;
    try {
        rhs = BigInteger(input);
    } catch (const std::invalid_argument &e) {
        lhs.setstate(std::ios::failbit);
    }
    return lhs;
}

#endif

/** Rational part*/

class BigRational
{
    friend std::ostream& operator<<(std::ostream &lhs, const BigRational &rhs);

    friend BigRational operator+(BigRational lhs, const BigRational& rhs);
    friend BigRational operator-(BigRational lhs, const BigRational& rhs);
    friend BigRational operator*(BigRational lhs, const BigRational& rhs);
    friend BigRational operator/(BigRational lhs, const BigRational& rhs);
    
    friend bool operator==(const BigRational& lhs, const BigRational& rhs);
    friend bool operator!=(const BigRational& lhs, const BigRational& rhs);
    friend bool operator<(const BigRational& lhs, const BigRational& rhs);
    friend bool operator>(const BigRational& lhs, const BigRational& rhs);
    friend bool operator<=(const BigRational& lhs, const BigRational& rhs);
    friend bool operator>=(const BigRational& lhs, const BigRational& rhs);

public:
    // constructors
    BigRational() {
        Numerator = 0;
        Denominator = 1;
        first_sign_is_negative_RN = false;
    }
    // constructor with int parameters
    BigRational(int64_t a, int64_t b){
        if ( b == 0)
                throw std::invalid_argument("Denominator can't == 0"); 
        
        try {
            Numerator = BigInteger(a);
            Denominator = BigInteger(b);
        } catch (const std::invalid_argument &e) {
            throw std::invalid_argument("Invalid BigRational");
        }

        if (Numerator == 0) {
            first_sign_is_negative_RN = false;
            Denominator = 1;
        } else {
            if (Denominator < 0) {
                Numerator = -Numerator;
                Denominator = -Denominator;
            }
            first_sign_is_negative_RN = (Numerator < 0);
        }
        
        normalize();
    }
    // constructor with str parameters
    BigRational(const std::string& a, const std::string& b) {
        try {
            Numerator = BigInteger(a);
            Denominator = BigInteger(b);
        } catch (const std::invalid_argument &e) {
            throw std::invalid_argument("Invalid BigRational");
        }

        if (Denominator == 0) throw std::invalid_argument("Denominator can't == 0");
        if (Numerator.get_first_sign() == true){first_sign_is_negative_RN = true;
        } else {first_sign_is_negative_RN = false;}
        normalize();
    }

    // copy
    BigRational(const BigRational& other) = default;
    BigRational& operator=(const BigRational& rhs) = default;
    // unary operators
    const BigRational& operator+() const { return *this; }
    BigRational operator-() const {
        BigRational res = *this;
        res.first_sign_is_negative_RN = !res.first_sign_is_negative_RN;
        res.Numerator = -res.Numerator;
        return res;
    }
    // binary arithmetics operators
    BigRational& operator+=(const BigRational& rhs) {
        // no need to - >> // it has 4 cases similar to the BigInteger
        // 1. + + + 
        // 2. + + - -> sub + - + 
        // 3. - + + -> sub +(2) - +(1)
        // 4. - + -

        if (first_sign_is_negative_RN && !rhs.first_sign_is_negative_RN) {
            // case 3
            BigRational temp_rhs = rhs;
            BigRational temp_this = *this;
            temp_this.first_sign_is_negative_RN = false; // temp_this -> +
            temp_this.Numerator = -temp_this.Numerator;
            
            *this = temp_rhs - temp_this;
            return *this;
        } else if (!first_sign_is_negative_RN && rhs.first_sign_is_negative_RN) {
            // case 2
            *this -= (-rhs);
            return *this;
        }

        // num have the same signs, + + + or - + - -> addition
        if (Denominator != rhs.Denominator){
            //find NSD
            BigInteger nsd = find_nsd(Denominator, rhs.Denominator);
            BigInteger nok = (Denominator * rhs.Denominator) / nsd;
            Numerator = (Numerator * (nok / Denominator)) + (rhs.Numerator * (nok / rhs.Denominator));
            Denominator = nok;

        } else {Numerator += rhs.Numerator;}

        normalize();

        return *this;
    }
    BigRational& operator-=(const BigRational& rhs) {
        // it has 4 cases: 
        // 1. + - + 
        // 2. + - - -> add + + + 
        // 3. - - + -> add - + - 
        // 4. - - - -> sub +(2) - +(1)

        if (!first_sign_is_negative_RN && rhs.first_sign_is_negative_RN) {
            // case 2
            *this += -rhs;
            return *this;
        } else if (first_sign_is_negative_RN && !rhs.first_sign_is_negative_RN) {
            // case 3
            *this += -rhs;
            return *this;
        } else if ( first_sign_is_negative_RN && rhs.first_sign_is_negative_RN) {
            // case 4
            BigRational temp_rhs = rhs;
            BigRational temp_this = *this;
            temp_rhs.first_sign_is_negative_RN = false;
            temp_rhs.Numerator = -temp_rhs.Numerator;

            *this = temp_rhs + temp_this;

            return *this;
        }

        // num have the same signs, + - + -> substraction
        if (Denominator != rhs.Denominator){
            //find NSD
            BigInteger nsd = find_nsd(Denominator, rhs.Denominator);
            BigInteger nok = (Denominator * rhs.Denominator) / nsd;
            Numerator = (Numerator * (nok / Denominator)) - (rhs.Numerator * (nok / rhs.Denominator));
            Denominator = nok;
            
        } else {
            Numerator -= rhs.Numerator;
            if (Numerator < 0) {first_sign_is_negative_RN = true;} 
            else { first_sign_is_negative_RN = false;}
        }
        normalize();
        
        return *this;
    }
    BigRational& operator*=(const BigRational& rhs) {
        Numerator *= rhs.Numerator;
        Denominator *= rhs.Denominator;
        first_sign_is_negative_RN = !(first_sign_is_negative_RN == rhs.first_sign_is_negative_RN);
        normalize();
        return *this;
    }
    BigRational& operator/=(const BigRational& rhs) {
        if (rhs.Numerator == 0) throw std::invalid_argument("Division by zero");

        Numerator *= rhs.Denominator;
        Denominator *= rhs.Numerator;
        first_sign_is_negative_RN = !(first_sign_is_negative_RN == rhs.first_sign_is_negative_RN);
        normalize();
        return *this;
    }
    

    double sqrt() const;
#if SUPPORT_MORE_OPS == 1
    BigInteger isqrt() const;
#endif
private:
    // here you can add private data and members, but do not add stuff to 
    // public interface, also you can declare friends here if you want

    BigInteger Numerator;
    BigInteger Denominator;
    bool first_sign_is_negative_RN;

    BigInteger find_nsd(BigInteger a, BigInteger b) {
        if (b == 0)  return a;
        return find_nsd(b, a % b);
    }

    void normalize() {
        if (Denominator == 0)  throw std::invalid_argument("Denominator can't be 0");
        if (Numerator == 0) {
            Denominator = 1;
            first_sign_is_negative_RN = false;
        }

        if (Denominator == 1) return;

        BigInteger nsd = find_nsd(Numerator, Denominator);
        Numerator /= nsd;
        Denominator /= nsd;

        // 3/-4 -> -3/4
        if (Denominator < 0) {
            Numerator = -Numerator;
            Denominator = -Denominator;
        }
    }
};

double BigRational::sqrt() const {
    double n_sqrt = Numerator.sqrt();
    double d_sqrt = Denominator.sqrt();
    double res = n_sqrt / d_sqrt;
    
    return res;
}

BigInteger BigRational::isqrt() const {
    if (first_sign_is_negative_RN && Numerator != 0)
        throw std::invalid_argument("Negative number");

    BigInteger n_isqrt = Numerator;
    BigInteger d_isqrt = Denominator;

    n_isqrt -= n_isqrt % d_isqrt;

    if (n_isqrt < d_isqrt) return BigInteger(0);
    BigInteger res = n_isqrt / d_isqrt;

    return res.isqrt();
}

inline BigRational operator+(BigRational lhs, const BigRational& rhs) {return lhs += rhs;}
inline BigRational operator-(BigRational lhs, const BigRational& rhs) {return lhs -= rhs;}
inline BigRational operator*(BigRational lhs, const BigRational& rhs) {return lhs *= rhs;}
inline BigRational operator/(BigRational lhs, const BigRational& rhs) {return lhs /= rhs;}

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigRational& lhs, const BigRational& rhs);
// idea is, that all comparison should work, it is not important how you do it
inline bool operator==(const BigRational& lhs, const BigRational& rhs) {
    if (lhs.Numerator == rhs.Numerator && lhs.Denominator == rhs.Denominator) return true;
    else return false;
}
inline bool operator!=(const BigRational& lhs, const BigRational& rhs) { return !(lhs == rhs); }
inline bool operator<(const BigRational& lhs, const BigRational& rhs) {return (lhs.Numerator * rhs.Denominator) < (rhs.Numerator * lhs.Denominator);}

inline bool operator>(const BigRational& lhs, const BigRational& rhs) {
    if (lhs < rhs || lhs == rhs)return false;
    else return true;
}
inline bool operator<=(const BigRational& lhs, const BigRational& rhs) {
    if (lhs < rhs || lhs == rhs) return true;
    else return false;
}
inline bool operator>=(const BigRational& lhs, const BigRational& rhs) {
    if (lhs > rhs || lhs == rhs) return true;
    else return false;
}

inline std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs) {
    if (rhs.Denominator == BigInteger(1)) lhs << rhs.Numerator;
    else if (rhs.Numerator == BigInteger(0)) lhs << 0;
    else lhs << rhs.Numerator << '/' << rhs.Denominator;

    return lhs;
}

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to 
// whitespace, consumed characters etc...
// bonus
inline std::istream& operator>>(std::istream& lhs, BigRational& rhs) {
    std::string input;
    lhs >> input;

    std::string Numerator;
    std::string Denominator;

    size_t it = 0; // remove whitespaces in begin
    while (it < input.size() && std::isspace(static_cast<unsigned char>(input[it])))
        ++it;

    if (input.empty()) {
        lhs.setstate(std::ios::failbit);
        return lhs;
    }

    size_t find_slesh_sign = input.find('/');
    if (find_slesh_sign == std::string::npos) {
        try {
            size_t find_first_whitespaces = input.find(' ');
            if (find_first_whitespaces != std::string::npos) {
                Numerator = input.substr(it, find_first_whitespaces);
                Denominator = "1";
            } else {
                Numerator = input;
                Denominator = "1";
            }
        } catch (const std::invalid_argument &e) {
            lhs.setstate(std::ios::failbit);
        }
    } else{
        Numerator = input.substr(0, find_slesh_sign);
        Denominator = input.substr(find_slesh_sign + 1);
    }

    try {
        rhs = BigRational(Numerator, Denominator);
    } catch (const std::invalid_argument &e) {
        lhs.setstate(std::ios::failbit);
    }

    return lhs;
} 
#endif
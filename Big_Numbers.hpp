#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint> // int64_t, uint8_t
#include <stdexcept> // invalid_argument
#include <cctype> // isdigit

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
#define SUPPORT_IFSTREAM 0
#define SUPPORT_MORE_OPS 0
#define SUPPORT_EVAL 0 // special bonus

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



public:
    // constructors
    // BigInteger();
    // BigInteger(int64_t n);
    // explicit BigInteger(const std::string& str);
    /**Constructors implementation*/
    // Single constructor without parameters 
    BigInteger(){
        digits_of_BN.push_back(0);
        first_sign_is_negative = false;
    }

    // Constructor with int64_t parameter
    BigInteger(int64_t num){
        if (num == 0) {
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
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
        for (int i = str.size() - 1; i >= 0; i--) {
            // skip thw first sign
            if (str[i] == '-' || str[i] == '+')
                continue;
            if (!std::isdigit(str[i]))
                throw std::invalid_argument("Invalid string, it's not a number or incorrect formated number");
            // Convers char to int 
            digits_of_BN.push_back(str[i] - '0');
        }

        normalize();
    }

    // copy
    BigInteger(const BigInteger& other) = default;
    BigInteger& operator=(const BigInteger& rhs) = default;
    // unary operators, modificated 9.12.2024
    const BigInteger& operator+() const { return *this; }
    //BigInteger operator-() const;

    BigInteger operator-() const {
        // change the sign of the number
        BigInteger result = *this;        
        result.first_sign_is_negative = !result.first_sign_is_negative;
        
        return result;
        // add -0 check 
    }
    BigInteger operator*() const {return *this;}

    // binary arithmetics operators
    BigInteger& operator+=(const BigInteger& rhs) {
        // it has 4 cases: 
        // 1. + + + 
        // 2. + + - -> sub + - + 
        // 3. - + + -> sub +(2) - +(1)
        // 4. - + -

        // check 2 conditions: 
        // if ( first_sign_is_negative && !rhs.first_sign_is_negative) {
        //     rhs.first_sign_is_negative = true;
        //     *this -= rhs;
        //     return *this;
        // } else if (!first_sign_is_negative && rhs.first_sign_is_negative) {
        //     first_sign_is_negativ = true;
        //     rhs -= *this;

        //     *this = rhs;
        //     return *this;
        // // }

        if (first_sign_is_negative && !rhs.first_sign_is_negative) {
            // case 3
            BigInteger temp_rhs = rhs;       // Create a non-const copy
            BigInteger temp_this = *this;    // Create a copy of *this
            temp_this.first_sign_is_negative = false; // temp_this -> +
            *this = temp_rhs - temp_this;
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
                if (i == digits_of_BN.size()) 
                    digits_of_BN.push_back(0);
                
                int sum = carry + digits_of_BN[i];
                if (i < rhs.digits_of_BN.size())
                    sum += rhs.digits_of_BN[i];
                
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
            BigInteger temp_rhs = rhs;       // Create a non-const copy
            BigInteger temp_this = *this;    // Create a copy of *this
            temp_rhs.first_sign_is_negative = false; // temp_this -> +
            /*TEST */ // std::cout << "case 4: " << *this << "and " << temp_rhs << std::endl;

            *this = temp_rhs + temp_this;
            return *this;
        }


        // num have the same signs, + - + -> substraction
        BigInteger lhs_num = *this;
        BigInteger rhs_num = rhs;

        // To return thre result with the larger number sign
        bool this_is_larger = compare_abs(lhs_num, rhs_num) >= 0;
        
        const BigInteger& larger = this_is_larger ? lhs_num : rhs_num;
        const BigInteger& smaller = this_is_larger ? rhs_num : lhs_num;
        
        digits_of_BN = larger.digits_of_BN;
        first_sign_is_negative = this_is_larger ? first_sign_is_negative : !first_sign_is_negative;
        
        int base = 10;
        int borrow = 0; // borrow to the next digit
        for (size_t i = 0; i < digits_of_BN.size(); ++i) {
            int diff = digits_of_BN[i] - borrow;
            if (i < smaller.digits_of_BN.size()) 
                diff -= smaller.digits_of_BN[i];
            
            if (diff < 0) {
                diff += base;
                borrow = 1;
            } else {
                borrow = 0;
            }

            digits_of_BN[i] = diff;
        }
        
        normalize();
        return *this;
    }

    BigInteger& operator*=(const BigInteger& rhs){
        // Implementation of the multiplication

        //error that it chack num 000645453..., and do first case
        // solve
        // check neec of normalize

        // if (digits_of_BN[0] == 0 && digits_of_BN.size() != 1) 
        //     normalize();
        // else if (rhs.digits_of_BN[0] == 0 && rhs.digits_of_BN.size() == 1)
        //     normalize();
    

        if (rhs.digits_of_BN[0] == 0 && rhs.digits_of_BN.size() == 1) {
            digits_of_BN.clear();
            digits_of_BN.push_back(0);

            first_sign_is_negative = false;
            return *this;
        } else if (rhs.digits_of_BN[0] == 1 && rhs.digits_of_BN.size() == 1) {
            // if multiplyt by -1 change the sign
            if (rhs.first_sign_is_negative)
                first_sign_is_negative = !first_sign_is_negative;
            return *this;
        }

        if (digits_of_BN[0] == 0 && digits_of_BN.size() == 1) {
            first_sign_is_negative = false;
            return *this;
        } else if (digits_of_BN[0] == 1 && digits_of_BN.size() == 1) {
            // if multiplyt by -1 change the sign
            first_sign_is_negative = (first_sign_is_negative != rhs.first_sign_is_negative);
            digits_of_BN = rhs.digits_of_BN;
            return *this;
        }

        // Multiplaing the numbers
        
        // Determine the first sign of the result of the multiplication
        bool first_sign_of_multiplication = !(first_sign_is_negative == rhs.first_sign_is_negative);
        
        // Standart base, number of digits in number system 0,1,2...9
        const int base = 10;

        //multiply(a[1..p], b[1..q], base)  // Operands containing rightmost digits at index 1
        //product = [1..p+q]      // Allocate space for result
        std::vector<uint8_t> product(digits_of_BN.size() + rhs.digits_of_BN.size(), 0);

        //for b_i = 1 to q                     // for all digits in b
        for (size_t i = 0; i < rhs.digits_of_BN.size(); i++) {
            int carry = 0;
            //for a_i = 1 to p            // for all digits in a
            for ( size_t j = 0; j < digits_of_BN.size(); j++) {
                int current_product = product[i + j] + carry + digits_of_BN[j] * rhs.digits_of_BN[i];
                carry = current_product / base;
                product[i + j] = current_product % base;
            }

            if (carry > 0)
                product[i + digits_of_BN.size()] = carry;
            // product
            //     product[a_i + b_i - 1] += carry + a[a_i] * b[b_i] // -1 due to shift 0 index 
            //     carry = product[a_i + b_i - 1] / base
            //     product[a_i + b_i - 1] = product[a_i + b_i - 1] mod base
            //     product[b_i + p] = carry                               // last digit comes from final carry
        }
        ///return product

        first_sign_is_negative = first_sign_of_multiplication;
        digits_of_BN = product;

        normalize();
        return *this;
    }
    BigInteger& operator/=(const BigInteger& rhs){
        // check normalization 
        // if (digits_of_BN[0] == 0 && digits_of_BN.size() != 1) 
        //     normalize();
        // else if (rhs.digits_of_BN[0] == 0 && rhs.digits_of_BN.size() == 1)
        //     normalize();

        //check division by zer o
        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 0)
            throw std::invalid_argument("Division by zero");

        if (rhs.digits_of_BN.size() == 1 && rhs.digits_of_BN[0] == 1)
            return *this;

        if (digits_of_BN.size() < rhs.digits_of_BN.size()) {
            digits_of_BN.clear();
            digits_of_BN.push_back(0);
            first_sign_is_negative = false;
            return *this;
        } 
        // else if (digits_of_BN == rhs.digits_of_BN) {
        //     digits_of_BN.clear();
        //     digits_of_BN.push_back(1);
        //     first_sign_is_negative = !(first_sign_is_negative == rhs.first_sign_is_negative);
        //     return *this;
        // }


        bool first_sign_after_dividion = !(first_sign_is_negative == rhs.first_sign_is_negative);

        BigInteger dividend = *this;
        BigInteger divisor = rhs;
        //src:https://en.wikipedia.org/wiki/Division_algorithm
        //https://stackoverflow.com/questions/27801397/newton-raphson-division-with-big-integers

        // exmpl 
        // 98765 anf 123
        // [0 0 8 0 2] and remainder 119/123
        // 1 stem take form devidend first digit 9, we cnn't devide by 123(ani jeden krat),
        // go to the next step take second digit whaile we don't be able to devide by 123
        // take 3r digit -> 987 
        // 2 step calculate how many times we can devide 987 by 123
        // 3 step write the result to the product in the right slot [0 0 8
        // continue 
        // result 98765/123 = 802, and we don't able to devide whole remainder 65 by 123

        BigInteger product;
        product.digits_of_BN.resize(dividend.digits_of_BN.size(), 0);

        BigInteger current; // current is the num that contains the digits, updated after each step,
        // 9->98->987--->3->36->365--->119 
        // ---> return processed num by code:  
            // while (divisor * (x + 1) <= current)x++;
            //     current = current - divisor * x;
        // where x is != 0

        // disable the first sign due to, after compparing the numbers we don't nedd to consider the first sitn 
        dividend.first_sign_is_negative = false;
        divisor.first_sign_is_negative = false;
        for (int i = dividend.digits_of_BN.size() - 1; i >= 0; --i) {
            if (current.digits_of_BN.size() == 1 && current.digits_of_BN[0] == 0) current.digits_of_BN.clear();
            current.digits_of_BN.insert(current.digits_of_BN.begin(), dividend.digits_of_BN[i]);

            int x = 0; // x in the scope of 0 to 9
            while (divisor * (x + 1) <= current)x++;
            current = current - divisor * x;
            product.digits_of_BN[i] = x;
            //std::cout << x << " and " << current << std::endl;
        }

        // don't need to normalize
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
    
        // Both first signs is the same 
        bool first_sign_after_dividion = first_sign_is_negative;

        // Similar to the division, but return only remainder 
        BigInteger dividend = *this;
        BigInteger divisor = rhs;
        BigInteger current; 
        
        // disable the first sign due to, after compparing the numbers we don't nedd to consider the first sitn 
        dividend.first_sign_is_negative = false;
        divisor.first_sign_is_negative = false;

       // bool get_last = false;
        for (int i = dividend.digits_of_BN.size() - 1; i >= 0; --i) {
            if (current.digits_of_BN.size() == 1 && current.digits_of_BN[0] == 0)current.digits_of_BN.clear();//get_last = true;
            current.digits_of_BN.insert(current.digits_of_BN.begin(), dividend.digits_of_BN[i]);
            int x = 0;
            while (divisor * (x + 1) <= current)x++;
            current = current - divisor * x;
        }

        current.first_sign_is_negative = first_sign_after_dividion;
        *this = current;
        return *this;
    }

    // to get the first sign of the number
    bool get_first_sign() const {
        return first_sign_is_negative;
    }

    // void set_first_sign_is_negative() {
    //     if (first_sign_is_negative) first_sign_is_negative = false;
    //     else first_sign_is_negative = true;
    // }

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
        // compare length of the numbers
        if (lhs.digits_of_BN.size() != rhs.digits_of_BN.size())
            return lhs.digits_of_BN.size() < rhs.digits_of_BN.size() ? -1 : 1;
        
        for (int i = lhs.digits_of_BN.size() - 1; i >= 0; --i) {
            // step by step compare number signes
            if (lhs.digits_of_BN[i] != rhs.digits_of_BN[i])
                return lhs.digits_of_BN[i] < rhs.digits_of_BN[i] ? -1 : 1;
        }
        // equal numbers
        return 0;
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

inline std::ostream &operator<<(std::ostream &lhs, const BigInteger &rhs) {
    if (rhs.first_sign_is_negative) lhs << '-';
    // else {
    //     lhs << '+';//???
    // }

    // rbegin form the end, rend iterator to the begin
    //https://www.geeksforgeeks.org/vector-rbegin-and-rend-function-in-c-stl/
    //https://en.cppreference.com/w/cpp/iterator/rbegin
    for (auto it = rhs.digits_of_BN.rbegin(); it != rhs.digits_of_BN.rend(); ++it)
        lhs << static_cast<char>(*it + '0'); // get number -> char -> add to stream

  //lhs << rhs.to_string();
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

    // for (size_t i = 0; i < lhs.digits_of_BN.size(); i++)
    //     if (lhs.digits_of_BN[i] != rhs.digits_of_BN[i])
    //         return false;
    return lhs.digits_of_BN == rhs.digits_of_BN;
}

inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs) { 
    return !(lhs == rhs); 
}

inline bool operator<(const BigInteger& lhs, const BigInteger& rhs) {

   // std::cout << "lhs: " << lhs << " and rhs: " << rhs << "|"<< lhs.first_sign_is_negative << " and " << rhs.first_sign_is_negative << std::endl;

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

    if ( (lhs < rhs) && lhs != rhs)
        return false;
    else
        return true;
}

inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs < rhs || lhs == rhs)
        return true;
    else
        return false;
}
inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs > rhs || lhs == rhs)
        return true;
    else
        return false;
}


#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to 
// whitespace, consumed characters etc...
inline std::istream& operator>>(std::istream& lhs, BigInteger& rhs); // bonus
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
        Numerator = a;
        Denominator = b;

        if ( b == 0)
            throw std::invalid_argument("Denominator can't be 0"); 

        if (a == 0 && b == 0) {
            first_sign_is_negative_RN = false;
        }else {
            if (Numerator < 0)
                first_sign_is_negative_RN = true;
            else
                first_sign_is_negative_RN = false;
        }

        normalize();
    }
    // constructor with str parameters
    BigRational(const std::string& a, const std::string& b) {
        Numerator = BigInteger(a);
        Denominator = BigInteger(b);

        if (Denominator == 0) throw std::invalid_argument("Denominator can't be 0");

        if (Numerator.get_first_sign() == 1){
            first_sign_is_negative_RN = true;
        } else {
            first_sign_is_negative_RN = false;
        }

        normalize();

        //std::cout << "Numerator: " << Numerator << " and Denominator: " << Denominator << std::endl;
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
        // res.Numerator.set_first_sign_is_negative();
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
            BigRational temp_rhs = rhs;       // Create a non-const copy
            BigRational temp_this = *this;    // Create a copy of *this
            temp_this.first_sign_is_negative_RN = false; // temp_this -> +
            /// !!!
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
            BigInteger bigger_denominator;
            BigInteger smaller_denominator;

            if (Denominator > rhs.Denominator) {
                bigger_denominator = Denominator;
                smaller_denominator = rhs.Denominator;
            } else {
                bigger_denominator = rhs.Denominator;
                smaller_denominator = Denominator;
            }

            BigInteger nsd = find_nsd(bigger_denominator, smaller_denominator); 
            BigInteger nok = (bigger_denominator * smaller_denominator) / nsd;

            // std::cout << "NOK: " << nok << " and NSD: " << nsd << std::endl;
            // std::cout << "Numerator: " << Numerator << " and Denominator: " << Denominator << std::endl;
            // std::cout << "rhs.Numerator: " << rhs.Numerator << " and rhs.Denominator: " << rhs.Denominator << std::endl;

            Numerator = (Numerator * (nok / Denominator)) + (rhs.Numerator * (nok / rhs.Denominator));
            if ( first_sign_is_negative_RN && rhs.first_sign_is_negative_RN) {
                Denominator = -nok;
            } else {
                Denominator = nok;
            }

        } else {
            Numerator += rhs.Numerator;
        }

        //std::cout << "result: " << *this <<  " and " <<  first_sign_is_negative_RN << std::endl;

       // normalize();

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
           // std::cout << "case 3!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            *this += -rhs;
            return *this;
        } else if ( first_sign_is_negative_RN && rhs.first_sign_is_negative_RN) {
            // case 4
            BigRational temp_rhs = rhs;
            BigRational temp_this = *this;
            temp_rhs.first_sign_is_negative_RN = false; // temp_this -> +
            temp_rhs.Numerator = -temp_rhs.Numerator;

            *this = temp_rhs + temp_this;
            return *this;
        }

        // num have the same signs, + - + -> substraction
        if (Denominator != rhs.Denominator){
            //find NSD
            BigInteger bigger_denominator;
            BigInteger smaller_denominator;

            if (Denominator > rhs.Denominator) {
                bigger_denominator = Denominator;
                smaller_denominator = rhs.Denominator;
            } else {
                bigger_denominator = rhs.Denominator;
                smaller_denominator = Denominator;
            }

            BigInteger nsd = find_nsd(bigger_denominator, smaller_denominator); 
            BigInteger nok = (bigger_denominator * smaller_denominator) / nsd;

            //std::cout << "NOK: " << nok << " and NSD: " << nsd << std::endl;

            Numerator = (Numerator * (nok / Denominator)) - (rhs.Numerator * (nok / rhs.Denominator));
            Denominator = nok;

            // upadate the first sign statatment 
            if (Numerator < 0) {
                first_sign_is_negative_RN = true;
            } else {
                first_sign_is_negative_RN = false;
            }
        } else {
            Numerator -= rhs.Numerator;
            if (Numerator < 0) {first_sign_is_negative_RN = true;} 
            else { first_sign_is_negative_RN = false;}
        }
        //normalize();
        
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
        // if need that normalize the number, -42/21 -> -2/1 -> -2

        if (Denominator == 0)  throw std::invalid_argument("Denominator can't be 0");
        if (Numerator == 0) {
            Denominator = 1;
            first_sign_is_negative_RN = false;
        }

        BigInteger nsd = find_nsd(Numerator, Denominator);
        Numerator /= nsd;
        Denominator /= nsd;

        if (Denominator < 0) {
            Numerator = -Numerator;
            Denominator = -Denominator;
        }

        // while (true) {
        //     // While we can devide two part by NSD 
        //     BigInteger nsd = find_nsd(Numerator, Denominator);
        //     if (nsd != 1) { // can't to devide each more
        //         Numerator = Numerator / nsd;
        //         Denominator = Denominator / nsd;
        //     } else {
        //         break;
        //     }
        // }
    }
};

inline BigRational operator+(BigRational lhs, const BigRational& rhs) {
    BigRational result = lhs;
    result += rhs;
    return result;
}
inline BigRational operator-(BigRational lhs, const BigRational& rhs) {
    BigRational result = lhs;
    result -= rhs;
    return result;
}
inline BigRational operator*(BigRational lhs, const BigRational& rhs) {
    BigRational result = lhs;
    result *= rhs;
    return result;
}
inline BigRational operator/(BigRational lhs, const BigRational& rhs) {
    BigRational result = lhs;
    result /= rhs;
    return result;
}

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigRational& lhs, const BigRational& rhs);
// idea is, that all comparison should work, it is not important how you do it
inline bool operator==(const BigRational& lhs, const BigRational& rhs) {
    if (lhs.Numerator == rhs.Numerator && lhs.Denominator == rhs.Denominator)
        return true;
    else
        return false;
}
inline bool operator!=(const BigRational& lhs, const BigRational& rhs) { return !(lhs == rhs); }

inline bool operator<(const BigRational& lhs, const BigRational& rhs) {
    // if ((lhs.Numerator / lhs.Denominator) < (rhs.Numerator / rhs.Denominator))
    //     return true;
    // else if ((lhs.Numerator / lhs.Denominator) > (rhs.Numerator / rhs.Denominator)){
    //     std::cout << lhs.first_sign_is_negative_RN << rhs.first_sign_is_negative_RN << std::endl;
    //     return false;
    // }
    // else if ((lhs.Numerator / lhs.Denominator) == (rhs.Numerator / rhs.Denominator)) {
    //     std::cout << lhs.first_sign_is_negative_RN << rhs.first_sign_is_negative_RN << std::endl;
    //     if (lhs.first_sign_is_negative_RN && !rhs.first_sign_is_negative_RN)
    //         return true;
    //     else if (!lhs.first_sign_is_negative_RN && rhs.first_sign_is_negative_RN)
    //         return false;
    //     else i
    //     } else {
    //         if 
    //     }
    // }
    //https://www.youtube.com/watch?v=uy-pGMtIJeY
    //std::cout << "lhs: " << lhs << " and rhs: " << rhs << "|"<< lhs.first_sign_is_negative_RN << " and " << rhs.first_sign_is_negative_RN << std::endl;
    return (lhs.Numerator * rhs.Denominator) < (rhs.Numerator * lhs.Denominator);
}
inline bool operator>(const BigRational& lhs, const BigRational& rhs) {
    if (lhs < rhs || lhs == rhs)
        return false;
    else
        return true;
}
inline bool operator<=(const BigRational& lhs, const BigRational& rhs) {
    if (lhs < rhs || lhs == rhs)
        return true;
    else
        return false;
}
inline bool operator>=(const BigRational& lhs, const BigRational& rhs) {
    if (lhs > rhs || lhs == rhs)
        return true;
    else
        return false;
}

inline std::ostream& operator<<(std::ostream& lhs, const BigRational& rhs) {
    if (rhs.Denominator == BigInteger(1)) lhs << rhs.Numerator;
    else lhs << rhs.Numerator << '/' << rhs.Denominator;

    return lhs;
}

#if SUPPORT_IFSTREAM == 1
// this should behave exactly the same as reading int with respect to 
// whitespace, consumed characters etc...
inline std::istream& operator>>(std::istream& lhs, BigRational& rhs); // bonus
#endif

#if SUPPORT_EVAL == 1
inline BigInteger eval(const std::string&);
#endif
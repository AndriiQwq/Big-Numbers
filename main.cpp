#include <iostream>
#include "Big_Numbers.hpp"

int main(){
    //std::string str = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";

    // BigInteger Big_Numbers();
    // std::cout << Big_Numbers << std::endl;

    BigInteger int_big_num1(-000100);
    //std::cout << int_big_num1 << '\n';

    BigInteger int_big_num2(543545);
    //std::cout << int_big_num2 << '\n';

    BigInteger big_num1("000435445454545454553455454554543545345354543534534543545");
    //std::cout << big_num1 << '\n';

    BigInteger big_num2("-0");

    BigInteger big_num3("-1");

    std::cout << big_num2 << "and " << big_num3 << '\n';

    //std::cout << big_num2 - big_num3 << '\n';
    std::cout << big_num2 - big_num3 << '\n';


    int_big_num2 = big_num1;
    // std::cout << int_big_num2 << '\n';

    //test out 
    //std::cout << int_big_num2 + big_num3 << '\n';
    // std::cout << int_big_num2 + '1' << '\n';
    // std::cout << int_big_num2 + 1 << '\n';
}
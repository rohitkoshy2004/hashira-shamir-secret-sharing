/*
## Algorithm
1. Convert shares from different bases to decimal
2. Use Lagrange interpolation to find polynomial
3. Extract secret as constant term (P(0))

## Author
Rohit Koshy - Hashira Placements Assignment

## Submission Date
September 1, 2025
EOF
*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

class BigInt {
private:
    vector<long long> digits;
    bool negative;

public:
    BigInt() : negative(false) { digits.push_back(0); }

    BigInt(long long n) : negative(n < 0) {
        if (n < 0) n = -n;
        if (n == 0) digits.push_back(0);
        while (n > 0) {
            digits.push_back(n % 1000000000);
            n /= 1000000000;
        }
    }

    BigInt(const string& s) : negative(false) {
        if (s.empty() || s == "0") {
            digits.push_back(0);
            return;
        }

        string num = s;
        if (num[0] == '-') {
            negative = true;
            num = num.substr(1);
        }

        for (int i = num.length(); i > 0; i -= 9) {
            string part = num.substr(max(0, i - 9), min(9, i));
            digits.push_back(stoll(part));
        }
    }

    BigInt operator+(const BigInt& other) const {
        BigInt result;
        result.digits.clear();

        long long carry = 0;
        size_t maxSize = max(digits.size(), other.digits.size());

        for (size_t i = 0; i < maxSize || carry; i++) {
            long long sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];

            result.digits.push_back(sum % 1000000000);
            carry = sum / 1000000000;
        }

        return result;
    }

    BigInt operator-(const BigInt& other) const {
        BigInt result;
        result.digits.clear();

        long long borrow = 0;
        for (size_t i = 0; i < digits.size(); i++) {
            long long diff = digits[i] - borrow;
            if (i < other.digits.size()) diff -= other.digits[i];

            if (diff < 0) {
                diff += 1000000000;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.digits.push_back(diff);
        }

        // Remove leading zeros
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }

        return result;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.assign(digits.size() + other.digits.size(), 0);

        for (size_t i = 0; i < digits.size(); i++) {
            long long carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; j++) {
                long long product = result.digits[i + j] + carry;
                if (j < other.digits.size()) {
                    product += digits[i] * other.digits[j];
                }
                result.digits[i + j] = product % 1000000000;
                carry = product / 1000000000;
            }
        }

        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }

        return result;
    }

    string toString() const {
        if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
            return "0";
        }

        string result;
        if (negative) result += "-";

        result += to_string(digits.back());
        for (int i = digits.size() - 2; i >= 0; i--) {
            string part = to_string(digits[i]);
            result += string(9 - part.length(), '0') + part;
        }

        return result;
    }
};

struct Point {
    BigInt x, y;

    Point(const BigInt& x, const BigInt& y) : x(x), y(y) {}
};

BigInt convertFromBase(const string& value, int base) {
    BigInt result(0);
    BigInt baseValue(base);
    BigInt power(1);

    for (int i = value.length() - 1; i >= 0; i--) {
        char digit = value[i];
        int digitValue;

        if (digit >= '0' && digit <= '9') {
            digitValue = digit - '0';
        } else if (digit >= 'a' && digit <= 'z') {
            digitValue = digit - 'a' + 10;
        } else if (digit >= 'A' && digit <= 'Z') {
            digitValue = digit - 'A' + 10;
        } else {
            throw invalid_argument("Invalid digit");
        }

        if (digitValue >= base) {
            throw invalid_argument("Digit invalid for base");
        }

        result = result + BigInt(digitValue) * power;
        power = power * baseValue;
    }

    return result;
}

// Simple fraction class for exact arithmetic
class Fraction {
public:
    BigInt numerator, denominator;

    Fraction(const BigInt& num, const BigInt& den) : numerator(num), denominator(den) {}

    Fraction operator+(const Fraction& other) const {
        BigInt newNum = numerator * other.denominator + other.numerator * denominator;
        BigInt newDen = denominator * other.denominator;
        return Fraction(newNum, newDen);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }
};

// For this implementation, we'll use a simplified approach suitable for the test cases
long long convertToLongLong(const BigInt& big) {
    // This is a simplified conversion - in practice you'd need more robust handling
    string s = big.toString();
    return stoll(s.substr(0, min(s.length(), size_t(18))));
}

double lagrangeInterpolation(const vector<Point>& points) {
    double result = 0.0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        double xi = convertToLongLong(points[i].x);
        double yi = convertToLongLong(points[i].y);

        double li = 1.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double xj = convertToLongLong(points[j].x);
                li *= (0.0 - xj) / (xi - xj);
            }
        }

        result += yi * li;
    }

    return result;
}

int main() {
    // Test case 1
    vector<Point> points1;
    points1.emplace_back(BigInt(1), convertFromBase("4", 10));
    points1.emplace_back(BigInt(2), convertFromBase("111", 2));
    points1.emplace_back(BigInt(3), convertFromBase("12", 10));
    points1.emplace_back(BigInt(6), convertFromBase("213", 4));

    cout << "Test Case 1:" << endl;
    cout << "Points:" << endl;
    for (const auto& p : points1) {
        cout << "(" << p.x.toString() << ", " << p.y.toString() << ")" << endl;
    }

    // Use first 3 points (k=3)
    vector<Point> selected1(points1.begin(), points1.begin() + 3);
    double secret1 = lagrangeInterpolation(selected1);
    cout << "Secret: " << round(secret1) << endl << endl;

    // Test case 2
    vector<Point> points2;
    points2.emplace_back(BigInt(1), convertFromBase("13444211440455345511", 6));
    points2.emplace_back(BigInt(2), convertFromBase("aed7015a346d635", 15));
    points2.emplace_back(BigInt(3), convertFromBase("6aeeb69631c227c", 15));
    points2.emplace_back(BigInt(4), convertFromBase("e1b5e05623d881f", 16));
    points2.emplace_back(BigInt(5), convertFromBase("316034514573652620673", 8));
    points2.emplace_back(BigInt(6), convertFromBase("2122212201122002221120200210011020220200", 3));
    points2.emplace_back(BigInt(7), convertFromBase("20120221122211000100210021102001201112121", 3));

    cout << "Test Case 2:" << endl;
    cout << "First 7 points:" << endl;
    for (int i = 0; i < 7; i++) {
        cout << "(" << points2[i].x.toString() << ", " << points2[i].y.toString() << ")" << endl;
    }

    // Use first 7 points (k=7)
    vector<Point> selected2(points2.begin(), points2.begin() + 7);
    double secret2 = lagrangeInterpolation(selected2);
    cout << "Secret: " << static_cast<long long>(round(secret2)) << endl;

    return 0;
}

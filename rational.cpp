#include "rational.h"
#include <numeric>

template <typename T>
void Relax(T& numer, T& denom) {
    if (denom == 0) {
        throw RationalDivisionByZero();
    }
    T g = std::gcd(numer, denom);
    numer /= g;
    denom /= g;
    if (denom < 0) {
        numer *= -1;
        denom *= -1;
    }
}

Rational::Rational() : numer_(0), denom_(1) {
}

Rational::Rational(int32_t value) : numer_(value), denom_(1) {
}

Rational::Rational(int32_t numer, int32_t denom) {
    Relax(numer, denom);
    numer_ = numer;
    denom_ = denom;
}

int32_t Rational::GetNumerator() const {
    return numer_;
}

int32_t Rational::GetDenominator() const {
    return denom_;
}

void Rational::SetNumerator(int32_t value) {
    numer_ = value;
    Relax(numer_, denom_);
}

void Rational::SetDenominator(int32_t value) {
    if (value == 0) {
        throw RationalDivisionByZero();
    }
    denom_ = value;
    Relax(numer_, denom_);
}

void Rational::Set(int64_t numer, int64_t denom) {
    Relax(numer, denom);
    numer_ = static_cast<int32_t>(numer);
    denom_ = static_cast<int32_t>(denom);
}

Rational& operator+=(Rational& lhs, const Rational& rhs) {
    lhs.Set(static_cast<int64_t>(lhs.numer_) * rhs.denom_ + static_cast<int64_t>(rhs.numer_) * lhs.denom_,
            static_cast<int64_t>(lhs.denom_) * rhs.denom_);
    return lhs;
}

Rational& operator*=(Rational& lhs, const Rational& rhs) {
    lhs.Set(static_cast<int64_t>(lhs.numer_) * rhs.numer_, static_cast<int64_t>(lhs.denom_) * rhs.denom_);
    return lhs;
}

Rational operator+(const Rational& ratio) {
    return ratio;
}

Rational operator-(const Rational& ratio) {
    return Rational(-ratio.GetNumerator(), ratio.GetDenominator());
}

Rational& operator-=(Rational& lhs, const Rational& rhs) {
    lhs += -rhs;
    return lhs;
}

Rational& operator/=(Rational& lhs, const Rational& rhs) {
    Rational reversed(rhs.GetDenominator(), rhs.GetNumerator());
    return lhs *= reversed;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res += rhs;
    return res;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res -= rhs;
    return res;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res *= rhs;
    return res;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res /= rhs;
    return res;
}
Rational& operator++(Rational& ratio) {
    ratio.numer_ += ratio.denom_;
    return ratio;
}

Rational& operator--(Rational& ratio) {
    ratio.numer_ -= ratio.denom_;
    return ratio;
}

Rational operator++(Rational& ratio, int) {
    Rational res = ratio;
    ++ratio;
    return res;
}

Rational operator--(Rational& ratio, int) {
    Rational res = ratio;
    --ratio;
    return res;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    int32_t g = std::gcd(lhs.GetNumerator(), rhs.GetDenominator());
    int32_t g2 = std::gcd(rhs.GetNumerator(), lhs.GetDenominator());
    return static_cast<int64_t>(lhs.GetNumerator() / g) * rhs.GetDenominator() <
           static_cast<int64_t>(rhs.GetNumerator() / g2) * lhs.GetDenominator();
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return rhs.GetDenominator() == lhs.GetDenominator() && rhs.GetNumerator() == lhs.GetNumerator();
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    return rhs <= lhs;
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}
std::ostream& operator<<(std::ostream& os, const Rational& ratio) {
    if (ratio.GetDenominator() == 1) {
        os << ratio.GetNumerator();
    } else {
        os << ratio.GetNumerator() << "/" << ratio.GetDenominator();
    }
    return os;
}

std::istream& operator>>(std::istream& is, Rational& ratio) {
    std::string inp;
    is >> inp;
    size_t p = std::find(inp.begin(), inp.end(), '/') - inp.begin();
    if (p == inp.size()) {
        ratio.SetNumerator(std::stoi(inp));
    } else {
        ratio.SetNumerator(std::stoi(inp.substr(0, p)));
        ratio.SetDenominator(std::stoi(inp.substr(p + 1)));
    }
    return is;
}

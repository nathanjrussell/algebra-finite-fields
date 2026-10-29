#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <algebra/finite_fields/prime_field.hpp>
#include <algebra/polynomials/polynomial.hpp>

namespace algebra {

template<std::uint32_t P>
Polynomial<PrimeFieldElement<P>> next_polynomial(Polynomial<PrimeFieldElement<P>> value) {
    using Coefficient = PrimeFieldElement<P>;
    for (std::size_t i = 0;; ++i) {
        const Coefficient next = value.coefficient(i).add(Coefficient::one());
        value.set_coefficient(i, next);
        if (!next.is_zero()) {
            return value;
        }
    }
}

template<std::uint32_t P>
bool is_irreducible(const Polynomial<PrimeFieldElement<P>>& polynomial) {
    using Coefficient = PrimeFieldElement<P>;
    using Poly = Polynomial<Coefficient>;
    if (polynomial.is_zero() || *polynomial.degree() == 0) {
        return false;
    }
    const auto degree = *polynomial.degree();
    if (degree == 1) {
        return true;
    }
    Coefficient argument = Coefficient::zero();
    for (std::uint64_t count = 0; count < P; ++count) {
        if (polynomial.evaluate(argument).is_zero()) {
            return false;
        }
        argument = argument.add(Coefficient::one());
    }
    for (std::size_t factor_degree = 2; factor_degree <= degree / 2; ++factor_degree) {
        const auto count = checked_power(P, factor_degree);
        for (std::uint64_t index = 0; index < count; ++index) {
            Poly divisor;
            divisor.set_coefficient(factor_degree, Coefficient::one());
            std::uint64_t digits = index;
            for (std::size_t i = 0; i < factor_degree; ++i) {
                divisor.set_coefficient(i, Coefficient::from_index(digits % P));
                digits /= P;
            }
            if ((polynomial % divisor).is_zero()) {
                return false;
            }
        }
    }
    return true;
}

struct FirstEnumeratedIrreducible {
    template<std::uint32_t P, std::size_t K>
    static Polynomial<PrimeFieldElement<P>> polynomial() {
        static_assert(K > 0);
        using Coefficient = PrimeFieldElement<P>;
        using Poly = Polynomial<Coefficient>;
        Poly candidate = Poly::one();
        for (;;) {
            if (candidate.degree() == K && candidate.leading_coefficient().is_one() &&
                is_irreducible(candidate)) {
                return candidate;
            }
            if (candidate.degree() > K) {
                throw std::logic_error("No irreducible polynomial was found");
            }
            candidate = next_polynomial(std::move(candidate));
        }
    }
};

template<std::int64_t... Coefficients>
struct PolynomialModulus {
    template<std::uint32_t P, std::size_t K>
    static Polynomial<PrimeFieldElement<P>> polynomial() {
        using Coefficient = PrimeFieldElement<P>;
        return Polynomial<Coefficient>{Coefficient::from_integer(Coefficients)...};
    }
};

}

#include <iostream>
#include <njr/algebra/finite_fields/finite_fields.hpp>

using namespace njr::algebra;

int main() {
    using F = FiniteField<11, 3>;
    const auto alpha = F::alpha();
    std::cout << F::modulus() << '\n';
    std::cout << alpha.multiply(alpha).multiply(alpha) << '\n';
    std::cout << alpha.multiply(alpha.reciprocal()) << '\n';
    std::cout << discover_elements<F>().size() << '\n';
}

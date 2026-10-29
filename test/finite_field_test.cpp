#include <array>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <gtest/gtest.h>
#include <algebra/finite_fields/finite_fields.hpp>

using namespace algebra;

template<class A, class B>
concept CanAdd = requires(const A& a, const B& b) { a + b; };

using F = FiniteField<11, 3>;
using E = F::Element;
using Base = PrimeField<Cyclic<11>>;
using Coefficient = Base::Element;
using OtherF8 = FiniteField<2, 3, PolynomialModulus<1, 0, 1, 1>>;

static_assert(FieldElement<E>);
static_assert(!FieldElement<Polynomial<E>>);
static_assert(!GroupElement<E, Multiplication>);
static_assert(AbelianGroupElement<Nonzero<E>, Multiplication>);
static_assert(!CanAdd<E, FiniteField<11, 2>::Element>);
static_assert(!CanAdd<FiniteField<2, 3>::Element, OtherF8::Element>);

TEST(PrimeFields, SatisfiesFieldLawsAndComputesEveryInverse) {
    for (int a = 0; a < 11; ++a) {
        const auto x = Base::element(a);
        EXPECT_TRUE((x + -x).is_zero());
        if (a != 0) {
            EXPECT_TRUE((x * x.reciprocal()).is_one());
        }
        for (int b = 0; b < 11; ++b) {
            const auto y = Base::element(b);
            for (int c = 0; c < 11; ++c) {
                EXPECT_TRUE(distributive_law(x, y, Base::element(c)));
            }
        }
    }
    EXPECT_THROW(Base::zero().reciprocal(), std::domain_error);
    EXPECT_EQ(Base::element(-1).representative(), 10u);
}

TEST(PolynomialEnumeration, CarriesAcrossCoefficients) {
    using Poly = Polynomial<Coefficient>;
    EXPECT_EQ(next_polynomial(Poly{Coefficient::from_integer(10)}), Poly::variable());
    const Poly next = next_polynomial(Poly{Coefficient::from_integer(10), Coefficient::from_integer(10)});
    EXPECT_EQ(next, Poly::variable() * Poly::variable());
}

TEST(Moduli, SelectsTheFirstIrreducibleCubicInBaseElevenOrder) {
    const auto& modulus = F::modulus();
    EXPECT_EQ(modulus, (F::PolynomialType{Coefficient::from_integer(4), Coefficient::one(),
                                        Coefficient::zero(), Coefficient::one()}));
    for (int constant = 0; constant < 4; ++constant) {
        const F::PolynomialType previous{Coefficient::from_integer(constant), Coefficient::one(),
                                         Coefficient::zero(), Coefficient::one()};
        EXPECT_FALSE(is_irreducible(previous));
    }
    EXPECT_TRUE(is_irreducible(modulus));
}

TEST(Moduli, DetectsRootlessReducibleQuartics) {
    using C3 = PrimeFieldElement<3>;
    using Poly = Polynomial<C3>;
    const auto factor = Poly::variable() * Poly::variable() + Poly::one();
    EXPECT_TRUE(is_irreducible(factor));
    EXPECT_FALSE(is_irreducible(factor * factor));
}

TEST(Moduli, ValidatesExplicitDefiningPolynomials) {
    using Good = FiniteField<11, 3, PolynomialModulus<4, 1, 0, 1>>;
    using Reducible = FiniteField<11, 3, PolynomialModulus<1, 0, 0, 1>>;
    using WrongDegree = FiniteField<11, 3, PolynomialModulus<1, 0, 1>>;
    using Nonmonic = FiniteField<11, 3, PolynomialModulus<4, 1, 0, 2>>;
    EXPECT_EQ(Good::modulus(), F::modulus());
    EXPECT_THROW(Reducible::initialize(), std::invalid_argument);
    EXPECT_THROW(WrongDegree::initialize(), std::invalid_argument);
    EXPECT_THROW(Nonmonic::initialize(), std::invalid_argument);
}

TEST(ExtensionFields, ReducesPolynomialProducts) {
    const auto alpha = F::alpha();
    EXPECT_EQ(alpha * alpha * alpha, F::element({7, 10}));
    const auto result = (alpha * alpha + E::one()) * (alpha + E::from_integer(2));
    EXPECT_EQ(result, F::element({9, 0, 2}));
    EXPECT_TRUE(E::one().is_one());
    EXPECT_FALSE(F::element({1, 1}).is_one());
}

TEST(ExtensionFields, SeparatesIntegerEmbeddingFromEnumerationLabels) {
    EXPECT_TRUE(E::from_integer(11).is_zero());
    EXPECT_EQ(E::from_index(11), F::alpha());
    EXPECT_THROW(E::from_index(1331), std::out_of_range);
    EXPECT_THROW(E::one().coefficient(3), std::out_of_range);
    EXPECT_EQ(F::cardinality(), 1331u);
    EXPECT_THROW((FiniteField<2, 64>::cardinality()), std::overflow_error);
}

TEST(ExtensionFields, InvertsEveryNonzeroElementWithoutTables) {
    for (std::uint64_t i = 1; i < F::cardinality(); ++i) {
        const auto value = E::from_index(i);
        const auto inverse = value.reciprocal();
        ASSERT_TRUE(value.multiply(inverse).is_one()) << i;
        ASSERT_TRUE(inverse.multiply(value).is_one()) << i;
    }
    EXPECT_THROW(E::zero().reciprocal(), std::domain_error);
}

TEST(ExtensionFields, GeneratesAllElementsThroughAdditionAndMultiplication) {
    const auto elements = discover_elements<F>();
    std::set<std::array<std::uint32_t, 3>> coefficients;
    bool found_zero = false;
    for (const auto& value : elements) {
        const auto c = value.coefficients();
        coefficients.insert({c[0].representative(), c[1].representative(), c[2].representative()});
        found_zero = found_zero || value.is_zero();
    }
    EXPECT_EQ(elements.size(), 1331u);
    EXPECT_EQ(coefficients.size(), 1331u);
    EXPECT_TRUE(elements.front().is_one());
    EXPECT_TRUE(found_zero);
}

TEST(ExtensionFields, ChecksSmallFieldsExhaustively) {
    using Small = FiniteField<2, 3>;
    const auto elements = discover_elements<Small>();
    for (const auto& a : elements) {
        for (const auto& b : elements) {
            for (const auto& c : elements) {
                EXPECT_TRUE(distributive_law(a, b, c));
                EXPECT_TRUE((associative_law<Small::Element, Multiplication>(a, b, c)));
            }
        }
    }
    using Quartic = FiniteField<2, 4>;
    for (std::uint64_t i = 1; i < 16; ++i) {
        const auto value = Quartic::Element::from_index(i);
        EXPECT_TRUE((value * value.reciprocal()).is_one());
    }
}

TEST(ExtensionFields, SupportsDegreeOneAndVirtualFieldOperations) {
    using DegreeOne = FiniteField<5>;
    EXPECT_EQ(DegreeOne::modulus(), DegreeOne::PolynomialType::variable());
    const auto two = DegreeOne::element(2);
    EXPECT_TRUE((two * two.reciprocal()).is_one());
    F field;
    const interfaces::Field<E>& view = field;
    EXPECT_TRUE(view.multiply(F::alpha(), view.reciprocal(F::alpha())).is_one());
}

#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <algebra/core/core.hpp>
#include <algebra/rings/residue_ring.hpp>

namespace algebra {

template<class AdditiveGroup>
class PrimeField;

template<std::uint32_t P>
class PrimeFieldElement : public AdditiveOperators<PrimeFieldElement<P>>,
                          public MultiplicativeOperators<PrimeFieldElement<P>>,
                          public DivisionOperators<PrimeFieldElement<P>> {
    static_assert(is_prime(P));
    using ResidueValue = typename Residue<P>::Element;
    ResidueValue value_;

    explicit PrimeFieldElement(ResidueValue value) : value_(value) {}

public:
    using parent_type = PrimeField<Cyclic<P>>;
    using algebra = FieldDescriptor;

    PrimeFieldElement() = default;

    static PrimeFieldElement from_integer(std::int64_t value) {
        return PrimeFieldElement(ResidueValue::from_integer(value));
    }

    static PrimeFieldElement from_index(std::uint64_t value) {
        if (value >= P) {
            throw std::out_of_range("Prime field element index is out of range");
        }
        return from_integer(static_cast<std::int64_t>(value));
    }

    static PrimeFieldElement zero() {
        return PrimeFieldElement{};
    }

    static PrimeFieldElement one() {
        return from_integer(1);
    }

    PrimeFieldElement add(const PrimeFieldElement& other) const {
        return PrimeFieldElement(value_.add(other.value_));
    }

    PrimeFieldElement multiply(const PrimeFieldElement& other) const {
        return PrimeFieldElement(value_.multiply(other.value_));
    }

    PrimeFieldElement negate() const {
        return PrimeFieldElement(value_.negate());
    }

    PrimeFieldElement reciprocal() const {
        if (is_zero()) {
            throw std::domain_error("Zero has no reciprocal");
        }
        PrimeFieldElement candidate = one();
        for (std::uint64_t count = 1; count < P; ++count) {
            if (multiply(candidate).is_one()) {
                return candidate;
            }
            candidate = candidate.add(one());
        }
        throw std::logic_error("A prime field inverse was not found");
    }

    bool is_zero() const {
        return value_.is_zero();
    }

    bool is_one() const {
        return value_.is_one();
    }

    std::uint32_t representative() const {
        return value_.representative();
    }

    const parent_type& parent() const {
        static const parent_type instance;
        return instance;
    }

    bool operator==(const PrimeFieldElement& other) const {
        return value_ == other.value_;
    }

    friend std::ostream& operator<<(std::ostream& out, const PrimeFieldElement& value) {
        return out << value.representative();
    }
};

template<std::uint32_t P>
class PrimeField<Cyclic<P>> : public FieldStructure<PrimeFieldElement<P>> {
    static_assert(is_prime(P));

public:
    using Element = PrimeFieldElement<P>;

    static Element element(std::int64_t value) {
        return Element::from_integer(value);
    }

    static Element zero() {
        return Element::zero();
    }

    static Element one() {
        return Element::one();
    }

    static constexpr std::uint64_t cardinality() {
        return P;
    }
};

}

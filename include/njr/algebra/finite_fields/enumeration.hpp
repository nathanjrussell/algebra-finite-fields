#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <njr/algebra/finite_fields/finite_field.hpp>

namespace njr::algebra {

template<class Field>
    requires FieldElement<typename Field::Element>
std::vector<typename Field::Element> discover_elements() {
    using E = typename Field::Element;
    const E one = E::one();
    const E alpha = Field::alpha();
    const auto count = Field::cardinality();
    std::vector<E> found;
    if (count > found.max_size()) {
        throw std::length_error("The field is too large to enumerate in memory");
    }
    found.reserve(static_cast<std::size_t>(count));
    found.push_back(one);
    auto append = [&](const E& candidate) {
        for (const E& existing : found) {
            if (existing == candidate) {
                return;
            }
        }
        found.push_back(candidate);
    };
    for (std::size_t i = 0; i < found.size(); ++i) {
        const E current = found[i];
        append(current.add(one));
        append(current.multiply(alpha));
    }
    if (found.size() != count) {
        throw std::logic_error("The generated elements do not cover the field");
    }
    return found;
}

}

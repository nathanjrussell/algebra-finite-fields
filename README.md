# Algebra finite fields

Prime and extension fields using polynomial modular arithmetic.

```cmake
include(FetchContent)

FetchContent_Declare(
    algebra_finite_fields
    GIT_REPOSITORY git@github.com:nathanjrussell/algebra-finite-fields.git
    GIT_TAG v0.2.0
)

FetchContent_MakeAvailable(algebra_finite_fields)

target_link_libraries(my_app PRIVATE algebra::finite_fields)
```

# Algebra finite fields

Prime and extension fields using polynomial modular arithmetic.

```cmake
include(FetchContent)

FetchContent_Declare(
    njr_algebra_finite_fields
    GIT_REPOSITORY git@github.com:nathanjrussell/algebra-finite-fields.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(njr_algebra_finite_fields)

target_link_libraries(my_app PRIVATE njr::algebra_finite_fields)
```

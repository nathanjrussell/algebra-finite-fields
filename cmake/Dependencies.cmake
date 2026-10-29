include(FetchContent)
if(NOT TARGET algebra::rings)
    FetchContent_Declare(
        algebra_rings
        GIT_REPOSITORY git@github.com:nathanjrussell/algebra-rings.git
        GIT_TAG 2a11ac8b8ea89d9ccf2ce551a8512dde213fde50
    )
    FetchContent_MakeAvailable(algebra_rings)
endif()
if(NOT TARGET algebra::polynomials)
    FetchContent_Declare(
        algebra_polynomials
        GIT_REPOSITORY git@github.com:nathanjrussell/algebra-polynomials.git
        GIT_TAG 068f53f336d9ee9067f4fc419bc1dc6b83f4618f
    )
    FetchContent_MakeAvailable(algebra_polynomials)
endif()

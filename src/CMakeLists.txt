add_library(algebra_finite_fields INTERFACE)
add_library(algebra::finite_fields ALIAS algebra_finite_fields)
target_compile_features(algebra_finite_fields INTERFACE cxx_std_20)
target_include_directories(algebra_finite_fields INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(algebra_finite_fields INTERFACE
    algebra::rings
    algebra::polynomials
)

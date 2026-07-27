add_library(njr_algebra_finite_fields INTERFACE)
add_library(njr::algebra_finite_fields ALIAS njr_algebra_finite_fields)
target_compile_features(njr_algebra_finite_fields INTERFACE cxx_std_20)
target_include_directories(njr_algebra_finite_fields INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(njr_algebra_finite_fields INTERFACE
    njr::algebra_rings
    njr::algebra_polynomials
)

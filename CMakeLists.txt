cmake_minimum_required (VERSION 3.31)

project (MatrixLibrary VERSION 1.0.0
  DESCRIPTION "Matrix Library with Linear Algebra Features"
  LANGUAGES CXX)

option(MATRIXLIBRARY_BUILD_TESTS "Build unit tests" OFF)
option(MATRIXLIBRARY_BUILD_BENCHMARKS "Build benchmarks" OFF)

# Compiler requirements
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

# Find libraries
find_package(nlohmann_json REQUIRED)
find_package(Armadillo REQUIRED)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

# Library
add_library(MatrixLibrary)

add_library(MatrixLibrary::MatrixLibrary ALIAS MatrixLibrary)

target_compile_features(MatrixLibrary PUBLIC cxx_std_17)

target_sources(
  MatrixLibrary
  PRIVATE src/matrix_utilfuncs.cpp
          src/matrix_basic_func.cpp
          src/matrix_operators.cpp
          src/matrix_eigendecomp.cpp
          src/helper_func.cpp
)

target_include_directories(
  MatrixLibrary
  PRIVATE src
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

find_package(HighFive REQUIRED)

target_include_directories(MatrixLibrary PUBLIC ${HDF5_INCLUDE_DIRS})

target_link_libraries(MatrixLibrary 
  PUBLIC
    Eigen3::Eigen
    armadillo
    nlohmann_json::nlohmann_json
    BLAS::BLAS
    LAPACK::LAPACK
    HighFive::HighFive
    ${HDF5_C_LIBRARIES}
    ${HDF5_CXX_LIBRARIES}
)

add_compile_options(-Wa,--gsframe=no) # suppresses a useless warning on x86 machines


###########################
# TESTING AND BENCHMARKING
###########################

if (MATRIXLIBRARY_BUILD_TESTS)
  include(FetchContent)

  # GoogleTest
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)

  add_executable(matrix_tests
    test/test_matrix_arithmetic.cpp
    test/test_matrix_basics.cpp
    test/test_matrix_eigsym.cpp
  )

  target_link_libraries(matrix_tests PRIVATE MatrixLibrary GTest::gtest_main)

  enable_testing()

  include(GoogleTest)
  gtest_discover_tests(matrix_tests)

  # eigsym accuracy data generator (NOT a unit test)
  add_executable(bench_eigsym_accuracy
    test/benchmark_eigsym_accuracy.cpp
  )

  target_link_libraries(bench_eigsym_accuracy PRIVATE MatrixLibrary)

  target_include_directories(bench_eigsym_accuracy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test
  )
endif()


if (MATRIXLIBRARY_BUILD_BENCHMARKS)
  # Google Benchmark
  include(FetchContent)
  find_package(Threads REQUIRED)

  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.zip
  )
  FetchContent_MakeAvailable(googlebenchmark)

  add_executable(matrix_benchmarks
    benchmarking/benchmark_main.cpp
    benchmarking/benchmark_addition.cpp
    benchmarking/benchmark_multiplication.cpp
    benchmarking/benchmark_initialization.cpp
    benchmarking/benchmark_transpose.cpp
    benchmarking/benchmark_accessor.cpp
    benchmarking/benchmark_eigsym.cpp
  )

  target_link_libraries(matrix_benchmarks 
    PRIVATE MatrixLibrary 
    benchmark::benchmark
    Threads::Threads)
endif()

##########################
# BEGIN INSTALL SECTION
##########################

install(TARGETS MatrixLibrary
				EXPORT MatrixLibraryTargets
				LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
				ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
				RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
				INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/MatrixLibraryConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/MatrixLibraryConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/MatrixLibrary
)
	
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/MatrixLibraryConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/MatrixLibraryConfig.cmake
				${CMAKE_CURRENT_BINARY_DIR}/MatrixLibraryConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/MatrixLibrary
)
if(EXISTS "C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/bin/Debug/sparse_tests.exe")
  if(NOT EXISTS "C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test/sparse_tests[1]_tests-Debug.cmake" OR
     NOT "C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test/sparse_tests[1]_tests-Debug.cmake" IS_NEWER_THAN "C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/bin/Debug/sparse_tests.exe" OR
     NOT "C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test/sparse_tests[1]_tests-Debug.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.31/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/bin/Debug/sparse_tests.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[sparse_tests_TESTS]==]
      CTEST_FILE [==[C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test/sparse_tests[1]_tests-Debug.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/Users/User/source/repos/kayanut26/mp2-lab2-sparse-matrix/build/test/sparse_tests[1]_tests-Debug.cmake")
else()
  add_test(sparse_tests_NOT_BUILT sparse_tests_NOT_BUILT)
endif()

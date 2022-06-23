---
author: 胡森
date: 2022/05/25
email: hscivitasv@gmail.com
---

This module defines functions to help use the Google Test infrastructure.
该模块主要提供了两个命令：*gtest_add_tests()* 和 *gtest_discover_tests*。

## gtest_add_tests

```cmake
gtest_add_tests(TARGET target
                [SOURCES src1...]
                [EXTRA_ARGS arg1...]
                [WORKING_DIRECTORY dir]
                [TEST_PREFIX prefix]
                [TEST_SUFFIX suffix]
                [SKIP_DEPENDENCY]
                [TEST_LIST outVar]
)
```

Test discovery occurs at configure time.

`TARGET target`: Specifies the Google Test executable.

`SOURCES src1...`: When provided, only the listed files will be scanned for test cases. If this option is not given, the [`SOURCES`](https://cmake.org/cmake/help/git-stage/prop_tgt/SOURCES.html#prop_tgt:SOURCES "SOURCES") property of the specified `target` will be used to obtain the list of sources.

`EXTRA_ARGS arg1...`: Any extra arguments to pass on the command line to each test case.

`WORKING_DIRECTORY dir`: Specifies the directory in which to run the discovered test cases. If this option is not provided, the current binary directory is used.

`TEST_PREFIX prefix`: Specifies a `prefix` to be prepended to the name of each discovered test case. This can be useful when the same source files are being used in multiple calls to `gtest_add_test()` but with different `EXTRA_ARGS`.

`TEST_SUFFIX suffix`: Similar to `TEST_PREFIX` except the `suffix` is appended to the name of every discovered test case. Both `TEST_PREFIX` and `TEST_SUFFIX` may be specified.

`SKIP_DEPENDENCY`: Normally, the function creates a dependency which will cause CMake to be re-run if any of the sources being scanned are changed. This is to ensure that the list of discovered tests is updated. If this behavior is not desired (as may be the case while actually writing the test cases), this option can be used to prevent the dependency from being added.

`TEST_LIST outVar`: The variable named by `outVar` will be populated in the calling scope with the list of discovered test cases. This allows the caller to do things like manipulate test properties of the discovered tests.

## gtest_discover_tests

```cmake
gtest_discover_tests(target
                     [EXTRA_ARGS arg1...]
                     [WORKING_DIRECTORY dir]
                     [TEST_PREFIX prefix]
                     [TEST_SUFFIX suffix]
                     [TEST_FILTER expr]
                     [NO_PRETTY_TYPES] [NO_PRETTY_VALUES]
                     [PROPERTIES name1 value1...]
                     [TEST_LIST var]
                     [DISCOVERY_TIMEOUT seconds]
                     [XML_OUTPUT_DIR dir]
                     [DISCOVERY_MODE <POST_BUILD|PRE_TEST>]
)
```

Test discovery occurs at build time.

`target`: Specifies the Google Test executable, which must be a known CMake executable target. CMake will substitute the location of the built executable when running the test.

`EXTRA_ARGS arg1...`: Any extra arguments to pass on the command line to each test case.

`WORKING_DIRECTORY dir`: Specifies the directory in which to run the discovered test cases. If this option is not provided, the current binary directory is used.

`TEST_PREFIX prefix`: Specifies a `prefix` to be prepended to the name of each discovered test case. This can be useful when the same test executable is being used in multiple calls to `gtest_discover_tests()` but with different `EXTRA_ARGS`.

`TEST_SUFFIX suffix`: Similar to `TEST_PREFIX` except the `suffix` is appended to the name of every discovered test case. Both `TEST_PREFIX` and `TEST_SUFFIX` may be specified.

More options see [googletest](https://cmake.org/cmake/help/git-stage/module/GoogleTest.html).

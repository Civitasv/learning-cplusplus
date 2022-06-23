---
author: 胡森
date: 2022/05/25
email: hscivitasv@gmail.com
---

An assertion's result can be *success*, *nonfatal failure*, or *fatal failure*.
A *test suite* contains one or many tests.
A *test program* contains one or many test suites.

`ASSERT_*` versions generate fatal failures when they fail, and **abort the current function**.
`EXPECT_*` versions generate nonfatal failures, which don’t abort the current function. 

## Custom Failure Message

```c++
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```

## Simple Tests

To create a test:

1. Use the `TEST` macro to define and name a test function.
2. In this function, along with any valid C++ statements you want to include, use the various googletest assertions to check values.

Format:

```c++
TEST(TESTSuiteName, TestName){
	... test body ...
}
```

The *first* argument is the name of the test suite, and the *second* argument is the test's name within the test suite. Both names must be valid C++ identifiers, and they **should not** contain any underscores(\_).

When naming your test suites and tests, you should follow the same convention as for [naming functions and classes](https://google.github.io/styleguide/cppguide.html#Function_Names).

## Test Fixtures

If you find yourself writing two or more tests that operate on similar data, you can use a _test fixture_. This allows you to reuse the same configuration of objects for several different tests.

To create a fixture:

1.  Derive a class from `::testing::Test` . Start its body with `protected:`, as we’ll want to access fixture members from sub-classes.
2.  Inside the class, declare any objects you plan to use.
3.  If necessary, write a default constructor or `SetUp()` function to prepare the objects for each test. A common mistake is to spell `SetUp()` as **`Setup()`** with a small `u` - Use `override` in C++11 to make sure you spelled it correctly.
4.  If necessary, write a destructor or `TearDown()` function to release any resources you allocated in `SetUp()` . To learn when you should use the constructor/destructor and when you should use `SetUp()/TearDown()`, read the [FAQ](https://google.github.io/googletest/faq.html#CtorVsSetUp).
5.  If needed, define subroutines for your tests to share.

When using a fixture, use `TEST_F()` instead of `TEST()` as it allows you to access objects and subroutines in the test fixture:

```c++
TEST_F(TestFixtureName, TestName) {
  ... test body ...
}
```

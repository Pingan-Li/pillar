/**
 * @file functor_traits_tests.cc
 * @author lipingan (lipingan.dev@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pillar/thread/functor_traits.h"

#include "gtest/gtest.h"
#include <cstddef>
#include <functional>
#include <type_traits>

namespace pillar {

template <typename Functor, typename... Args>
typename FunctorTraits<Functor>::ResultType Call(Functor &&functor,
                                                 Args &&...args) {
  return std::invoke(std::forward<Functor>(functor),
                     std::forward<Args>(args)...);
}

// function test cases.

int function0(int a, int b) { return a + b; }

TEST(FunctorTraits, PlainFunction_0) {
  // plain function.
  ASSERT_TRUE((std::is_same<FunctorTraits<int()>::ResultType, int>::value));
  auto result = Call(function0, 1, 2);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
  ASSERT_EQ(result, 3);
}

int const &function1(int a, int b, int &c) {
  c = a + b;
  return c;
}

TEST(FuncotrTraits, PlainFunction_1) {
  // plain function with int return type
  ASSERT_TRUE((std::is_same<FunctorTraits<int const &()>::ResultType,
                            int const &>::value));
  int a = 1;
  int b = 1;
  int c = 0;
  auto &d = Call(function1, a, b, c);
  ASSERT_EQ(d, 2);
  ASSERT_TRUE((std::is_same<decltype(d), int const &>::value));
}

int const *function2() { return nullptr; }

TEST(FuncotrTraits, PlainFunction_2) {
  // plain function with int const * return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int const *()>::ResultType,
                            int const *>::value));
  auto result = Call(function2);
  ASSERT_EQ(result, nullptr);
  ASSERT_TRUE((std::is_same<decltype(result), int const *>::value));
}

TEST(FuncotrTraits, PlainFunction_3) {
  // plain function with int const & return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int const &()>::ResultType,
                            int const &>::value));
}

TEST(FuncotrTraits, PlainFunction_4) {
  // plain function with int && return type.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<int && ()>::ResultType, int &&>::value));
}

TEST(FuncotrTraits, PlainFunction_5) {
  // plain function with int const && return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int const && ()>::ResultType,
                            int const &&>::value));
}

TEST(FunctorTraits, PointerToFunction_0) {
  // pointer to function.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<void (*)()>::ResultType, void>::value));
}
void *PointerToFunction_1() { return nullptr; }
TEST(FunctorTraits, PointerToFunction_1) {
  // pointer to function with void * return type.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<void *(*)()>::ResultType, void *>::value));
  auto result = Call(&PointerToFunction_1);
  ASSERT_TRUE((std::is_same<decltype(result), void *>::value));
  ASSERT_EQ(result, nullptr);
}

int *PointerToFunction_2() { return nullptr; }
TEST(FuncotrTraits, PointerToFunction_2) {
  // pointer to a function with int return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int (*)()>::ResultType, int>::value));

  auto result = Call(&PointerToFunction_2);
  ASSERT_TRUE((std::is_same<decltype(result), int *>::value));
  ASSERT_EQ(result, nullptr);
}

TEST(FuncotrTraits, LvalueReferenceToFunction_0) {
  // lvalue reference to function.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<void (&)()>::ResultType, void>::value));
}

int LvalueReferenceToFunction_1() { return 1; }
TEST(FuncotrTraits, LvalueReferenceToFunction_1) {
  // lvalue reference to a function with int return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int (&)()>::ResultType, int>::value));
  int (&ref)() = LvalueReferenceToFunction_1;
  auto result = Call(&ref);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
  ASSERT_EQ(result, 1);
}

TEST(FuncotrTraits, RvalueReferenceToFunction_0) {
  // rvalue reference to function.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<void(&&)()>::ResultType, void>::value));
}

int RvalueReferenceToFunction_1() { return 1; }
TEST(FuncotrTraits, RvalueReferenceToFunction_1) {
  // rvalue reference to a function with int return type.
  ASSERT_TRUE((std::is_same<FunctorTraits<int(&&)()>::ResultType, int>::value));
  int(&&ref)() = RvalueReferenceToFunction_1;
  auto result = Call(ref);
  ASSERT_EQ(result, 1);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
}

class Clz {
public:
  static int StaticMethod(int a, int b) { return a + b; }
  int Method(int a, int b) { return a + b; }
};

TEST(FuncotrTraits, MemberFunction_0) {
  // static memeber functiion.
  ASSERT_TRUE((std::is_same<FunctorTraits<int (Clz::*)(int, int)>::ResultType,
                            int>::value));
  auto result = Call(Clz::StaticMethod, 1, 2);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
  ASSERT_EQ(result, 3);
}

TEST(FuncotrTraits, MemberFunction_1) {
  // static memeber functiion.
  ASSERT_TRUE((std::is_same<FunctorTraits<int &(Clz::*)(int, int)>::ResultType,
                            int &>::value));
  Clz obj;
  auto result = Call(&Clz::Method, &obj, 4, 2);
  ASSERT_EQ(result, 6);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
}

TEST(FuncotrTraits, MemberFunction_2) {
  // static memeber functiion.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<int && (Clz::*)(int, int)>::ResultType,
                    int &&>::value));
}

TEST(FuncotrTraits, MemberFunction_3) {
  // static memeber functiion.
  ASSERT_TRUE((std::is_same<FunctorTraits<int *(Clz::*)(int, int)>::ResultType,
                            int *>::value));
}

TEST(FuncotrTraits, MemberFunction_4) {
  // static memeber functiion.
  ASSERT_TRUE((std::is_same<FunctorTraits<void (Clz::*)(int, int)>::ResultType,
                            void>::value));
}

TEST(FuncotrTraits, MemberFunction_5) {
  // static memeber functiion.
  ASSERT_TRUE((std::is_same<FunctorTraits<void *(Clz::*)(int, int)>::ResultType,
                            void *>::value));
}

TEST(FuncotrTraits, MemberFunction_6) {
  // static memeber functiion.
  ASSERT_TRUE(
      (std::is_same<FunctorTraits<void *(Clz::*const)(int, int)>::ResultType,
                    void *>::value));
}

// lamda has type (::*)(...) const
TEST(FuncotrTraits, Lambda_0) {
  auto f = [](int a, int b) -> int { return a + b; };
  auto result = Call(f, 4, 3);
  ASSERT_EQ(result, 7);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
}

TEST(FuncotrTraits, Lambda_1) {
  auto f1 = [](int a, int b) -> int { return a + b; };
  auto f2 = [](int a, int b) -> int { return a + b; };
  ASSERT_FALSE((std::is_same<decltype(f1), decltype(f2)>::value));
}

TEST(FuncotrTraits, Lambda_2) {
  auto f1 = [](int a, int b) -> int { return a + b; };
  auto f2 = f1;
  int result1 = f1(1, 2);
  int result2 = f2(1, 2);
  ASSERT_EQ(result1, result2);
}

class AnyFuncotr {
public:
  int operator()(int a, int b) { return a + b; }
};

TEST(FuncotrTraits, Functor_0) {
  AnyFuncotr f;
  auto result = Call(f, 3, 4);
  ASSERT_EQ(result, 7);
  ASSERT_TRUE((std::is_same<decltype(result), int>::value));
}

void fun() {}

TEST(FuncotrTraits, Decay) {
  // all decay to function pointer.
  ASSERT_TRUE((std::is_same<void (*)(), std::decay<void()>::type>::value));
  ASSERT_TRUE((std::is_same<void (*)(), std::decay<void (*)()>::type>::value));
  ASSERT_TRUE((std::is_same<void (*)(), std::decay<void (&)()>::type>::value));
  ASSERT_TRUE((std::is_same<void (*)(), std::decay<void(&&)()>::type>::value));
  // good!
  void (*ptr0)() = &fun;
  (void)ptr0;
  void (*ptr1)() = fun;
  (void)ptr1;
  auto &lref = fun;
  void (*ptr2)() = lref;
  (void)ptr2;
  auto &&rref = fun;
  void (*ptr3)() = rref;
  (void)ptr3;
}

} // namespace pillar
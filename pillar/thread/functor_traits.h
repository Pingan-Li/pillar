/**
 * @file functor_traits.h
 * @author lipingan (lipingan.dev@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PILLAR_THREAD_FUNCTORTRAITS
#define PILLAR_THREAD_FUNCTORTRAITS

#include <memory>
#include <type_traits>
namespace pillar {

/**
 * @brief
 *
 * @tparam R
 * @tparam Args
 */
template <typename R, typename... Args> struct FunctorTraitsBase {
  using ResultType = R;
};

/**
 * @brief
 *
 * @tparam Sigature
 */
template <typename Sigature> struct FunctorTraits;

/**
 * @brief
 *
 * @tparam R
 * @tparam Args
 */
template <typename R, typename... Args>
struct FunctorTraits<R(Args...)> : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief
 *
 * @tparam R
 * @tparam Args
 */
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)> : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief
 *
 * @tparam R
 * @tparam Args
 */
template <typename R, typename... Args>
struct FunctorTraits<R (&)(Args...)> : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief
 *
 * @tparam R
 * @tparam Args
 */
template <typename R, typename... Args>
struct FunctorTraits<R(&&)(Args...)> : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief
 *
 * @tparam R
 * @tparam Clz
 * @tparam Args
 */
template <typename R, typename Clz, typename... Args>
struct FunctorTraits<R (Clz::*)(Args...)>
    : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief - A const pointer point to a member function. It's kinda rare.
 *
 * @tparam R 
 * @tparam Clz
 * @tparam Args
 */
template <typename R, typename Clz, typename... Args>
struct FunctorTraits<R (Clz::*const)(Args...)>
    : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief - lambda
 *
 * @tparam R
 * @tparam Clz
 * @tparam Args
 */
template <typename R, typename Clz, typename... Args>
struct FunctorTraits<R (Clz::*)(Args...) const>
    : public FunctorTraitsBase<R, Args...> {};

/**
 * @brief -
 *
 * @tparam Functor - A class which overloads operator()
 */
template <typename Functor>
struct FunctorTraits
    : public FunctorTraits<
          decltype(&std::remove_reference<Functor>::type::operator())> {};
} // namespace pillar

#endif
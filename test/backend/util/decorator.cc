// IWYU pragma: no_include <typeinfo>
// IWYU pragma: no_include "jessica/util/math/hash.h"

#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <utility>

#include <jessica/helper/accessor.h>
#include <jessica/helper/poo.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/macro.h>

using namespace jessica;

class DecoratorTest final
{
 public:
  DecoratorTest() = default;
  RULE_OF_FIVE_COPY_AND_CLONE(DecoratorTest)

  [[nodiscard]] double B() const { return b_; }

  [[nodiscard]] std::shared_ptr<DecoratorTest> SetB(const double b) const
  {
    auto retval = Clone();
    retval->b_ = b;
    return retval;
  }

 private:
  double b_ = std::numeric_limits<double>::quiet_NaN();
};

template <typename T>
class DecoratorMiddle
{
 public:
  using RootType = typename T::RootType;

  template <typename... Args>
  // cppcheck-suppress constParameter
  explicit DecoratorMiddle(std::shared_ptr<RootType>& impl, Args&&... args)
      : t(impl, std::forward<Args>(args)...)
  {
  }
  RULE_OF_FIVE_COPY(DecoratorMiddle)

  template <uint64_t Action, uint64_t... U, typename... Args>
  [[nodiscard]] auto f(const RootType& classe, const Args&&... args) const
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return t.template f<Action, U...>(classe,
                                      std::forward<const Args>(args)...);
  }

  template <uint64_t Action, uint64_t V>
  requires EqualUL<Action, "Set"_f> && EqualUL<V, "B"_f>
  [[nodiscard]] std::shared_ptr<RootType> f(const RootType& classe,
                                            const double b) const
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    auto retval = t.template f<Action, V>(classe, b + 1.);
    return retval;
  }

 private:
  T t;
};

template <typename T>
class DECORATOR_START_HEADER(Test)
{
 public:
  DECORATOR_START_RULE_OF_FIVE(Test);

  DECORATOR_CLONE(Test)

  DECORATOR_START_GET(B)
  DECORATOR_START_SET(Test, B)
};

template <typename T>
class DECORATOR_END_HEADER(Test)
{
 public:
  using RootType = T;

  // cppcheck-suppress constParameter
  DECORATOR_END_RULE_OF_FIVE(Test);

  DECORATOR_END_GET(B)
  DECORATOR_END_SET(B)
};

JTEST_NAME(decorator, Test)  // NOLINT
{
  using Decorator =
      DecoratorStartTest<DecoratorMiddle<DecoratorEndTest<DecoratorTest>>>;

  const auto classe = std::make_shared<Decorator>();
  const auto classe2 = classe->SetB(10.);
  JTEST_EQ(classe2->B(), 11.);
}

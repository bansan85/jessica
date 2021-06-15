#include <limits>
#include <memory>

#include <jessica/compat.h>
#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/test/test.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/start.h>

using namespace jessica;

class DecoratorTest final
{
 public:
  DecoratorTest() = default;
  DecoratorTest(const DecoratorTest&) = default;
  DecoratorTest(DecoratorTest&&) = delete;
  DecoratorTest& operator=(const DecoratorTest&) = delete;
  DecoratorTest& operator=(DecoratorTest&&) = delete;

  ~DecoratorTest() = default;

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::Clone>
  [[nodiscard]] std::shared_ptr<DecoratorTest> f() const
  {
    return std::make_shared<DecoratorTest>(*this);
  }

  template <F Action, F T>
  requires Equals<F, Action, F::Get> && Equals<F, T, F::B>
  [[nodiscard]] double f() const { return b_; }

  template <F Action, F T>
  requires Equals<F, Action, F::Set> && Equals<F, T, F::B>
  [[nodiscard]] std::shared_ptr<DecoratorTest> f(const double b) const
  {
    auto retval = f<F::Set, F::Clone>();
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
  DecoratorMiddle(const DecoratorMiddle&) = default;
  DecoratorMiddle(DecoratorMiddle&&) = delete;
  DecoratorMiddle& operator=(const DecoratorMiddle&) = delete;
  DecoratorMiddle& operator=(DecoratorMiddle&&) = delete;

  ~DecoratorMiddle() = default;

  template <F Action, F... U, typename... Args>
  [[nodiscard]] auto f(const RootType& classe, const Args&&... args) const
  {
    std::cout << "DecoratorLogger " << typeid(T).name() << std::endl;
    return t.template f<Action, U...>(classe,
                                      std::forward<const Args>(args)...);
  }

  template <F Action, F V>
  requires Equals<F, Action, F::Set> && Equals<F, V, F::B>
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

JTEST_NAME(decorator, Test)  // NOLINT
{
  using Decorator =
      DecoratorStart<DecoratorMiddle<DecoratorEnd<DecoratorTest>>>;

  const auto classe = std::make_shared<Decorator>();
  const auto classe2 = classe->f<F::Set, F::B>(10.);
  JTEST_EQ((classe2->f<F::Get, F::B>()), 11.);
}

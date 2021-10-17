#include <cstdint>
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

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "Clone"_f>
  [[nodiscard]] std::shared_ptr<DecoratorTest> f() const
  {
    return std::make_shared<DecoratorTest>(*this);
  }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Get"_f> && EqualUL<T, "B"_f>
  [[nodiscard]] double f() const { return b_; }

  template <uint64_t Action, uint64_t T>
  requires EqualUL<Action, "Set"_f> && EqualUL<T, "B"_f>
  [[nodiscard]] std::shared_ptr<DecoratorTest> f(const double b) const
  {
    auto retval = f<Action, "Clone"_f>();
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

JTEST_NAME(decorator, Test)  // NOLINT
{
  using Decorator =
      DecoratorStart<DecoratorMiddle<DecoratorEnd<DecoratorTest>>>;

  const auto classe = std::make_shared<Decorator>();
  const auto classe2 = classe->f<"Set"_f, "B"_f>(10.);
  JTEST_EQ((classe2->f<"Get"_f, "B"_f>()), 11.);
}

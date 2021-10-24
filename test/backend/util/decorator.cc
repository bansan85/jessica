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

  [[nodiscard]] std::shared_ptr<DecoratorTest> Clone() const
  {
    return std::make_shared<DecoratorTest>(*this);
  }

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

template <typename T>
class JESSICA_DLL_PUBLIC DecoratorStartTest
    : public DecoratorStart<T, DecoratorStartTest<T>>
{
 public:
  template <typename... Args>
  explicit DecoratorStartTest(Args&&... args)
      : DecoratorStart<T, DecoratorStartTest<T>>(std::forward<Args>(args)...)
  {
  }
  DecoratorStartTest(const DecoratorStartTest&) = default;
  DecoratorStartTest(DecoratorStartTest&&) = delete;
  DecoratorStartTest& operator=(const DecoratorStartTest&) = delete;
  DecoratorStartTest& operator=(DecoratorStartTest&&) = delete;

  ~DecoratorStartTest() override = default;

  [[nodiscard]] std::shared_ptr<DecoratorStartTest<T>> Clone() const override
  {
    return std::make_shared<DecoratorStartTest<T>>(*this);
  }

  [[nodiscard]] double B() const { return this->template f<"Get"_f, "B"_f>(); }

  [[nodiscard]] auto SetB(double b) const
  {
    return std::static_pointer_cast<DecoratorStartTest<T>>(
        this->template f<"Set"_f, "B"_f>(b));
  }
};

template <typename T>
class JESSICA_DLL_PUBLIC DecoratorEndTest : public DecoratorEnd<T>
{
 public:
  using RootType = T;

  template <typename... Args>
  // cppcheck-suppress constParameter
  explicit DecoratorEndTest(std::shared_ptr<T>& impl, Args&&... args)
      : DecoratorEnd<T>(impl, std::forward<Args>(args)...)
  {
  }
  DecoratorEndTest(const DecoratorEndTest&) = default;
  DecoratorEndTest(DecoratorEndTest&&) = delete;
  DecoratorEndTest& operator=(const DecoratorEndTest&) = delete;
  DecoratorEndTest& operator=(DecoratorEndTest&&) = delete;

  ~DecoratorEndTest() override = default;

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "B"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.B(); }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "B"_f>
  [[nodiscard]] auto f(const T& classe, const double b) const
  {
    return classe.SetB(b);
  }
};

JTEST_NAME(decorator, Test)  // NOLINT
{
  using Decorator =
      DecoratorStartTest<DecoratorMiddle<DecoratorEndTest<DecoratorTest>>>;

  const auto classe = std::make_shared<Decorator>();
  const auto classe2 = classe->SetB(10.);
  JTEST_EQ(classe2->B(), 11.);
}

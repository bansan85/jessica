#pragma once

#include <cstddef>
#include <memory>
#include <utility>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/start.h>

namespace jessica
{
template <typename T>
class JESSICA_DLL_PUBLIC DecoratorStartMeyerhof
    : public DecoratorStart<T, DecoratorStartMeyerhof<T>>
{
 public:
  template <typename... Args>
  explicit DecoratorStartMeyerhof(Args&&... args)
      : DecoratorStart<T, DecoratorStartMeyerhof<T>>(
            std::forward<Args>(args)...)
  {
  }
  DecoratorStartMeyerhof(const DecoratorStartMeyerhof&) = default;
  DecoratorStartMeyerhof(DecoratorStartMeyerhof&&) = delete;
  DecoratorStartMeyerhof& operator=(const DecoratorStartMeyerhof&) = delete;
  DecoratorStartMeyerhof& operator=(DecoratorStartMeyerhof&&) = delete;

  ~DecoratorStartMeyerhof() override = default;

  [[nodiscard]] std::shared_ptr<DecoratorStartMeyerhof<T>>
      Clone() const override
  {
    return std::make_shared<DecoratorStartMeyerhof<T>>(*this);
  }

  [[nodiscard]] auto B_() const { return this->template f<"Get"_f, "B_"_f>(); }

  [[nodiscard]] auto Qref() const
  {
    return this->template f<"Get"_f, "Qref"_f>();
  }

  template <typename... Args>
  [[nodiscard]] auto Foundation(Args&&... args) const
  {
    return this->template f<"Get"_f, "Foundation"_f>(
        std::forward<Args>(args)...);
  }

  template <typename... Args>
  [[nodiscard]] auto SetFoundation(Args&&... args) const
  {
    return this->template f<"Set"_f, "Foundation"_f>(
        std::forward<Args>(args)...);
  }

  template <typename... Args>
  [[nodiscard]] auto Load(Args&&... args) const
  {
    return this->template f<"Get"_f, "Load"_f>(std::forward<Args>(args)...);
  }

  template <typename... Args>
  [[nodiscard]] auto SetLoad(Args&&... args) const
  {
    return this->template f<"Set"_f, "Load"_f>(std::forward<Args>(args)...);
  }
};

template <typename T>
class JESSICA_DLL_PUBLIC DecoratorEndMeyerhof : public DecoratorEnd<T>
{
 public:
  using RootType = T;

  template <typename... Args>
  explicit DecoratorEndMeyerhof(std::shared_ptr<T>& impl, Args&&... args)
      : DecoratorEnd<T>(impl, std::forward<Args>(args)...)
  {
  }
  DecoratorEndMeyerhof(const DecoratorEndMeyerhof&) = default;
  DecoratorEndMeyerhof(DecoratorEndMeyerhof&&) = delete;
  DecoratorEndMeyerhof& operator=(const DecoratorEndMeyerhof&) = delete;
  DecoratorEndMeyerhof& operator=(DecoratorEndMeyerhof&&) = delete;

  ~DecoratorEndMeyerhof() override = default;

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "B_"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.B_(); }

  template <uint64_t Action, uint64_t U>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "Qref"_f>
  [[nodiscard]] auto f(const T& classe) const { return classe.Qref(); }

  template <uint64_t Action, uint64_t U, typename... Args>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "Foundation"_f>
  [[nodiscard]] auto f(const T& classe, Args&&... args) const
  {
    return classe.Foundation(std::forward<Args>(args)...);
  }

  template <uint64_t Action, uint64_t U, typename... Args>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "Foundation"_f>
  [[nodiscard]] auto f(const T& classe, Args&&... args) const
  {
    return classe.SetFoundation(std::forward<Args>(args)...);
  }

  template <uint64_t Action, uint64_t U, typename... Args>
  requires EqualUL<Action, "Get"_f> && EqualUL<U, "Load"_f>
  [[nodiscard]] auto f(const T& classe, Args&&... args) const
  {
    return classe.Load(std::forward<Args>(args)...);
  }

  template <uint64_t Action, uint64_t U, typename... Args>
  requires EqualUL<Action, "Set"_f> && EqualUL<U, "Load"_f>
  [[nodiscard]] auto f(const T& classe, Args&&... args) const
  {
    return classe.SetLoad(std::forward<Args>(args)...);
  }
};
}  // namespace jessica

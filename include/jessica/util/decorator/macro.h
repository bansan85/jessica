#pragma once

#include <memory>
#include <utility>

#include <jessica/helper/accessor.h>
#include <jessica/helper/template.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/start.h>

#define DECORATOR_START_HEADER(CLASS) \
  DecoratorStart##CLASS : public DecoratorStart<T, DecoratorStart##CLASS<T>>

#define DECORATOR_START_RULE_OF_FIVE(CLASS)                                    \
  template <typename... Args>                                                  \
  explicit DecoratorStart##CLASS(Args&&... args)                               \
      : DecoratorStart<T, DecoratorStart##CLASS<T>>(                           \
            std::forward<Args>(args)...)                                       \
  {                                                                            \
  }                                                                            \
  DecoratorStart##CLASS(const DecoratorStart##CLASS&) = default;               \
  DecoratorStart##CLASS(DecoratorStart##CLASS&&) noexcept = delete;            \
  DecoratorStart##CLASS& operator=(const DecoratorStart##CLASS&) = delete;     \
  DecoratorStart##CLASS& operator=(DecoratorStart##CLASS&&) noexcept = delete; \
  ~DecoratorStart##CLASS() override = default

#define DECORATOR_END_RULE_OF_FIVE(CLASS)                                \
  template <typename... Args>                                            \
  explicit DecoratorEnd##CLASS(std::shared_ptr<T>& impl, Args&&... args) \
      : DecoratorEnd<T>(impl, std::forward<Args>(args)...)               \
  {                                                                      \
  }                                                                      \
  DecoratorEnd##CLASS(const DecoratorEnd##CLASS&) = default;             \
  DecoratorEnd##CLASS(DecoratorEnd##CLASS&&) = delete;                   \
  DecoratorEnd##CLASS& operator=(const DecoratorEnd##CLASS&) = delete;   \
  DecoratorEnd##CLASS& operator=(DecoratorEnd##CLASS&&) = delete;        \
  ~DecoratorEnd##CLASS() override = default

#define DECORATOR_CLONE(CLASS)                                    \
  [[nodiscard]] std::shared_ptr<DecoratorStart##CLASS<T>> Clone() \
      const override                                              \
  {                                                               \
    return std::make_shared<DecoratorStart##CLASS<T>>(*this);     \
  }

#define DECORATOR_START_GET(FUNCTION)                 \
  template <typename... Args>                         \
  [[nodiscard]] auto FUNCTION(Args&&... args) const   \
  {                                                   \
    return this->template f<"Get"_f, #FUNCTION ""_f>( \
        std::forward<Args>(args)...);                 \
  }

#define DECORATOR_START_SET(CLASS, FUNCTION)                   \
  template <typename... Args>                                  \
  [[nodiscard]] auto Set##FUNCTION(Args&&... args) const       \
  {                                                            \
    return std::static_pointer_cast<DecoratorStart##CLASS<T>>( \
        this->template f<"Set"_f, #FUNCTION ""_f>(             \
            std::forward<Args>(args)...));                     \
  }

#define DECORATOR_END_HEADER(CLASS) DecoratorEnd##CLASS : public DecoratorEnd<T>

#define DECORATOR_END_GET(FUNCTION)                               \
  template <uint64_t Action, uint64_t U, typename... Args>        \
  requires EqualUL<Action, "Get"_f> && EqualUL<U, #FUNCTION ""_f> \
  [[nodiscard]] auto f(const T& classe, Args&&... args) const     \
  {                                                               \
    return classe.FUNCTION(std::forward<Args>(args)...);          \
  }

#define DECORATOR_END_SET(FUNCTION)                               \
  template <uint64_t Action, uint64_t U, typename... Args>        \
  requires EqualUL<Action, "Set"_f> && EqualUL<U, #FUNCTION ""_f> \
  [[nodiscard]] auto f(const T& classe, Args&&... args) const     \
  {                                                               \
    return classe.Set##FUNCTION(std::forward<Args>(args)...);     \
  }

#define POCO_GET_FUNCTION(FUNCTION, FIELD)                         \
  template <typename Fct, typename... Args>                        \
  [[nodiscard]] auto FUNCTION(const Fct fct, Args&&... args) const \
  {                                                                \
    return (FIELD.get()->*fct)(std::forward<Args>(args)...);       \
  }

#define POCO_SET_FUNCTION(CLASS, FUNCTION, FIELD)                             \
  template <typename Fct, typename... Args>                                   \
  [[nodiscard]] std::shared_ptr<CLASS> Set##FUNCTION(const Fct fct,           \
                                                     Args&&... args) const    \
  {                                                                           \
    auto retval = Clone();                                                    \
    retval->FIELD = (retval->FIELD.get()->*fct)(std::forward<Args>(args)...); \
    return retval;                                                            \
  }

#define RULE_OF_FIVE_COPY_AND_CLONE(CLASS)           \
  CLASS(const CLASS&) = default;                     \
  CLASS(CLASS&&) = delete;                           \
  CLASS& operator=(const CLASS&) = delete;           \
  CLASS& operator=(CLASS&&) = delete;                \
  ~CLASS() = default;                                \
  [[nodiscard]] std::shared_ptr<CLASS> Clone() const \
  {                                                  \
    return std::make_shared<CLASS>(*this);           \
  }

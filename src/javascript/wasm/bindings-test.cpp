#include <iostream>
#include <memory>

#include <emscripten/bind.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>
#include <jessica/util/decorator/end.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>
#include <jessica/util/decorator/start.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jessica)
{
  using namespace jessica;

  function("SpdlogStdoutMt",
           &spdlog::stdout_logger_mt<spdlog::synchronous_factory>);

  class_<spdlog::logger>("logger")
      .constructor<std::string>()
      .smart_ptr<std::shared_ptr<spdlog::logger>>("logger");

  class_<FoundationStrip>("FoundationStrip")
      .constructor<>()
      .smart_ptr<std::shared_ptr<FoundationStrip>>("FoundationStrip")
      .function("clone", &FoundationStrip::f<"Set"_f, "Clone"_f>)
      .function("getB", static_cast<double (FoundationStrip::*)() const>(
                            &FoundationStrip::f<"Get"_f, "B"_f>))
      .function("setB", static_cast<std::shared_ptr<FoundationStrip> (
                            FoundationStrip::*)(const double) const>(
                            &FoundationStrip::f<"Set"_f, "B"_f>));

  class_<VerticalEccentric>("VerticalEccentric")
      .constructor<>()
      .smart_ptr<std::shared_ptr<VerticalEccentric>>("VerticalEccentric")
      .function("clone", &VerticalEccentric::f<"Set"_f, "Clone"_f>)
      .function("getE", static_cast<double (VerticalEccentric::*)() const>(
                            &VerticalEccentric::f<"Get"_f, "E"_f>))
      .function("setE", static_cast<std::shared_ptr<VerticalEccentric> (
                            VerticalEccentric::*)(const double) const>(
                            &VerticalEccentric::f<"Set"_f, "E"_f>))
      .function("getV", static_cast<double (VerticalEccentric::*)() const>(
                            &VerticalEccentric::f<"Get"_f, "V"_f>))
      .function("setV", static_cast<std::shared_ptr<VerticalEccentric> (
                            VerticalEccentric::*)(const double) const>(
                            &VerticalEccentric::f<"Set"_f, "V"_f>));

  class_<DecoratorStart<LogCall<LogDuration<DecoratorEnd<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>>(
      "MeyerhofShallowFoundation")
      .constructor<std::shared_ptr<spdlog::logger>,
                   std::shared_ptr<spdlog::logger>,
                   const std::shared_ptr<VerticalEccentric>,
                   const std::shared_ptr<FoundationStrip>>()
      .function(
          "getQref",
          static_cast<double (
              DecoratorStart<
                  LogCall<LogDuration<DecoratorEnd<MeyerhofShallowFoundation<
                      VerticalEccentric, FoundationStrip>>>>>::*)() const>(
              &DecoratorStart<
                  LogCall<LogDuration<DecoratorEnd<MeyerhofShallowFoundation<
                      VerticalEccentric, FoundationStrip>>>>>::f<"Get"_f,
                                                                 "Qref"_f>))
      .function(
          "getB_",
          static_cast<double (
              DecoratorStart<
                  LogCall<LogDuration<DecoratorEnd<MeyerhofShallowFoundation<
                      VerticalEccentric, FoundationStrip>>>>>::*)() const>(
              &DecoratorStart<
                  LogCall<LogDuration<DecoratorEnd<MeyerhofShallowFoundation<
                      VerticalEccentric, FoundationStrip>>>>>::f<"Get"_f,
                                                                 "B_"_f>));
}

#include <iostream>
#include <memory>

#include <emscripten/bind.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <jessica/calc/geotechnical/decorator_meyerhof.h>
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
      .function("clone", &FoundationStrip::Clone)
      .function("getB", &FoundationStrip::B)
      .function("setB", &FoundationStrip::SetB);

  class_<VerticalEccentric>("VerticalEccentric")
      .constructor<>()
      .smart_ptr<std::shared_ptr<VerticalEccentric>>("VerticalEccentric")
      .function("clone", &VerticalEccentric::Clone)
      .function("getE", &VerticalEccentric::E)
      .function("setE", &VerticalEccentric::SetE)
      .function("getV", &VerticalEccentric::V)
      .function("setV", &VerticalEccentric::SetV);

  class_<DecoratorStartMeyerhof<LogCall<LogDuration<DecoratorEndMeyerhof<
      MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>>>>>(
      "MeyerhofShallowFoundation")
      .constructor<std::shared_ptr<spdlog::logger>,
                   std::shared_ptr<spdlog::logger>,
                   const std::shared_ptr<VerticalEccentric>,
                   const std::shared_ptr<FoundationStrip>>()
      .function("qref",
                &DecoratorStartMeyerhof<
                    LogCall<LogDuration<DecoratorEndMeyerhof<MeyerhofShallowFoundation<
                        VerticalEccentric, FoundationStrip>>>>>::Qref)
      .function("b_",
                &DecoratorStartMeyerhof<
                    LogCall<LogDuration<DecoratorEndMeyerhof<MeyerhofShallowFoundation<
                        VerticalEccentric, FoundationStrip>>>>>::B_);
}

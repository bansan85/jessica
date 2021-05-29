#include <iostream>
#include <memory>

#include <emscripten/bind.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jessica)
{
  using namespace jessica;
  class_<FoundationStrip<FoundationStripImpl>>("FoundationStripRaw")
      .constructor<>()
      .smart_ptr<std::shared_ptr<FoundationStrip<FoundationStripImpl>>>(
          "FoundationStripRaw")
      .function("clone", &FoundationStrip<FoundationStripImpl>::Clone)
      .function(
          "getB",
          static_cast<double (FoundationStrip<FoundationStripImpl>::*)() const>(
              &FoundationStrip<FoundationStripImpl>::f<F::Get, F::B>))
      .function(
          "setB",
          static_cast<std::shared_ptr<FoundationStrip<FoundationStripImpl>> (
              FoundationStrip<FoundationStripImpl>::*)(const double&&) const>(
              &FoundationStrip<FoundationStripImpl>::f<F::Set, F::B>));

  class_<VerticalEccentric<VerticalEccentricImpl>>("VerticalEccentricRaw")
      .constructor<>()
      .smart_ptr<std::shared_ptr<VerticalEccentric<VerticalEccentricImpl>>>(
          "VerticalEccentricRaw")
      .function("clone", &VerticalEccentric<VerticalEccentricImpl>::Clone)
      .function("getE",
                static_cast<double (
                    VerticalEccentric<VerticalEccentricImpl>::*)() const>(
                    &VerticalEccentric<VerticalEccentricImpl>::f<F::Get, F::E>))
      .function("setE",
                static_cast<
                    std::shared_ptr<VerticalEccentric<VerticalEccentricImpl>> (
                        VerticalEccentric<VerticalEccentricImpl>::*)(
                        const double&&) const>(
                    &VerticalEccentric<VerticalEccentricImpl>::f<F::Set, F::E>))
      .function("getV",
                static_cast<double (
                    VerticalEccentric<VerticalEccentricImpl>::*)() const>(
                    &VerticalEccentric<VerticalEccentricImpl>::f<F::Get, F::V>))
      .function("setV",
                static_cast<
                    std::shared_ptr<VerticalEccentric<VerticalEccentricImpl>> (
                        VerticalEccentric<VerticalEccentricImpl>::*)(
                        const double&&) const>(
                    &VerticalEccentric<VerticalEccentricImpl>::f<F::Set, F::V>));

  class_<MeyerhofShallowFoundation<
      LogCall<LogDuration<MeyerhofShallowFoundationImpl<
          VerticalEccentricImpl, FoundationStripImpl>>>>>(
      "MeyerhofShallowFoundationDeco")
      .constructor<
          const std::shared_ptr<VerticalEccentric<VerticalEccentricImpl>>,
          const std::shared_ptr<FoundationStrip<FoundationStripImpl>>>()
      .function("clone",
                &MeyerhofShallowFoundation<
                    LogCall<LogDuration<MeyerhofShallowFoundationImpl<
                        VerticalEccentricImpl, FoundationStripImpl>>>>::Clone)
      .function("getQref",
                static_cast<double (
                    MeyerhofShallowFoundation<
                        LogCall<LogDuration<MeyerhofShallowFoundationImpl<
                            VerticalEccentricImpl, FoundationStripImpl>>>>::*)()
                                const>(
                    &MeyerhofShallowFoundation<
                        LogCall<LogDuration<MeyerhofShallowFoundationImpl<
                            VerticalEccentricImpl, FoundationStripImpl>>>>::
                        f<F::Get, F::Qref>))
      .function("getB_",
                static_cast<double (
                    MeyerhofShallowFoundation<
                        LogCall<LogDuration<MeyerhofShallowFoundationImpl<
                            VerticalEccentricImpl, FoundationStripImpl>>>>::*)()
                                const>(
                    &MeyerhofShallowFoundation<
                        LogCall<LogDuration<MeyerhofShallowFoundationImpl<
                            VerticalEccentricImpl, FoundationStripImpl>>>>::
                        f<F::Get, F::B_>));
}

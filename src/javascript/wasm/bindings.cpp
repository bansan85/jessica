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
  class_<Jessica::Data::Geotechnical::FoundationStrip<
      Jessica::Data::Geotechnical::FoundationStripImpl>>("FoundationStripRaw")
      .constructor<>()
      .smart_ptr<std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<
          Jessica::Data::Geotechnical::FoundationStripImpl>>>(
          "FoundationStripRaw")
      .function("clone",
                &Jessica::Data::Geotechnical::FoundationStrip<
                    Jessica::Data::Geotechnical::FoundationStripImpl>::Clone)
      .function("getB",
                static_cast<double (
                    Jessica::Data::Geotechnical::FoundationStrip<
                        Jessica::Data::Geotechnical::FoundationStripImpl>::*)()
                                const>(
                    &Jessica::Data::Geotechnical::FoundationStrip<
                        Jessica::Data::Geotechnical::FoundationStripImpl>::
                        f<false,
                          Jessica::Data::Geotechnical::FoundationStripImpl::B>))
      .function(
          "setB",
          static_cast<
              std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<
                  Jessica::Data::Geotechnical::FoundationStripImpl>> (
                  Jessica::Data::Geotechnical::FoundationStrip<
                      Jessica::Data::Geotechnical::FoundationStripImpl>::*)(
                  const double&&) const>(
              &Jessica::Data::Geotechnical::FoundationStrip<
                  Jessica::Data::Geotechnical::FoundationStripImpl>::
                  f<true,
                    Jessica::Data::Geotechnical::FoundationStripImpl::B>));

  class_<Jessica::Data::Load::VerticalEccentric<
      Jessica::Data::Load::VerticalEccentricImpl>>("VerticalEccentricRaw")
      .constructor<>()
      .smart_ptr<std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
          Jessica::Data::Load::VerticalEccentricImpl>>>("VerticalEccentricRaw")
      .function("clone", &Jessica::Data::Load::VerticalEccentric<
                             Jessica::Data::Load::VerticalEccentricImpl>::Clone)
      .function(
          "getE",
          static_cast<double (Jessica::Data::Load::VerticalEccentric<
                              Jessica::Data::Load::VerticalEccentricImpl>::*)()
                          const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::
                  f<false, Jessica::Data::Load::VerticalEccentricImpl::E>))
      .function(
          "setE",
          static_cast<std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
              Jessica::Data::Load::VerticalEccentricImpl>> (
              Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::*)(
              const double&&) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::
                  f<true, Jessica::Data::Load::VerticalEccentricImpl::E>))
      .function(
          "getV",
          static_cast<double (Jessica::Data::Load::VerticalEccentric<
                              Jessica::Data::Load::VerticalEccentricImpl>::*)()
                          const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::
                  f<false, Jessica::Data::Load::VerticalEccentricImpl::V>))
      .function(
          "setV",
          static_cast<std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
              Jessica::Data::Load::VerticalEccentricImpl>> (
              Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::*)(
              const double&&) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::
                  f<true, Jessica::Data::Load::VerticalEccentricImpl::V>));

  class_<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
              Jessica::Data::Load::VerticalEccentricImpl,
              Jessica::Data::Geotechnical::FoundationStripImpl>>>>>(
      "MeyerhofShallowFoundationDeco")
      .constructor<
          const std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
              Jessica::Data::Load::VerticalEccentricImpl>>,
          const std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<
              Jessica::Data::Geotechnical::FoundationStripImpl>>>()
      .function("clone",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl<
                                    Jessica::Data::Load::VerticalEccentricImpl,
                                    Jessica::Data::Geotechnical::
                                        FoundationStripImpl>>>>::Clone)
      .function(
          "getQref",
          static_cast<double (
              Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Calc::Geotechnical::
                              MeyerhofShallowFoundationImpl<
                                  Jessica::Data::Load::VerticalEccentricImpl,
                                  Jessica::Data::Geotechnical::
                                      FoundationStripImpl>>>>::*)() const>(
              &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Calc::Geotechnical::
                              MeyerhofShallowFoundationImpl<
                                  Jessica::Data::Load::VerticalEccentricImpl,
                                  Jessica::Data::Geotechnical::
                                      FoundationStripImpl>>>>::
                  f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        Jessica::Data::Load::VerticalEccentricImpl,
                        Jessica::Data::Geotechnical::FoundationStripImpl>::
                        Qref>))
      .function(
          "getB_",
          static_cast<double (
              Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Calc::Geotechnical::
                              MeyerhofShallowFoundationImpl<
                                  Jessica::Data::Load::VerticalEccentricImpl,
                                  Jessica::Data::Geotechnical::
                                      FoundationStripImpl>>>>::*)() const>(
              &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Calc::Geotechnical::
                              MeyerhofShallowFoundationImpl<
                                  Jessica::Data::Load::VerticalEccentricImpl,
                                  Jessica::Data::Geotechnical::
                                      FoundationStripImpl>>>>::
                  f<false,
                    Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl<
                        Jessica::Data::Load::VerticalEccentricImpl,
                        Jessica::Data::Geotechnical::FoundationStripImpl>::
                        B_>));
}

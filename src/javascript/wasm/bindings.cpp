#include <iostream>
#include <memory>

#include <emscripten/bind.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/calc/geotechnical/meyerhof_impl.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/geotechnical/foundation_strip_impl.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric_impl.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jessica)
{
  class_<Jessica::Data::Geotechnical::IFoundationStrip>("IFoundationStrip")
      .smart_ptr<
          std::shared_ptr<Jessica::Data::Geotechnical::IFoundationStrip>>(
          "IFoundationStrip")
      .function(
          "setB",
          select_overload<std::shared_ptr<
              Jessica::Data::Geotechnical::IFoundationStrip>(double) const>(
              &Jessica::Data::Geotechnical::IFoundationStrip::B))
      .function("getB", select_overload<double() const>(
                            &Jessica::Data::Geotechnical::IFoundationStrip::B));

  class_<Jessica::Data::Geotechnical::FoundationStrip<
             Jessica::Util::Decorator::LogCall<
                 Jessica::Util::Decorator::LogDuration<
                     Jessica::Data::Geotechnical::FoundationStripImpl>>>,
         base<Jessica::Data::Geotechnical::IFoundationStrip>>(
      "FoundationStripDeco")
      .constructor<>()
      .function(
          "clone",
          &Jessica::Data::Geotechnical::FoundationStrip<
              Jessica::Util::Decorator::LogCall<
                  Jessica::Util::Decorator::LogDuration<
                      Jessica::Data::Geotechnical::FoundationStripImpl>>>::
              Clone)
      .function(
          "setB",
          select_overload<std::shared_ptr<
              Jessica::Data::Geotechnical::IFoundationStrip>(double) const>(
              &Jessica::Data::Geotechnical::FoundationStrip<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Geotechnical::FoundationStripImpl>>>::
                  B))
      .function(
          "getB",
          select_overload<double() const>(
              &Jessica::Data::Geotechnical::FoundationStrip<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Geotechnical::FoundationStripImpl>>>::
                  B));

  class_<Jessica::Data::Load::IVerticalEccentric>("IVerticalEccentric")
      .smart_ptr<std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>>(
          "IVerticalEccentric")
      .function("setE",
                select_overload<std::shared_ptr<
                    Jessica::Data::Load::IVerticalEccentric>(double) const>(
                    &Jessica::Data::Load::IVerticalEccentric::E))
      .function("setV",
                select_overload<std::shared_ptr<
                    Jessica::Data::Load::IVerticalEccentric>(double) const>(
                    &Jessica::Data::Load::IVerticalEccentric::V))
      .function("getE", select_overload<double() const>(
                            &Jessica::Data::Load::IVerticalEccentric::E))
      .function("getV", select_overload<double() const>(
                            &Jessica::Data::Load::IVerticalEccentric::V));

  class_<
      Jessica::Data::Load::VerticalEccentric<Jessica::Util::Decorator::LogCall<
          Jessica::Util::Decorator::LogDuration<
              Jessica::Data::Load::VerticalEccentricImpl>>>,
      base<Jessica::Data::Load::IVerticalEccentric>>("VerticalEccentricDeco")
      .constructor<>()
      .function(
          "clone",
          &Jessica::Data::Load::VerticalEccentric<
              Jessica::Util::Decorator::LogCall<
                  Jessica::Util::Decorator::LogDuration<
                      Jessica::Data::Load::VerticalEccentricImpl>>>::Clone)
      .function(
          "setE",
          select_overload<std::shared_ptr<
              Jessica::Data::Load::IVerticalEccentric>(double) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Load::VerticalEccentricImpl>>>::E))
      .function(
          "getE",
          select_overload<double() const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Load::VerticalEccentricImpl>>>::E))
      .function(
          "setV",
          select_overload<std::shared_ptr<
              Jessica::Data::Load::IVerticalEccentric>(double) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Load::VerticalEccentricImpl>>>::V))
      .function(
          "getV",
          select_overload<double() const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Util::Decorator::LogCall<
                      Jessica::Util::Decorator::LogDuration<
                          Jessica::Data::Load::VerticalEccentricImpl>>>::V));

  class_<Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation>(
      "IMeyerhofShallowFoundation")
      .smart_ptr<std::shared_ptr<
          Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation>>(
          "IMeyerhofShallowFoundation")
      .function("getQref",
                &Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation::Qref)
      .function("getB_",
                &Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation::B_);

  class_<
      Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Util::Decorator::LogCall<
              Jessica::Util::Decorator::LogDuration<
                  Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl>>>,
      base<Jessica::Calc::Geotechnical::IMeyerhofShallowFoundation>>(
      "MeyerhofShallowFoundationDeco")
      .constructor<
          const std::shared_ptr<Jessica::Data::Load::IVerticalEccentric>&,
          const std::shared_ptr<
              Jessica::Data::Geotechnical::IFoundationStrip>&>()
      .function("clone",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl>>>::Clone)
      .function("getQref",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl>>>::Qref)
      .function("getB_",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl>>>::B_);
}

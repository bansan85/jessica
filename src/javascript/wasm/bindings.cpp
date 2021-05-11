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
      .function(
          "setB",
          select_overload<
              std::shared_ptr<Jessica::Data::Geotechnical::FoundationStrip<
                  Jessica::Data::Geotechnical::FoundationStripImpl>>(double)
                  const>(&Jessica::Data::Geotechnical::FoundationStrip<
                         Jessica::Data::Geotechnical::FoundationStripImpl>::B))
      .function("getB",
                select_overload<double() const>(
                    &Jessica::Data::Geotechnical::FoundationStrip<
                        Jessica::Data::Geotechnical::FoundationStripImpl>::B));

  class_<Jessica::Data::Load::VerticalEccentric<
      Jessica::Data::Load::VerticalEccentricImpl>>("VerticalEccentricRaw")
      .constructor<>()
      .smart_ptr<std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
          Jessica::Data::Load::VerticalEccentricImpl>>>("VerticalEccentricRaw")
      .function("clone", &Jessica::Data::Load::VerticalEccentric<
                             Jessica::Data::Load::VerticalEccentricImpl>::Clone)
      .function(
          "setE",
          select_overload<
              std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>>(double) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::E))
      .function("getE", select_overload<double() const>(
                            &Jessica::Data::Load::VerticalEccentric<
                                Jessica::Data::Load::VerticalEccentricImpl>::E))
      .function(
          "setV",
          select_overload<
              std::shared_ptr<Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>>(double) const>(
              &Jessica::Data::Load::VerticalEccentric<
                  Jessica::Data::Load::VerticalEccentricImpl>::V))
      .function("getV",
                select_overload<double() const>(
                    &Jessica::Data::Load::VerticalEccentric<
                        Jessica::Data::Load::VerticalEccentricImpl>::V));

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
      .function("getQref",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl<
                                    Jessica::Data::Load::VerticalEccentricImpl,
                                    Jessica::Data::Geotechnical::
                                        FoundationStripImpl>>>>::Qref)
      .function("getB_",
                &Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
                    Jessica::Util::Decorator::LogCall<
                        Jessica::Util::Decorator::LogDuration<
                            Jessica::Calc::Geotechnical::
                                MeyerhofShallowFoundationImpl<
                                    Jessica::Data::Load::VerticalEccentricImpl,
                                    Jessica::Data::Geotechnical::
                                        FoundationStripImpl>>>>::B_);
}

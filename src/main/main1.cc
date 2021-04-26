#include <iostream>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof_impl.h>
#include <jessica/data/geotechnical/foundation_strip_impl.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric_impl.h>
#include <jessica/util/decorator/log_call.h>
#include <jessica/util/decorator/log_duration.h>

int main()
{
  const auto load = std::make_shared<Jessica::Data::Load::VerticalEccentric<
      Jessica::Util::Decorator::LogCall<Jessica::Util::Decorator::LogDuration<
          Jessica::Data::Load::VerticalEccentricImpl>>>>()
                        ->E(0.2)
                        ->V(100.);
  const auto foundation =
      std::make_shared<Jessica::Data::Geotechnical::FoundationStrip<
          Jessica::Util::Decorator::LogCall<
              Jessica::Util::Decorator::LogDuration<
                  Jessica::Data::Geotechnical::FoundationStripImpl>>>>()
          ->B(1.);
  const auto calc =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Util::Decorator::LogCall<
              Jessica::Util::Decorator::LogDuration<
                  Jessica::Calc::Geotechnical::
                      MeyerhofShallowFoundationImpl>>>>(load, foundation);

  std::cout << calc->Qref() << " and " << calc->B_() << "\n";
}

#include <iostream>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof_impl.h>
#include <jessica/data/geotechnical/foundation_strip_impl.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/data/load/vertical_eccentric_impl.h>

int main()
{
  const auto load = std::make_shared<Jessica::Data::Load::VerticalEccentric<
      Jessica::Data::Load::VerticalEccentricImpl>>()
                        ->E(0.2)
                        ->V(100.);
  const auto foundation =
      std::make_shared<Jessica::Data::Geotechnical::FoundationStrip<
          Jessica::Data::Geotechnical::FoundationStripImpl>>()
          ->B(1.);
  const auto calc =
      std::make_shared<Jessica::Calc::Geotechnical::MeyerhofShallowFoundation<
          Jessica::Calc::Geotechnical::MeyerhofShallowFoundationImpl>>(
          load, foundation);

  std::cout << calc->Qref() << " and " << calc->B_() << "\n";
}

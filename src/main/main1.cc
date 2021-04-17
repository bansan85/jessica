#include <iostream>
#include <memory>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>

int main()
{
  const auto load =
      Jessica::Data::Load::Create::VerticalEccentric()->E(0.2)->V(100.);
  const auto foundation =
      Jessica::Data::Geotechnical::Create::FoundationStrip()->B(1.);
  const auto calc =
      Jessica::Calc::Geotechnical::Create::MeyerhofShallowFoundation(
          load, foundation);

  std::cout << calc->Qref() << " and " << calc->B_() << "\n";
}

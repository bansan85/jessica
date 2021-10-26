#include <iostream>
#include <memory>

#include <emscripten/bind.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>
#include <jessica/helper/accessor.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jessica)
{
  using namespace jessica;

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

  class_<MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      "MeyerhofShallowFoundation")
      .constructor<const std::shared_ptr<VerticalEccentric>,
                   const std::shared_ptr<FoundationStrip>>()
      .function(
          "qref",
          &MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>::Qref)
      .function(
          "b_",
          &MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>::B_);
}

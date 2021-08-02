#include <iostream>
#include <memory>

#include <emscripten/bind.h>

#include <jessica/calc/geotechnical/meyerhof.h>
#include <jessica/data/geotechnical/foundation_strip.h>
#include <jessica/data/load/vertical_eccentric.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jessica)
{
  using namespace jessica;

  class_<FoundationStrip>("FoundationStrip")
      .constructor<>()
      .smart_ptr<std::shared_ptr<FoundationStrip>>("FoundationStrip")
      .function("clone", &FoundationStrip::f<F::Set, F::Clone>)
      .function("getB", static_cast<double (FoundationStrip::*)() const>(
                            &FoundationStrip::f<F::Get, F::B>))
      .function("setB", static_cast<std::shared_ptr<FoundationStrip> (
                            FoundationStrip::*)(const double) const>(
                            &FoundationStrip::f<F::Set, F::B>));

  class_<VerticalEccentric>("VerticalEccentric")
      .constructor<>()
      .smart_ptr<std::shared_ptr<VerticalEccentric>>("VerticalEccentric")
      .function("clone", &VerticalEccentric::f<F::Set, F::Clone>)
      .function("getE", static_cast<double (VerticalEccentric::*)() const>(
                            &VerticalEccentric::f<F::Get, F::E>))
      .function("setE", static_cast<std::shared_ptr<VerticalEccentric> (
                            VerticalEccentric::*)(const double) const>(
                            &VerticalEccentric::f<F::Set, F::E>))
      .function("getV", static_cast<double (VerticalEccentric::*)() const>(
                            &VerticalEccentric::f<F::Get, F::V>))
      .function("setV", static_cast<std::shared_ptr<VerticalEccentric> (
                            VerticalEccentric::*)(const double) const>(
                            &VerticalEccentric::f<F::Set, F::V>));

  class_<MeyerhofShallowFoundation<VerticalEccentric, FoundationStrip>>(
      "MeyerhofShallowFoundation")
      .constructor<const std::shared_ptr<VerticalEccentric>,
                   const std::shared_ptr<FoundationStrip>>()
      .function(
          "getQref",
          static_cast<double (MeyerhofShallowFoundation<
                              VerticalEccentric, FoundationStrip>::*)() const>(
              &MeyerhofShallowFoundation<VerticalEccentric,
                                         FoundationStrip>::f<F::Get, F::Qref>))
      .function(
          "getB_",
          static_cast<double (MeyerhofShallowFoundation<
                              VerticalEccentric, FoundationStrip>::*)() const>(
              &MeyerhofShallowFoundation<VerticalEccentric,
                                         FoundationStrip>::f<F::Get, F::B_>));
}

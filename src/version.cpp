#include "version.hpp"

#include <cstdio>

namespace Fourier {

VersionNumber getCVersion() {
  int32_t maj, min, rev;
  fourierGetLibraryVersion(&maj, &min, &rev);
  return VersionNumber(maj, min, rev);
}

// fourierc++ VERSION
VersionNumber getCppVersion() { return VersionNumber(0, 6, 1); }

}  // namespace Fourier

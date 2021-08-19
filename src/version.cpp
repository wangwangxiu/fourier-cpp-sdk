#include "version.hpp"

#include <cstdio>

namespace Amber {

VersionNumber getCVersion() {
  int32_t maj, min, rev;
  amberGetLibraryVersion(&maj, &min, &rev);
  return VersionNumber(maj, min, rev);
}

// amberc++ VERSION
VersionNumber getCppVersion() { return VersionNumber(0, 4, 0); }

} // namespace Amber

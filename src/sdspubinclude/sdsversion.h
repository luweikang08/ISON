#include <string>
#include <sstream>

namespace ison{
namespace sds{

#define MAJOR 2
#define MINOR 0
#define BUILD 2

inline int VERSION_MAJOR() {
  return MAJOR;
}

inline int VERSION_MINOR() {
  return MINOR;
}

inline int VERSION_BUILD() {
  return BUILD;
}

inline int version() {
  return BUILD +MINOR * 100 +MAJOR * 10000;
}

inline std::string version_string() {
  std::ostringstream is;
  is <<MAJOR << "." <<MINOR << "." <<BUILD;
  return is.str();
}

} //namespace ison
} //namespace sds

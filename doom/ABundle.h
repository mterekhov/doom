#ifndef SPCDOOMENGINE_ABUNDLE_H
#define SPCDOOMENGINE_ABUNDLE_H

#include <string>

namespace spcDoomEngine {

class ABundle {
public:
    std::string resourceFullPath(const std::string& resourceName);
    std::vector<uint8_t> readFile(const std::string& filename);
};

}   //  namespace spcDoomEngine

#endif  //  SPCDOOMENGINE_ABUNDLE_H

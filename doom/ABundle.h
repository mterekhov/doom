#ifndef ABUNDLE_H
#define ABUNDLE_H

#include <string>

namespace DoomEngine {

class ABundle {
public:
    std::string resourceFullPath(const std::string& resourceName);
    std::vector<uint8_t> readFile(const std::string& filename);
};

}

#endif /* ABUNDLE_H */

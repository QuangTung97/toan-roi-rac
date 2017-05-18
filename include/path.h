#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

namespace tung {

typedef std::vector<int> Path;

class PathWriter {
public:
	void write(const std::string& filename, 
            const Path& path);
};

}

#endif

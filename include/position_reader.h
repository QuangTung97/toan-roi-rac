#ifndef POSITION_READER
#define POSITION_READER

#include <vector>
#include <string>


namespace tung{

struct Position {
	int x, y;
};

class PositionReader {
private:

public:
	std::vector<Position> read(std::string filename);

};

} // namespace tung

#endif

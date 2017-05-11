#include <position_reader.h>
#include <stdio.h>


namespace tung {

using std::vector;
using std::string;

vector<Position> PositionReader::read(string filename) {
	vector<Position> positions;
	Position pos;
	int n;

	FILE *file = fopen(filename.c_str(), "r");
	if (file == nullptr)
		return positions;

	fscanf(file, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(file, "%d %d", &pos.x, &pos.y);
		positions.push_back(pos);
	}

	fclose(file);

	return positions;
}

}

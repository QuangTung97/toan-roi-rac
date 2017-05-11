#include <overflow_time.h>
#include <fstream>

namespace tung {

using std::vector;
using std::string;
using std::ifstream;
using std::ios;

vector<int> OverflowTime::read(string filename) {
	vector<int> overflow_time;
	ifstream file;
	file.open(filename, ios::in);

	int n, time;
	file >> n;

	for (int i = 0; i < n; i++) {
		file >> time;
		overflow_time.push_back(time);
	}

	file.close();
	return overflow_time;
}

}

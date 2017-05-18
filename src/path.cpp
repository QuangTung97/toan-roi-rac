#include <path.h>
#include <fstream>

namespace tung {

using std::ofstream;
using std::ios;
using std::endl;

void PathWriter::write(
        const std::string& filename, 
        const Path& path) 
{
	ofstream file;
	file.open(filename, ios::out);

    size_t N = path.size();
    file << N << endl;
    for (auto v: path) {
        file << v << " ";
    }

    file.close();
}

}

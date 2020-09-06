#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream> 
#include <vector>

using namespace std;

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

ostringstream generateHeader(string constantName, std::vector<char> value) {
	ostringstream output;
	output << "#pragma once\n\n";
	output << "#include <stdlib.h>\n\n";
	output << "namespace poc {\n\n";
	output << "    constexpr unsigned char " << constantName << "[] = {\n";
	int lineCount = 0;
	auto buffer = reinterpret_cast<unsigned char*>(value.data());
	for (size_t i = 0; i < value.size(); ++i) {
		uint32_t c = buffer[i];
		output << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << c << ", ";
		if ((++lineCount % 10) == 0) { output << "\n"; }
	}
	output << "    };\n\n";
	output << "    constexpr size_t " << constantName << "Length = sizeof(" << constantName << ");\n\n";
	output << "}";
	return output;
}

void writeFile(const std::string& filename, ostringstream& buffer) {
	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}
	file << buffer.str();
	file.close();
}

int main(int argc, char** argv)
{
	if (argc < 4) {
		cerr << "Usage: " << argv[0] << " <constant_name> <input_file> <output_file>" << endl;
		return EXIT_FAILURE;
	}

	vector<char> buffer = readFile(argv[2]);
	ostringstream output = generateHeader(argv[1], buffer);
	writeFile(string(argv[3]), output);
	return EXIT_SUCCESS;
}

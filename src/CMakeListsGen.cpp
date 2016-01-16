#include <iostream>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace boost;
using namespace filesystem;
using namespace program_options;
using namespace std;

int main(int argc, char* argv[]) {
	#pragma region CMD HELP
	options_description desc("Allowed options");
	desc.add_options()
	("help", "What?")
	("path,I", value<string>(), "Path to dir with source code")
	;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (argc < 2) {
		cout << "This program use for generating CMakeLists.txt with source code." << "\n\n";
		cout << desc << "\n";

		return 1;
	}

	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}

	if (vm.count("path")) {
		cout << "Paths are: "
			 << vm["path"].as<string>() << "\n";
	}

	#pragma endregion

	path dir = vm["path"].as<string>();
	if (!dir.empty()) {
		set<string> sources, dirs;
		vector<string> exts({"c", "cpp", "cxx", "h", "hpp"});

		for (recursive_directory_iterator it(dir); it != recursive_directory_iterator(); ++it) {
			auto currentItem = it->path();

			auto m_dir = currentItem.parent_path().string();
			replace(m_dir.begin(), m_dir.end(), '\\', '/');
			dirs.insert(m_dir);

			vector<string> details;
			split(details, currentItem.string(), is_any_of("."));

			for (auto pos = details.begin(); pos != details.end(); ++pos)
				if (find(exts.begin(), exts.end(), *pos) != exts.end()) {
					auto m_file = it->path().string();
					replace(m_file.begin(), m_file.end(), '\\', '/');
					sources.insert(m_file);
				}
		}

		auto cmakeFile = current_path() / "CMakeLists.txt";
		ofstream fout(cmakeFile.string());

		vector<string> details;
		split(details, dir.string(), is_any_of("\\"));
		auto projectName = details[details.size() - 1];

		fout << "cmake_minimum_required(VERSION 2.8)" << endl;
		fout << "project(" << projectName << ")" << endl << endl;

		fout << "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=c++11\")" << endl << endl;

		fout << "include_directories(";
		for (auto path : dirs)
			fout << "  " << path << endl;
		fout << ")" << endl << endl;


		fout << "set(SOURCE_FILES";
		for (auto file : sources)
			fout << "  " << file << endl;
		fout << ")" << endl << endl;

		fout << "add_executable(" << projectName << " ${SOURCE_FILES})" << endl;

		cout << "Done!" << endl;
	}

	int _what;
	cin >> _what;

	return 0;
}


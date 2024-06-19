#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <queue>
#include <regex>

#include "thread_pool.h"

struct GrepTask : public MT::Task {
	std::vector<std::string> Lines;
	std::string Filename;
	std::regex Expression;

	GrepTask(
		const std::string& id, 
		std::string filename, 
		std::regex expression
	) 
		: Task(id)
		, Filename(std::move(filename))
		, Expression(std::move(expression))
	{
	};

	void one_thread_method() override {
		std::ifstream input(Filename);
		long line_index = 0;
		for(std::string line; getline(input, line);) {
			line_index++;
			if (std::regex_search(line, Expression)) {
				Lines.push_back(Filename + " at line: " + std::to_string(line_index));
			}
		}
	}
};

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	if (argc != 3) {
		std::cout << "usage: ./grep [pattern] [directory_path]" << std::endl;
		return 0;
	}

	MT::ThreadPool thread_pool(10);
	thread_pool.start();


	std::string text_expression(argv[1]);
	std::regex expression(text_expression);
	std::string path(argv[2]);
	std::vector<MT::task_id> tasks;
	for (const auto& entry : fs::recursive_directory_iterator(path)) {
		std::string filename = entry.path();
		tasks.push_back(
			thread_pool.add_task(
				GrepTask(filename + " " + text_expression, filename, expression)
			)
		);
	}

	thread_pool.wait();
	for (auto& task: tasks) {
        auto result = thread_pool.get_result<GrepTask>(task);
		for (auto& item: result->Lines) {
			std::cout << item << std::endl;
		}
	}
}

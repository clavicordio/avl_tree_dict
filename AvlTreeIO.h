#pragma once
#include "AvlTree.h"
#include <fstream>
#include <stdexcept>

namespace avl_tree { class AvlTree; }

namespace avl_tree_io
{
	class IO
	{
	public:
		static avl_tree::AvlTree ReadParametersFromFile(std::ifstream& fs);
		static void PrintTree(const avl_tree::AvlTree& tree);
		static void PrintDict(const avl_tree::AvlTree& tree);
	};

	class TreeReadError : public std::runtime_error
	{
	public:
		TreeReadError(const char* m) : std::runtime_error(m) { };
	};
} // namespace




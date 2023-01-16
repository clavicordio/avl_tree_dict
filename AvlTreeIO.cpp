#include "AvlTreeIO.h"
#include <string>
#include <sstream>
#include <iostream>

namespace avl_tree_io
{
	static std::string strip(std::string s)
	{
		unsigned int ltrim_pos = 0;
		while (std::isspace(s[ltrim_pos]))
			++ltrim_pos;

		unsigned int rtrim_pos = s.size() - 1;
		while (std::isspace(s[rtrim_pos]))
			--rtrim_pos;

		return s.substr(ltrim_pos, rtrim_pos);
	}

	avl_tree::AvlTree IO::ReadParametersFromFile(std::ifstream& fs)
	{
		avl_tree::AvlTree parameters;
		std::string line;
		while (std::getline(fs, line))
		{
			std::istringstream ss(line);
			std::string value_type;
			std::string key;
			
			ss >> value_type;
			ss >> key;
			
			if (value_type == "int")
			{
				int value;
				ss >> value;
				parameters[key] = value;
			}
			else if (value_type == "double")
			{
				double value;
				ss >> value;
				parameters[key] = value;
			}
			else if (value_type == "string")
			{
				unsigned int stream_pos = ss.tellg();
				std::string value = strip(line.substr(stream_pos));
				parameters[key] = value;
			}
			else if (value_type == "intarray")
			{
				std::vector<int> value;
				unsigned int len;

				ss >> len;
				for (unsigned int i = 0; i < len; ++i)
				{
					int x;
					ss >> x;
					value.push_back(x);
				}
				parameters[key] = value;
			}
			else if (value_type == "doublearray")
			{
				std::vector<double> value;
				unsigned int len;

				ss >> len;
				for (unsigned int i = 0; i < len; ++i)
				{
					double x;
					ss >> x;
					value.push_back(x);
				}
				parameters[key] = value;
			}
			else
			{
				throw TreeReadError("Can't parse data type.");
			}
		}
		return parameters;
	}

	static std::string toString(avl_tree::TValue value)
	{
		try
		{
			auto value_int = std::get<int>(value);
			return std::to_string(value_int);
		}
		catch (std::bad_variant_access) {}

		try
		{
			auto value_double = std::get<double>(value);
			return std::to_string(value_double);
		}
		catch (std::bad_variant_access) {}

		try
		{
			std::vector<int>& value_int_vector = std::get< std::vector<int> >(value);
			std::string string_representation = "[ ";
			for (auto x : value_int_vector)
			{
				string_representation += std::to_string(x) + " ";
			}
			string_representation += "]";
			return string_representation;
		}
		catch (std::bad_variant_access) {}

		try
		{
			std::vector<double>& value_double_vector = std::get< std::vector<double> >(value);
			std::string string_representation = "[ ";
			for (auto x : value_double_vector)
			{
				string_representation += std::to_string(x) + " ";
			}
			string_representation += "]";
			return string_representation;
		}
		catch (std::bad_variant_access) {}

		try
		{
			return std::get<std::string>(value);
		}
		catch (std::bad_variant_access) {}

		return "(empty)";

	}

	static void printBT(const std::string& prefix, const std::shared_ptr<avl_tree::AvlTreeNode>& node, bool isLeft)
	{
		if (node != nullptr)
		{
			std::cout << prefix;
			std::cout << (isLeft ? "|--" : "L--");
			std::cout << "" << node->key;
			if (!std::holds_alternative<std::monostate>(node->value))
				std::cout << ":" << toString(node->value);
			std::cout << std::endl;

			if (node->right != nullptr || node->left != nullptr)
			{
				printBT(prefix + (isLeft ? "|   " : "    "), node->right, true);
				printBT(prefix + (isLeft ? "|   " : "    "), node->left, false);
			}
		}
		else
		{
			std::cout << prefix;
			std::cout << (isLeft ? "|--" : "L--");
			std::cout << std::endl;
		}
	}
	
	void IO::PrintTree(const avl_tree::AvlTree& tree)
	{
		printBT("", tree.root_node_, false);
	}

	void IO::PrintDict(const avl_tree::AvlTree& tree)
	{
		//std::string s;
		std::cout << "{\n";
		for (std::string key : tree.keys())
		{
			std::cout << "  " + key + " : " + toString(tree.get(key)) + "\n";
		}
		std::cout << "}\n";
	}


} // namespace
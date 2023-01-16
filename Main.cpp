#include <iostream>
#include "AvlTree.h"
#include "AvlTreeIO.h"


void AvlTreeDemo()
{
	avl_tree::AvlTree tree;
	for (auto key : { "Ca", "De", "Ef", "G", "He", "Af", "To", "Do", "Ba", "Foo", "Sa", "Pa", "Za", "Oo" })
	{
		std::cout << "Inserting " << key << "...\n";
		tree[key];
	}
	avl_tree_io::IO::PrintTree(tree);

	for (auto key : { "Za", "To", "Ef", "G" , "Ba", "Pa", "Foo", "Do", "Oo", "De", "Af", "Ca", "Sa", "He" })
	{
		std::cout << "Deleting " << key << " ...\n";
		tree.pop(key);
		avl_tree_io::IO::PrintTree(tree);
		std::cout << std::endl;
	}
}

void MapDemo()
{
	avl_tree::AvlTree map;
	map["hello"] = 10;
	map["salam"] = std::vector<int>({ 10,20,30,50 });
	map["privet"] = 300;
	map["hola"] = "sinhrofazatron";
	map["bonjour"] = std::vector<double>({ 12,14,15,16 });
	map.pop("salam");

	try
	{
		map.pop("ohlala");
	}
	catch (avl_tree::KeyDoesntExistError& e)
	{
		std::cout << e.what() << "\n";
	}

	avl_tree_io::IO::PrintDict(map);
}

void ParametersDemo()
{
	auto fs = std::ifstream("parameters.txt");
	auto parameters = avl_tree_io::IO::ReadParametersFromFile(fs);

	avl_tree_io::IO::PrintDict(parameters);
}

int main()
{
	//AvlTreeDemo();
	//MapDemo();
	ParametersDemo();
}

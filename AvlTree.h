#pragma once

#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace avl_tree_io {	class IO; }

namespace avl_tree 
{
	using TValue = std::variant<std::monostate, int, double, std::vector<int>, std::vector<double>, std::string>;
	std::string toString(TValue);

	struct AvlTreeNode
	{
		AvlTreeNode(std::string key, TValue value = TValue()) : key(key), value(value) {};

		std::string key;
		TValue value;

		std::shared_ptr<AvlTreeNode> left = nullptr;
		std::shared_ptr<AvlTreeNode> right = nullptr;

		unsigned int height = 0;
	};

	
	class AvlTree
	{
		friend class avl_tree_io::IO;

	public:

		TValue& operator[] (const std::string& key);
		TValue const& get(const std::string& key) const;
		TValue pop(const std::string& key);

		std::vector<std::string> keys() const;

	private:
		std::shared_ptr<AvlTreeNode> Insert_(const std::string& key, const TValue& value = TValue());
		std::shared_ptr<AvlTreeNode> Search_(const std::string& key) const;
		TValue Delete_(const std::string& key);

		std::shared_ptr<AvlTreeNode> root_node_ = nullptr;
		
	};

	struct KeyDoesntExistError : public std::runtime_error
	{
		KeyDoesntExistError(const std::string& key) : std::runtime_error("Key " + key + " doesn't exist.") { this->key = key; };
		std::string key;
	};

	struct KeyAlreadyExistsError : public std::runtime_error
	{
		KeyAlreadyExistsError(const std::string& key) : std::runtime_error("Key " + key + " already exists.") { this->key = key; };
		std::string key;
	};

} // namespace avl_tree
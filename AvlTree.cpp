#include "AvlTree.h"
#include <algorithm>
#include <cstdlib>
#include <assert.h>

namespace avl_tree
{

    static std::shared_ptr<AvlTreeNode> insertNode(std::shared_ptr<AvlTreeNode>* node, const std::string& key, const TValue& value);
    static TValue deleteNode(std::shared_ptr<AvlTreeNode>* node, const std::string& key);
    static void doBalancing(std::shared_ptr<AvlTreeNode>* node);

    static void rotateLeft(std::shared_ptr<AvlTreeNode>* node);
    static void rotateLeftBig(std::shared_ptr<AvlTreeNode>* node);
    static void rotateRight(std::shared_ptr<AvlTreeNode>* node);
    static void rotateRightBig(std::shared_ptr<AvlTreeNode>* node);

    static inline unsigned int getHeight(const std::shared_ptr<AvlTreeNode>& node);
    static inline signed int getBalanceFactor(const std::shared_ptr<AvlTreeNode>& node);
    static inline void updateHeight(std::shared_ptr<AvlTreeNode>& node);

    static inline unsigned int getHeight(const std::shared_ptr<AvlTreeNode>& node)
    {
        return (node == nullptr) ? 0 : node->height;
    }

    static inline signed int getBalanceFactor(const std::shared_ptr<AvlTreeNode>& node)
    {
        int left_height = getHeight(node->left);
        int right_height = getHeight(node->right);
        return right_height - left_height;
    }

    static inline void updateHeight(std::shared_ptr<AvlTreeNode>& node)
    {
        auto left_height = getHeight(node->left);
        auto right_height = getHeight(node->right);
        node->height = 1 + std::max(left_height, right_height);
    }

    static void doBalancing(std::shared_ptr<AvlTreeNode>* node)
    {
        int balance_factor = getBalanceFactor(*node);
        assert(std::abs(balance_factor) <= 2);

        if (balance_factor == 2)
        {
            std::shared_ptr<AvlTreeNode> C = (*node)->right->left;
            std::shared_ptr<AvlTreeNode> R = (*node)->right->right;

            if (getHeight(C) <= getHeight(R))
                rotateLeft(node);
            else
                rotateLeftBig(node);
        }
        else if (balance_factor == -2)
        {
            std::shared_ptr<AvlTreeNode> C = (*node)->left->right;
            std::shared_ptr<AvlTreeNode> L = (*node)->left->left;

            if (getHeight(C) <= getHeight(L))
                rotateRight(node);
            else
                rotateRightBig(node);
        }

        assert(std::abs(getBalanceFactor(*node)) <= 1);
    }

    static void rotateLeft(std::shared_ptr<AvlTreeNode>* node)
    {
        std::shared_ptr<AvlTreeNode> A = (*node);
        std::shared_ptr<AvlTreeNode> B = (*node)->right;
        std::shared_ptr<AvlTreeNode> C = (*node)->right->left;

        A->right = C;
        B->left = A;
        *node = B;

        updateHeight(A);
        updateHeight(B);
    }

    static void rotateLeftBig(std::shared_ptr<AvlTreeNode>* node)
    {
        std::shared_ptr<AvlTreeNode> A = (*node);
        std::shared_ptr<AvlTreeNode> B = (*node)->right;
        std::shared_ptr<AvlTreeNode> C = (*node)->right->left;

        std::shared_ptr<AvlTreeNode> M = C->left;
        std::shared_ptr<AvlTreeNode> N = C->right;

        A->right = M;
        B->left = N;
        C->left = A;
        C->right = B;
        (*node) = C;

        updateHeight(A);
        updateHeight(B);
        updateHeight(C);
    }

    static void rotateRight(std::shared_ptr<AvlTreeNode>* node)
    {
        std::shared_ptr<AvlTreeNode> A = (*node);
        std::shared_ptr<AvlTreeNode> B = (*node)->left;
        std::shared_ptr<AvlTreeNode> C = (*node)->left->right;

        A->left = C;
        B->right = A;
        (*node) = B;

        updateHeight(A);
        updateHeight(B);
    }

    static void rotateRightBig(std::shared_ptr<AvlTreeNode>* node)
    {
        std::shared_ptr<AvlTreeNode> A = (*node);
        std::shared_ptr<AvlTreeNode> B = (*node)->left;
        std::shared_ptr<AvlTreeNode> C = (*node)->left->right;

        std::shared_ptr<AvlTreeNode> M = C->left;
        std::shared_ptr<AvlTreeNode> N = C->right;

        A->left = N;
        B->right = M;
        C->left = B;
        C->right = A;

        (*node) = C;

        updateHeight(A);
        updateHeight(B);
        updateHeight(C);
    }

    static std::shared_ptr<AvlTreeNode> insertNode(std::shared_ptr<AvlTreeNode>* node, const std::string& key, const TValue& value)
    {
        if (*node == nullptr)
        {
            (*node) = std::make_shared<AvlTreeNode>(key, value);
            (*node)->height = 1;
            return *node;
        }

        std::shared_ptr<AvlTreeNode> new_node;
        {
            auto compare_result = (*node)->key.compare(key);

            if (compare_result < 0)
                new_node = insertNode(&((*node)->left), key, value);
            else if (compare_result > 0)
                new_node = insertNode(&((*node)->right), key, value);
            else
                throw KeyAlreadyExistsError(key);
        }

        updateHeight(*node);
        doBalancing(node);

        if ((*node)->left && (*node)->right)
            assert((*node)->left->key.compare((*node)->right->key) > 0);

        return new_node;
    }

    static TValue deleteNode(std::shared_ptr<AvlTreeNode>* node, const std::string& key)
    {
        TValue deleted_node_value;

        if ((*node) == nullptr)
        {
            throw KeyDoesntExistError(key);
        }

        auto compare_result = (*node)->key.compare(key);

        if (compare_result < 0)
            deleted_node_value = deleteNode(&((*node)->left), key);
        else if (compare_result > 0)
            deleted_node_value = deleteNode(&((*node)->right), key);
        else
        {
            // match
            if ((*node)->left == nullptr && (*node)->right == nullptr)
            {
                deleted_node_value = (*node)->value;
                *node = nullptr;
                return deleted_node_value;
            }
            else if ((*node)->right != nullptr)
            {
                std::shared_ptr<AvlTreeNode>* node_next = &((*node)->right);
                while ((*node_next)->left != nullptr)
                    node_next = &((*node_next)->left);

                std::string next_key = (*node_next)->key;
                TValue next_value = (*node_next)->value;

                deleted_node_value = deleteNode(&((*node)->right), next_key);
                (*node)->key = next_key;
                (*node)->value = next_value;
            }
            else
            {
                std::shared_ptr<AvlTreeNode>* node_prev = &((*node)->left);
                while ((*node_prev)->right != nullptr)
                    node_prev = &((*node_prev)->right);

                std::string prev_key = (*node_prev)->key;
                TValue prev_value = (*node_prev)->value;

                deleted_node_value = deleteNode(&((*node)->left), prev_key);
                (*node)->key = prev_key;
                (*node)->value = prev_value;
            }
        }

        updateHeight(*node);
        doBalancing(node);

        if ((*node)->left && (*node)->right)
            assert((*node)->left->key.compare((*node)->right->key) > 0);

        return deleted_node_value;
    }

    std::shared_ptr<AvlTreeNode> AvlTree::Insert_(const std::string& key, const TValue& value)
    {
        return insertNode(&root_node_, key, value);
    }

    TValue AvlTree::Delete_(const std::string& key)
    {
        return deleteNode(&root_node_, key);
    }

    std::shared_ptr<AvlTreeNode> AvlTree::Search_(const std::string& key) const
    {
        std::shared_ptr<AvlTreeNode> node = root_node_;
        while (node != nullptr)
        {
            auto compare_result = node->key.compare(key);
            if (compare_result == 0)
                return node;
            else if (compare_result < 0)
                node = node->left;
            else
                node = node->right;
        }
        return nullptr;

    }

    TValue AvlTree::get(const std::string& key) const
    {
        auto search_result = Search_(key);

        if (search_result == nullptr)
            throw KeyDoesntExistError(key);

        return search_result->value;
    }

    TValue& AvlTree::operator[](const std::string& key)
    {
        auto search_result = Search_(key);

        if (search_result == nullptr)
            search_result = Insert_(key);

        return search_result->value;
    }

    TValue AvlTree::pop(const std::string& key)
    {
        return Delete_(key);
    }

    static void findKeys(std::shared_ptr<AvlTreeNode> node, std::vector<std::string>& keys)
    {
        if (node == nullptr)
            return;
        keys.push_back(node->key);
        findKeys(node->left, keys);
        findKeys(node->right, keys);
    }

    std::vector<std::string> AvlTree::keys() const
    {
        std::vector<std::string> keys;
        findKeys(root_node_, keys);
        return keys;
    }
}
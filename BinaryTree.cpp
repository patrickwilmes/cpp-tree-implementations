#include <memory>
#include <iostream>
#include <queue>

template<typename T>
class TreeNode {
public:
    explicit TreeNode(T data) : _data(data) {}

    std::shared_ptr<TreeNode> &getLeft() {
        return _left;
    }

    std::shared_ptr<TreeNode> &getRight() {
        return _right;
    }

    void setRight(const std::shared_ptr<TreeNode<T>> &data) {
        _right = data;
    }

    void setLeft(const std::shared_ptr<TreeNode<T>> &data) {
        _left = data;
    }

    T getData() {
        return _data;
    }

    void setData(T data) {
        _data = data;
    }

private:
    T _data{};
    std::shared_ptr<TreeNode> _left = nullptr;
    std::shared_ptr<TreeNode> _right = nullptr;
};

enum TraversalStrategy {
    INORDER,
    PREORDER,
    POSTORDER,
    LEVELORDER,
};

template<typename T>
class BinaryTree {
public:
    void traverse(TraversalStrategy strategy) {
        switch (strategy) {
            case INORDER:
                std::cout << "INORDER\n" << std::flush;
                printInorder(_rootNode);
                std::cout << "\n" << std::flush;
                break;
            case PREORDER:
                std::cout << "PREORDER\n" << std::flush;
                printPreorder(_rootNode);
                std::cout << "\n" << std::flush;
                break;
            case POSTORDER:
                std::cout << "POSTORDER\n" << std::flush;
                printPostorder(_rootNode);
                std::cout << "\n" << std::flush;
                break;
            case LEVELORDER:
                std::cout << "LEVELORDER\n" << std::flush;
                printLevelorder(_rootNode);
                std::cout << "\n" << std::flush;
                break;
        }
    }

    std::shared_ptr<TreeNode<T>> find(T value) {
        std::shared_ptr<TreeNode<T>> parent = _rootNode;
        if (parent == nullptr)
            return nullptr;
        std::shared_ptr<TreeNode<T>> child;
        if (parent->getData() < value) {
            child = parent->getRight();
        } else if (parent->getData() > value) {
            child = parent->getLeft();
        } else {
            return parent;
        }
        while (child != nullptr) {
            if (child->getData() == value) {
                return child;
            }
            if (child->getData() > value) {
                child = child->getLeft();
            } else {
                child = child->getRight();
            }
        }
        return nullptr;
    }

    std::shared_ptr<TreeNode<T>> remove(T value) {
        _rootNode = removeNode(_rootNode, value);
        return _rootNode;
    }

    bool insert(T value) {
        std::shared_ptr<TreeNode<T>> parent = _rootNode;
        std::shared_ptr<TreeNode<T>> node = std::make_shared<TreeNode<T>>(value);
        // if the parent is null, we just set the current node as root
        if (parent == nullptr) {
            _rootNode = node;
            return true;
        }
        // initially we should check the direction we should go down. Otherwise, the tree will degenerate
        // but keep in mind, that this is just preventing simple degeneration and is not balancing
        // the tree.
        std::shared_ptr<TreeNode<T>> child = nullptr;
        if (parent->getData() < value) {
            child = parent->getRight();
        } else {
            child = parent->getLeft();
        }
        while (child != nullptr) {
            parent = child;
            if (child->getData() == value) {
                return false;
            } else {
                if (child->getData() > value) {
                    child = child->getLeft();
                } else {
                    child = child->getRight();
                }
            }
        }
        if (parent->getData() > value) {
            parent->setLeft(node);
        } else {
            parent->setRight(node);
        }
        node->setLeft(nullptr);
        node->setRight(nullptr);
        return true;
    }

private:
    void printInorder(const std::shared_ptr<TreeNode<T>> &n) {
        if (n == nullptr) {
            return;
        }
        printInorder(n->getLeft());
        std::cout << n->getData() << " ";
        printInorder(n->getRight());
    }

    void printPreorder(const std::shared_ptr<TreeNode<T>> &n) {
        if (n == nullptr) {
            return;
        }
        std::cout << n->getData() << " ";
        printPreorder(n->getLeft());
        printPreorder(n->getRight());
    }

    void printPostorder(const std::shared_ptr<TreeNode<T>> &n) {
        if (n == nullptr) {
            return;
        }
        printPostorder(n->getLeft());
        printPostorder(n->getRight());
        std::cout << n->getData() << " ";
    }

    void printLevelorder(const std::shared_ptr<TreeNode<T>> &n) {
        if (n == nullptr) {
            return;
        }
        std::queue<std::shared_ptr<TreeNode<T>>> queue;
        queue.push(n);

        while (!queue.empty()) {
            std::shared_ptr<TreeNode<T>> node = queue.front();
            std::cout << node->getData() << " ";
            queue.pop();

            if (node->getLeft() != nullptr) {
                queue.push(node->getLeft());
            }

            if (node->getRight() != nullptr) {
                queue.push(node->getRight());
            }
        }
    }

    std::shared_ptr<TreeNode<T>> removeNode(std::shared_ptr<TreeNode<T>> node, T value) {
        if (node == nullptr) {
            return node;
        }
        if (value < node->getData()) {
            node->setLeft(removeNode(node->getLeft(), value));
        } else if (value > node->getData()) {
            node->setRight(removeNode(node->getRight(), value));
        } else {
            if (node->getLeft() == nullptr) {
                return node->getRight();
            } else if (node->getRight() == nullptr) {
                return node->getLeft();
            }
            node->setData(minValue(node->getRight()));
            node->setRight(removeNode(node->getRight(), node->getData()));
        }
        return node;
    }

    T minValue(std::shared_ptr<TreeNode<T>> node) {
        T minV = node->getData();
        while (node->getLeft() != nullptr) {
            minV = node->getLeft()->getData();
            node = node->getLeft();
        }
        return minV;
    }

private:
    std::shared_ptr<TreeNode<T>> _rootNode{};
};

void findValueInTree(BinaryTree<int> &binaryTree, int value);

int main() {
    BinaryTree<int> binaryTree;
    binaryTree.insert(8);
    binaryTree.insert(9);
    binaryTree.insert(4);
    binaryTree.insert(7);
    binaryTree.insert(2);
    binaryTree.remove(4);
    std::cout << "Done" << std::endl;
    findValueInTree(binaryTree, 9);
    findValueInTree(binaryTree, 4);
    binaryTree.traverse(LEVELORDER);
    binaryTree.traverse(PREORDER);
    binaryTree.traverse(POSTORDER);
    binaryTree.traverse(INORDER);
}

void findValueInTree(BinaryTree<int> &binaryTree, int value) {
    auto node = binaryTree.find(value);
    if (node == nullptr) {
        std::cout << "Element not found" << std::endl;
    } else {
        std::cout << "Node value: " << node->getData() << std::endl;
    }
}

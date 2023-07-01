#include <string>
#include <vector>

class TreeNode
{
public:
    int key;
    std::string data;
    TreeNode* left;
    TreeNode* right;
};

class BinarySearchTree
{

public:

    BinarySearchTree() : m_root(nullptr) {};

    BinarySearchTree(std::vector<std::pair<int,std::string>> temp_vec)
    {
        m_root = buildBalancedBST(temp_vec, 0, temp_vec.size());
    }

    ~BinarySearchTree() {};

    void balanced()
    {
        std::vector<std::pair<int, std::string>> temp_vec;
        inorderTraversal(m_root, temp_vec);
        m_root = help_balanced(temp_vec, 0, temp_vec.size());
    }

    void add(int key, std::string value)
    {
        m_root = help_add(m_root, key, value);
    }

    std::string find(int key)
    {
        TreeNode* node = help_find(m_root, key);

        if (node == nullptr){
            return "";
        }
        else {
            return node->data;
        }
    }

    void deleter(int key){
        m_root = help_deleter(m_root, key);
    }

    std::vector<std::pair<int, std::string>> printStraight()
    {
        std::vector<std::pair<int, std::string>> temp_vec;
        help_printStraight(m_root, temp_vec);
        return temp_vec;
    }
    std::vector<std::pair<int, std::string>> printBackward()
    {
        std::vector<std::pair<int, std::string>> vec;
        help_printBackward(m_root, vec);
        return vec;
    }
    std::vector<std::pair<int, std::string>> printAscending()
    {
        std::vector<std::pair<int, std::string>> vec;
        inorderTraversal(m_root, vec);
        return vec;
    }

protected:

    TreeNode* m_root;

    TreeNode* buildBalancedBST(std::vector<std::pair<int, std::string>>& temp_vec, int start, int end)
    {
        if (start >= end)
        {
            return nullptr;
        }

        size_t mid = (start + end) / 2;

        TreeNode* root = new TreeNode();
        root->key = temp_vec[mid].first;
        root->data = temp_vec[mid].second;
        root->left = buildBalancedBST(temp_vec, start, mid);
        root->right = buildBalancedBST(temp_vec, mid + 1, end);

        return root;
    }

    TreeNode* help_balanced(std::vector<std::pair<int, std::string>>& vec, int start, int end)
    {
        if (start >= end)
        {
            return nullptr;
        }

        int mid = (start + end) / 2;

        TreeNode* root = new TreeNode();
        root->key = vec[mid].first;
        root->data = vec[mid].second;
        root->left = help_balanced(vec, start, mid);
        root->right = help_balanced(vec, mid + 1, end);

        return root;
    }

    TreeNode* help_add(TreeNode* node,int key, std::string value)
    {
        if (node == nullptr)
        {
            TreeNode* newNode = new TreeNode();
            newNode->key = key;
            newNode->data = value;
            return newNode;
        }

        if (key < node->key)
        {
            node->left = help_add(node->left, key, value);
        }
        else
        {
            if (key > node->key)
            {
                node->right = help_add(node->right, key, value);
            }
            else
            {
                node->data = value;
            }
        }

        return node;
    }

    TreeNode* help_find(TreeNode* node, int key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (key < node->key)
        {
            return help_find(node->left, key);
        }
        else
        {
            if (key > node->key)
            {
                return help_find(node->right, key);
            }
            else
            {
                return node;
            }
        }
    }

    TreeNode* help_deleter(TreeNode* node, int key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (key < node->key)
        {
            node->left = help_deleter(node->left, key);
        }
        else
        {
            if (key > node->key)
            {
                node->right = help_deleter(node->right, key);
            }
            else
            {
                if (node->left == nullptr && node->right == nullptr)
                {
                    delete node;
                    return nullptr;
                }
                else
                {
                    if (node->left == nullptr)
                    {
                        TreeNode* temp = node->right;
                        delete node;
                        return temp;
                    }
                    else
                    {
                        if (node->right == nullptr)
                        {
                            TreeNode* temp = node->left;
                            delete node;
                            return temp;
                        }
                        else
                        {
                            TreeNode* minRight = findMin(node->right);

                            node->key = minRight->key;
                            node->data = minRight->data;
                            node->right = help_deleter(node->right, minRight->key);
                        }
                    }
                }
            }
        }
        return node;
    }

    void help_printStraight(TreeNode* node, std::vector<std::pair<int, std::string>>& temp_vec)
    {
        if (node == nullptr)
        {
            return;
        }

        temp_vec.push_back({ node->key, node->data });

        help_printStraight(node->left, temp_vec);

        help_printStraight(node->right, temp_vec);
    }

    void help_printBackward(TreeNode* node, std::vector<std::pair<int, std::string>>& temp_vec)
    {
        if (node == nullptr)
        {
            return;
        }

        help_printBackward(node->right, temp_vec);

        temp_vec.push_back({ node->key, node->data });

        help_printBackward(node->left, temp_vec);
    }

    void inorderTraversal(TreeNode* node, std::vector<std::pair<int, std::string>>& temp_vec)
    {
        if (node == nullptr)
        {
            return;
        }

        inorderTraversal(node->left, temp_vec);

        temp_vec.push_back({ node->key, node->data });

        inorderTraversal(node->right, temp_vec);
    }

    TreeNode* findMin(TreeNode* node)
    {
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
        }

        return node;
    }

    int height(TreeNode* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return (1 + std::max(height(node->left), height(node->right)));
    }

};


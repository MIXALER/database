#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

int N = 3;
long long max_val = 1e9 + 1;

typedef struct node
{
    vector<int> keys;
    vector<node *> pointers;
    bool isLeaf;
    vector<int> counts;
} Node;


Node *create_node(bool isLeaf)
{
    Node *node = (Node *) malloc(sizeof(Node));
    node->isLeaf = isLeaf;
    if (isLeaf)
        node->pointers.push_back(NULL);
    return node;
}

Node *insert_value(Node *node, int val)
{
    if (node->isLeaf)
    {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), val);

        if (node->keys.size() != 0 and *it == val)
        {
            node->counts[it - node->keys.begin()]++;
        } else
        {
            it = node->keys.insert(it, val);
            auto itc = node->counts.begin() + (it - node->keys.begin());
            node->counts.insert(itc, 1);
        }
        Node *retnode = NULL;
        if (node->keys.size() > N)
        {
            retnode = create_node(true);
            int num = ceil((float) (N + 1) / (float) 2);
            it = node->keys.begin();
            while (num--)
            {
                it++;
            }
            while (it != node->keys.end())
            {
                retnode->keys.push_back(*it);
                node->keys.erase(it);
                auto itc = node->counts.begin() + (it - node->keys.begin());
                retnode->counts.push_back(*itc);
                node->counts.erase(itc);
            }
            retnode->pointers[0] = node->pointers[0];
            node->pointers[0] = retnode;
        }
        return retnode;
    }
    auto it = upper_bound(node->keys.begin(), node->keys.end(), val);
    auto itp = node->pointers.begin() + (it - node->keys.begin());
    Node *retnode = insert_value(node->pointers[itp - node->pointers.begin()], val);
    if (retnode)
    {
        node->keys.insert(it, retnode->keys.front());
        node->pointers.insert(++itp, retnode);
        if (retnode->isLeaf != true)
        {
            retnode->keys.erase(retnode->keys.begin());
        }
        Node *newnode = NULL;
        if (node->keys.size() > N)
        {
            newnode = create_node(false);
            int num = ceil((float) N / (float) 2);
            it = node->keys.begin();
            while (num--)
            {
                it++;
            }
            while (it != node->keys.end())
            {
                newnode->keys.push_back(*it);
                node->keys.erase(it);
            }
            num = ceil((float) (N + 2) / (float) 2);
            itp = node->pointers.begin();
            while (num--)
            {
                itp++;
            }
            while (itp != node->pointers.end())
            {
                newnode->pointers.push_back(*itp);
                node->pointers.erase(itp);
            }
        }
        return newnode;
    }
    return retnode;
}

Node *insert(Node *root, int val)
{
    Node *retnode = insert_value(root, val);
    if (retnode)
    {
        Node *node = create_node(false);
        node->keys.push_back(retnode->keys.front());
        node->pointers.push_back(root);
        node->pointers.push_back(retnode);
        if (retnode->isLeaf != true)
        {
            retnode->keys.erase(retnode->keys.begin());
        }
        root = node;
    }
    return root;
}

Node *search(Node *curr, int val)
{
    if (curr->isLeaf)
    {
        if (binary_search(curr->keys.begin(), curr->keys.end(), val))
        {
            cout << "YES" << endl;
        } else
        {
            cout << "NO" << endl;
        }
        return curr;
    }
    auto it = upper_bound(curr->keys.begin(), curr->keys.end(), val);
    curr = curr->pointers[it - curr->keys.begin()];
    return search(curr, val);
}

int count(Node *node, int val)
{
    if (node->isLeaf)
    {
        auto it = find(node->keys.begin(), node->keys.end(), val);
        if (it != node->keys.end())
        {
            auto itc = node->counts.begin() + (it - node->keys.begin());
            return node->counts[itc - node->counts.begin()];
        } else
            return 0;
    }
    auto it = upper_bound(node->keys.begin(), node->keys.end(), val);
    return count(node->pointers[it - node->keys.begin()], val);
}

int range(Node *node, int x, int y)
{
    if (node->isLeaf)
    {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), x);
        int total = 0;
        while (*it <= y)
        {
            auto itc = node->counts.begin() + (it - node->keys.begin());
            total += *itc;
            it++;
            if (it == node->keys.end())
            {
                if (node->pointers[0] == NULL)
                    break;
                node = node->pointers[0];
                it = node->keys.begin();
            }
        }
        return total;
    }
    auto it = upper_bound(node->keys.begin(), node->keys.end(), x);
    return range(node->pointers[it - node->keys.begin()], x, y);
}

void print(node *node, int level)
{
    cout << "Level " << level << ":";
    for (int i = 0; i < node->keys.size(); i++)
    {
        cout << node->keys[i] << " ";
    }
    cout << endl;
    if (node->isLeaf == false)
    {
        for (int i = 0; i < node->pointers.size(); i++)
            print(node->pointers[i], level + 1);
    }
}

void printMenu()
{
    cout << endl;
    cout << "1:????????????;" << endl;
    cout << "2:????????????;" << endl;
    cout << "3:???????????????;" << endl;
    cout << "4:?????? b+ ???;" << endl;
    cout << "0:??????" << endl;
}

int main()
{
    cout << "????????? b+ ?????????????????????, b+ ????????????????????? " << N << endl;
    Node *root = create_node(true);
    printMenu();
    int x, y;
    int cmd;
    int ans;

    while (cin >> cmd)
    {
        switch (cmd)
        {
            case 1:
                cout << "????????? key ???: ";
                cin >> x;
                root = insert(root, x);
                break;
            case 2:
                cout << "????????????????????? key ??????";
                cin >> x;
                ans = count(root, x);
                if (ans)
                    cout << "??? b+ ??????";
                else
                    cout << "?????? b+ ??????";
                break;
            case 3:
                cout << "??????????????????????????? x y ???";
                cin >> x >> y;
                ans = range(root, x, y);

                cout << "???????????? key ???????????? " << ans << endl;
                break;
            case 4:
                print(root, 0);
                break;
            case 0:
                print(root, 0);
                return 0;
            default:
                cout << "invalid cmmand!" << endl;
                break;
        }
        printMenu();
    }
    return 0;
}
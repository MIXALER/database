#include <limits>
#include <vector>
#include <iostream>

using namespace std;

int hashval = 2;
int R = 0;
int N = 0;
const float threshold = 0.75;
const int num_entries = 256;

typedef struct bucket
{
    int entries[num_entries];
    bucket *overflow;
} Bucket;

vector<Bucket *> hashtable;

Bucket *create_bucket()
{
    Bucket *temp = (Bucket *) malloc(sizeof(Bucket));
    for (int i = 0; i < num_entries; i++)
        temp->entries[i] = INT32_MIN;
    temp->overflow = NULL;
    N++;
    return temp;
}

int nearest_power(int n)
{
    int p = 2;
    while (p <= n)
        p *= 2;
    if (p > n)
        p /= 2;
    return p;
}

void print()
{
    for (int i = 0; i < N; i++)
    {
        cout << "Bucket " << i << ": ";
        Bucket *curr = hashtable[i];
        while (curr)
        {
            for (int j = 0; j < num_entries; j++)
            {
                if (hashtable[i]->entries[j] != INT32_MIN)
                    cout << hashtable[i]->entries[j] << " ";
            }
            curr = curr->overflow;
        }
        cout << endl;
    }
}

int search(int val)
{
    int x = val % hashval;
    if (x < 0)
    {
        x += hashval;
    }

    if (x >= hashtable.size())
    {
        x -= hashval / 2;
    }

    return x;
}

bool count(int val)
{
    int bucket_num = search(val);
    Bucket *curr = hashtable[bucket_num];
    while (curr)
    {
        for (int i = 0; i < num_entries; i++)
        {
            if (curr->entries[i] == val)
            {
                return true;
            }
        }
        curr = curr->overflow;
    }
    return false;
}

void insert_val(int bucket_num, int val)
{
    Bucket *curr = hashtable[bucket_num];
    int ins = 0;
    while (curr)
    {
        for (int i = 0; i < num_entries; i++)
        {
            if (curr->entries[i] == INT32_MIN)
            {
                curr->entries[i] = val;
                ins = 1;
                break;
            }
        }
        if (ins == 1)
            break;
        if (curr->overflow == NULL)
        {
            Bucket *temp = create_bucket();
            curr->overflow = temp;
        }
        curr = curr->overflow;
    }
    return;
}

bool del(int val)
{
    int bucket_num = search(val);
    Bucket *curr = hashtable[bucket_num];
    while (curr)
    {
        for (int i = 0; i < num_entries; i++)
        {
            if (curr->entries[i] == val)
            {
                curr->entries[i] = INT32_MIN;
                return true;
            }
        }
        curr = curr->overflow;
    }
    return false;
}


void split_bucket()
{
    int insert_at = hashtable.size();
    hashtable.push_back(create_bucket());
    int split_bucket_num = insert_at - nearest_power(insert_at);
    hashval = (hashtable.size() > hashval) ? 2 * hashval : hashval;

    Bucket *curr = hashtable[split_bucket_num];
    while (curr)
    {
        for (int i = 0; i < num_entries; i++)
        {
            int x = curr->entries[i] % hashval;
            if (x < 0)
                x += hashval;
            if (x == insert_at)
            {
                insert_val(insert_at, curr->entries[i]);
                curr->entries[i] = INT32_MIN;
            }
        }
        curr = curr->overflow;
    }
}

void insert(int val)
{
    int bucket_num = search(val);
    Bucket *curr = hashtable[bucket_num];
    while (curr)
    {
        for (int i = 0; i < num_entries; i++)
        {
            if (curr->entries[i] == val)
            {
                cout << "表中已经存在该 key 值，重复插入！" << endl;
                return;
            }
        }
        curr = curr->overflow;
    }
    insert_val(bucket_num, val);
    R++;
    float fr = (float) R / (float) (N * num_entries);
    if (fr >= threshold)
        split_bucket();
}

void printMenu()
{
    cout << endl;
    cout << "1:插入数据;" << endl;
    cout << "2:查询数据;" << endl;
    cout << "3:删除数据;" << endl;
    cout << "4:打印哈希表;" << endl;
    cout << "0:退出" << endl;
}

int main()
{
    for (int i = 0; i < 2; i++)
    {
        Bucket *temp = create_bucket();
        hashtable.push_back(temp);
    }
    cout << "初始化哈希表，表长度为 " <<  hashval << " 默认桶大小为 "  << num_entries << " 默认分裂阈值为 " << threshold << endl;
    printMenu();
    int cmd;
    int key;
    bool res;
    while (cin >> cmd)
    {
        switch (cmd)
        {
            case 1:
                cout << "请输入 key 值: ";
                cin >> key;
                insert(key);
                break;
            case 2:
                cout << "请输入待查询的 key 值：";
                cin >> key;
                res = count(key);
                if (res)
                    cout << "在哈希表中";
                else
                    cout << "不在哈希表中";
                break;
            case 3:
                cout << "请输入待删除的 key 值：";
                cin >> key;
                res = del(key);
                if (res)
                {
                    cout << "删除值： " << key << "成功！";
                } else
                {
                    cout << "错误！ 值："  <<key << "不在哈希表中！" <<endl;
                }
                break;
            case 4:
                print();
                break;
            case 0:
                print();
                return 0;
            default:
                cout << "invalid command!" << endl;
                break;
        }
        printMenu();
    }
    return 0;
}
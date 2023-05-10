#define _CRT_SECURE_NO_WARNINGS 1
#include "fixed_memory.h"
#include <vector>
using namespace std;

void Test_FixedMemory()
{
    struct TreeNode
    {
        int _val;
        TreeNode* _left;
        TreeNode* _right;
        TreeNode()
            :_val(0)
            , _left(nullptr)
            , _right(nullptr)
        {}
    };


    // 申请释放的轮次
    const size_t Rounds = 6;
    // 每轮申请释放多少次
    const size_t N = 100000;


    size_t begin1 = clock();
    std::vector<TreeNode*> v1;
    v1.reserve(N);
    for (size_t j = 0; j < Rounds; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            v1.push_back(new TreeNode);
        }
        for (int i = 0; i < N; ++i)
        {
            delete v1[i];
        }
        v1.clear();
    }
    size_t end1 = clock();


    cjj_memory_pool::FixedMemoryPool<TreeNode> TNPool;
    size_t begin2 = clock();
    std::vector<TreeNode*> v2;
    v2.reserve(N);
    for (size_t j = 0; j < Rounds; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            v2.push_back(TNPool.New());
        }
        for (int i = 0; i < 100000; ++i)
        {
            TNPool.Delete(v2[i]);
        }
        v2.clear();
    }
    size_t end2 = clock();
    cout << "new time:" << end1 - begin1 << endl;
    cout << "pool time:" << end2 - begin2 << endl;
}




int main()
{
    Test_FixedMemory();
    return 0;
}

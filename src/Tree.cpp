#include "Tree.h"

std::vector<Tree*> Tree::GetBranches(const int depth)
{
    std::vector<Tree*> output;
    std::vector<Tree*>* outptr = &output;
    outptr = GetBranches(depth, outptr);
    return output;
}

std::vector<Tree*>* Tree::GetBranches(const int depth, std::vector<Tree*>* branch)
{
    if (layer < depth)
    {
        for (int i = 0; i < n_branch; ++i)
        {
            branch = branches[i]->GetBranches(depth, branch);
        }
    }

    else
    {
        for (int i = 0; i < n_branch; ++i)
        {
            branch->push_back(branches[i]);
        }
    }

    return branch;
}

// void Tree::Branch(Position*& pos)
// {
//     branches.push_back(new Tree(pos, layer + 1));
//     ++n_branch;
// }

Tree::Tree(Position*& pos, int l) : 
layer(l),
n_branch(),
position(pos),
alive()
{
    branches = std::vector<Tree*>();
}

Tree::~Tree()
{
    if (position)
    {
        delete position;
    }

    int n_branch = branches.size();

    for (int i = 0; i < n_branch; ++i)
    {
        delete branches[i];
    }
}
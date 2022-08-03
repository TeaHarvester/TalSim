#include "Tree.h"

std::vector<Tree*> Tree::GetBranches(const int depth)
{
    std::vector<Tree*> output;
    output = GetBranches(depth, output);
    return output;
}

std::vector<Tree*>& Tree::GetBranches(const int depth, std::vector<Tree*>& branch)
{
    if (layer < depth)
    {
        for (auto branchlet = branches.begin(); branchlet < branches.end(); ++branchlet)
        {
            branch = (*branchlet)->GetBranches(depth, branch);
        }
    }

    else
    {
        size_t canopy = branches.size();
        branch.resize(branch.size() + canopy);
        auto harvest = branch.end() - canopy;

        for (auto branchlet = branches.begin(); branchlet < branches.end(); ++branchlet, ++harvest)
        {
            *harvest = *branchlet;
        }
    }

    return branch;
}

// void Tree::Branch(Position*& pos)
// {
//     branches.push_back(new Tree(pos, layer + 1));
//     ++n_branch;
// }

Tree::Tree(Position& pos, int l) : 
layer(l),
position(pos),
branches(),
alive(true)
{}

Tree::Tree() : 
layer(),
position(),
branches(),
alive(true)
{}

Tree::~Tree()
{
    int n_branch = branches.size();

    for (int i = 0; i < n_branch; ++i)
    {
        delete branches[i];
    }
}
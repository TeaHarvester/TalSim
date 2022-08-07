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
        auto leaf = alive.begin();

        for (auto branchlet = branches.begin(); branchlet < branches.end(); ++branchlet, ++leaf)
        {
            if (*leaf)
            {
                branch = (*branchlet)->GetBranches(depth, branch);
            }
        }
    }

    else
    {
        // size_t canopy = branches.size();
        // branch.resize(branch.size() + canopy);
        // auto harvest = branch.end() - canopy;

        auto leaf = alive.begin();

        for (auto branchlet = branches.begin(); branchlet < branches.end(); ++branchlet, ++leaf)
        {         
            if (*leaf)
            {
                branch.push_back(*branchlet);
            }  
        }
    }
    return branch;
}

Tree::Tree(Position& pos, int l) : 
layer(l),
position(pos),
branches(),
alive()
{}

Tree::Tree() : 
layer(),
position(),
branches(),
alive()
{}

Tree::~Tree()
{
    int n_branch = branches.size();

    for (int i = 0; i < n_branch; ++i)
    {
        delete branches[i];
    }
}
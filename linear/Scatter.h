#ifndef SCATTER_H
#define SCATTER_H

/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    Scatter.h
 * @brief   Maps global variable indices to slot indices
 * @author  Richard Roberts
 * @author  Frank Dellaert
 * @date    June 2015
 */

#include "../inference/Ordering.h"
#include "../linear/GaussianFactorGraph.h"

namespace minisam
{

class GaussianFactorGraph;
class Ordering;

/// One SlotEntry stores the slot index for a variable, as well its dim.
struct  SlotEntry
{
    int key;
    int dimension;
    SlotEntry(int _key, int _dimension) : key(_key), dimension(_dimension) {}
    std::string toString() const;
    friend bool operator<(const SlotEntry& p, const SlotEntry& q)
    {
        return p.key < q.key;
    }
    static bool Zero(const SlotEntry& p)
    {
        return p.dimension==0;
    }
};

/**
 * Scatter is an intermediate data structure used when building a HessianFactor
 * incrementally, to get the keys in the right order. In spirit, it is a map
 * from global variable indices to slot indices in the union of involved
 * variables. We also include the dimensionality of the variable.
 */
class Scatter : public std::vector<SlotEntry>
{
public:
    /// Default Constructor
    Scatter() {}

    // Construct from gaussian factor graph, with optional (partial or complete) ordering
    Scatter(const GaussianFactorGraph& gfg,
            const Ordering& ordering);
    Scatter(const std::vector<const RealGaussianFactor*>&factors, const Ordering& ordering);
    Scatter(const GaussianFactorGraph& gfg);
    /// Add a key/dim pair
    void add(int key, int dim);

private:

    /// Find the SlotEntry with the right key (linear time worst case)
    iterator find(int key);
};
};
#endif // SCATTER_H

#ifndef VARIABLEINDEX_H
#define VARIABLEINDEX_H

/**
 * @file    VariableSlots.h
 * @brief   VariableSlots describes the structure of a combined factor in terms of where each block
 *        comes from in the source factors.
 * @author
 * @date     */

#include "../inference/FactorGraph.h"
#include "../inference/Factor.h"
#include <iostream>
#include <string>

//namespace gtsam {

/** A combined factor is assembled as one block of rows for each component
*   factor.  In each row-block (factor), some of the column-blocks (variables)
*   may be empty since factors involving different sets of variables are
*   interleaved.
*
*   VariableSlots describes the 2D block structure of the combined factor.  It
*   is a map<Key, vector<size_t> >.  The Key is the real
*   variable index of the combined factor slot.  The vector<size_t> tells, for
*   each row-block (factor), which column-block (variable slot) from the
*   component factor appears in this block of the combined factor.
*
*   As an example, if the combined factor contains variables 1, 3, and 5, then
*   "variableSlots[3][2] == 0" indicates that column-block 1 (corresponding to
*   variable index 3), row-block 2 (also meaning component factor 2), comes from
*   column-block 0 of component factor 2.
*
*   \nosubgrouping */

class VariableSlots:public std::map<int, std::vector<int>>
{

public:

// typedef FastMap<Key, FastVector<size_t> > Base;
    static const int Empty;

    /// @name Standard Constructors
    /// @{

    /**
     * Constructor from a set of factors to be combined.  Sorts the variables
     * and keeps track of which variable from each factor ends up in each slot
     * of the combined factor, as described in the class comment.
     */
    template<class TPFactor>
    VariableSlots(const FactorGraph<TPFactor>& factorGraph)
    {

        //gttic(VariableSlots_constructor);
        static const bool debug = false;

        // Compute a mapping (called variableSlots) *from* each involved
        // variable that will be in the new joint factor *to* the slot in each
        // removed factor in which that variable appears.  For each variable,
        // this is stored as a vector of slot numbers, stored in order of the
        // removed factors.  The slot number is the max integer value if the
        // factor does not involve that variable.
        int jointFactorPos = 0;
        for(const Factor& factor: factorGraph.factors_)
        {
            //assert(factor);
            int factorVarSlot = 0;
            std::vector<int>::const_iterator involvedVariable=factor.begin();
            for(; involvedVariable!=factor.end(); involvedVariable++)
            {
                // Set the slot in this factor for this variable.  If the
                // variable was not already discovered, create an array for it
                // that we'll fill with the slot indices for each factor that
                // we're combining.  Initially we put the max integer value in
                // the array entry for each factor that will indicate the factor
                // does not involve the variable.
                std::map<int, std::vector<int>>::iterator thisVarSlots;
                bool inserted;
                std::tie(thisVarSlots, inserted) = this->insert(std::make_pair(*involvedVariable, std::vector<int>()));
                if(inserted)
                    thisVarSlots->second.resize(factorGraph.size(), Empty);
                thisVarSlots->second[jointFactorPos] = factorVarSlot;
                if(debug)
                    std::cout << "  var " << *involvedVariable << " rowblock " << jointFactorPos << " comes from factor's slot " << factorVarSlot << std::endl;
                ++ factorVarSlot;
            }
            ++ jointFactorPos;
        }

    }

    /// @}

    /// @name Testable
    /// @{

    /** print */
// GTSAM_EXPORT void print(const std::string& str = "VariableSlots: ") const;

    /** equals */
    //GTSAM_EXPORT bool equals(const VariableSlots& rhs, double tol = 0.0) const;

    /// @}
};

/// traits
//template<> struct traits<VariableSlots> : public Testable<VariableSlots> {};

/* *************************************************************************
//template<class FG>
VariableSlots::VariableSlots(const FG& factorGraph)
{
  gttic(VariableSlots_constructor);
  static const bool debug = false;

  // Compute a mapping (called variableSlots) *from* each involved
  // variable that will be in the new joint factor *to* the slot in each
  // removed factor in which that variable appears.  For each variable,
  // this is stored as a vector of slot numbers, stored in order of the
  // removed factors.  The slot number is the max integer value if the
  // factor does not involve that variable.
  size_t jointFactorPos = 0;
  for(const typename FG::sharedFactor& factor: factorGraph) {
    assert(factor);
    size_t factorVarSlot = 0;
    for(const Key involvedVariable: *factor) {
      // Set the slot in this factor for this variable.  If the
      // variable was not already discovered, create an array for it
      // that we'll fill with the slot indices for each factor that
      // we're combining.  Initially we put the max integer value in
      // the array entry for each factor that will indicate the factor
      // does not involve the variable.
      iterator thisVarSlots;
       bool inserted;
        boost::tie(thisVarSlots, inserted) = this->insert(std::make_pair(involvedVariable, FastVector<size_t>()));
      if(inserted)
        thisVarSlots->second.resize(factorGraph.size(), Empty);
      thisVarSlots->second[jointFactorPos] = factorVarSlot;
      if(debug) std::cout << "  var " << involvedVariable << " rowblock " << jointFactorPos << " comes from factor's slot " << factorVarSlot << std::endl;
      ++ factorVarSlot;
    }
    ++ jointFactorPos;
  }
}
*/
//}

#endif // VARIABLEINDEX_H

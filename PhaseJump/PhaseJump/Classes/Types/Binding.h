//
//  Binding.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 12/17/22.
//

#ifndef PJBINDING_H
#define PJBINDING_H

#include "SomeBinding.h"

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 11/26/22
 */
namespace PJ
{
    template <class T>
    class Binding : public SomeBinding<T>
    {
    public:
        using GetFunc = std::function<T()>;
        using SetFunc = std::function<void(T const&)>;

        GetFunc getFunc;
        SetFunc setFunc;

        Binding(GetFunc getFunc, SetFunc setFunc) : getFunc(getFunc), setFunc(setFunc) {
        }

        T Value() override { return getFunc(); }
        void SetValue(T const& value) override { setFunc(value); }
    };
}

#endif

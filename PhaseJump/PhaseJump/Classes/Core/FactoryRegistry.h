#ifndef PJFACTORYREGISTRY_H
#define PJFACTORYREGISTRY_H

#include <string>
#include <map>
#include "Factory.h"
#include "Core.h"
#include "_Map.h"

/*
 RATING: 5 stars
 Simple wrapper class with unit tests
 CODE REVIEW: 11/10/22
 */
namespace PJ
{
    /// <summary>
    /// Store factories accessed by a class ID
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    template <class Type> class FactoryRegistry : public Map<std::string, std::shared_ptr<Factory<Type>>>
    {
    public:
        FactoryRegistry() {
        }

        Type New(String id)
        {
            if (this->contains(id)) {
                auto factory = (*this)[id];
                auto result = factory->New();

                // Make sure the object gets the classID when created
                auto coreResult = dynamic_pointer_cast<Core*>(result);
                if (coreResult) {
                    coreResult->SetClassId(id);
                }

                return result;
            }

            return Type();
        }
    };
}

#endif

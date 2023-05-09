#ifndef PJMACROS_COLLECTIONS_H
#define PJMACROS_COLLECTIONS_H

#include <algorithm>
#include <functional>

#define COLLECTION_METHODS(Collection, Type) \
size_t Count() const { return this->size(); } \
bool IsEmpty() const { return Count() <= 0; } \
\
void Remove(Type const& value) { \
    auto i = std::find(this->begin(), this->end(), value); \
    if (i == this->end()) { return; } \
\
    this->erase(i); \
} \
\
bool RemoveFirstIf(std::function<bool(Type const&)> check) { \
    for (auto i = this->begin(); i != this->end(); i++) { \
        if (check(*i)) { \
            this->erase(i); \
            return true; \
        } \
    } \
\
    return false; \
} \
\
void RemoveIf(std::function<bool(T const&)> check) { \
    this->erase(std::remove_if(this->begin(), this->end(), check), this->end()); \
} \
\
bool Contains(T const& value) const \
{ \
    auto findResult = std::find(this->begin(), this->end(), value); \
    return findResult != this->end(); \
} \
bool ContainsWhere(std::function<bool(Type const&)> check) const \
{ \
    auto i = std::find_if(this->begin(), this->end(), check); \
    return i != this->end(); \
} \
\
void AddRange(Collection const& source) { \
    this->insert(this->end(), source.begin(), source.end()); \
} \
\
std::optional<size_t> IndexOf(T const& item) const { \
    auto i = std::find(this->begin(), this->end(), item); \
\
    if (i != this->end()) { \
        return std::make_optional(std::distance(this->begin(), i)); \
    } \
\
    return std::nullopt; \
}

#endif

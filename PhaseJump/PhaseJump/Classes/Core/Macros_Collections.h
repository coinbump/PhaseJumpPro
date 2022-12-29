#ifndef PJMACROS_COLLECTIONS_H
#define PJMACROS_COLLECTIONS_H

#define COLLECTION_METHODS(Type) \
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
}

#endif

#include "amaranth/database.hpp"

#include <algorithm>

namespace amaranth {

Collection& Database::create_collection(const std::string& name,
                                        std::size_t dimension,
                                        MetricType metric) {
    if (name.empty()) {
        throw AmaranthError("collection name must not be empty");
    }
    if (collections_.find(name) != collections_.end()) {
        throw AmaranthError("collection already exists: " + name);
    }

    auto collection = std::make_unique<Collection>(name, dimension, metric);
    auto& ref = *collection;
    collections_.emplace(name, std::move(collection));
    return ref;
}

bool Database::drop_collection(const std::string& name) {
    return collections_.erase(name) > 0;
}

bool Database::has_collection(const std::string& name) const {
    return collections_.find(name) != collections_.end();
}

Collection& Database::collection(const std::string& name) {
    auto it = collections_.find(name);
    if (it == collections_.end()) {
        throw NotFoundError("collection not found: " + name);
    }
    return *it->second;
}

const Collection& Database::collection(const std::string& name) const {
    auto it = collections_.find(name);
    if (it == collections_.end()) {
        throw NotFoundError("collection not found: " + name);
    }
    return *it->second;
}

std::vector<std::string> Database::list_collections() const {
    std::vector<std::string> names;
    names.reserve(collections_.size());
    for (const auto& [name, _] : collections_) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

}  // namespace amaranth

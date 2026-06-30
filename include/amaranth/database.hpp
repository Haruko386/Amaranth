#pragma once

#include "amaranth/collection.hpp"
#include "amaranth/errors.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace amaranth {

class Database {
public:
    Collection& create_collection(const std::string& name,
                                  std::size_t dimension,
                                  MetricType metric = MetricType::Cosine);

    bool drop_collection(const std::string& name);
    bool has_collection(const std::string& name) const;
    Collection& collection(const std::string& name);
    const Collection& collection(const std::string& name) const;
    std::vector<std::string> list_collections() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Collection>> collections_;
};

}  // namespace amaranth

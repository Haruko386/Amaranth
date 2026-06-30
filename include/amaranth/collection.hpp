#pragma once

#include "amaranth/flat_index.hpp"
#include "amaranth/storage.hpp"

#include <filesystem>
#include <memory>
#include <string>

namespace amaranth {

class Collection {
public:
    Collection(std::string name, std::size_t dimension, MetricType metric);

    const std::string& name() const;
    std::size_t dimension() const;
    MetricType metric() const;
    std::size_t size() const;

    void upsert(const Document& document);
    bool remove(const VectorID& id);
    bool contains(const VectorID& id) const;
    std::vector<SearchResult> search(const Vector& query, std::size_t top_k) const;

    void save(const std::filesystem::path& path) const;
    static Collection load(std::string name, const std::filesystem::path& path);

private:
    std::string name_;
    std::unique_ptr<VectorIndex> index_;
};

}  // namespace amaranth

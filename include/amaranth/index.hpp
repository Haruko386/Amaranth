#pragma once

#include "amaranth/types.hpp"

#include <cstddef>
#include <vector>

namespace amaranth {

class VectorIndex {
public:
    virtual ~VectorIndex() = default;

    virtual void upsert(const Document& document) = 0;
    virtual bool remove(const VectorID& id) = 0;
    virtual std::vector<SearchResult> search(const Vector& query, std::size_t top_k) const = 0;
    virtual bool contains(const VectorID& id) const = 0;
    virtual std::size_t size() const = 0;
    virtual std::size_t dimension() const = 0;
    virtual MetricType metric() const = 0;
    virtual std::vector<Document> documents() const = 0;
};

}  // namespace amaranth

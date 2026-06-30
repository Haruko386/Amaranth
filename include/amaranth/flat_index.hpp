#pragma once

#include "amaranth/index.hpp"

#include <unordered_map>

namespace amaranth {

class FlatIndex final : public VectorIndex {
public:
    FlatIndex(std::size_t dimension, MetricType metric);

    void upsert(const Document& document) override;
    bool remove(const VectorID& id) override;
    std::vector<SearchResult> search(const Vector& query, std::size_t top_k) const override;
    bool contains(const VectorID& id) const override;
    std::size_t size() const override;
    std::size_t dimension() const override;
    MetricType metric() const override;
    std::vector<Document> documents() const override;

private:
    void check_dimension(const Vector& vector) const;

    std::size_t dimension_;
    MetricType metric_;
    std::unordered_map<VectorID, Document> documents_;
};

}  // namespace amaranth

#include "amaranth/flat_index.hpp"
#include "amaranth/distance.hpp"
#include "amaranth/errors.hpp"

#include <algorithm>

namespace amaranth {

FlatIndex::FlatIndex(std::size_t dimension, MetricType metric)
    : dimension_(dimension), metric_(metric) {
    if (dimension == 0) {
        throw AmaranthError("dimension must be greater than zero");
    }
}

void FlatIndex::check_dimension(const Vector& vector) const {
    if (vector.size() != dimension_) {
        throw DimensionMismatchError(dimension_, vector.size());
    }
}

void FlatIndex::upsert(const Document& document) {
    if (document.id.empty()) {
        throw AmaranthError("document id must not be empty");
    }
    check_dimension(document.vector);
    documents_[document.id] = document;
}

bool FlatIndex::remove(const VectorID& id) {
    return documents_.erase(id) > 0;
}

std::vector<SearchResult> FlatIndex::search(const Vector& query, std::size_t top_k) const {
    check_dimension(query);

    std::vector<SearchResult> results;
    results.reserve(documents_.size());

    for (const auto& [id, document] : documents_) {
        SearchResult result;
        result.id = id;
        result.score = score_by_metric(query, document.vector, metric_);
        result.rank = 0;
        result.metadata = document.metadata;
        results.push_back(std::move(result));
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& lhs, const SearchResult& rhs) {
        if (lhs.score == rhs.score) {
            return lhs.id < rhs.id;
        }
        return lhs.score > rhs.score;
    });

    if (top_k < results.size()) {
        results.resize(top_k);
    }

    for (std::size_t i = 0; i < results.size(); ++i) {
        results[i].rank = static_cast<std::uint32_t>(i + 1);
    }

    return results;
}

bool FlatIndex::contains(const VectorID& id) const {
    return documents_.find(id) != documents_.end();
}

std::size_t FlatIndex::size() const {
    return documents_.size();
}

std::size_t FlatIndex::dimension() const {
    return dimension_;
}

MetricType FlatIndex::metric() const {
    return metric_;
}

std::vector<Document> FlatIndex::documents() const {
    std::vector<Document> output;
    output.reserve(documents_.size());
    for (const auto& [_, document] : documents_) {
        output.push_back(document);
    }
    return output;
}

}  // namespace amaranth

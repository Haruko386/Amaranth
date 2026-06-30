#include "amaranth/collection.hpp"

namespace amaranth {

Collection::Collection(std::string name, std::size_t dimension, MetricType metric)
    : name_(std::move(name)), index_(std::make_unique<FlatIndex>(dimension, metric)) {}

const std::string& Collection::name() const {
    return name_;
}

std::size_t Collection::dimension() const {
    return index_->dimension();
}

MetricType Collection::metric() const {
    return index_->metric();
}

std::size_t Collection::size() const {
    return index_->size();
}

void Collection::upsert(const Document& document) {
    index_->upsert(document);
}

bool Collection::remove(const VectorID& id) {
    return index_->remove(id);
}

bool Collection::contains(const VectorID& id) const {
    return index_->contains(id);
}

std::vector<SearchResult> Collection::search(const Vector& query, std::size_t top_k) const {
    return index_->search(query, top_k);
}

void Collection::save(const std::filesystem::path& path) const {
    PersistedCollection persisted;
    persisted.dimension = dimension();
    persisted.metric = metric();
    persisted.documents = index_->documents();
    save_collection_to_file(path, persisted);
}

Collection Collection::load(std::string name, const std::filesystem::path& path) {
    auto persisted = load_collection_from_file(path);
    Collection collection(std::move(name), persisted.dimension, persisted.metric);
    for (const auto& document : persisted.documents) {
        collection.upsert(document);
    }
    return collection;
}

}  // namespace amaranth

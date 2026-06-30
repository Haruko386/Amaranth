#include "amaranth/storage.hpp"
#include "amaranth/errors.hpp"

#include <cstdint>
#include <fstream>
#include <string>

namespace amaranth {

namespace {
constexpr std::uint32_t kMagic = 0x414D524E;  // AMRN
constexpr std::uint32_t kVersion = 1;

void write_u32(std::ofstream& out, std::uint32_t value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void write_u64(std::ofstream& out, std::uint64_t value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

std::uint32_t read_u32(std::ifstream& in) {
    std::uint32_t value = 0;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

std::uint64_t read_u64(std::ifstream& in) {
    std::uint64_t value = 0;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

void write_string(std::ofstream& out, const std::string& value) {
    write_u64(out, static_cast<std::uint64_t>(value.size()));
    out.write(value.data(), static_cast<std::streamsize>(value.size()));
}

std::string read_string(std::ifstream& in) {
    const auto size = read_u64(in);
    std::string value(size, '\0');
    in.read(value.data(), static_cast<std::streamsize>(size));
    return value;
}
}  // namespace

void save_collection_to_file(const std::filesystem::path& path,
                             const PersistedCollection& collection) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw AmaranthError("failed to open file for write: " + path.string());
    }

    write_u32(out, kMagic);
    write_u32(out, kVersion);
    write_u64(out, static_cast<std::uint64_t>(collection.dimension));
    write_u32(out, static_cast<std::uint32_t>(collection.metric));
    write_u64(out, static_cast<std::uint64_t>(collection.documents.size()));

    for (const auto& document : collection.documents) {
        write_string(out, document.id);
        write_u64(out, static_cast<std::uint64_t>(document.vector.size()));
        out.write(reinterpret_cast<const char*>(document.vector.data()),
                  static_cast<std::streamsize>(document.vector.size() * sizeof(float)));

        write_u64(out, static_cast<std::uint64_t>(document.metadata.size()));
        for (const auto& [key, value] : document.metadata) {
            write_string(out, key);
            write_string(out, value);
        }
    }
}

PersistedCollection load_collection_from_file(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw AmaranthError("failed to open file for read: " + path.string());
    }

    const auto magic = read_u32(in);
    if (magic != kMagic) {
        throw AmaranthError("invalid amaranth collection file: bad magic");
    }

    const auto version = read_u32(in);
    if (version != kVersion) {
        throw AmaranthError("unsupported amaranth collection version");
    }

    PersistedCollection collection;
    collection.dimension = static_cast<std::size_t>(read_u64(in));
    collection.metric = static_cast<MetricType>(read_u32(in));

    const auto count = read_u64(in);
    collection.documents.reserve(static_cast<std::size_t>(count));

    for (std::uint64_t i = 0; i < count; ++i) {
        Document document;
        document.id = read_string(in);

        const auto vector_size = read_u64(in);
        document.vector.resize(static_cast<std::size_t>(vector_size));
        in.read(reinterpret_cast<char*>(document.vector.data()),
                static_cast<std::streamsize>(document.vector.size() * sizeof(float)));

        const auto metadata_count = read_u64(in);
        for (std::uint64_t j = 0; j < metadata_count; ++j) {
            auto key = read_string(in);
            auto value = read_string(in);
            document.metadata.emplace(std::move(key), std::move(value));
        }

        collection.documents.push_back(std::move(document));
    }

    return collection;
}

}  // namespace amaranth

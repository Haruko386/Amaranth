#include "amaranth/amaranth.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

void print_usage() {
    std::cout << "Amaranth CLI v0.1\n\n"
              << "Usage:\n"
              << "  amaranth_cli demo\n"
              << "  amaranth_cli search <db_file> <top_k> <v1,v2,...>\n"
              << "  amaranth_cli make-sample <db_file>\n\n"
              << "Examples:\n"
              << "  amaranth_cli demo\n"
              << "  amaranth_cli make-sample ./papers.amdb\n"
              << "  amaranth_cli search ./papers.amdb 3 0.1,0.2,0.3\n";
}

std::vector<float> parse_vector(const std::string& raw) {
    std::vector<float> vector;
    std::stringstream ss(raw);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            vector.push_back(std::stof(item));
        }
    }
    return vector;
}

amaranth::Document make_doc(std::string id, std::vector<float> vector, std::string paper_id, std::string page) {
    amaranth::Document doc;
    doc.id = std::move(id);
    doc.vector = std::move(vector);
    doc.metadata["paper_id"] = std::move(paper_id);
    doc.metadata["page"] = std::move(page);
    return doc;
}

void print_results(const std::vector<amaranth::SearchResult>& results) {
    for (const auto& result : results) {
        std::cout << "#" << result.rank << " id=" << result.id
                  << " score=" << result.score;
        const auto paper = result.metadata.find("paper_id");
        const auto page = result.metadata.find("page");
        if (paper != result.metadata.end()) {
            std::cout << " paper_id=" << paper->second;
        }
        if (page != result.metadata.end()) {
            std::cout << " page=" << page->second;
        }
        std::cout << "\n";
    }
}

void run_demo() {
    amaranth::Database db;
    auto& collection = db.create_collection("papers", 3, amaranth::MetricType::Cosine);

    collection.upsert(make_doc("chunk_001", {0.10F, 0.20F, 0.30F}, "paper_001", "1"));
    collection.upsert(make_doc("chunk_002", {0.20F, 0.10F, 0.00F}, "paper_001", "2"));
    collection.upsert(make_doc("chunk_003", {0.90F, 0.10F, 0.20F}, "paper_002", "5"));

    const auto results = collection.search({0.10F, 0.20F, 0.31F}, 3);
    print_results(results);
}

void make_sample(const std::filesystem::path& path) {
    amaranth::Collection collection("papers", 3, amaranth::MetricType::Cosine);
    collection.upsert(make_doc("chunk_001", {0.10F, 0.20F, 0.30F}, "paper_001", "1"));
    collection.upsert(make_doc("chunk_002", {0.20F, 0.10F, 0.00F}, "paper_001", "2"));
    collection.upsert(make_doc("chunk_003", {0.90F, 0.10F, 0.20F}, "paper_002", "5"));
    collection.save(path);
    std::cout << "saved sample collection to " << path << "\n";
}

void search_file(const std::filesystem::path& path, std::size_t top_k, const std::vector<float>& query) {
    auto collection = amaranth::Collection::load("papers", path);
    const auto results = collection.search(query, top_k);
    print_results(results);
}

}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            print_usage();
            return 1;
        }

        const std::string command = argv[1];
        if (command == "demo") {
            run_demo();
            return 0;
        }

        if (command == "make-sample") {
            if (argc != 3) {
                print_usage();
                return 1;
            }
            make_sample(argv[2]);
            return 0;
        }

        if (command == "search") {
            if (argc != 5) {
                print_usage();
                return 1;
            }
            search_file(argv[2], static_cast<std::size_t>(std::stoul(argv[3])), parse_vector(argv[4]));
            return 0;
        }

        print_usage();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    }
}

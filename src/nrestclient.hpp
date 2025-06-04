#pragma once

#ifdef _WIN32
    #define CURL_STATICLIB
#endif

#include <string>
#include <optional>
#include <iostream>
#include "../lib/nlohmann/json.hpp"
#include <curl/curl.h>

// Structure to hold track information.
struct TrackInfo {
    std::string artist;
    std::string title;
    std::string album;
    std::string duration;
};

// Callback function to write received data into a std::string.
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* recvBuffer = static_cast<std::string*>(userp);
    size_t totalSize = size * nmemb;
    recvBuffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// A REST client that fetches and parses the "now playing" JSON using libcurl.
class nrestclient {
public:
    nrestclient(const std::string& baseUrl,
                const std::string& username,
                const std::string& password)
        : baseUrl_(baseUrl), username_(username), password_(password)
    {
        // Initialize libcurl if needed (curl_global_init can be called once in main)
        // curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~nrestclient() {
        // Optionally, perform cleanup (if you called curl_global_init earlier)
        // curl_global_cleanup();
    }

    std::optional<TrackInfo> getCurrentTrack() {
        std::string url = baseUrl_ + "/rest/getNowPlaying.view?u=" + username_ +
                          "&p=" + password_ + "&v=1.16.1&c=navicord&f=json";

        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize libcurl\n";
            return std::nullopt;
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " 
                      << curl_easy_strerror(res) << "\n";
            curl_easy_cleanup(curl);
            return std::nullopt;
        }
        curl_easy_cleanup(curl);

        try {
            auto j = nlohmann::json::parse(readBuffer);
            const auto& subResp   = j.at("subsonic-response");
            const auto& nowPlaying = subResp.at("nowPlaying");
            auto parsedEntries    = nowPlaying.at("entry");
            // Some responses might return a single object instead of an array.
            if (!parsedEntries.is_array()) {
                parsedEntries = nlohmann::json::array({ parsedEntries });
            }
            if (parsedEntries.empty()) {
                return std::nullopt;
            }

            const auto& track = parsedEntries.at(0);
            TrackInfo info;
            info.artist   = track.value("artist", "");
            info.title    = track.value("title", "");
            info.album    = track.value("album", "");
            info.duration = std::to_string(track.value("duration", 0));

            return info;
        }
        catch (nlohmann::json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << "\n";
        }
        return std::nullopt;
    }

    // Stub for getting album art URL.
    std::string getAlbumArtUrl() {
        return "";
    }

private:
    std::string baseUrl_;
    std::string username_;
    std::string password_;
};
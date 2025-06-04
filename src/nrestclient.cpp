#include "nrestclient.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

nrestclient::nrestclient(const string& baseUrl, const string& username, const string& password)
    : baseUrl_(baseUrl), username_(username), password_(password);

optional<TrackInfo> nrestclient::getCurrentTrack() {
    auto response = cpr::Get(
        cpr::Url{baseUrl_ + "/rest/getNowPlaying.view"},
        cpr::Parameters{
            {"u", username_},
            {"p", password_},
            {"v", "1.16.1"},
            {"c", "navicord"},
            {"f", "json"}
        },
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Accept", "application/json"}
        }
    );

    if (response.status_code != 200) return nullopt;

    json j = json::parse(response.text);
    if (!j.contains("nowPlaying") || j["nowPlaying"].empty()) return nullopt;

    auto track = j["nowPlaying"][0];
    return TrackInfo{
        track.value("artist", ""),
        track.value("title", ""),
        track.value("album", ""),
        to_string(track.value("duration", 0))
    };
}

string albumArtURL nrestclient::getAlbumArtUrl() {
    return "none";
}
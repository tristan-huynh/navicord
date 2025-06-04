#pragma once

#include <string>
#include <optional>

using namespace std;

struct TrackInfo {
    string artist;
    string title;
    string album;
    string duration;
};
// http://uri:4533/rest/getNowPlaying.view?u=administrator&p=administrator&v=1.16.1&c=navicord&f=json

class nrestclient {
public:
    explicit nrestclient(const string& baseUrl, const string& username, const string& password);
    optional<TrackInfo> getCurrentTrack();
    string getAlbumArtUrl();

private:
    string baseUrl_;
    string username_;
    string password_;
};
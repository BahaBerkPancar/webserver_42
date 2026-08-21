#include "../include/Router.hpp"

const LocationConfig* Router::match(const ServerConfig& server, const std::string& path)
{
    const LocationConfig* best = NULL;
    size_t best_len = 0;

    for (size_t i = 0; i < server.locations.size(); ++i)
    {
        const std::string& loc_path = server.locations[i].path;
        if (path.substr(0, loc_path.size()) == loc_path)
        {
            if (loc_path.size() > best_len)
            {
                best_len = loc_path.size();
                best = &server.locations[i];
            }
        }
    }
    return best;
}
#pragma once
#include <unordered_map>
#include <memory>
#include "../Gacha/Banner.h"

class BannerRegistry {
public:
    static BannerRegistry& Instance() {
        static BannerRegistry instance;
        return instance;
    }

    void Register(int id, std::shared_ptr<Banner> banner) {
        banners[id] = banner;
    }

    std::shared_ptr<Banner> Get(int id) {
        if (banners.count(id)) return banners.at(id);
        return nullptr;
    }

    const std::unordered_map<int, std::shared_ptr<Banner>>& GetAll() const {
        return banners;
    }

private:
    std::unordered_map<int, std::shared_ptr<Banner>> banners;
};

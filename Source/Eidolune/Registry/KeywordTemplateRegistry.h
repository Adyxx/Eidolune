#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "KeywordEffectTemplate.h"

class KeywordEffectTemplateRegistry {
public:
    static KeywordEffectTemplateRegistry& Instance() {
        static KeywordEffectTemplateRegistry instance;
        return instance;
    }

    void Register(const std::shared_ptr<KeywordEffectTemplate>& tmpl) {
        templatesById[tmpl->Id] = tmpl;
        templatesByName[tmpl->Name] = tmpl;
    }

    std::shared_ptr<KeywordEffectTemplate> Get(int id) const {
        auto it = templatesById.find(id);
        return (it != templatesById.end()) ? it->second : nullptr;
    }

    std::shared_ptr<KeywordEffectTemplate> GetByName(const std::string& name) const {
        auto it = templatesByName.find(name);
        return (it != templatesByName.end()) ? it->second : nullptr;
    }

    const std::unordered_map<int, std::shared_ptr<KeywordEffectTemplate>>& GetAll() const {
        return templatesById;
    }

private:
    std::unordered_map<int, std::shared_ptr<KeywordEffectTemplate>> templatesById;
    std::unordered_map<std::string, std::shared_ptr<KeywordEffectTemplate>> templatesByName;
};

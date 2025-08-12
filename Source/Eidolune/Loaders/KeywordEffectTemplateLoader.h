#pragma once
#include <vector>
#include <memory>
#include "../Core/KeywordEffectTemplate.h"

class KeywordEffectTemplateLoader {
public:
    static std::vector<std::shared_ptr<KeywordEffectTemplate>> LoadAll();
};

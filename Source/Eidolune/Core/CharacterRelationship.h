
#pragma once
#include <string>
#include <memory>
#include <vector>

class Character;

enum class RelationshipType {
    PARTNER,
    RIVAL,
    LOVER,
    MENTOR,
    UNKNOWN
};

class CharacterRelationship {
public:
    std::shared_ptr<Character> Source;
    std::shared_ptr<Character> Target;
    RelationshipType Type;
    std::string Description;

    CharacterRelationship(std::shared_ptr<Character> source,
                          std::shared_ptr<Character> target,
                          RelationshipType type,
                          const std::string& description = "")
        : Source(source), Target(target), Type(type), Description(description) {}

    bool IsMutualPartnerWith(const std::shared_ptr<Character>& other,
                             const std::vector<std::shared_ptr<CharacterRelationship>>& allRelationships) const;

    std::string ToString() const;
};


#include "CharacterRelationship.h"
#include "Character.h"

CharacterRelationship::CharacterRelationship(
    std::shared_ptr<Character> source,
    std::shared_ptr<Character> target,
    RelationshipType type,
    const std::string& description
) : Source(source), Target(target), Type(type), Description(description) {}


bool CharacterRelationship::IsMutualPartnerWith(
    const std::shared_ptr<Character>& other,
    const std::vector<std::shared_ptr<CharacterRelationship>>& allRelationships
) const {
    for (const auto& relationship : allRelationships) {
        if (relationship->Source == Target && relationship->Target == Source &&
            relationship->Type == RelationshipType::PARTNER &&
            relationship->Source == other) {
            return true;
        }
    }
    return false;
}

std::string CharacterRelationship::ToString() const {
    return Source->ToString() + " - " + Target->ToString() + " (" + std::to_string(static_cast<int>(Type)) + ")";
}
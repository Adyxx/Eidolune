#include "Card.h"
#include "Faction.h"
#include "Character.h"
#include "Subtype.h"
#include "CardEffectBinding.h"

Card::Card() {

}

void Card::Validate() const {

}

std::string Card::ToString() const {
    return Name;
}

bool Card::HasSubtype(const std::string& targetSubtype) const {
    for (const auto& subtype : Subtypes) {
        if (subtype && subtype->Name == targetSubtype) return true;
    }
    return false;
}
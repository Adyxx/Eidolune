
#pragma once
#include <memory>
#include <optional>
#include <string>

class Trigger;
class Effect;
class Condition;
class Restriction;
class Gear;

class GearEffectBinding {
public:
    std::shared_ptr<Gear> OwnerGear;
    std::shared_ptr<Trigger> BoundTrigger;
    std::shared_ptr<Effect> BoundEffect;
    std::shared_ptr<Condition> BoundCondition;
    std::shared_ptr<Restriction> BoundRestriction;

    std::optional<int> Value;
    std::string TargetSpec;

    GearEffectBinding(std::shared_ptr<Gear> gear,
                      std::shared_ptr<Trigger> trigger,
                      std::shared_ptr<Effect> effect,
                      std::shared_ptr<Condition> condition = nullptr,
                      std::optional<int> value = std::nullopt,
                      const std::string& targetSpec = "");

    std::string ToString() const;
};

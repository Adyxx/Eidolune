

#include "GearEffectBinding.h"


GearEffectBinding::GearEffectBinding(std::shared_ptr<Gear> gear,
                      std::shared_ptr<Trigger> trigger,
                      std::shared_ptr<Effect> effect,
                      std::shared_ptr<Condition> condition = nullptr,
                      std::shared_ptr<Restriction> restriction = nullptr,
                      std::optional<int> value = std::nullopt,
                      const std::string& targetSpec = "")
        : OwnerGear(gear), BoundTrigger(trigger), BoundEffect(effect),
          BoundCondition(condition), BoundRestriction(restriction),
          Value(value), TargetSpec(targetSpec) {}


std::string GearEffectBinding::ToString() const {

}
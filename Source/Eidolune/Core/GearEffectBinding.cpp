

#include "GearEffectBinding.h"
#include "Gear.h"
#include "Trigger.h"
#include "Effect.h"
#include "Condition.h"

GearEffectBinding::GearEffectBinding(std::shared_ptr<Gear> gear,
                      std::shared_ptr<Trigger> trigger,
                      std::shared_ptr<Effect> effect,
                      std::shared_ptr<Condition> condition,
                      std::optional<int> value,
                      const std::string& targetSpec)
        : OwnerGear(gear), BoundTrigger(trigger), BoundEffect(effect),
          BoundCondition(condition),
          Value(value), TargetSpec(targetSpec) {}


std::string GearEffectBinding::ToString() const {
  return "GearEffectBinding: " + (OwnerGear ? OwnerGear->ToString() : "Unknown Gear") +
         ", Trigger: " + (BoundTrigger ? BoundTrigger->ToString() : "None") +
         ", Effect: " + (BoundEffect ? BoundEffect->ToString() : "None") +
         ", Condition: " + (BoundCondition ? BoundCondition->ToString() : "None") +
         ", Value: " + (Value.has_value() ? std::to_string(Value.value()) : "None") +
         ", TargetSpec: " + TargetSpec;
}
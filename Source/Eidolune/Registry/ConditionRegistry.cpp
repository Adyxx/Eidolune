#include "ConditionRegistry.h"

ConditionRegistry& ConditionRegistry::Instance() {
    static ConditionRegistry instance;
    return instance;
}

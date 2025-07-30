#include "EffectRegistry.h"

EffectRegistry& EffectRegistry::Instance() {
    static EffectRegistry instance;
    return instance;
}

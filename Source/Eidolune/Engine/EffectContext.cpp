
#include "EffectContext.h"

EffectCallContext& GetEffectCallContext() {
    static thread_local EffectCallContext context;
    return context;
}

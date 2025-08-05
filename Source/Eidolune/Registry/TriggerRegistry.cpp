#include "TriggerRegistry.h"

TriggerRegistry& TriggerRegistry::Instance() {
    static TriggerRegistry instance;
    return instance;
}

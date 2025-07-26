#pragma once
#include <string>

class InventoryItem {
public:
    std::string Type; 
    std::string ItemId; 
    int Quantity = 0;
};

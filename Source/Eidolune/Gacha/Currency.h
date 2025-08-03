enum class CurrencyType {
    UNIVERSAL,
    STANDARD_PULL,
    LIMITED_PULL
};

struct UserCurrency {
    CurrencyType Type;
    int Amount;
};

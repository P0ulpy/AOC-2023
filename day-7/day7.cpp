#include <array>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <optional>
#include <functional>

namespace std {
    using uint128_t = unsigned long long;
}

template <typename ArrayType>
struct StdArrayInfo {};

template <typename T, size_t Size>
struct StdArrayInfo<std::array<T, Size>>
{ 
    static constexpr size_t size { Size }; 
};

using Card = char;

const std::array<Card, 13> Cards = { 
    'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'
};

uint16_t GetCardScore(Card card)
{
    int32_t score = Cards.size();
    for(size_t i = 0; i < Cards.size(); ++i)
    {
        if(Cards[i] == card) 
            return score;
        
        --score;
    }

    std::__throw_runtime_error((std::string("Can't find card score : Invalid card type ") + card).c_str());
}

using Hand = std::array<Card, 5>;
constexpr size_t HandSize = StdArrayInfo<Hand>::size;

std::optional<Card> HasNCardOfSameKind(const Hand& hand, uint8_t n, std::optional<Card> exclude = {})
{
    for(std::size_t i = 0; i < hand.size(); ++i)
    {
        Card kind = hand[i];

        uint8_t sameCount = 1;

        for(std::size_t j = 0; j < hand.size(); ++j)
        {
            if (j == i) continue;

            if(kind == hand[j])
            {
                if(!exclude.has_value() || exclude.value() != kind)
                    ++sameCount;
            }
        }

        if(sameCount == n)
            return kind;
    }

    return {};
}

std::optional<Card> IsFiveOfAKind(const Hand& hand)  { return HasNCardOfSameKind(hand, 5); }
std::optional<Card> IsFourOfAKind(const Hand& hand)  { return HasNCardOfSameKind(hand, 4); }
std::optional<Card> IsThreeOfAKind(const Hand& hand) { return HasNCardOfSameKind(hand, 3); }
std::optional<Card> IsPairOfAKind(const Hand& hand)  { return HasNCardOfSameKind(hand, 2); }

bool IsFullHouse(const Hand& hand)
{
    if(auto pairKind = IsPairOfAKind(hand))
        if(auto threeKind = IsThreeOfAKind(hand))
        {
            if(pairKind.value() != threeKind.value())
                return true;
        }

    return false;
}

bool IsTowPair(const Hand& hand)
{
    if(auto pair1Kind = IsPairOfAKind(hand))
        if(auto pair2Kind = HasNCardOfSameKind(hand, 2, pair1Kind))
            return true;

    return false;
}

uint16_t ComputeHandType(const Hand& hand)
{
    static std::array<std::function<bool(const Hand& hand)>, 6> Validators {
        [](const Hand& h) -> bool { return IsFiveOfAKind(h).has_value(); } ,
        [](const Hand& h) -> bool { return IsFourOfAKind(h).has_value(); },
        [](const Hand& h) -> bool { return IsFullHouse(h); },
        [](const Hand& h) -> bool { return IsThreeOfAKind(h).has_value(); },
        [](const Hand& h) -> bool { return IsTowPair(h); },
        [](const Hand& h) -> bool { return IsPairOfAKind(h).has_value(); },
    };

    uint16_t score = Validators.size();
    for(auto validator : Validators)
    {
        if(validator(hand)) return score;
        --score;
    }

    return score;
}

bool CompareHands(const Hand& hand1, const Hand& hand2)
{
    for(size_t cardIndex = 0; cardIndex < HandSize; ++cardIndex)
    {
        auto hand1CardScore = GetCardScore(hand1[cardIndex]);
        auto hand2CardScore = GetCardScore(hand2[cardIndex]);

        

        if(hand1CardScore > hand2CardScore) return true;
        if(hand1CardScore < hand2CardScore) return false;
        else continue;
    }

    std::__throw_runtime_error("No winner found");
}

std::vector<Hand> hands {
    // { 'K', 'K', 'K', 'K', 'K' },
    // { '2', '2', 'K', '2', '2' },
    // { '3', '2', '3', '2', '3' },
    // { 'T', '5', 'T', 'J', '5' },
    // { 'K', 'T', 'A', 'J', 'T' },
    // { 'Q', 'T', 'J', 'A', '2' },
    
    { '3', '2', 'T', '3', 'K' },
    { 'T', '5', '5', 'J', '5' },
    { 'K', 'K', '6', '7', '7' },
    { 'K', 'T', 'J', 'J', 'T' },
    { 'Q', 'Q', 'Q', 'J', 'A' },
};

int main()
{
    std::uint128_t totalScore = 0;

    for(const auto& hand : hands)
    {
        std::cout << "Hand: ";
        for(const auto card : hand)
        {
            std::cout << card << ", ";
        }

        std::cout << "type: " << ComputeHandType(hand) << std::endl;
    }

    

    return 0;
}
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

using Card = char;

const std::array<Card, 13> Cards = { 
    'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'
};

uint16_t GetCardScore(Card card)
{
    for(int32_t score = Cards.size(); score > -1 ; --score)
        if(Cards[score] == card) return score;

    std::__throw_runtime_error((std::string("Can't find card score : Invalid card type ") + card).c_str());
}

using Hand = std::array<Card, 5>;

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

uint16_t ComputeHandValue(const Hand& hand)
{
    static std::array<std::function<bool(const Hand& hand)>, 5> Validators {
        [](const Hand& h) -> bool { return IsFiveOfAKind(h).has_value(); } ,
        [](const Hand& h) -> bool { return IsFourOfAKind(h).has_value(); },
        [](const Hand& h) -> bool { return IsFullHouse(h); },
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
        for(const auto& card : hand)
        {
            std::cout << card << ", ";
        }
        std::cout << '\n';

        std::cout << "Hand value :" << ComputeHandValue(hand) << std::endl;
    }

    return 0;
}
#include <array>
#include <cstdint>
#include <string_view>
#include <iostream>

struct CardEntry {
    std::array<uint32_t, 5> winning;
    std::array<uint32_t, 8> myNumbers;
};

std::array<CardEntry, 6> cards = {
    CardEntry 
        { { 41, 48, 83, 86, 17 }, { 83, 86, 6, 31, 17, 9, 48, 53 } },
    CardEntry 
        { { 13, 32, 20, 16, 61 }, { 61, 30, 68, 82, 17, 32, 24, 19 } },
    CardEntry 
        { { 1, 21, 53, 59, 44 }, { 69, 82, 63, 72, 16, 21, 14, 1 } },
    CardEntry 
        { { 41, 92, 73, 84, 69 }, { 59, 84, 76, 51, 58, 5, 54, 83 } },
    CardEntry 
        { { 87, 83, 26, 28, 32 }, { 88, 30, 70, 12, 93, 22, 82, 36 } },
    CardEntry 
        { { 31, 18, 13, 56, 72 }, { 74, 77, 10, 23, 35, 67, 36, 11 } },
};

std::string_view inputFilePath = "input"; 


uint32_t ComputeCardEntry(const CardEntry& cardEntry);

int main()
{
    for(const auto& card : cards)
    {
        std::cout << ComputeCardEntry(card) << std::endl;
    }
    
    return 0;
}

uint32_t ComputeCardEntry(const CardEntry& cardEntry)
{
    auto isWinnner = [&](uint32_t number) -> bool 
    {
        return std::find(cardEntry.winning.cbegin(), cardEntry.winning.cend(), number) != cardEntry.winning.cend();
    };

    uint32_t score = 0;

    for(const auto& myNumbers : cardEntry.myNumbers)
    {
        if(isWinnner(myNumbers))
        {
            if(score == 0) 
            {
                score = 1;
                continue;
            }

            score *= score;
        }
    }

    return score;
}
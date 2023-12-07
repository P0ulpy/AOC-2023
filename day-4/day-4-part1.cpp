#include <cstdint>

#include <array>
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>

#include <string>

struct CardEntry 
{
    std::array<uint32_t, 10> winning;
    std::array<uint32_t, 25> myNumbers;
};

std::vector<CardEntry> ParseInputFile();
template <size_t SetSize> void PopulateNumberSet(const std::string& numberSetString, std::array<uint32_t, SetSize>& numberSet);
uint32_t ComputeCardEntry(const CardEntry& cardEntry);

const char* inputFilePath = "input";

int main()
{
    std::vector<CardEntry> cards = ParseInputFile();

    uint64_t score = 0;

    for(const auto& card : cards)
    {
        score += ComputeCardEntry(card);
    }

    std::cout << "Score: " << score << std::endl;
    
    return EXIT_SUCCESS;
}

std::vector<CardEntry> ParseInputFile()
{
    std::stringstream fileContent;
    {
        std::fstream file(inputFilePath, std::ios::in);
        
        if(!file.is_open())
        {
            std::cerr << "ERROR : Input file not found at `" << inputFilePath << '`' << std::endl;
            exit(EXIT_FAILURE);
        }

        fileContent << file.rdbuf();
    }

    std::vector<CardEntry> entries;

    // Sice this is a verry strict format it is ok to do that
    for (std::string line; std::getline(fileContent, line);)
    {
        // The size for a line is exactly 117
        // winners numbers start at index 9 and end at 39
        // myNumbers start at index 41 and end at 116
        // a number set is 3 character long (including an empty space)

        CardEntry entry;

        std::string winners = line.substr(9, 30);
        std::string myNumbers = line.substr(41, 75);
        
        PopulateNumberSet(winners, entry.winning);
        PopulateNumberSet(myNumbers, entry.myNumbers);

        entries.push_back(entry);
    }

    return entries;
}

template <size_t SetSize>
void PopulateNumberSet(const std::string& numberSetString, std::array<uint32_t, SetSize>& numberSet) 
{
    for(size_t cell = 0; cell < SetSize; ++cell)
    {
        constexpr size_t cellSize = 3;
        size_t strIndex = cell * cellSize;

        std::string cellContent = numberSetString.substr(strIndex, cellSize);
        numberSet[cell] = std::stoi(cellContent);
    }
}

uint32_t ComputeCardEntry(const CardEntry& cardEntry)
{
    auto isWinnner = [&](uint32_t number) -> bool 
    {
        return std::find(cardEntry.winning.cbegin(), cardEntry.winning.cend(), number) 
            != cardEntry.winning.cend();
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

            score = score << 1;
        }
    }

    return score;
}
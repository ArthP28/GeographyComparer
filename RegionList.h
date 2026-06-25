#pragma once
#include <vector>
#include "Country.h"

using std::vector;

// This is a vector that holds all the countries of a particular region or all the divisions of a certain country (i.e. US States)
class RegionList{
    public:
        RegionList(string nameForList); // Constructor gives the region list a title
        ~RegionList(); // Deconstructor for deleting all countries from list
        void PrintAllCountries(); // Outputs all the countries within the vector
        void InsertCountry(Country* _country); // Places a country to the back of the region list
        void RemoveCountryAtIndex(int index); // Removes a country at a specific index, but not necessarily erasing all its contents
        void DeleteCountryAtIndex(int index); // Removes a country at a specific index INCLUDING erasing all its data
        void RemoveAllCountries(); // Deletes every country from the region list


        // Quicksort algorithms that arrange all countries in the list by area/population/density/name
        void SortByLandArea(int start, int end);
        void SortByPopulation(int start, int end);
        void SortByPopulationDensity(int start, int end);
        void SortByName(int start, int end);

        // Binary Search Algorithms that return the closest approximant to the target country
        void SearchForClosestByLandArea(Country* _targetCountry, Country*& _closestApproximitant);
        void SearchForClosestByPopulation(Country* _targetCountry, Country*& _closestApproximitant);
        void SearchForClosestByPopulationDensity(Country* _targetCountry, Country*& _closestApproximitant);
        // Helper functions that handle certain scenarios during the binary approximant search 
        Country* getClosestApproximitantForLandArea(Country* currentApproximitant, int middleIndex, Country* targetCountry);
        Country* getClosestApproximitantForPopulation(Country* currentApproximitant, int middleIndex, Country* targetCountry);
        Country* getClosestApproximitantForPopulationDensity(Country* currentApproximitant, int middleIndex, Country* targetCountry);

        // Linear Search algorithm that searches for a country by name
        Country* SearchCountryInList(string countryName);

        // Getter Functions
        vector<Country*> getAllCountries(); // Accesses all the countries within the region list itself
        bool isSortedBySize(); // Accesses Size Flag
        bool isSortedByPopulation(); // Accesses Population Flag
        bool isSortedByPopulationDensity(); // Accesses Population Density Flag
        bool isSortedByName(); // Accesses Name Flag
        string getNameOfList(); // Accesses name of the Region List

        // Setter Functions
        void ChangeSortedBySize(bool newState); // Sets Size Flag
        void ChangeSortedByPopulation(bool newState); // Sets Population Flag
        void ChangeSortedByPopulationDensity(bool newState); // Sets Population Density Flag
        void ChangeSortedByName(bool newState); // Sets Name Flag

    private:
        vector<Country*> allCountries; // Contains all countries within the list
        string _listName;

        // Helping Partition Functions that aid in their respective Quicksort algorithms
        int PartitionByLand(int start, int end);
        int PartitionByPopulation(int start, int end);
        int PartitionByPopulationDensity(int start, int end);
        int PartitionByName(int start, int end);

        string ConvertStringToLower(string textToConvert);
        
        /*
            Flags for sorted by population/land area/name
                When a certain sorting flag is turned on, that particular type of sorting does not happen again
                This especially helps with keeping the algorithm speed at a O(nlogn) notation.
                When a different sorting method is called, all the other flags are turned off
                    i.e.
                        SortByPopulation -> Turn on flag for SortByPopulation -> SortByPopulation = N
                        SortByPopulation -> Turn on flag -> SortByLandArea -> Reset all flags = Y
        */
       bool _sortedBySize = false;
       bool _sortedByPopulation = false;
       bool _sortedByPopulationDensity = false;
       bool _sortedByName = true;
};
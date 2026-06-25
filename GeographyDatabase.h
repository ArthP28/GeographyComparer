#pragma once
#include "RegionList.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class GeographyDatabase{
    public:
        // Only a deconstructor is required
        ~GeographyDatabase();
        // Simple comparison functions between two specific countries
        void CompareByLandArea(string country1, string country2);
        void CompareByPopulation(string country1, string country2);
        void CompareByPopulationDensity(string country1, string country2);

        // These functions search for a country in the world or within a region that is closest in area/population/density to a particular other
        void FindClosestByLandArea(string countryToCompare);
        void FindClosestByLandArea(string countryToCompare, string regionToCompare);
        void FindClosestByPopulation(string countryToCompare);
        void FindClosestByPopulation(string countryToCompare, string regionToCompare);
        void FindClosestByPopulationDensity(string countryToCompare);
        void FindClosestByPopulationDensity(string countryToCompare, string regionToCompare);

        // Functions that print out every country either in the world or within a region sorted by area/population/density/name.
        void PrintCountryInDatabase(string countryToPrint);
        void PrintAllCountriesByLandArea();
        void PrintAllCountriesByLandArea(string regionToPrint);
        void PrintAllCountriesByPopulation();
        void PrintAllCountriesByPopulation(string regionToPrint);
        void PrintAllCountriesByPopulationDensity();
        void PrintAllCountriesByPopulationDensity(string regionToPrint);
        void PrintAllCountriesByName();
        void PrintAllCountriesByName(string regionToPrint);
        void PrintAllRegionLists();

        // Converts a new .csv file into a region list and appends it to the vector
        bool AddDataSheet(string filePath, string listName);
        bool RemoveDataSheet(string listName);
    private:
        // Sorts out all the Region Lists of the database by area/population/density/name
        void SortAllListsBySize();
        void SortAllListsByPopulation();
        void SortAllListsByPopulationDensity();
        void SortAllListsByName();

        // Finds a country or region within the database
        Country* SearchCountry(string countryToSearch);
        RegionList* SearchRegion(string regionToSearch);
        
        string ConvertStringToLower(string textToConvert);
        vector<RegionList*> allRegions; // A vector of all Region Lists each containing every country from a specific part of the world
};
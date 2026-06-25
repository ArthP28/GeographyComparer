#include "GeographyDatabase.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <cstring>

using std::ifstream;
using std::cout;
using std::perror;
using std::getline;
using std::endl;
using std::fabs;
using std::tolower;
using std::find;

void SortListBySize(RegionList*& _targetRegion);
void SortListByPopulation(RegionList*& _targetRegion);
void SortListByPopulationDensity(RegionList*& _targetRegion);
void SortListByName(RegionList*& _targetRegion);
bool CheckForDisambiguation(string& CountryToCheck);
bool CheckBothCountriesExisting(Country*& country1, Country*& country2);
bool CheckIfCountryExists(Country*& countryToCheck);
bool CheckIfRegionExists(RegionList*& regionToCheck);

// -X- DECONSTRUCTOR -X-

// As the program quits, remove all countries and their regions from the database to avoid a memory leak.
GeographyDatabase::~GeographyDatabase(){
    while(!allRegions.empty()){
        delete allRegions.at(0);
        allRegions.at(0) = NULL;
        allRegions.erase(allRegions.begin());
    }
}

// -- PUBLIC METHODS --

// Converts a .csv file into a Region List of countries
// This is a bool function because when a data sheet does get successfully imported,
// a message should be displayed confirming that. Ditto for the removing function down below.
bool GeographyDatabase::AddDataSheet(string filePath, string listName){
    bool _dataSheetAdded = false;
    RegionList* _regionList = new RegionList(listName);
    ifstream infile;
    filePath = "default_dbs/" + filePath;
    infile.open(filePath);

    // First check if the file exists:
    // If it does, then process the .csv file and create the countries for the region list
    // Else if not, throw an exception
    if(infile.good()){
        string dataLine;
        while(getline(infile, dataLine)){
            // This contains the line data from the .csv file that will be used when creating a country in the database
            string countryData[5];
            int k = 0; // Iterator for countryData array
            while(dataLine.length() > 0){
                int foundIndex = 0; // Location of first comma, carriage return ('\r'), or the end of the line
                int endIndex = 0; // Indicates the end of the substring that will eventually be added to countryData[]
                // Find first occurrence of a comma
                if(dataLine.find(',') != std::string::npos){
                    foundIndex = dataLine.find(',');
                    endIndex = foundIndex + 1;
                // Else, find the carriage return indicating the end of line
                } else if (dataLine.find('\r') != std::string::npos){
                    foundIndex = dataLine.find('\r');
                    endIndex = foundIndex + 1;
                // Else, include the end of line
                } else {
                    foundIndex = dataLine.length();
                    endIndex = foundIndex;
                }
                string subS = dataLine.substr(0, foundIndex);
                countryData[k] = subS;
                dataLine = dataLine.substr(endIndex); // Chops off the already used part of the data line
                k++;
            }
            // Order: 0 = Name, 1 = Region, 2 = Official Name, 3 = Land Area, 4 = Population
            Country* newCountry = new Country(countryData[0], countryData[1], countryData[2], std::stod(countryData[3]), std::stod(countryData[4]));
            _regionList->InsertCountry(newCountry);
        }
        allRegions.push_back(_regionList);
        infile.close();
        _dataSheetAdded = true;
    } else {
        cout << "\033[31mError:\033[m \"" + filePath + "\" cannot be opened!" << endl;
    }

    return _dataSheetAdded;
}

// Searches for a country list and then removes it from the Geography Comparer
bool GeographyDatabase::RemoveDataSheet(string listName){
    bool _dataSheetRemoved = false;
    RegionList* _listToDelete = NULL;
    int i = 0;
    // Search for the list first
    while (i < allRegions.size() && !_listToDelete){
        if(ConvertStringToLower(allRegions.at(i)->getNameOfList()) == ConvertStringToLower(listName)){
            _listToDelete = allRegions.at(i);
        } else {
            i++;
        }
    }
    // Delete all its contents and remove it from the Database's underlying vector.
    if(CheckIfRegionExists(_listToDelete)){
        delete _listToDelete;
        _listToDelete = NULL;
        _dataSheetRemoved = true;
        allRegions.erase(allRegions.begin() + i);
    }
    return _dataSheetRemoved;
}

// - BASIC COMPARISON FUNCTIONS -
/* 
    The following three functions take in two user-inputted countries, print out their respective data, and generate
    a simple message stating whether country1 is bigger/smaller than country 2
*/

void GeographyDatabase::CompareByLandArea(string country1, string country2){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(country1) && CheckForDisambiguation(country2)){
        // Try to find the two countries to compare first
        Country* _countryToCompare1 = SearchCountry(country1);
        Country* _countryToCompare2 = SearchCountry(country2);

        // Before comparing, make sure that both of them are found
        if(CheckBothCountriesExisting(_countryToCompare1, _countryToCompare2)){
            cout << "\033[4mResults of comparison - " << _countryToCompare1->GetName() << " and " << _countryToCompare2->GetName() << ":\033[m\n" << endl;
            _countryToCompare1->PrintCountryData();
            cout << endl;
            _countryToCompare2->PrintCountryData();
            cout << endl;
            // As well as printing out the general information of both countries, print out a message stating whether
            // Country A < Country B, Country A > Country B, or, in rare cases, Country A == Country B, depending on the comparison
            if(_countryToCompare1->GetLandArea() < _countryToCompare2->GetLandArea()){
                cout << _countryToCompare1->GetName() << " is \033[1msmaller\033[m than " << _countryToCompare2->GetName() << endl;
            }
            else if (_countryToCompare1->GetLandArea() > _countryToCompare2->GetLandArea()){
                cout << _countryToCompare1->GetName() << " is \033[1mbigger\033[m than " << _countryToCompare2->GetName() << endl;
            }
            else {
                cout << _countryToCompare1->GetName() << " is \033[1msimilar in size\033[m to " << _countryToCompare2->GetName() << endl;
            }
        }
    }
}

void GeographyDatabase::CompareByPopulation(string country1, string country2){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(country1) && CheckForDisambiguation(country2)){
        // Try to find the two countries to compare first
        Country* _countryToCompare1 = SearchCountry(country1);
        Country* _countryToCompare2 = SearchCountry(country2);

        // Before comparing, make sure that both of them are found
        if(CheckBothCountriesExisting(_countryToCompare1, _countryToCompare2)){
            cout << "\033[4mResults of comparison - " << _countryToCompare1->GetName() << " and " << _countryToCompare2->GetName() << ":\033[m\n" << endl;
            _countryToCompare1->PrintCountryData();
            cout << endl;
            _countryToCompare2->PrintCountryData();
            cout << endl;
            // As well as printing out the general information of both countries, print out a message stating whether
            // Country A < Country B, Country A > Country B, or, in rare cases, Country A == Country B, depending on the comparison
            if(_countryToCompare1->GetPopulation() < _countryToCompare2->GetPopulation()){
                cout << _countryToCompare1->GetName() << " has \033[1mless people\033[m than " << _countryToCompare2->GetName() << endl;
            }
            else if (_countryToCompare1->GetPopulation() > _countryToCompare2->GetPopulation()){
                cout << _countryToCompare1->GetName() << " has \033[1mmore people\033[m than " << _countryToCompare2->GetName() << endl;
            }
            else {
                cout << _countryToCompare1->GetName() << " is \033[1msimilar in population\033[m to " << _countryToCompare2->GetName() << endl;
            }
        }
    }

}

void GeographyDatabase::CompareByPopulationDensity(string country1, string country2){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(country1) && CheckForDisambiguation(country2)){
        // Try to find the two countries to compare first
        Country* _countryToCompare1 = SearchCountry(country1);
        Country* _countryToCompare2 = SearchCountry(country2);

        // Before comparing, make sure that both of them are found
        if(CheckBothCountriesExisting(_countryToCompare1, _countryToCompare2)){
            cout << "\033[4mResults of comparison - " << _countryToCompare1->GetName() << " and " << _countryToCompare2->GetName() << ":\033[m\n" << endl;
            _countryToCompare1->PrintCountryData();
            cout << endl;
            _countryToCompare2->PrintCountryData();
            cout << endl;
            // As well as printing out the general information of both countries, print out a message stating whether
            // Country A < Country B, Country A > Country B, or, in rare cases, Country A == Country B, depending on the comparison
            if(_countryToCompare1->GetPopulationDensity() < _countryToCompare2->GetPopulationDensity()){
                cout << _countryToCompare1->GetName() << "'s population is \033[1mless dense\033[m than " << _countryToCompare2->GetName() << "'s" << endl;
            }
            else if (_countryToCompare1->GetPopulationDensity() > _countryToCompare2->GetPopulationDensity()){
                cout << _countryToCompare1->GetName() << "'s population is \033[1mmore dense\033[m than " << _countryToCompare2->GetName() << "'s" << endl;
            }
            else {
                cout << _countryToCompare1->GetName() << "'s population is \033[1msimilar in densit\033[m to " << _countryToCompare2->GetName() << "'s" << endl;
            }
        }
    }

}

// - CLOSEST APPROXIMANT FUNCTIONS -
/* 
    The following six functions find the country closest in size, population, or population density to the one inputted in their parameters.
    Below each function is an overloaded equivalent that takes in a specific region as well for more interesting comparisons
*/

// Void Function scans through the database to find and print the country closest in size to the target
void GeographyDatabase::FindClosestByLandArea(string countryToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // The database needs to be sorted first. The following code is a binary search algorithm
        SortAllListsBySize();
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country exists (It has already been found)
        if (CheckIfCountryExists(_targetCountry)){
            // While iterating through the database, perform a binary search on each one to find the closest approximitant
            for(int i = 0; i < allRegions.size(); i++){
                allRegions.at(i)->SearchForClosestByLandArea(_targetCountry, _closestApproximant);
            }
            cout << _targetCountry->GetName() << " is closest in size to " << _closestApproximant->GetName() << ".\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}

// Overloaded Function finds the closest approximitant only within a certain target region
void GeographyDatabase::FindClosestByLandArea(string countryToCompare, string regionToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        RegionList* _targetRegion = SearchRegion(regionToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country and region exists (They have already been found)
        if (CheckIfCountryExists(_targetCountry) && CheckIfRegionExists(_targetRegion)){
            SortListBySize(_targetRegion);
            // Perform a binary search on the target region list to find the closest approximitant
            _targetRegion->SearchForClosestByLandArea(_targetCountry, _closestApproximant);
            cout << _targetCountry->GetName() << " is closest in size to " << _closestApproximant->GetName() << ".\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}

// Void Function scans through the database to find and print the country closest in population to the target
void GeographyDatabase::FindClosestByPopulation(string countryToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // The database needs to be sorted first. The following code is a binary search algorithm
        SortAllListsByPopulation();
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country exists (It has already been found)
        if (CheckIfCountryExists(_targetCountry)){
            // While iterating through the database, perform a binary search on each one to find the closest approximitant
            for(int i = 0; i < allRegions.size(); i++){
                allRegions.at(i)->SearchForClosestByPopulation(_targetCountry, _closestApproximant);
            }
            cout << _targetCountry->GetName() << " is closest in population to " << _closestApproximant->GetName() << ".\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}


// Overloaded Function finds the closest approximitant only within a certain target region
void GeographyDatabase::FindClosestByPopulation(string countryToCompare, string regionToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        RegionList* _targetRegion = SearchRegion(regionToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country and region exists (They have already been found)
        if (CheckIfCountryExists(_targetCountry) && CheckIfRegionExists(_targetRegion)){
            SortListByPopulation(_targetRegion);
            // Perform a binary search on the target region list to find the closest approximitant
            _targetRegion->SearchForClosestByPopulation(_targetCountry, _closestApproximant);
            cout << _targetCountry->GetName() << " is closest in population to " << _closestApproximant->GetName() << ".\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}

// Void Function scans through the database to find and print the country closest in population density to the target
void GeographyDatabase::FindClosestByPopulationDensity(string countryToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // The database needs to be sorted first. The following code is a binary search algorithm
        SortAllListsByPopulationDensity();
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country exists (It has already been found)
        if (CheckIfCountryExists(_targetCountry)){
            // While iterating through the database, perform a binary search on each one to find the closest approximitant
            for(int i = 0; i < allRegions.size(); i++){
                allRegions.at(i)->SearchForClosestByPopulationDensity(_targetCountry, _closestApproximant);
            }
            cout << _targetCountry->GetName() << "'s population is closest in density to " << _closestApproximant->GetName() << "'s.\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}

// Overloaded Function finds the closest approximitant only within a certain target region
void GeographyDatabase::FindClosestByPopulationDensity(string countryToCompare, string regionToCompare){
    // These are hardcoded errors pertaining to geographic similarities, like Russia being in both Europe and Asia, and two Georgias existing.
    if (CheckForDisambiguation(countryToCompare)){
        // First, try to find the country that has the inputted name
        Country* _targetCountry = SearchCountry(countryToCompare);
        RegionList* _targetRegion = SearchRegion(regionToCompare);
        Country* _closestApproximant = NULL;
        int closestSize = 0;
        // This algorithm will only execute if the target country and region exists (They have already been found)
        if (CheckIfCountryExists(_targetCountry) && CheckIfRegionExists(_targetRegion)){
            SortListByPopulationDensity(_targetRegion);
            // Perform a binary search on the target region list to find the closest approximitant
            _targetRegion->SearchForClosestByPopulationDensity(_targetCountry, _closestApproximant);
            cout << _targetCountry->GetName() << "'s population is closest in density to " << _closestApproximant->GetName() << "'s.\n" << endl;
            _targetCountry->PrintCountryData();
            cout << endl;
            _closestApproximant->PrintCountryData();
            cout << endl << endl;
        }
    }
}

// - PRINTER FUNCTIONS -
/*
    The following six functions output a list of all the countries in the world sorted by size, population, population density, or name.
    Below each function is an overloaded equivalent that prints out all the countries of a specific region only.
*/
// Prints out only the information of one specified country
void GeographyDatabase::PrintCountryInDatabase(string countryToPrint){
    if(CheckForDisambiguation(countryToPrint)){
        Country* _targetCountry = SearchCountry(countryToPrint);
        if(CheckIfCountryExists(_targetCountry)){
            _targetCountry->PrintCountryData();
            cout << endl;
        }
    }
}
// Print out all the countries in the world sorted by size
void GeographyDatabase::PrintAllCountriesByLandArea(){
    // Create a brand new list and store every country in the Geography Database into it
    RegionList* allCountriesInWorld = new RegionList("All Countries");

    for(int i = 0; i < allRegions.size(); i++){
        for(int k = 0; k < allRegions.at(i)->getAllCountries().size(); k++){
            allCountriesInWorld->InsertCountry(allRegions.at(i)->getAllCountries().at(k));
        }
    }
    // Sort the new list by land area and print out every country in the now organized list
    SortListBySize(allCountriesInWorld);

    cout << "\033[4mAll Countries in the World sorted by land area:\033[m" << endl << endl;

    int i = 0;
    while(!allCountriesInWorld->getAllCountries().empty()){
        cout << allCountriesInWorld->getAllCountries().size() << ". "; allCountriesInWorld->getAllCountries().front()->PrintCountryData(); cout << endl << endl;
        allCountriesInWorld->RemoveCountryAtIndex(0); // Remove the printed country from the allCountriesInWorld list because we do not need it anymore
        i++;
    }

    delete allCountriesInWorld; // Delete this list once the entire contents of it are emptied out
}

// Overloaded function only prints out the countries within a specific region
void GeographyDatabase::PrintAllCountriesByLandArea(string regionToPrint){
    // Instead of managing a whole new list, use an already existing list of the target region
    RegionList* _targetRegion = SearchRegion(regionToPrint);

    if (CheckIfRegionExists(_targetRegion)){
        SortListBySize(_targetRegion);

        cout << "\033[4mAll Countries in " << _targetRegion->getNameOfList() << " sorted by land area:\033[m" << endl << endl;

        for(int i = 0; i < _targetRegion->getAllCountries().size(); i++){
            cout << (_targetRegion->getAllCountries().size() - i) << ". "; _targetRegion->getAllCountries().at(i)->PrintCountryData(); cout << endl << endl;
        }
    }
}

// Print out all the countries in the world sorted by population
void GeographyDatabase::PrintAllCountriesByPopulation(){
    // Create a brand new list and store every country in the Geography Database into it
    RegionList* allCountriesInWorld = new RegionList("All Countries");

    for(int i = 0; i < allRegions.size(); i++){
        for(int k = 0; k < allRegions.at(i)->getAllCountries().size(); k++){
            allCountriesInWorld->InsertCountry(allRegions.at(i)->getAllCountries().at(k));
        }
    }
    // Sort the new list by population and print out every country in the now organized list
    SortListByPopulation(allCountriesInWorld);

    cout << "\033[4mAll Countries in the World sorted by population:\033[m" << endl << endl;

    int i = 0;
    while(!allCountriesInWorld->getAllCountries().empty()){
        cout << allCountriesInWorld->getAllCountries().size() << ". "; allCountriesInWorld->getAllCountries().front()->PrintCountryData(); cout << endl << endl;
        allCountriesInWorld->RemoveCountryAtIndex(0); // Remove the printed country from the allCountriesInWorld list because we do not need it anymore
        i++;
    }

    delete allCountriesInWorld; // Delete this list once the entire contents of it are emptied out
}

// Overloaded function only prints out the countries within a specific region
void GeographyDatabase::PrintAllCountriesByPopulation(string regionToPrint){
    // Instead of managing a whole new list, use an already existing list of the target region
    RegionList* _targetRegion = SearchRegion(regionToPrint);

    if (CheckIfRegionExists(_targetRegion)){
        SortListByPopulation(_targetRegion);

        cout << "\033[4mAll Countries in " << _targetRegion->getNameOfList() << " sorted by population:\033[m" << endl << endl;

        for(int i = 0; i < _targetRegion->getAllCountries().size(); i++){
            cout << (_targetRegion->getAllCountries().size() - i) << ". "; _targetRegion->getAllCountries().at(i)->PrintCountryData(); cout << endl << endl;
        }
    }
}

// Print out all the countries in the world sorted by population density
void GeographyDatabase::PrintAllCountriesByPopulationDensity(){
    // Create a brand new list and store every country in the Geography Database into it
    RegionList* allCountriesInWorld = new RegionList("All Countries");

    for(int i = 0; i < allRegions.size(); i++){
        for(int k = 0; k < allRegions.at(i)->getAllCountries().size(); k++){
            allCountriesInWorld->InsertCountry(allRegions.at(i)->getAllCountries().at(k));
        }
    }
    // Sort the new list by population density and print out every country in the now organized list
    SortListByPopulationDensity(allCountriesInWorld);

    cout << "\033[4mAll Countries in the World sorted by population density:\033[m" << endl << endl;

    int i = 0;
    while(!allCountriesInWorld->getAllCountries().empty()){
        cout << allCountriesInWorld->getAllCountries().size() << ". "; allCountriesInWorld->getAllCountries().front()->PrintCountryData(); cout << endl << endl;
        allCountriesInWorld->RemoveCountryAtIndex(0); // Remove the printed country from the allCountriesInWorld list because we do not need it anymore
        i++;
    }

    delete allCountriesInWorld; // Delete this list once the entire contents of it are emptied out
}

// Overloaded function only prints out the countries within a specific region
void GeographyDatabase::PrintAllCountriesByPopulationDensity(string regionToPrint){
    // Instead of managing a whole new list, use an already existing list of the target region
    RegionList* _targetRegion = SearchRegion(regionToPrint);

    if (CheckIfRegionExists(_targetRegion)){
        SortListByPopulationDensity(_targetRegion);

        cout << "\033[4mAll Countries in " << _targetRegion->getNameOfList() << " sorted by population density:\033[m" << endl << endl;

        for(int i = 0; i < _targetRegion->getAllCountries().size(); i++){
            cout << (_targetRegion->getAllCountries().size() - i) << ". "; _targetRegion->getAllCountries().at(i)->PrintCountryData(); cout << endl << endl;
        }
    }
}

// Print out all the countries in the world sorted alphabetically
void GeographyDatabase::PrintAllCountriesByName(){
    // Create a brand new list and store every country in the Geography Database into it
    RegionList* allCountriesInWorld = new RegionList("All Countries");

    for(int i = 0; i < allRegions.size(); i++){
        for(int k = 0; k < allRegions.at(i)->getAllCountries().size(); k++){
            allCountriesInWorld->InsertCountry(allRegions.at(i)->getAllCountries().at(k));
        }
    }
    // Sort the new list by name and print out every country in the now organized list
    SortListByName(allCountriesInWorld);

    cout << "\033[4mAll Countries in the World sorted by name:\033[m" << endl << endl;

    int i = 0;
    while(!allCountriesInWorld->getAllCountries().empty()){
        cout << i + 1 << ". "; allCountriesInWorld->getAllCountries().front()->PrintCountryData(); cout << endl << endl;
        allCountriesInWorld->RemoveCountryAtIndex(0); // Remove the printed country from the allCountriesInWorld list because we do not need it anymore
        i++;
    }

    delete allCountriesInWorld; // Delete this list once the entire contents of it are emptied out
}

// Overloaded function only prints out the countries within a specific region
void GeographyDatabase::PrintAllCountriesByName(string regionToPrint){
    // Instead of managing a whole new list, use an already existing list of the target region
    RegionList* _targetRegion = SearchRegion(regionToPrint);

    if (CheckIfRegionExists(_targetRegion)){
        SortListByName(_targetRegion);

        cout << "\033[4mAll Countries in " << _targetRegion->getNameOfList() << " sorted by name:\033[m" << endl << endl;

        for(int i = 0; i < _targetRegion->getAllCountries().size(); i++){
            cout << i + 1 << ". "; _targetRegion->getAllCountries().at(i)->PrintCountryData(); cout << endl << endl;
        }
    }
}

void GeographyDatabase::PrintAllRegionLists(){
    for(int i = 0; i < allRegions.size(); i++){
        cout << (i + 1) << ". " << allRegions.at(i)->getNameOfList() << endl;
    }
}

// - SORTER FUNCTIONS -
/* 
    The following four functions deal with arranging the contents of all Region Lists by size, population, population density, and name respectively.

    Notice the if statement nestled into the loop of every sorting function. This checks if the list at location i is already sorted.
    If it is, then do not sort out the Region List again.
    Since the inner SortListByX() function directs the program to a recursive QuickSort algorithm, this ensures that the function is
    executed only as needed, and at a fast notation of O(nlogn). Otherwise, QuickSorting an already sorted Region List would yield a Big O
    notation of O(n), meaning that the program would take longer to process all the elements within the Region List.
*/

void GeographyDatabase::SortAllListsBySize(){
    for(int i = 0; i < allRegions.size(); i++){
        if(!allRegions.at(i)->isSortedBySize()){
            SortListBySize(allRegions.at(i));
        }
    }
}
void GeographyDatabase::SortAllListsByPopulation(){
    for(int i = 0; i < allRegions.size(); i++){
        if(!allRegions.at(i)->isSortedByPopulation()){
            SortListByPopulation(allRegions.at(i));
        }
    }
}
void GeographyDatabase::SortAllListsByPopulationDensity(){
    for(int i = 0; i < allRegions.size(); i++){
        if(!allRegions.at(i)->isSortedByPopulationDensity()){
            SortListByPopulationDensity(allRegions.at(i));
        }
    }
}
void GeographyDatabase::SortAllListsByName(){
    for(int i = 0; i < allRegions.size(); i++){
        if(!allRegions.at(i)->isSortedByName()){
            SortListByName(allRegions.at(i));
        }
    }
}

// - MISCELLANEOUS FUNCTIONS -

// This function finds and returns a country if it exists within the database
Country* GeographyDatabase::SearchCountry(string countryToSearch){
    Country* countryFound = NULL;
    int i = 0;
    // The following if/else statement handles abbreviations of certain countries (Such as the UK and the USA)
    // rather than rejecting them entirely from the function.
    if(ConvertStringToLower(countryToSearch) == ConvertStringToLower("UK")){
        countryToSearch = "United Kingdom";
    } else if(ConvertStringToLower(countryToSearch) == ConvertStringToLower("US") || ConvertStringToLower(countryToSearch) == ConvertStringToLower("USA")){
        countryToSearch = "United States";
    }
    while (i < allRegions.size() && !countryFound){
        countryFound = allRegions.at(i)->SearchCountryInList(countryToSearch);
        i++;
    }
    return countryFound;
}

// This function finds and returns a region if it exists within the database
RegionList* GeographyDatabase::SearchRegion(string regionToSearch){
    RegionList* regionFound = NULL;
    int i = 0;
    while (i < allRegions.size() && !regionFound){
        if(ConvertStringToLower(allRegions.at(i)->getNameOfList()) == ConvertStringToLower(regionToSearch)){
            regionFound = allRegions.at(i);
        } else {
            i++;
        }
    }
    return regionFound;
}

// This minor function turns an entire string into lowercase via a for loop
string GeographyDatabase::ConvertStringToLower(string textToConvert){
    string resultText = textToConvert;
    for(int i = 0; i < textToConvert.length(); i++){
        resultText[i] = tolower(resultText[i]);
    }
    return resultText;
}

// - Helper Methods -
// These four helper methods sort out the list while setting up the appropriate flags

void SortListBySize(RegionList*& _targetRegion){
    _targetRegion->ChangeSortedByPopulation(false);
    _targetRegion->ChangeSortedByName(false);
    _targetRegion->ChangeSortedByPopulationDensity(false);
    _targetRegion->SortByLandArea(0, _targetRegion->getAllCountries().size() - 1);
    _targetRegion->ChangeSortedBySize(true);
}

void SortListByPopulation(RegionList*& _targetRegion){
    _targetRegion->ChangeSortedBySize(false);
    _targetRegion->ChangeSortedByName(false);
    _targetRegion->ChangeSortedByPopulationDensity(false);
    _targetRegion->SortByPopulation(0, _targetRegion->getAllCountries().size() - 1);
    _targetRegion->ChangeSortedByPopulation(true);
}

void SortListByPopulationDensity(RegionList*& _targetRegion){
    _targetRegion->ChangeSortedBySize(false);
    _targetRegion->ChangeSortedByName(false);
    _targetRegion->ChangeSortedByPopulation(false);
    _targetRegion->SortByPopulationDensity(0, _targetRegion->getAllCountries().size() - 1);
    _targetRegion->ChangeSortedByPopulationDensity(true);
}

void SortListByName(RegionList*& _targetRegion){
    _targetRegion->ChangeSortedBySize(false);
    _targetRegion->ChangeSortedByPopulationDensity(false);
    _targetRegion->ChangeSortedByPopulation(false);
    _targetRegion->SortByName(0, _targetRegion->getAllCountries().size() - 1);
    _targetRegion->ChangeSortedByName(true);
}

// These other four methods print out an error message if their respective if statement is true.

bool CheckForDisambiguation(string& CountryToCheck){
    bool hasNoAmbiguity = false;
    if (CountryToCheck == "Russia") {
        cout << "\033[31mError:\033[m There are two instances of a country/state named \"Russia.\"" << endl;
        cout << "To compare the European part of Russia, type in \"Russia (European).\"\nTo compare the Asian part of Russia (AKA Siberia), type in \"Russia (Asian).\"" << endl;
        cout << "Comparison aborted." << endl;
    } else if (CountryToCheck == "Georgia") {
        cout << "\033[31mError:\033[m There are two instances of a country/state named \"Georgia.\"" << endl;
        cout << "To compare Georgia the US State, type in \"Georgia (US).\"\nTo compare Georgia the Country, type in \"Georgia (Sakartvelo).\"" << endl;
        cout << "Comparison aborted." << endl;
    } else {
        hasNoAmbiguity = true;
    }
    return hasNoAmbiguity;
}

bool CheckBothCountriesExisting(Country*& country1, Country*& country2){
    bool doesExist = false;
    if(country1 == NULL || country2 == NULL){
        cout << "\033[31mError:\033[m One or both countries in your comparison cannot be identified.\nCannot compare!" << endl;
    } else {
        doesExist = true;
    }
    return doesExist;
}

bool CheckIfCountryExists(Country*& countryToCheck){
    bool doesExist = false;
    if(countryToCheck == NULL){
        cout << "\033[31mError:\033[m The country that you want to compare cannot be identified.\nCannot compare!" << endl;
    } else {
        doesExist = true;
    }
    return doesExist;
}

bool CheckIfRegionExists(RegionList*& regionToCheck){
    bool doesExist = false;
    if(regionToCheck == NULL){
        cout << "\033[31mError:\033[m The region that you want to search in cannot be identified.\nCannot compare!" << endl;
    } else {
        doesExist = true;
    }
    return doesExist;
}

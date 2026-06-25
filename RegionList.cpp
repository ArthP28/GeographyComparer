#include "RegionList.h"
#include <iostream>
#include <math.h>
#include <cctype>

using std::fabs;
using std::tolower;

RegionList::RegionList(string nameForList){
    _listName = nameForList;
}

// -X- DECONSTRUCTOR -X-
RegionList::~RegionList(){
    RemoveAllCountries();
}

// -- PUBLIC METHODS --

// Outputs all countries within the vector list
void RegionList::PrintAllCountries(){
    for(int i = 0; i < allCountries.size(); i++){
        allCountries.at(i)->PrintCountryData();
    }
}
// Removes every country from the region list
void RegionList::RemoveAllCountries(){
    while(!allCountries.empty()){
        DeleteCountryAtIndex(0);
    }
}
// Places a country into the underlying vector as the very last element.
void RegionList::InsertCountry(Country* _country){
    allCountries.push_back(_country);
}
// Removes it from the list but does not delete its contents.
// Primarily used after printing out every country in the world when all the countries are copied into a giant region list.
void RegionList::RemoveCountryAtIndex(int index){
    allCountries.at(index) = NULL;
    allCountries.erase(allCountries.begin() + index);
}
// DELETES the country's contents and removes it from the list.
void RegionList::DeleteCountryAtIndex(int index){
    delete allCountries.at(index);
    allCountries.at(index) = NULL;
    allCountries.erase(allCountries.begin() + index);
}
// Find the country within the region list (Iterative as of now)
Country* RegionList::SearchCountryInList(string countryName){
    Country* countryFound = NULL;
    for(int i = 0; i < allCountries.size(); i++){
        if(allCountries.at(i)->LowerCase() == ConvertStringToLower(countryName)){
            countryFound = allCountries.at(i);
        }
    }
    return countryFound;
}

// -- QUICKSORT FUNCTIONS --
// Quicksort by Land Area
void RegionList::SortByLandArea(int start, int end){
    if(start < end){
        int partitionIndex = PartitionByLand(start, end);
        SortByLandArea(start, partitionIndex - 1);
        SortByLandArea(partitionIndex + 1, end);
    }
}
// Quicksort by Population
void RegionList::SortByPopulation(int start, int end){
    if(start < end){
        int partitionIndex = PartitionByPopulation(start, end);
        SortByPopulation(start, partitionIndex - 1);
        SortByPopulation(partitionIndex + 1, end);
    }
}
// Quicksort by Population Density
void RegionList::SortByPopulationDensity(int start, int end){
    if(start < end){
        int partitionIndex = PartitionByPopulationDensity(start, end);
        SortByPopulationDensity(start, partitionIndex - 1);
        SortByPopulationDensity(partitionIndex + 1, end);
    }
}
// Quicksort by Name
void RegionList::SortByName(int start, int end){
    if(start < end){
        int partitionIndex = PartitionByName(start, end);
        SortByName(start, partitionIndex - 1);
        SortByName(partitionIndex + 1, end);
    }
}
/*
    -- APPROXIMATE-SEARCHING FUNCTIONS --
    These are binary search algorithms that return a country that is the closest to land area, population, or population density
    to the target country. Below each one is a helper method that handles exceptions,
    such as when the returning approximate is the same as the target country, and when an approximate already exists and one is found
    that is closer to the target country.
*/
void RegionList::SearchForClosestByLandArea(Country* _targetCountry, Country*& _closestApproximitant){
    // Handle the corner cases first
    if (_targetCountry->GetLandArea() <= allCountries.front()->GetLandArea()){
        // If the land area of the target country is smaller than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, 0, _targetCountry);
    } else if (_targetCountry->GetLandArea() >= allCountries.back()->GetLandArea()){
        // If the land area of the target country is larger than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, allCountries.size()-1, _targetCountry);
    } else {
        int k = 0; // Current index, set to the start
        int end = allCountries.size(); // End of the current list
        int mid = 0; // Middle of the list
        while (k < end){
            mid = (k + end) / 2;
            // Calculate middle
            if(_targetCountry->GetLandArea() == allCountries.at(mid)->GetLandArea()){
                _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, mid, _targetCountry);
                break;
            }
            // Compare Left (Smaller than mid)
            if(_targetCountry->GetLandArea() < allCountries.at(mid)->GetLandArea()){
                if(mid > 0 && _targetCountry->GetLandArea() > allCountries.at(mid - 1)->GetLandArea()){
                    if(abs(_targetCountry->GetLandArea() - allCountries.at(mid - 1)->GetLandArea()) >= abs(_targetCountry->GetLandArea() - allCountries.at(mid)->GetLandArea())){
                        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, mid - 1, _targetCountry); // Set approximitant to the element before the middle
                    }
                    break;
                }
                end = mid; // Move endpoint to the middle
            }
            else // Compare Right (Larger than mid)
            {
                if (mid < allCountries.size() - 1 && _targetCountry->GetLandArea() < allCountries.at(mid + 1)->GetLandArea()){
                    if(abs(_targetCountry->GetLandArea() - allCountries.at(mid)->GetLandArea()) >= abs(_targetCountry->GetLandArea() - allCountries.at(mid + 1)->GetLandArea())){
                        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, mid + 1, _targetCountry); // Set approximitant to the element after the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForLandArea(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    }
                    break;
                }
                k = mid + 1; // Move endpoint to the element after the middle
            }
        }
    }
}

// This helper function determines if the selected country is closest in size to the target country
Country* RegionList::getClosestApproximitantForLandArea(Country* currentApproximitant, int middleIndex, Country* targetCountry){
    Country* approximitantToReturn = NULL;
    if(allCountries.at(middleIndex) == targetCountry){ // If the selected country is the target country
        if (targetCountry->GetLandArea() <= allCountries.front()->GetLandArea()){
            approximitantToReturn = allCountries.at(middleIndex + 1); // Closest Approximitant is the country after the target if it is the smallest in the allCountries
        } else if (targetCountry->GetLandArea() >= allCountries.back()->GetLandArea()){
            approximitantToReturn = allCountries.at(middleIndex - 1); // Closest Approximitant is the country before the target if it is the largest in the allCountries
        } else if(targetCountry->GetLandArea() - allCountries.at(middleIndex - 1)->GetLandArea() >= (allCountries.at(middleIndex + 1)->GetLandArea() - targetCountry->GetLandArea())){ // Calculate which of the surrounding countries are similar in size to the target country
            approximitantToReturn = allCountries.at(middleIndex + 1);
        } else {
            approximitantToReturn = allCountries.at(middleIndex - 1);
        }
    } else {
        approximitantToReturn = allCountries.at(middleIndex);
    }
    if(currentApproximitant){ // If a current approximitant already exists, it will check the new approximitant with the old approximitant
        // If the difference between the approximitantToReturn and the target is larger than the difference with the existing currentApproximate, the currentApproximate is kept and returned
        if(fabs(targetCountry->GetLandArea() - currentApproximitant->GetLandArea()) < fabs(targetCountry->GetLandArea() - approximitantToReturn->GetLandArea())){
            approximitantToReturn = currentApproximitant;
        }
    }
    return approximitantToReturn;
}


void RegionList::SearchForClosestByPopulation(Country* _targetCountry, Country*& _closestApproximitant){
    // Handle the corner cases first
    if (_targetCountry->GetPopulation() <= allCountries.front()->GetPopulation()){
        // If the population of the target country is smaller than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, 0, _targetCountry);
    } else if (_targetCountry->GetPopulation() >= allCountries.back()->GetPopulation()){
        // If the population of the target country is larger than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, allCountries.size()-1, _targetCountry);
    } else {
        int k = 0; // Current index, set to the start
        int end = allCountries.size(); // End of the current list
        int mid = 0; // Middle of the list
        while (k < end){
            mid = (k + end) / 2;
            // Calculate middle
            if(_targetCountry->GetPopulation() == allCountries.at(mid)->GetPopulation()){
                _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, mid, _targetCountry);
                break;
            }
            // Compare Left (Smaller than mid)
            if(_targetCountry->GetPopulation() < allCountries.at(mid)->GetPopulation()){
                if(mid > 0 && _targetCountry->GetPopulation() > allCountries.at(mid - 1)->GetPopulation()){
                    if(abs(_targetCountry->GetPopulation() - allCountries.at(mid - 1)->GetPopulation()) >= abs(_targetCountry->GetPopulation() - allCountries.at(mid)->GetPopulation())){
                        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, mid - 1, _targetCountry); // Set approximitant to the element before the middle
                    }
                    break;
                }
                end = mid; // Move endpoint to the middle
            }
            else // Compare Right (Larger than mid)
            {
                if (mid < allCountries.size() - 1 && _targetCountry->GetPopulation() < allCountries.at(mid + 1)->GetPopulation()){
                    if(abs(_targetCountry->GetPopulation() - allCountries.at(mid)->GetPopulation()) >= abs(_targetCountry->GetPopulation() - allCountries.at(mid + 1)->GetPopulation())){
                        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, mid + 1, _targetCountry); // Set approximitant to the element after the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForPopulation(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    }
                    break;
                }
                k = mid + 1; // Move endpoint to the element after the middle
            }
        }
    }
}

// This helper function determines if the selected country is closest in population to the target country
Country* RegionList::getClosestApproximitantForPopulation(Country* currentApproximitant, int middleIndex, Country* targetCountry){
    Country* approximitantToReturn = NULL;
    if(allCountries.at(middleIndex) == targetCountry){ // If the selected country is the target country
        if (targetCountry->GetPopulation() <= allCountries.front()->GetPopulation()){
            approximitantToReturn = allCountries.at(middleIndex + 1); // Closest Approximitant is the country after the target if it is the least populous in the allCountries
        } else if (targetCountry->GetPopulation() >= allCountries.back()->GetPopulation()){
            approximitantToReturn = allCountries.at(middleIndex - 1); // Closest Approximitant is the country before the target if it is the most populous in the allCountries
        } else if(abs(targetCountry->GetPopulation() - allCountries.at(middleIndex - 1)->GetPopulation()) >= abs(targetCountry->GetPopulation() - allCountries.at(middleIndex + 1)->GetPopulation())){ // Calculate which of the surrounding countries are similar in population to the target country
            approximitantToReturn = allCountries.at(middleIndex + 1);
        } else {
            approximitantToReturn = allCountries.at(middleIndex - 1);
        }
    } else {
        approximitantToReturn = allCountries.at(middleIndex);
    }
    if(currentApproximitant){ // If a current approximitant already exists, it will check the new approximitant with the old approximitant
        // If the difference between the approximitantToReturn and the target is larger than the difference with the existing currentApproximate, the currentApproximate is kept and returned
        if(fabs(targetCountry->GetPopulation() - currentApproximitant->GetPopulation()) < fabs(targetCountry->GetPopulation() - approximitantToReturn->GetPopulation())){
            approximitantToReturn = currentApproximitant;
        }
    }
    return approximitantToReturn;
}

void RegionList::SearchForClosestByPopulationDensity(Country* _targetCountry, Country*& _closestApproximitant){
    // Handle the corner cases first
    if (_targetCountry->GetPopulationDensity() <= allCountries.front()->GetPopulationDensity()){
        // If the population density of the target country is smaller than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, 0, _targetCountry);
    } else if (_targetCountry->GetPopulationDensity() >= allCountries.back()->GetPopulationDensity()){
        // If the population density of the target country is larger than any of the countries on the current list
        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, allCountries.size()-1, _targetCountry);
    } else {
        int k = 0; // Current index, set to the start
        int end = allCountries.size(); // End of the current list
        int mid = 0; // Middle of the list
        while (k < end){
            mid = (k + end) / 2;
            // Calculate middle
            if(_targetCountry->GetPopulationDensity() == allCountries.at(mid)->GetPopulationDensity()){
                _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, mid, _targetCountry);
                break;
            }
            // Compare Left (Smaller than mid)
            if(_targetCountry->GetPopulationDensity() < allCountries.at(mid)->GetPopulationDensity()){
                if(mid > 0 && _targetCountry->GetPopulationDensity() > allCountries.at(mid - 1)->GetPopulationDensity()){
                    if(abs(_targetCountry->GetPopulationDensity() - allCountries.at(mid - 1)->GetPopulationDensity()) >= abs(_targetCountry->GetPopulationDensity() - allCountries.at(mid)->GetPopulationDensity())){
                        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, mid - 1, _targetCountry); // Set approximitant to the element before the middle
                    }
                    break;
                }
                end = mid; // Move endpoint to the middle
            }
            else // Compare Right (Larger than mid)
            {
                if (mid < allCountries.size() - 1 && _targetCountry->GetPopulationDensity() < allCountries.at(mid + 1)->GetPopulationDensity()){
                    if(abs(_targetCountry->GetPopulationDensity() - allCountries.at(mid)->GetPopulationDensity()) >= abs(_targetCountry->GetPopulationDensity() - allCountries.at(mid + 1)->GetPopulationDensity())){
                        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, mid + 1, _targetCountry); // Set approximitant to the element after the middle
                    } else {
                        _closestApproximitant = getClosestApproximitantForPopulationDensity(_closestApproximitant, mid, _targetCountry); // Set approximitant to the middle
                    }
                    break;
                }
                k = mid + 1; // Move endpoint to the element after the middle
            }
        }
    }
}

// This helper function determines if the selected country is closest in population to the target country
Country* RegionList::getClosestApproximitantForPopulationDensity(Country* currentApproximitant, int middleIndex, Country* targetCountry){
    Country* approximitantToReturn = NULL;
    if(allCountries.at(middleIndex) == targetCountry){ // If the selected country is the target country
        if (targetCountry->GetPopulationDensity() <= allCountries.front()->GetPopulationDensity()){
            approximitantToReturn = allCountries.at(middleIndex + 1); // Closest Approximitant is the country after the target if it is the least densely population in the allCountries
        } else if (targetCountry->GetPopulationDensity() >= allCountries.back()->GetPopulationDensity()){
            approximitantToReturn = allCountries.at(middleIndex - 1); // Closest Approximitant is the country before the target if it is the most densely populated in the allCountries
        } else if(abs(targetCountry->GetPopulationDensity() - allCountries.at(middleIndex - 1)->GetPopulationDensity()) >= abs(targetCountry->GetPopulationDensity() - allCountries.at(middleIndex + 1)->GetPopulationDensity())){ // Calculate which of the surrounding countries are similar in population density to the target country
            approximitantToReturn = allCountries.at(middleIndex + 1);
        } else {
            approximitantToReturn = allCountries.at(middleIndex - 1);
        }
    } else {
        approximitantToReturn = allCountries.at(middleIndex);
    }
    if(currentApproximitant){ // If a current approximitant already exists, it will check the new approximitant with the old approximitant
        // If the difference between the approximitantToReturn and the target is larger than the difference with the existing currentApproximate, the currentApproximate is kept and returned
        if(fabs(targetCountry->GetPopulationDensity() - currentApproximitant->GetPopulationDensity()) < fabs(targetCountry->GetPopulationDensity() - approximitantToReturn->GetPopulationDensity())){
            approximitantToReturn = currentApproximitant;
        }
    }
    return approximitantToReturn;
}

// - GETTER METHODS -

vector<Country*> RegionList::getAllCountries(){
    return allCountries;
}
bool RegionList::isSortedBySize(){
    return _sortedBySize;
}
bool RegionList::isSortedByPopulation(){
    return _sortedByPopulation;
}
bool RegionList::isSortedByPopulationDensity(){
    return _sortedByPopulationDensity;
}
bool RegionList::isSortedByName(){
    return _sortedByName;
}
string RegionList::getNameOfList(){
    return _listName;
}

// - SETTER METHODS -

void RegionList::ChangeSortedBySize(bool newState){
    _sortedBySize = newState;
}
void RegionList::ChangeSortedByPopulation(bool newState){
    _sortedByPopulation = newState;
}
void RegionList::ChangeSortedByPopulationDensity(bool newState){
    _sortedByPopulationDensity = newState;
}
void RegionList::ChangeSortedByName(bool newState){
    _sortedByName = newState;
}

// - Private/Helper Methods -
/*
    -- PARTITION FUNCTIONS --
    They are essential for the Quicksort functionality of the sorting algorithms.
    A pivot is placed at the end, and all the countries less than the pivot are
    sorted.
*/
int RegionList::PartitionByLand(int start, int end){
    Country* pivot = allCountries.at(end);
    int partitionIndex = start;

    for(int i = start; i < end; i++){
        if(allCountries.at(i)->GetLandArea() < pivot->GetLandArea()){
            Country* temp = allCountries.at(i);
            allCountries.at(i) = allCountries.at(partitionIndex);
            allCountries.at(partitionIndex) = temp;
            partitionIndex++;
        }
    }
    Country* temp = allCountries.at(end);
    allCountries.at(end) = allCountries.at(partitionIndex);
    allCountries.at(partitionIndex) = temp;

    return partitionIndex;
}

int RegionList::PartitionByPopulation(int start, int end){
    Country* pivot = allCountries.at(end);
    int partitionIndex = start;

    for(int i = start; i < end; i++){
        if(allCountries.at(i)->GetPopulation() < pivot->GetPopulation()){
            Country* temp = allCountries.at(i);
            allCountries.at(i) = allCountries.at(partitionIndex);
            allCountries.at(partitionIndex) = temp;
            partitionIndex++;
        }
    }
    Country* temp = allCountries.at(end);
    allCountries.at(end) = allCountries.at(partitionIndex);
    allCountries.at(partitionIndex) = temp;

    return partitionIndex;
}

int RegionList::PartitionByPopulationDensity(int start, int end){
    Country* pivot = allCountries.at(end);
    int partitionIndex = start;

    for(int i = start; i < end; i++){
        if(allCountries.at(i)->GetPopulationDensity() < pivot->GetPopulationDensity()){
            Country* temp = allCountries.at(i);
            allCountries.at(i) = allCountries.at(partitionIndex);
            allCountries.at(partitionIndex) = temp;
            partitionIndex++;
        }
    }
    Country* temp = allCountries.at(end);
    allCountries.at(end) = allCountries.at(partitionIndex);
    allCountries.at(partitionIndex) = temp;

    return partitionIndex;
}

int RegionList::PartitionByName(int start, int end){
    Country* pivot = allCountries.at(end);
    int partitionIndex = start;

    for(int i = start; i < end; i++){
        if(allCountries.at(i)->LowerCase() < pivot->LowerCase()){
            Country* temp = allCountries.at(i);
            allCountries.at(i) = allCountries.at(partitionIndex);
            allCountries.at(partitionIndex) = temp;
            partitionIndex++;
        }
    }
    Country* temp = allCountries.at(end);
    allCountries.at(end) = allCountries.at(partitionIndex);
    allCountries.at(partitionIndex) = temp;

    return partitionIndex;
}

string RegionList::ConvertStringToLower(string textToConvert){
    string resultText = textToConvert;
    for(int i = 0; i < textToConvert.length(); i++){
        resultText[i] = tolower(resultText[i]);
    }
    return resultText;
}
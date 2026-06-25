#include "Country.h"
#include <iostream>
#include <cctype>

using std::cout;
using std::endl;
using std::tolower;

Country::Country(string name, string region, string longName, double landArea, double populationData){
    _officialName = longName;
    _shortName = name;
    _location = region;
    _landArea = landArea;
    _population = populationData;
    _populationDensity = (double)(_population/_landArea);
}
// Displays all the information of the country
void Country::PrintCountryData(){
    cout << "\033[44m\033[1;37m" << _officialName << "\033[m" << endl << "Location: " << _location << endl << "Land Area: " << _landArea << " km^2" << endl << "Population: " << _population << endl << "Population Density: " << _populationDensity << " per km^2" << endl;
}
// Turns the country's short name into lowercase for comparing purposes.
string Country::LowerCase(){
    string _lowercaseName = _shortName;
    for(int i = 0; i < _lowercaseName.length(); i++){
        _lowercaseName[i] = tolower(_lowercaseName[i]);
    }
    return _lowercaseName;
}

// - GETTER METHODS -

double Country::GetLandArea(){
    return _landArea;
}

int Country::GetPopulation(){
    return _population;
}

double Country::GetPopulationDensity(){
    return _populationDensity;
}

string Country::GetName(){
    return _shortName;
}
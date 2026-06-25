#pragma once
#include <string>

using std::string;

class Country{
    public:
        Country(string name, string region, string longName, double landArea, double populationData);
        void PrintCountryData();
        double GetLandArea();
        int GetPopulation();
        double GetPopulationDensity();
        string GetName();
        string LowerCase();
    private:
        string _officialName;
        string _shortName;
        string _location;
        double _landArea;
        int _population;
        double _populationDensity;
};
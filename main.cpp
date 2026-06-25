#include "Country.h"
#include "GeographyDatabase.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <thread>
#include <fstream>

using std::ifstream;
using std::cout;
using std::cin;
using std::endl;
using std::setprecision;
using std::fixed;
using std::tolower;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::getline;

// Main Menu Function
void ToMenu();
// Main Submenu Functions
void ToComparison();
void ToFindingApproximate();
void ToPrinting();
void ToSettings();
// Additional Submenu Functions (Pages) (Located under their parent submenus)
void ToPrintingCountryData();
void ToPrintingRegionData();
void ToAbout();
void ToListManager();
void DisplayAllRegionLists();
void ToAddingLists();
void ToRemovingLists();
void PrintAboutText(string title, string txtpath);

// The database is a global variable so that it can be accessed anywhere in the program.
GeographyDatabase _database;

const string DEFAULT_DB_HEADER = "default_dbs/";
const string EXTRA_DB_HEADER = "extra_dbs/";
const string TXT_FILES_HEADER = "txtfiles/";

/*
	Everything in the main.cpp file houses a different part of the Geography Comparer
	program that can be accessed via user input. Said parts are organized into "Menu" Functions.
	Some even have special 
*/

int main(int argc, char * argv[]) {
	cout << fixed << setprecision(2);
	// Add all of the default data sheets first before first starting up the comparer.
	//_database.AddDataSheet("USData.csv", "USA");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"EuropeData.csv", "Europe");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"AsiaData.csv", "Asia");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"AfricaData.csv", "Africa");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"NorthAmericaData.csv", "North America");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"SouthAmericaData.csv", "South America");
	_database.AddDataSheet(DEFAULT_DB_HEADER+"OceaniaData.csv", "Oceania");

	system("clear");
	cout << "Welcome to the Geographic Comparer!" << endl << endl; // This is the message that first appears when you start up the Geography Comparer
	ToMenu();
	return 0;
}
/*
	--- MENU ---
	This is the central hub of the Geography Comparer.
	Pressing a key will take you to a different part of the program
*/
void ToMenu(){
	bool programOn = true;
	// This entire function loops as long as the user does not quit the program.
	// This is primarily to avoid prematurely ending the program whenever the user exits one of the program's submenus.
	while(programOn){
		cout << "\033[4;4mMain Menu\033[m\n" << endl;
		cout << "Press on a key below to begin comparing." << endl;
		cout << "[C] - Compare Two Countries" << endl;
		cout << "[A] - Compare One Country to something most similar to" << endl;
		cout << "[P] - Display country information" << endl;
		cout << "[S] - Settings" << endl;
		cout << "[Q] - Quit Program\n" << endl;
		string userInput = "";
		while(userInput == ""){ // While there is no input yet, wait for the user to put in one and process the results
			getline(cin, userInput);
			switch(tolower(userInput[0])){
				case 'c': // To a Basic Comparison between two countries
					ToComparison();
					break;
				case 'a': // To finding the closest approximate country
					ToFindingApproximate();
					break;
				case 'p': // To printing out an entire list of countries
					ToPrinting();
					break;
				case 's': // To accessing the settings
					ToSettings();
					break;
				case 'q': // Quits the program
					system("clear"); // The entire screen/terminal is cleared first before quitting to ensure a clean shutdown
					cout << "Goodbye for now!" << endl;
					programOn = false; // Because the program is quitting, this bool is set to false and the flow of control exists the first encompassing while loop.
					break;
				default: // Invalid input
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					userInput = ""; // Input is reset back to "null" and the loop restarts.
					cout << "\033[A\033[2K"; // Clears the error message
			}
		}
	}
}
/*
	--- BASIC COMPARISON ---
	This is where the user inputs two countries in the database and outputs whichever country has more land/people/density than the other.
*/
void ToComparison(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){ // This function goes on as long as the user wants it to go
		system("clear"); // Clean the screen of any prior printed lines.
		cout << "\033[4;47m\033[30mBasic Comparison\033[m\n" << endl;
		string countryToCompare1;
		string countryToCompare2;
		cout << "Please type in the names of two countries to begin comparing:" << endl;
		// The input code is in a while loop because the program should not take empty inputs
		while(countryToCompare1 == ""){
			getline(cin, countryToCompare1);
			if(countryToCompare1 == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		// Ditto for the other.
		while(countryToCompare2 == ""){
			getline(cin, countryToCompare2);
			if(countryToCompare2 == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		// The program now asks if you want to compare the two countries by land area, population, or density.
		string comparisonChoice;
		cout << "\nWould you like to compare them by" << endl;
		cout << "[1] - Land Area\n[2] - Population\n[3] - Population Density" << endl;
		while(comparisonChoice == ""){
			getline(cin, comparisonChoice);
			switch(comparisonChoice[0]){ // Within this switch statement, a different function of the Geography Database is called depending on the user input
				case '1':
					system("clear");
					_database.CompareByLandArea(countryToCompare1, countryToCompare2);
					// After a comparison is completed, the program then asks the user either to do another one or go back to the main menu.
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '2':
					system("clear");
					_database.CompareByPopulation(countryToCompare1, countryToCompare2);
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '3':
					system("clear");
					_database.CompareByPopulationDensity(countryToCompare1, countryToCompare2);
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					comparisonChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
	system("clear"); // If the user decides to not do another comparison, the screen is cleaned as the program returns back to the main menu.
}
/*
	--- COMPARISON TO CLOSEST APPROXIMATE ---
	This is where a special comparison is performed where the function returns the country closest in size/population/density to the one the user requested.
*/
void ToFindingApproximate(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){
		system("clear");
		cout << "\033[4;47m\033[30mComparison to Closest Approximate\033[m\n" << endl;
		string countryToCompare;
		cout << "Type in the name of a country to start comparing." << endl;
		// This time, the function only needs the user to type in the name of one country only.
		while(countryToCompare == ""){
			getline(cin, countryToCompare);
			if(countryToCompare == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		// This part about typing in a region is optional and does not need a while loop with an error message.
		cout << "For more interesting comparisons, you can type in the name of a continent or region or federal country to only compare your selected country to others within a specific place." << endl;
		string regionToSearch;
		cout << "If you don't want to, hit enter again." << endl;
		getline(cin, regionToSearch);

		string comparisonChoice;
		cout << "\nWould you like to compare it by" << endl;
		cout << "[1] - Land Area\n[2] - Population\n[3] - Population Density" << endl;
		while(comparisonChoice == ""){
			getline(cin, comparisonChoice);
			switch(comparisonChoice[0]){
				case '1':
					system("clear");
					if(regionToSearch != ""){ // If the user has put in a region name, the Database's overloaded find function will execute instead of the normal one down below.
						_database.FindClosestByLandArea(countryToCompare, regionToSearch);
					} else {
						_database.FindClosestByLandArea(countryToCompare);
					}
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '2':
					system("clear");
					if(regionToSearch != ""){
						_database.FindClosestByPopulation(countryToCompare, regionToSearch);
					} else {
						_database.FindClosestByPopulation(countryToCompare);
					}
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '3':
					system("clear");
					if(regionToSearch != ""){
						_database.FindClosestByPopulationDensity(countryToCompare, regionToSearch);
					} else {
						_database.FindClosestByPopulationDensity(countryToCompare);
					}
					cout << "\nWould you like to do another comparison?" << endl;
					cout << "Press [y] to do it again, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					comparisonChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
	system("clear");
}
/*
	--- PRINTING ---
	This is where a special comparison is performed where the function returns the country closest in size/population/density to the one the user requested.
*/
void ToPrinting(){
	bool inPrintingMenu = true;
	while(inPrintingMenu){
		system("clear");
		cout << "\033[4;47m\033[30mDisplay Country Data\033[m\n" << endl;
		// This menu will first ask the user to print either the data of an individual country or multiple countries of an entire region.
		string printingChoice;
		cout << "Would you like to display the information of a country or a region, or would you like to exit?" << endl;
		cout << "[1] - Country\n[2] - Region\n[E] - Back to Menu" << endl;
		while(printingChoice == ""){
			getline(cin, printingChoice);
			switch(tolower(printingChoice[0])){ // The first two switch cases will direct the program to one of the two submenus within the print submenu.
				case '1':
					system("clear");
					ToPrintingCountryData();
					break;
				case '2':
					system("clear");
					ToPrintingRegionData();
					break;
				case 'e':
					// Unlike other submenu functions, pressing 'e' will set the bool below to false and take the program out of the overarching while loop and back into the main menu.
					inPrintingMenu = false;
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					printingChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
	system("clear");
}
/*
	-- Print Country Data --
	This is a submenu of the PRINTING parent menu that displays the information of an individual country
*/
void ToPrintingCountryData(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){
		system("clear");
		string countryToDisplay;
		cout << "What country would you like to see the information of?" << endl;
		while(countryToDisplay == ""){
			getline(cin, countryToDisplay);
			if(countryToDisplay == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		system("clear");
		_database.PrintCountryInDatabase(countryToDisplay);
		cout << "\nWould you like to take a look at another country?" << endl;
		cout << "Press [y] to display another country, or any other input to cancel." << endl;
		getline(cin, affirmationSignal);
	}
}
/*
	-- Print Region Data --
	This is a submenu of the PRINTING parent menu that displays the information of all individual country in a specific part of the world.
*/
void ToPrintingRegionData(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){
		system("clear");
		string regionToDisplay;
		cout << "What part of the world would you like to see the information of every country in there?" << endl;
		cout << "(Leave blank to display every country in the world)" << endl;
		getline(cin, regionToDisplay);

		string displayChoice;
		cout << "\nWould you like to sort all countries in the region by" << endl;
		cout << "[1] - Land Area\n[2] - Population\n[3] - Population Density\n[4] - Alphabetical Order" << endl;
		while(displayChoice == ""){
			getline(cin, displayChoice);
			switch(displayChoice[0]){ // This switch statement has an additional case that prints out all countries within a region in alphabetical order.
				case '1': // Land Area
					system("clear");
					if(regionToDisplay != ""){
						_database.PrintAllCountriesByLandArea(regionToDisplay);
					} else {
						_database.PrintAllCountriesByLandArea();
					}
					cout << "\nWould you like to display the information of another region?" << endl;
					cout << "Press [y] to display another region, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '2': // Population
					system("clear");
					if(regionToDisplay != ""){
						_database.PrintAllCountriesByPopulation(regionToDisplay);
					} else { // Leaving the region input blank will display every country in the world sorted by land area/population/density/name.
						_database.PrintAllCountriesByPopulation();
					}
					cout << "\nWould you like to display the information of another region?" << endl;
					cout << "Press [y] to display another region, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '3': // Population Density
					system("clear");
					if(regionToDisplay != ""){
						_database.PrintAllCountriesByPopulationDensity(regionToDisplay);
					} else {
						_database.PrintAllCountriesByPopulationDensity();
					}
					cout << "\nWould you like to display the information of another region?" << endl;
					cout << "Press [y] to display another region, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				case '4': // In Alphabetical Order
					system("clear");
					if(regionToDisplay != ""){
						_database.PrintAllCountriesByName(regionToDisplay);
					} else {
						_database.PrintAllCountriesByName();
					}
					cout << "\nWould you like to display the information of another region?" << endl;
					cout << "Press [y] to display another region, or any other input to cancel." << endl;
					getline(cin, affirmationSignal);
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					displayChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
}
/*
	-- SETTINGS --
		About: Describes about why you made this program, what motivated you to make it, what it can do, and some proposed new features
		Manage Country Lists: Add and remove data lists within the program. Also includes a how/to page on making your own Country List
*/
void ToSettings(){
	bool inSettings = true;
	while(inSettings){
		system("clear");
		cout << "\033[4;47m\033[30mSettings\033[m\n" << endl;
		string settingsChoice;
		cout << "Where would you like to go?" << endl;
		cout << "[1] - About\n[2] - Manage Regions\n[E] - Back to Menu" << endl;
		while (settingsChoice == ""){
			getline(cin, settingsChoice);
			switch(tolower(settingsChoice[0])){
				case '1':
					ToAbout();
					break;
				case '2':
					ToListManager();
					break;
				case 'e':
					inSettings = false;
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					settingsChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
	system("clear");
}
/* 	
	-- About Submenu --
	This subpage is a Q&A discussing the project, its purpose, its origins, and its upcoming features.
*/
void ToAbout(){
	bool inAboutPage = true;
	while(inAboutPage){
		system("clear");
		cout << "\033[4;47m\033[30mAbout\033[m\n" << endl;
		string aboutPageChoice;
		cout << "What part of this page would you like to view?" << endl;
		cout << "[1] - What can this program do?\n[2] - What is the purpose of this program?\n[3] - What inspired you to work on this?\n[4] - Future Features\n[E] - Exit" << endl;
		while (aboutPageChoice == ""){
			getline(cin, aboutPageChoice);
			switch(tolower(aboutPageChoice[0])){
				case '1':
					PrintAboutText("What can this program do?", "aboutwhatcanitdo.txt");
					break;
				case '2':
					PrintAboutText("What is the purpose of this program?", "aboutitspurpose.txt");
					break;
				case '3':
					PrintAboutText("What inspired you to work on this?", "aboutwhatinspiredyou.txt");
					break;
				case '4':
					PrintAboutText("Future Features", "aboutfuturefeatures.txt");
					break;
				case 'e':
					inAboutPage = false;
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					aboutPageChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
}
/*
	- Print About Text -
	Instead of printing one very long string, this function takes in a text file and prints out all its contents.
	This helps keep the main function nice and organized.
*/
void PrintAboutText(string title, string txtpath){
	ifstream infile;
	txtpath = TXT_FILES_HEADER + txtpath;
    infile.open(txtpath);

    system("clear");
	cout << "\033[4;47m\033[30m" << title << "\033[m\n" << endl;
    if(infile.good()){
        string dataLine;
        while(getline(infile, dataLine)){
            cout << dataLine << endl;
        }
		infile.close();
    } else {
        cout << "\033[31mError:\033[m \"" + txtpath + "\" cannot be opened!" << endl;
    }
	string exitPrompt;
	cout << "\nPress enter to exit." << endl;
	getline(cin, exitPrompt);
}
/*
	-- List Manager --
	This submenu allows users to import region lists and delete them from the program.
	Any changes made will be reset when the software restarts.
*/
void ToListManager(){
	bool inListsManager = true;
	while(inListsManager){
		system("clear");
		cout << "\033[4;47m\033[30mManage Regions\033[m\n" << endl;
		string managerPageChoice;
		cout << "What part of this page would you like to view?" << endl;
		cout << "[1] - Display All Lists\n[2] - Import Region File\n[3] - Delete Country List\n[4] - How to make your own region list\n[E] - Exit" << endl;
		while (managerPageChoice == ""){
			getline(cin, managerPageChoice);
			switch(tolower(managerPageChoice[0])){
				case '1': // Display current region lists in program
					DisplayAllRegionLists();
					break;
				case '2': // Imports a list into the program
					ToAddingLists();
					break;
				case '3': // Removes a list from the program
					ToRemovingLists();
					break;
				case '4': // A how-to guide on making your own country lists.
					PrintAboutText("How to make your own Country List", "abouthowtomakeregionlist.txt");
					break;
				case 'e':
					inListsManager = false;
					break;
				default:
					cout << "\033[A\033[2KInvalid Input" << endl;
					sleep_for(milliseconds(500));
					managerPageChoice = "";
					cout << "\033[A\033[2K";
			}
		}
	}
}
/*
	- Display All Region Lists -
	Shows all the country lists currently stored in the Geography Comparer
*/
void DisplayAllRegionLists(){
	system("clear");
	cout << "\033[4;47m\033[30mAll Region Lists\033[m\n" << endl;
	_database.PrintAllRegionLists();
	string exitToMenuInput;
	cout << "\nPress any character to return to the menu." << endl;
	getline(cin, exitToMenuInput);
}
/*
	- Importing Lists -
	Takes in a .csv file and converts it into a country list for use in the Comparer
*/
void ToAddingLists(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){
		system("clear");
		// The user must first put in the name of the .csv file to be converted as well as a title for later reference.
		string fileName;
		string fileTitle;
		cout << "Type in the name of the .csv file you want to add and give your region list a title." << endl;
		while (fileName == ""){
			cout << "File Name: "; getline(cin, fileName);
			if(fileName == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		while (fileTitle == ""){
			cout << "Title: "; getline(cin, fileTitle);
			if(fileTitle == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		fileName = EXTRA_DB_HEADER + fileName;
		if(_database.AddDataSheet("extra_dbs/" + fileName, fileTitle)){ // The user's input is then used to find the corresponding .csv file and turn it into a country list
			cout << "\n\033[32mData Sheet Successfully added!\033[m" << endl;
			cout << "Notice: Any data sheets you import into the Geography Comparer can be used as long as the program is running." << endl;
			cout << "If you decide to turn off the Geography Comparer, you will have to reimport your data tables the next time you use the program." << endl;
		}
		cout << "\nWould you like to add another data sheet?" << endl;
		cout << "Press [y] to import another data sheet, or another input to return to the List Manager." << endl;
		getline(cin, affirmationSignal);
	}
}
/*
	- Removing Lists -
	Search for the country list that the user requests and removes it from the program.
*/
void ToRemovingLists(){
	string affirmationSignal = "y";
	while(tolower(affirmationSignal[0]) == 'y'){
		system("clear");
		string fileTitle;
		cout << "Type in the name of the region list you want to remove from the program." << endl;
		while (fileTitle == ""){
			cout << "List Name: "; getline(cin, fileTitle);
			if(fileTitle == ""){
				cout << "\033[A\033[2KInput field is empty!" << endl;
				sleep_for(seconds(1));
				cout << "\033[A\033[2K";
			}
		}
		if(_database.RemoveDataSheet(fileTitle)){
			cout << "\n\033[32mData Sheet Successfully Removed!\033[m" << endl;
			cout << "Notice: Any data sheets you remove can no longer be used while the program is running." << endl;
			cout << "If you restart the Geography Comparer, the removed data sheets will return. This does not delete the .csv files within the program." << endl;
		}
		cout << "\nWould you like to add remove data sheet?" << endl;
		cout << "Press [y] to remove another data sheet, or another input to return to the List Manager." << endl;
		getline(cin, affirmationSignal);
	}
}
#include <iostream> 
#include <fstream> 
#include <string>
#include <vector> 
#include <algorithm> 
#include <map>

//create "Person" class with all relevant information
struct Person
{
	std::string name; 
	std::string gender; 
	std::string matchGender; 
	std::vector<std::string> extracurriculars; 
};

//FUNCTION OVERVIEW: 

std::vector<Person> createSiblingsVector(std::ifstream& input); // Creates vectors containing sibling information based on text file
std::vector<Person> filterSiblingsByGender(Person littleSibling, std::vector<Person> bigSiblings); // Ensures that siblings gender preferences are honored
double calculateCompatibility(Person littleSibling, Person bigSibling); //Calculates compatibility between a given sibling pair

int main()
{
	//create input files
	std::ifstream upperclassmen("Upperclassmen.txt");
	std::ifstream sophomores("Sophomores.txt");
	//create vectors containing information of all big and little siblings
	std::vector<Person> bigSiblings = createSiblingsVector(upperclassmen); 
	std::vector<Person> littleSiblings = createSiblingsVector(sophomores); 
	//take input from the user to see which little sibling to find matches for
	std::string littleSiblingName; 
	std::cout << "Please input the name (as spelled on Google Sheets) of the little sibling that you would like to match: " << "\n"; 
	getline(std::cin, littleSiblingName); 
	//identify the information of the little sibling in question
	Person littleSibling; 
	for (std::vector<Person>::iterator it = littleSiblings.begin(); it != littleSiblings.end(); it++)
	{
		if (it->name == littleSiblingName)
		{ 
			littleSibling.name = it->name;
			littleSibling.gender = it->gender; 
			littleSibling.matchGender = it->matchGender; 
			littleSibling.extracurriculars = it->extracurriculars; 
		}
	}
	//filter the possible big siblings based on mutual gender preferences
	bigSiblings = filterSiblingsByGender(littleSibling, bigSiblings); 
	//calculate compatibility scores of the possible siblings pairs
	std::map<std::string, double> compatibilityScores; 
	for (int i = 0; i < bigSiblings.size(); i++)
	{
		double compatibilityMeasure = calculateCompatibility(littleSibling, bigSiblings[i]); 
		compatibilityScores.insert(std::pair<std::string,double>(bigSiblings[i].name, compatibilityMeasure));
	} 
	//output to the user the compatibility scores
	double counter = 0; 
	//the value of 12 for "i" is because the scores are in intervals of 0.5 and the maximum possible scores is 6
	for (int i = 0; i<12; i++)
	{
		for (std::map<std::string, double>::iterator iter = compatibilityScores.begin(); iter != compatibilityScores.end(); iter++)
		{
			//the maximum compatibility value is 6, so we start displaying values from highest compatibility to lowest
			if (iter->second == 6-counter)
			{
				std::cout << iter->first << " " << iter->second << "\n";
			}
		}
		counter += 0.5; 
	}
	return 0; 
}

std::vector<Person> createSiblingsVector(std::ifstream& input)
{
	std::vector<Person> siblings;
	std::vector<std::string> activities; 
	std::string line;
	Person sibling;
	int count = 1;
	while (getline(input, line))
	{
		int lineType = count % 6;
		switch (lineType)
		{
		case 1:
			sibling.name = line;
			break;
		case 2:
			sibling.gender = line;
			break;
		case 3:
			sibling.matchGender = line;
			break;
		case 4:
			activities.push_back(line); 
			break; 
		case 5:
			activities.push_back(line);
			break;
		case 0:
			activities.push_back(line);
			sibling.extracurriculars = activities;
			activities.clear();
			siblings.push_back(sibling);
			break;
		}
		count++;
	}
	return siblings;
}

std::vector<Person> filterSiblingsByGender(Person littleSibling, std::vector<Person> bigSiblings)
{
	std::vector<Person> newBigSiblings;
	if (littleSibling.matchGender == "Female")
	{
		for (int i = 0; i < bigSiblings.size(); i++)
		{
			if (bigSiblings[i].gender == "Female" && ((bigSiblings[i].matchGender == littleSibling.gender)) || (bigSiblings[i].matchGender == "No Preference"))
			{
				newBigSiblings.push_back(bigSiblings[i]); 
			}
		}
	}
	else if (littleSibling.matchGender == "Male")
	{
		for (int i = 0; i < bigSiblings.size(); i++)
		{
			if (bigSiblings[i].gender == "Male" && ((bigSiblings[i].matchGender == littleSibling.gender)) || (bigSiblings[i].matchGender == "No Preference"))
			{
				newBigSiblings.push_back(bigSiblings[i]);
			}
		} 
	}
	else if (littleSibling.matchGender == "No Preference")
	{
		for (int i = 0; i < bigSiblings.size(); i++)
		{
			if ((bigSiblings[i].matchGender == littleSibling.gender) || (bigSiblings[i].matchGender == "No Preference"))
			{
				newBigSiblings.push_back(bigSiblings[i]);
			}
		}
	}
	return newBigSiblings;
}

double calculateCompatibility(Person littleSibling, Person bigSibling)
{
	double compatibilityMeasure = 0; 
	for (int i = 0; i < littleSibling.extracurriculars.size(); i++)
	{
		for (int j = 0; j < bigSibling.extracurriculars.size(); j++)
		{
			if (littleSibling.extracurriculars[i] == bigSibling.extracurriculars[j])
			{
				compatibilityMeasure += ((littleSibling.extracurriculars.size() - i) + (bigSibling.extracurriculars.size() - j)) * 0.5; 
			}
		}
	}
	return compatibilityMeasure; 
}
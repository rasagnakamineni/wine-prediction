#include <iostream>
#include <string>
#include <vector>
#include<utility>
#include<climits>
#include<algorithm>

using namespace std;

class FamilyNode
{
public:
    vector<FamilyNode *> children;
    string name;
    float income;
    string occupation;
    int age;
   // Constructor
    FamilyNode(std::string name, float income, std::string occupation, int age)
        : name(name), income(income), occupation(occupation), age(age)
    {
        // Initialize other members or perform additional setup if needed
    }
    // declare other members as per question's requirement
};

// Note:Input of nodes will be taken in order according to pre-order traversal of tree.
FamilyNode *buildTree()
{
    string name;
    cout << "Enter name of member" << endl;
    cin >> name;

    // Take other parameters as inputs

    int numOfChildren;
    cout << "Enter number of children" << endl;
    cin >> numOfChildren;

    float income;
    cout << "Enter income" << endl;
    cin >> income;

    string occupation;
    cout << "Enter occupation" << endl;
    cin >> occupation;

    int age;
    cout << "Enter age" << endl;
    cin >> age;

    FamilyNode *node = new FamilyNode(name, income, occupation, age);
    // Initialize the node

    for (int i = 0; i < numOfChildren; i++)
    {
        // Build tree rooted at child i. Add this to children vector of current node.
        node->children.push_back(buildTree());
    }

    return node;
}

// This function checks if a node with given parameters exists or not
bool checkIfNodeExists(FamilyNode *node, string name, int age, float income, string occupation)
{
    if(!node)return false;
    if(node->name==name){
        if(node->age==age && node->income == income && node->occupation == occupation){
            return true;
        }
        else return false;
    }
    for(auto&c:node->children){
        if(checkIfNodeExists(c, name, age, income, occupation)){
            return true;
        }
    }
    return false;
    // Complete the code
}

void removeNode(FamilyNode *node, FamilyNode *parent, string name, int age, float income, string occupation)
{
    if(!node)return;
    if (node->name.compare(name) == 0) // Complete the condition to check if current node is required node
    {
        float maxIncome=-1;
        FamilyNode* maxIncomeNode;
        for(auto &c:node->children){
            if(c->income>maxIncome){
                maxIncome=c->income;
                maxIncomeNode=c;
            }
        }
        
        //shift children of deleted node to maxincomenodec children
        for(auto&c:node->children){
            if(c!=maxIncomeNode){
                maxIncomeNode->children.push_back(c);
            }
        }

        //replace in parents children
        if(parent){
            int i;
            for(i=0; i<parent->children.size(); i++){
                if(parent->children[i]==node){
                    break;
                }
            }
            parent->children[i]=maxIncomeNode;
        }
        // Complete the code to remove current node from tree
        // Replace current node with children having highest income
        //  Update the children vector of current node and parent accordingly
        return;
    }

    else{
        for(auto&c:node->children){
            removeNode(c, node, name, age, income, occupation);
        }
    }

    // Continue the process for other nodes
}

// Complete the recursive function to calculate total income of a family
float getTotalIncome(FamilyNode *node)
{
    if(!node)return 0;
    float totalIncome=node->income; // Complete initialization of totalIncome

    for (int i = 0; i < node->children.size(); i++)
    {
        totalIncome+=getTotalIncome(node->children[i]);
        // Complete the code
    }

    return totalIncome;
}

// Complete the recursive function to calculate total income of a family
int getTotalAge(FamilyNode *node)
{
    if(!node)return 0;
    int totalAge=node->age; // Complete initialization of totalIncome

    for (int i = 0; i < node->children.size(); i++)
    {
        totalAge+=getTotalAge(node->children[i]);
        // Complete the code
    }

    return totalAge;
}

// The function returns the ancestor of family having highest income-to-age ratio
FamilyNode *getHighestIncomeAgeRatioFamily(vector<FamilyNode *> &familyTrees)
{
    // Complete the code
    float curmax=-1;
    FamilyNode* family;
    
    for (int i = 0; i < familyTrees.size(); i++)
    {
        float currFamilyTotalIncome = getTotalIncome(familyTrees[i]);
        int currFamilyTotalAge = getTotalAge(familyTrees[i]);
        float currFamilyRatio;
        if(currFamilyRatio>curmax){
            curmax=currFamilyRatio;
            family=familyTrees[i];
        }
        // Complete the code here
    }
    return family;
    // Return the ancestor node of family with highest income-to-age ratio
}

void printFamiliesInDecrOrder(vector<FamilyNode *> &familyTrees)
{
    vector<pair<float, string>> vec;
    
    for (int i = 0; i < familyTrees.size(); i++)
    {
        float currFamilyTotalIncome = getTotalIncome(familyTrees[i]);
        vec.push_back({currFamilyTotalIncome, familyTrees[i]->name});
    }
    sort(vec.begin(), vec.end());
    reverse(vec.begin(), vec.end());
    for(auto&v:vec){
        cout<<v.second<<endl;
    }
}

int main()
{
    int numOfFamilies;
    cout << "Enter the number of families\n";
    cin >> numOfFamilies;

    vector<FamilyNode *> familyTrees; // This vector stores ancestor nodes of all families

    for (int i = 0; i < numOfFamilies; i++)
    {
        familyTrees.push_back(buildTree());
    }

    while (1)
    {
        int choice;
        cout << "1. Remove node" << endl;
        cout << "2. Find Highest Income-To-Age-Ratio Family" << endl;
        cout << "3. Print Family(Ancestor) Names in Decreasing Income Order" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice" << endl;

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int num;
            cout << "Enter Family member whose node is to be removed" << endl;
            cin >> num;
            cout << "Enter name, age, income and occupation of member to be removed" << endl;
            string name, occupation;
            int age;
            float income;
            cin >> name >> age >> income >> occupation;

            if (!checkIfNodeExists(familyTrees[num], name, age, income, occupation))
                cout << "No such family member exists" << endl;
            else
                removeNode(familyTrees[num], NULL, name, age, income, occupation);
            break;
        }
        case 2:
        {
            FamilyNode *node = getHighestIncomeAgeRatioFamily(familyTrees);
            cout << "Name of ancestor of family with highest total income is " << node->name;
            break;
        }
        case 3:
        {
            printFamiliesInDecrOrder(familyTrees);
            break;
        }
        case 4:
        {
            return 0;
            break;
        }
        default:
        {
            cout << "Incorrect choice!!!" << endl;
            break;
        }
        }
    }

    return 0;
}
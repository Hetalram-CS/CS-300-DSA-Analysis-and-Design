//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Hetal Ram
// Course      : CS 300
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

//============================================================================
// Course structure
//============================================================================

// Stores information for one course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

//============================================================================
// Binary Search Tree Node
//============================================================================

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//============================================================================
// Helper functions
//============================================================================

// Remove spaces from the beginning and end of a string
string trim(string value) {
    size_t first = value.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

// Convert a course number to uppercase
string toUpperCase(string value) {
    transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });

    return value;
}

//============================================================================
// Binary Search Tree
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    // Insert a course recursively
    Node* insertNode(Node* node, Course course) {

        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        }
        else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        }

        return node;
    }

    // Print courses in alphanumeric order
    void inOrder(Node* node) {

        if (node != nullptr) {

            inOrder(node->left);

            cout << node->course.courseNumber
                << ", "
                << node->course.courseTitle
                << endl;

            inOrder(node->right);
        }
    }

    // Delete all nodes when the program ends
    void deleteTree(Node* node) {

        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);

            delete node;
        }
    }

public:

    // Constructor
    BinarySearchTree() {
        root = nullptr;
    }

    // Destructor
    ~BinarySearchTree() {
        deleteTree(root);
        root = nullptr;
    }

    // Insert a course
    void Insert(Course course) {
        root = insertNode(root, course);
    }

    // Search for a course
    Course Search(string courseNumber) {

        Node* current = root;

        while (current != nullptr) {

            if (courseNumber == current->course.courseNumber) {
                return current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        // Return an empty course if not found
        Course emptyCourse;
        return emptyCourse;
    }

    // Display all courses in sorted order
    void InOrder() {
        inOrder(root);
    }
};

//============================================================================
// Load course data from CSV file
//============================================================================

bool loadCourses(string fileName, BinarySearchTree& courseTree) {

    ifstream inputFile(fileName);

    // Check whether the file opened correctly
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open file "
            << fileName << "." << endl;

        return false;
    }

    string line;
    int courseCount = 0;

    while (getline(inputFile, line)) {

        // Skip blank lines
        if (trim(line).empty()) {
            continue;
        }

        stringstream lineStream(line);

        string courseNumber;
        string courseTitle;
        string prerequisite;

        // Read course number
        if (!getline(lineStream, courseNumber, ',')) {
            cout << "Error: Invalid course data." << endl;
            continue;
        }

        // Read course title
        if (!getline(lineStream, courseTitle, ',')) {
            cout << "Error: Invalid course data." << endl;
            continue;
        }

        courseNumber = toUpperCase(trim(courseNumber));
        courseTitle = trim(courseTitle);

        // Course number and title are required
        if (courseNumber.empty() || courseTitle.empty()) {
            cout << "Error: Course number or title is missing."
                << endl;

            continue;
        }

        Course course;

        course.courseNumber = courseNumber;
        course.courseTitle = courseTitle;

        // Read all remaining prerequisite fields
        while (getline(lineStream, prerequisite, ',')) {

            prerequisite = toUpperCase(trim(prerequisite));

            // Ignore empty prerequisite fields
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        // Insert the course into the Binary Search Tree
        courseTree.Insert(course);

        courseCount++;
    }

    inputFile.close();

    if (courseCount == 0) {
        cout << "Error: No course data was loaded." << endl;
        return false;
    }

    cout << "Courses loaded successfully." << endl;

    return true;
}

//============================================================================
// Display information for one course
//============================================================================

void displayCourseInformation(
    Course course,
    BinarySearchTree& courseTree) {

    cout << course.courseNumber
        << ", "
        << course.courseTitle
        << endl;

    // Display prerequisite information
    if (course.prerequisites.empty()) {

        cout << "Prerequisites: None" << endl;
    }
    else {

        cout << "Prerequisites:" << endl;

        for (const string& prerequisiteNumber : course.prerequisites) {

            Course prerequisiteCourse =
                courseTree.Search(prerequisiteNumber);

            if (!prerequisiteCourse.courseNumber.empty()) {

                cout << prerequisiteCourse.courseNumber
                    << ", "
                    << prerequisiteCourse.courseTitle
                    << endl;
            }
            else {

                // Handles invalid prerequisite course numbers
                cout << prerequisiteNumber
                    << " - Course information not found."
                    << endl;
            }
        }
    }
}

//============================================================================
// Main Program
//============================================================================

int main() {

    BinarySearchTree courseTree;

    bool dataLoaded = false;

    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;

        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        cout << "What would you like to do? ";

        // Validate numeric input
        if (!(cin >> choice)) {

            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid input. Please enter 1, 2, 3, or 9."
                << endl;

            continue;
        }

        cin.ignore(10000, '\n');

        //============================================================
        // Option 1: Load Course Data
        //============================================================

        if (choice == 1) {

            string fileName;

            cout << "What is the name of the course data file? ";
            getline(cin, fileName);

            dataLoaded = loadCourses(fileName, courseTree);
        }

        //============================================================
        // Option 2: Print Course List
        //============================================================

        else if (choice == 2) {

            if (!dataLoaded) {

                cout << "Please load the course data first."
                    << endl;
            }
            else {

                cout << "Here is a sample schedule:" << endl;

                // In-order traversal prints courses
                // alphanumerically from lowest to highest
                courseTree.InOrder();
            }
        }

        //============================================================
        // Option 3: Print One Course
        //============================================================

        else if (choice == 3) {

            if (!dataLoaded) {

                cout << "Please load the course data first."
                    << endl;
            }
            else {

                string courseNumber;

                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                // Allow lowercase input such as csci400
                courseNumber =
                    toUpperCase(trim(courseNumber));

                Course course =
                    courseTree.Search(courseNumber);

                if (course.courseNumber.empty()) {

                    cout << "Course "
                        << courseNumber
                        << " not found."
                        << endl;
                }
                else {

                    displayCourseInformation(
                        course,
                        courseTree);
                }
            }
        }

        //============================================================
        // Option 9: Exit
        //============================================================

        else if (choice == 9) {

            cout << "Thank you for using the course planner!"
                << endl;
        }

        //============================================================
        // Invalid Menu Option
        //============================================================

        else {

            cout << choice
                << " is not a valid option."
                << endl;
        }
    }

    return 0;
}
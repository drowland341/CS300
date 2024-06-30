//============================================================================
// Name        : CS300Project2.cpp
// Author      : Douglas Rowland
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Function to print course details
void printCourse(Course& course) {
    cout << "Course Number: " << course.courseNumber << endl;
    cout << "Course Title: " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites:" << endl;
        for (const string& prereq : course.prerequisites) {
            cout << prereq << endl;
        }
    }
    else {
        cout << "No prerequisites" << endl;
    }
    cout << endl;
}

// Node structure for the Binary Search Tree
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class definition
class BinarySearchTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, Course course) {
        if (node == nullptr) {
            return new TreeNode(course);
        }
        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            printCourse(node->course);
            inOrderTraversal(node->right);
        }
    }

    TreeNode* search(TreeNode* node, string courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber) {
            return node;
        }
        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insertCourse(Course course) {
        root = insert(root, course);
    }

    void printAllCourses() {
        inOrderTraversal(root);
    }

    void printCourseInfo(string courseNumber) {
        TreeNode* result = search(root, courseNumber);
        if (result == nullptr) {
            cout << "Course not found." << endl;
        }
        else {
            printCourse(result->course);
        }
    }
};

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void loadCourses(const string& filePath, BinarySearchTree& bst) {
    ifstream file(filePath);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    while (getline(file, line)) {
        vector<string> parts = split(line, ',');
        if (parts.size() < 2) {
            continue; // Skip malformed lines
        }

        Course course;
        course.courseNumber = parts[0];
        course.courseTitle = parts[1];
        for (size_t i = 2; i < parts.size(); i++) {
            course.prerequisites.push_back(parts[i]);
        }

        bst.insertCourse(course);
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
}

int main() {
    BinarySearchTree bst;
    int choice;

    while (true) {
        cout << "Menu:\n"
            << "1. Load the file data into the data structure\n"
            << "2. Print an alphanumeric list of all the courses\n"
            << "3. Print the course title and the prerequisites for any individual course\n"
            << "9. Exit the program\n"
            << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            string filePath = "CS 300 ABCU_Advising_Program_Input.csv";
            loadCourses(filePath, bst);
            break;
        }
        case 2:
            bst.printAllCourses();
            break;
        case 3: {
            string courseNumber;
            cout << "Enter the course number: ";
            getline(cin, courseNumber);
            bst.printCourseInfo(courseNumber);
            break;
        }
        case 9:
            cout << "See ya later." << endl;
            return 0;
        default:
            cout << "Option no good. Please try again." << endl;
        }
    }
}

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "raylib.h"

using json = nlohmann::json;

int main() {
    // Open and parse the JSON file
    std::ifstream input_file("resources/data.json");
    
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    json j;
    input_file >> j;

    // Extract the data from JSON
    std::string name = j["name"];
    int age = j["age"];
    bool is_student = j["is_student"];
    std::string student_status = is_student ? "Yes" : "No";
    
    // Explicit conversion to int
    int math_grade_value = j["grades"]["math"].get<int>();
    int science_grade_value = j["grades"]["science"].get<int>();
    std::string math_grade = std::to_string(math_grade_value);
    std::string science_grade = std::to_string(science_grade_value);

    std::string languages;
    for (const auto& lang : j["languages"]) {
        languages += lang.get<std::string>() + " ";
    }

    std::string street = j["address"]["street"];
    std::string city = j["address"]["city"];
    std::string zip_code = j["address"]["zip_code"];
    
    // Initialize raylib
    InitWindow(800, 600, "JSON Data Display with raylib");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display data using raylib's drawing functions
        DrawText(("Name: " + name).c_str(), 50, 50, 20, BLACK);
        DrawText(("Age: " + std::to_string(age)).c_str(), 50, 80, 20, BLACK);
        DrawText(("Is Student: " + student_status).c_str(), 50, 110, 20, BLACK);
        DrawText(("Math Grade: " + math_grade).c_str(), 50, 140, 20, BLACK);
        DrawText(("Science Grade: " + science_grade).c_str(), 50, 170, 20, BLACK);
        DrawText(("Languages: " + languages).c_str(), 50, 200, 20, BLACK);
        DrawText(("Address: " + street + ", " + city + ", " + zip_code).c_str(), 50, 230, 20, BLACK);

        EndDrawing();
    }

    // Close the window and clean up resources
    CloseWindow();

    return 0;
}

#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <stdexcept>
#include "Date.h"
#include "WorkoutLog.h"
#include "MealLog.h"

class Profile {
public:
    Profile(const int age, const double height, const double weight, 
            const std::string& firstName, const std::string& lastName,
            const std::string& username, const std::string& password)
        : age(age), height(height), weight(weight), 
          firstName(firstName), lastName(lastName),
          username(username), password(password),
          dateAccountCreated(Date()) {  // Initialize with current date
        validateProfileData();
    }

    Profile(const int age, const double height, const double weight,
            const std::string& firstName, const std::string& lastName,
            const Date& dateAccountCreated, const std::string& username,
            const std::string& password, const WorkoutLog& workoutLog,
            const MealLog& mealLog)
        : age(age), height(height), weight(weight),
          firstName(firstName), lastName(lastName),
          dateAccountCreated(dateAccountCreated), username(username),
          password(password), workoutLog(workoutLog), mealLog(mealLog) {
        validateProfileData();
    }

    // Const getters for read-only access
    int getAge() const { return age; }
    double getHeight() const { return height; }
    double getWeight() const { return weight; }
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return password; }
    const Date& getDateAccountCreated() const { return dateAccountCreated; }
    
    // Non-const getters for modifiable access to logs
    WorkoutLog& getWorkoutLog() { return workoutLog; }
    MealLog& getMealLog() { return mealLog; }
    
    // Const versions of log getters for read-only access
    const WorkoutLog& getWorkoutLog() const { return workoutLog; }
    const MealLog& getMealLog() const { return mealLog; }

private:
    void validateProfileData() {
        if (age <= 0)
            throw std::invalid_argument("Age must be positive");
        if (height <= 0)
            throw std::invalid_argument("Height must be positive");
        if (weight <= 0)
            throw std::invalid_argument("Weight must be positive");
        if (firstName.empty())
            throw std::invalid_argument("First name cannot be empty");
        if (lastName.empty())
            throw std::invalid_argument("Last name cannot be empty");
        if (username.empty())
            throw std::invalid_argument("Username cannot be empty");
        if (password.empty())
            throw std::invalid_argument("Password cannot be empty");
    }

    int age;
    double height;   // in inches
    double weight;   // in lbs
    std::string firstName;
    std::string lastName;
    Date dateAccountCreated;
    std::string username;
    std::string password;
    WorkoutLog workoutLog;
    MealLog mealLog;
};

#endif
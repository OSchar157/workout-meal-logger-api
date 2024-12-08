#ifndef MEAL_LOG_H
#define MEAL_LOG_H

#include <stdexcept>
#include "Meal.h"

class MealLog {
public:
    MealLog() = default;
    
    MealLog(std::vector<Meal> meals, double totalCalories, double totalProtein, 
            double totalCarbs, double totalFat, int mealCount) 
            : meals(meals), 
              totalCalories(totalCalories),
              totalProtein(totalProtein),
              totalCarbs(totalCarbs),
              totalFat(totalFat),
              mealCount(mealCount) {
        validateNutritionalValues();
        if (meals.size() != mealCount) {
            throw std::invalid_argument("Meal count does not match size of meals vector");
        }
    }

    double getTotalCalories() const { return totalCalories; }
    double getTotalProtein() const { return totalProtein; }
    double getTotalCarbs() const { return totalCarbs; }
    double getTotalFat() const { return totalFat; }
    int getMealCount() const { return mealCount; }
    const std::vector<Meal>& getMeals() const { return meals; }

    void addMeal(const Meal& meal) {
        // Validate meal's nutritional values are non-negative
        if (meal.getCalories() < 0 || meal.getProtein() < 0 || 
            meal.getCarbs() < 0 || meal.getFat() < 0) {
            throw std::invalid_argument("Meal nutritional values cannot be negative");
        }

        meals.push_back(meal);
        totalCalories += meal.getCalories();
        totalProtein += meal.getProtein();
        totalCarbs += meal.getCarbs();
        totalFat += meal.getFat();
        mealCount += 1;
    }

private:
    void validateNutritionalValues() {
        if (totalCalories < 0)
            throw std::invalid_argument("Total calories cannot be negative");
        if (totalProtein < 0)
            throw std::invalid_argument("Total protein cannot be negative");
        if (totalCarbs < 0)
            throw std::invalid_argument("Total carbs cannot be negative");
        if (totalFat < 0)
            throw std::invalid_argument("Total fat cannot be negative");
        if (mealCount < 0)
            throw std::invalid_argument("Meal count cannot be negative");
    }

    double totalCalories = 0;
    double totalProtein = 0;
    double totalCarbs = 0;
    double totalFat = 0;
    int mealCount = 0;
    std::vector<Meal> meals;
};

#endif
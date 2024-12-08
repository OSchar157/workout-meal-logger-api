#ifndef WORKOUT_LOG_H
#define WORKOUT_LOG_H

#include <vector>
#include <stdexcept>
#include "Workout.h"

class WorkoutLog {
public:
    WorkoutLog() = default;
    
    WorkoutLog(const std::vector<Workout>& workouts, int workoutCount, double totalWorkoutTime)
        : workouts(workouts),
          workoutCount(workoutCount),
          totalWorkoutTime(totalWorkoutTime) {
        validateWorkoutLogData();
        if (workouts.size() != workoutCount) {
            throw std::invalid_argument("Workout count does not match size of workouts vector");
        }
    }

    double getTotalWorkoutTime() const { return totalWorkoutTime; }
    int getWorkoutCount() const { return workoutCount; }
    const std::vector<Workout>& getWorkouts() const { return workouts; }
    
    void addWorkout(const Workout& workout) {
        if (workout.getLengthOfWorkout() <= 0) {
            throw std::invalid_argument("Workout length must be positive");
        }
        workouts.push_back(workout);
        totalWorkoutTime += workout.getLengthOfWorkout();
        workoutCount += 1;
    }

private:
    void validateWorkoutLogData() {
        if (workoutCount < 0) {
            throw std::invalid_argument("Workout count cannot be negative");
        }
        if (totalWorkoutTime < 0) {
            throw std::invalid_argument("Total workout time cannot be negative");
        }
    }

    std::vector<Workout> workouts;
    int workoutCount = 0;
    double totalWorkoutTime = 0.0;
};

#endif
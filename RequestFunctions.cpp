#include "RequestFunctions.h"
#include <iostream>

using namespace crow;
using namespace std;

extern vector<Profile*> allProfiles;

int findProfileIndex(const vector<Profile*>& allProfiles, const string& profRequested) {
    for(size_t i = 0; i < allProfiles.size(); i++) {
        if(allProfiles[i]->getUsername() == profRequested) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Profile* authenticateUser(const request& req) {
    if (!(req.url_params.get("username") && req.url_params.get("password"))) {
        throw std::invalid_argument("Missing required parameters. Please include: username, password");
    }

    string username = req.url_params.get("username");
    string password = req.url_params.get("password");
    cout << "Auth attempt - Username: " << username << endl;

    int idxInAllProfiles = findProfileIndex(allProfiles, username);
    if(idxInAllProfiles == -1) {
        cout << "User not found" << endl;
        throw std::invalid_argument("Incorrect username or password. Ensure you entered the correct information, or create a profile");
    }

    Profile* usersProfile = allProfiles[idxInAllProfiles];
    cout << "Stored password: " << usersProfile->getPassword() << ", Provided password: " << password << endl;
    
    if (usersProfile->getPassword() != password) {
        cout << "Password mismatch" << endl;
        throw std::invalid_argument("Incorrect username or password. Ensure you entered the correct information, or create a profile");
    }

    return usersProfile;
}

response viewProfile(const request& req) {
    try {
        if (!req.url_params.get("limited-view")) {
            return response(400, "Missing required parameter: limited-view\n");
        }

        string viewParameter = req.url_params.get("limited-view");
        if (viewParameter != "true" && viewParameter != "false") {
            return response(400, "Invalid view selection value\n");
        }

        Profile* usersProfile = authenticateUser(req);
        bool isLimitedView = viewParameter == "true";

        return response(200, 
            (isLimitedView ? 
                convertProfileToJsonLimited(*usersProfile) : 
                convertProfileToJsonFull(*usersProfile)
            ).dump() + "\n"
        );
    } catch (const std::invalid_argument& e) {
        return response(400, std::string(e.what()) + "\n");
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}

response createProfile(const request& req) {
    try {
        json::rvalue readValueJson = json::load(req.body);
        if (!readValueJson) {
            return response(400, "Invalid JSON format\n");
        }

        // Validate required fields
        const vector<string> requiredFields = {
            "age", "height", "weight", "firstName", 
            "lastName", "username", "password"
        };

        for (const auto& field : requiredFields) {
            if (!readValueJson.has(field)) {
                return response(400, "Missing required field: " + field + "\n");
            }
        }
        
        // Check for existing username
        for(const Profile* profile : allProfiles) {
            if (readValueJson["username"].s() == profile->getUsername()) {
                return response(400, "This username is already taken\n");
            }
        }

        try {
            Profile* newProfile = new Profile(
                static_cast<int>(readValueJson["age"].i()),
                readValueJson["height"].d(),
                readValueJson["weight"].d(),
                readValueJson["firstName"].s(),
                readValueJson["lastName"].s(),
                readValueJson["username"].s(),
                readValueJson["password"].s()
            );

            allProfiles.push_back(newProfile);
            return response(201, "Profile:\n" + convertProfileToJsonFull(*newProfile).dump() + "\n");
        } catch (const std::invalid_argument& e) {
            return response(400, std::string("Invalid profile data: ") + e.what() + "\n");
        }
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}

response viewWorkoutLog(const request& req) {
    try {
        Profile* usersProfile = authenticateUser(req);
        return response(200, convertWorkoutLogToJson(usersProfile->getWorkoutLog()).dump() + "\n");
    } catch (const std::invalid_argument& e) {
        return response(400, std::string(e.what()) + "\n");
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}

response viewMealLog(const request& req) {
    try {
        Profile* usersProfile = authenticateUser(req);
        return response(200, convertMealLogToJson(usersProfile->getMealLog()).dump() + "\n");
    } catch (const std::invalid_argument& e) {
        return response(400, std::string(e.what()) + "\n");
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}

response addToMealLog(const request& req) {
    try {
        Profile* usersProfile = authenticateUser(req);

        json::rvalue readValueJson = json::load(req.body);
        if (!readValueJson) {
            return response(400, "Invalid JSON format\n");
        }

        Meal newMeal = convertJsonToMeal(readValueJson);
        usersProfile->getMealLog().addMeal(newMeal);

        return response(200, convertMealLogToJson(usersProfile->getMealLog()).dump() + "\n");
    } catch (const std::invalid_argument& e) {
        return response(400, std::string(e.what()) + "\n");
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}

response addToWorkoutLog(const request& req) {
    try {
        Profile* usersProfile = authenticateUser(req);

        json::rvalue readValueJson = json::load(req.body);
        if (!readValueJson) {
            return response(400, "Invalid JSON format\n");
        }

        Workout newWorkout = convertJsonToWorkout(readValueJson);
        usersProfile->getWorkoutLog().addWorkout(newWorkout);

        return response(200, convertWorkoutLogToJson(usersProfile->getWorkoutLog()).dump() + "\n");
    } catch (const std::invalid_argument& e) {
        return response(400, std::string(e.what()) + "\n");
    } catch (const std::exception& e) {
        return response(500, "Internal server error\n");
    }
}
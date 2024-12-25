#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define PI 3.141592653589793
#define EARTH_RADIUS 6371.0 // Earth's radius in km.

// Structure to store location data
typedef struct {
    char name[50];
    double latitude;
    double longitude;
} Location;

// Converts degrees to radians
double toRadian(double x) {
    return (PI / 180) * x;
}

// Calculate distance using the Haversine formula
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = toRadian(lat1);
    lon1 = toRadian(lon1);
    lat2 = toRadian(lat2);
    lon2 = toRadian(lon2);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = pow(sin(dLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

// Convert kilometers to miles
double toMiles(double distance) {
    return distance / 1.61;
}

// Save distance information to a file
void saveToFile(Location *loc, double distance) {
    FILE *file = fopen("distances.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "Distance to %s: %.2lf km (%.2lf miles)\n", loc->name, distance, toMiles(distance));
    fclose(file);
}

// Recursive function to get location data from the user
void getLocationData(Location *loc) {
    printf("Enter location name: ");
    scanf(" %[^\n]", loc->name);
    printf("Enter its latitude: ");
    if (scanf("%lf", &loc->latitude) != 1) {
        printf("Invalid input for latitude. Try again.\n");
        getLocationData(loc); 
        return;
    }
    printf("Enter its longitude: ");
    if (scanf("%lf", &loc->longitude) != 1) {
        printf("Invalid input for longitude. Try again.\n");
        getLocationData(loc); 
    }
}

int main() {
    Location NIST = {"NIST University", 19.198703, 84.745957};
    int numberOfLocations;
    printf("How many locations do you want to compare with NIST University? ");
    scanf("%d", &numberOfLocations);

    // Dynamic memory allocation for an array of locations
    Location *locations = (Location *)malloc(numberOfLocations * sizeof(Location));
    if (locations == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < numberOfLocations; i++) {
        printf("\nEnter details for location %d:\n", i + 1);
        getLocationData(&locations[i]);
    }

    for (int i = 0; i < numberOfLocations; i++) {
        double distance = calculateDistance(NIST.latitude, NIST.longitude, locations[i].latitude, locations[i].longitude);
        printf("\nThe distance between NIST University and %s is %.2lf km or %.2lf miles.\n",
               locations[i].name, distance, toMiles(distance));
        saveToFile(&locations[i], distance); // Save data to a file
    }

    // Free allocated memory
    free(locations);

    printf("\nDistances saved to 'distances.txt'.\n");
    return 0;
}

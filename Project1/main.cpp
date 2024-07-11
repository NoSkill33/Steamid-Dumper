﻿#define CURL_STATICLIB
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "curl.h"

#include "termcolor.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool isProfileAvailable(const std::string& profileId) {
    std::string url = "https://steamcommunity.com/id/" + profileId;

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            if (response.find("The specified profile could not be found.") != std::string::npos) {
                return true;
            }
        }
    }

    return false;
}

int lenght;
std::string name;
std::string profileId;

std::string generateRandomProfileId() {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    std::string profileId = "";

    for (int i = 0; i < lenght; ++i) {
        profileId += alphabet[rand() % (sizeof(alphabet) - 1)];
    }

    return profileId;
}

int main() {
    srand(time(nullptr));
    std::string dynamicString = "error";
    int przeskanowane = 0;
    int dostepne = 0;
    std::ofstream uncheckedFile("unchecked.txt");
    if (!uncheckedFile.is_open()) {
        std::cerr << "cannot open unchecked.txt" << std::endl;
        return 1;
    }
    SetConsoleTitle("Steam url scanner (idle)");

    //@note: checks internet connection
    if (system("ping -n 1 google.com > nul") == 0) {
    }
    else {
        std::cout << "Internet connection is not available" << std::endl;
        Sleep(5000);
        return 0;
    }
    std::cout << termcolor::white << "[" << termcolor::yellow << "?" << termcolor::white << "] Target id to dump (use !ig to ignore): ";
    std::cin >> name;
    std::cout << termcolor::white << "[" << termcolor::yellow << "?" << termcolor::white << "] Additional lenght of target id: ";
    std::cin >> lenght;

    if (name != "!ig")
        uncheckedFile << "Mathemacy steamid dumper, ©NoSKill 2023\nTarget id: " << name << " + " << lenght << "\n\n[Valid / Scanned] ID" << std::endl;
    else
        uncheckedFile << "Mathemacy steamid dumper, ©NoSKill 2023\nTarget id: " << lenght << "\n\n[Valid / Scanned] ID" << std::endl;
    
    while (true) {

        if (name != "!ig") {
            profileId = name + generateRandomProfileId();
        }
        else {
            profileId = generateRandomProfileId();
        }
        //std::string profileId = name + generateRandomProfileId();
        przeskanowane++;
        uncheckedFile.flush();
        if (isProfileAvailable(profileId)) {
            dostepne++;
            std::cout << '\a';
            //std::cout << profileId << std::endl;
            uncheckedFile << "[" << dostepne << "/" << przeskanowane << "] " << profileId << std::endl;
        }
        else {
            //std::cout << profileId << " isn't available" << std::endl;
        }
        system("cls");
        std::cout << termcolor::white << "[" << termcolor::green << ">" << termcolor::white << "] " << dostepne << " of " << przeskanowane << " was valid" << std::endl;
        std::string titl = "Steam url scanner (busy)";
        SetConsoleTitle(titl.c_str());
    }

    uncheckedFile << "The scan has finished, and " << dostepne << " out of " << przeskanowane << " steam url's were found." << std::endl;
    uncheckedFile.close();
    return 0;
}

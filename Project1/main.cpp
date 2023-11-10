#define CURL_STATICLIB
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "termcolor.h"
#include "curl/curl.h"

size_t WriteCallback(void* contents, const size_t size, const size_t nmemb, void* userp)
{
    static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

bool isProfileAvailable(const std::string& profileId)
{
    const std::string url = "https://steamcommunity.com/id/" + profileId;

    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        const CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (res == CURLE_OK) if (response.find("The specified profile could not be found.") != std::string::npos) return true;
    }

    return false;
}

int length;
std::string name;
std::string profileId;

std::string generateRandomProfileId()
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    std::string profileId; // diagnostic-shadowed-variable

    for (int i = 0; i < length; ++i) profileId += alphabet[rand() % (sizeof(alphabet) - 1)]; // concurrency-mt-unsafe

    return profileId;
}

int main()
{
    srand(time(nullptr)); // cert-msc51-cpp, diagnostic-shorten-64-to-32
    std::string dynamicString = "error";
    int przeskanowane = 0;
    int dostepne = 0;
    std::ofstream uncheckedFile("unchecked.txt");
    if (!uncheckedFile.is_open())
    {
        std::cerr << "cannot open unchecked.txt" << std::endl;
        return 1;
    }
    SetConsoleTitle(reinterpret_cast<LPCWSTR>("Steam url scanner (idle)"));

    //@note: checks internet connection
    if (system("ping -n 1 google.com > nul") == 0)
    {
        // concurrency-mt-unsafe
    }
    else
    {
        std::cout << "Internet connection is not available" << std::endl;
        Sleep(5000);
        return 0;
    }
    std::cout << termcolor::white << "[" << termcolor::yellow << "?" << termcolor::white << "] Target id to dump (use !ig to ignore): ";
    std::cin >> name;
    std::cout << termcolor::white << "[" << termcolor::yellow << "?" << termcolor::white << "] Additional length of target id: ";
    std::cin >> length;

    if (name != "!ig") uncheckedFile << "Mathemacy steamid dumper, ©NoSKill 2023\nTarget id: " << name << " + " << length << "\n\n[Valid / Scanned] ID" << std::endl;
    else uncheckedFile << "Mathemacy steamid dumper, ©NoSKill 2023\nTarget id: " << length << "\n\n[Valid / Scanned] ID" << std::endl;

    while (true)
    {
        if (name != "!ig") profileId = name + generateRandomProfileId();
        else profileId = generateRandomProfileId();
        // std::string profileId = name + generateRandomProfileId();
        przeskanowane++;
        uncheckedFile.flush();
        if (isProfileAvailable(profileId))
        {
            dostepne++;
            std::cout << '\a';
            // std::cout << profileId << std::endl;
            uncheckedFile << "[" << dostepne << "/" << przeskanowane << "] " << profileId << std::endl;
        }
        else
        {
            // std::cout << profileId << " isn't available" << std::endl;
        }
        system("cls"); // concurrency-mt-unsafe
        std::cout << termcolor::white << "[" << termcolor::green << ">" << termcolor::white << "] " << dostepne << " of " << przeskanowane << " was valid" << std::endl;
        std::string title = "Steam url scanner (busy)";
        SetConsoleTitle(reinterpret_cast<LPCWSTR>(title.c_str()));
    }

    // unreachable code?
    uncheckedFile << "The scan has finished, and " << dostepne << " out of " << przeskanowane << " steam url's were found." << std::endl;
    uncheckedFile.close();
    return 0;
}

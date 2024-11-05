#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <curl/curl.h>

// Функция для записи данных в файл
void writeToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data;
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

// Callback функция для curl, сохраняет данные в строку
size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

// Функция для парсинга страницы
std::string parsePage(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
        curl_easy_cleanup(curl);
    }

    return response;
}

// Функция для извлечения ключа из HTML
std::string extractKey(const std::string& html) {
    size_t start = html.find("<textarea class=\"form-control shadow-none\" id=\"accessKey\"");
    if (start == std::string::npos) return "";

    start = html.find(">", start) + 1;
    size_t end = html.find("</textarea>", start);
    if (end == std::string::npos) return "";

    return html.substr(start, end - start);
}



int main() {
    int start_id = 5217; // Начальный ID
    int end_id = 6000;   // Конечный ID (для примера)


    for (int id = start_id; id <= end_id; ++id) {
        std::stringstream ss;
        ss << "https://outlinekeys.com/key/" << id;
        std::string url = ss.str();

        std::cout << "Visiting: " << url << std::endl;

        std::string html = parsePage(url);

        if (html.empty()) {
            continue; // Пропустить, если произошла ошибка при получении страницы
        }

        std::string key = extractKey(html);

        if (key.empty()) {
             std::cout << "Status: 404 (Not Found)" << std::endl;
        } else {
            std::stringstream filename;
            filename << "key_" << id << ".txt";
            writeToFile(filename.str(), key);
            std::cout << "Status: OK (Key saved to " << filename.str() << ")" << std::endl;
        }
    }

    return 0;
}

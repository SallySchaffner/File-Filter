#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

// Utility to parse a CSV line respecting quoted fields with commas
vector<string> parseCSVRow(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string field;
    bool inQuotes = false;
    char c;
    string temp;

    for (size_t i = 0; i < line.length(); ++i) {
        c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            result.push_back(temp);
            temp.clear();
        }
        else { 
        }
    }
    result.push_back(temp); // last field
    return result;
            temp += c;
}

int main() {
    ifstream inputFile("Summer2022.csv");
    ofstream outputFile("Filtered_Summer2022.csv");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string headerLine;
    getline(inputFile, headerLine);
    vector<string> headers = parseCSVRow(headerLine);

    unordered_map<string, int> columnIndex;
    for (int i = 0; i < headers.size(); ++i) {
        columnIndex[headers[i]] = i;
    }

    // Columns to extract
    vector<string> columnsToExtract = {
        "Subject", "Catalog", "Section", "Component", "Session", "Units", "TotEnrl", "CapEnrl", "Instructor"
    };

    // Validate and collect indices
    vector<int> indices;
    for (const auto& col : columnsToExtract) {
        if (columnIndex.find(col) != columnIndex.end()) {
            indices.push_back(columnIndex[col]);
        }
        else {
            cerr << "Column not found: " << col << endl;
            return 2;
        }
    }

    // Write new header
    for (size_t i = 0; i < indices.size(); ++i) {
        outputFile << headers[indices[i]];
        if (i != indices.size() - 1)
            outputFile << ",";
    }
    outputFile << "\n";

    // Read and write filtered data
    string line;
    while (getline(inputFile, line)) {
        vector<string> row = parseCSVRow(line);
        for (size_t i = 0; i < indices.size(); ++i) {
            int idx = indices[i];
            if (idx < row.size()) {
                // Wrap field in quotes if it contains commas or spaces
                string field = row[idx];
                bool hasCommaOrSpace = field.find(',') != string::npos || field.find(' ') != string::npos;
                if (hasCommaOrSpace && field.front() != '"') {
                    outputFile << "\"" << field << "\"";
                }
                else {
                    outputFile << field;
                }
            }
            if (i != indices.size() - 1)
                outputFile << ",";
        }
        outputFile << "\n";
    }

    inputFile.close();
    outputFile.close();

    cout << "Filtered file created: Filtered_Summer2022.csv" << endl;
    return 0;
}

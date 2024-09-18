#include <iostream>
#include <fstream>

// Design Purposes Only
#include <windows.h>
#include <thread>

using namespace std;

typedef struct CODE {
    char character;
    string code;
} Code;

class DECOMPRESSOR {
private:
    string inputFileName;
    string outputFileName;
    Code CODES[256];

public:
    void readDatabaseFile() {
        ifstream databaseFile("Compressed.txt");

        if (!databaseFile.is_open()) {
            SetColorGreen();
            LoadingAni();
            DisplayMessage("Error Opening Database File");
            return;
        }
        int numCodes;
        databaseFile >> numCodes;

        for (int i = 0; i < numCodes; ++i) {
            char character;
            string code;
            databaseFile >> character >> code;
            CODES[i] = {character, code};
        }
        databaseFile.close();
    }


        void beep() {
            Beep(440, 100);
        }

        void SetColorGreen() {
        system("color 02");
        }

        void LoadingAni() {
        SetColorGreen();
        int delay = 20;
        int iterations = 10;

        cout << "\n\n\n\t\t\t\t\tProcessing: [";
        for (int i = 0; i < iterations; i++) {
            cout << "=";
            beep();
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
        cout << "]" << endl;
    }

       void DisplayMessage(const string& message, int delay = 1000) {
        system("cls");
        int totalWidth = message.length() + 4;
        string border(totalWidth, '=');
        system("cls");
        cout << "\n\n\n" << endl;
        cout << "\t\t\t\t\t" << border << endl;
        cout << "\t\t\t\t\t" <<"= " << message << " =" << endl;
        cout << "\t\t\t\t\t" <<border << endl;


        if (delay > 0) {
            Sleep(delay);
        }
    }

        void MessageSpace(){
        cout << "\n" << endl;
        cout << "\t\t\t     ";
        }

    void ExpandCompress() {
        ifstream inFile("CompressedData.txt");
        ofstream outFile("temp.txt");

        if (!inFile.is_open() || !outFile.is_open()) {
            LoadingAni();
            DisplayMessage("Error Opening The File");
            return;
        }
        string line;
        while (getline(inFile, line)) {
            string expandedLine = "";
            for (size_t i = 0; i < line.length(); i++) {
                if (line[i] >= '3' && line[i] <= '9') {
                    int repeatCount = line[i] - '0';
                    char nextChar = line[i + 1];
                    for (int j = 0; j < repeatCount; j++) {
                        expandedLine += nextChar;
                    }
                    i++;
                } else {
                    expandedLine += line[i];
                }
            }
            outFile << expandedLine << endl;
        }
        inFile.close();
        outFile.close();
    }

    void Decode() {
        ExpandCompress();
        readDatabaseFile();

        ifstream inFile("temp.txt");
        ofstream outFile("decompressed.txt");

        if (!inFile.is_open() || !outFile.is_open()) {
            LoadingAni();
            DisplayMessage("Error Opening Input or Output File");
            return;
        }
        string line;
        while (getline(inFile, line)) {
            string decodedLine = "";
            string word = "";

            for (size_t i = 0; i < line.length(); ++i) {
                if (line[i] == ' ') {
                    string decodedWord = "";
                    string code = "";

                    for (size_t j = 0; j < word.length(); j++) {
                        code += word[j];
                        for (int k = 0; k < 256; ++k) {
                            if (code == CODES[k].code) {
                                decodedWord += CODES[k].character;
                                code = "";
                                break;
                            }
                        }
                    }
                    decodedLine += decodedWord + " ";
                    word = "";
                } else {
                    word += line[i];
                }
            }
            if (!word.empty()) {
                string decodedWord = "";
                string code = "";

                for (size_t j = 0; j < word.length(); j++) {
                    code += word[j];
                    for (int k = 0; k < 256; ++k) {
                        if (code == CODES[k].code) {
                            decodedWord += CODES[k].character;
                            code = "";
                            break;
                        }
                    }
                }
                decodedLine += decodedWord;
            }
            outFile << decodedLine << endl;
        }
        inFile.close();
        outFile.close();
        if (remove("temp.txt") != 0) {
            LoadingAni();
            DisplayMessage("Error Deleting Temporary File");
        }
    }
};

int main() {
    DECOMPRESSOR decompressor;
    string inputFileName = "CompressedData";
    string outputFileName = "Decompressed.txt";
    string userResponse;

    ifstream checkFile(inputFileName + ".txt");
    if (!checkFile.good()) {
        system("cls");
        decompressor.LoadingAni();
        decompressor.DisplayMessage("Compessed File Does Not Exist");
        system("pause");
        return 1;
    }
    checkFile.close();

    system("cls");
    decompressor.LoadingAni();
    decompressor.DisplayMessage("Renaming");
    decompressor.MessageSpace();
    cout << "Do you want to rename the decompressed file? (Y/N): ";
    cin >> userResponse;
    system("cls");
    decompressor.LoadingAni();
    if (userResponse == "Y" || userResponse == "y") {
        decompressor.DisplayMessage("Renaming");
        decompressor.MessageSpace();
        cout << "Enter the new file name: ";
        cin >> outputFileName;
        system("cls");
        if (outputFileName.size() >= 4 && outputFileName.substr(outputFileName.size() - 4) != ".txt") {
            outputFileName += ".txt";
        }
    }
    decompressor.Decode();
    decompressor.LoadingAni();
    system("cls");
    decompressor.DisplayMessage("Deletion");
    decompressor.MessageSpace();
    cout << "Do you want to delete the compressed file? (Y/N): ";
    cin >> userResponse;
    if (userResponse == "Y" || userResponse == "y") {
        ifstream checkCompressedFile(inputFileName + ".txt");
        if (checkCompressedFile.good()) {
            checkCompressedFile.close();
            if (remove((inputFileName + ".txt").c_str()) != 0) {
                system("cls");
                decompressor.LoadingAni();
                decompressor.DisplayMessage("Error Deleting File");
                system("pause");
                return 1;
            }
            system("cls");
            decompressor.LoadingAni();
            decompressor.DisplayMessage("Compressed File Deleted Successfully.");
            system("pause");
        } else {
            system("cls");
            decompressor.LoadingAni();
            decompressor.DisplayMessage("Compressed File Does Not Exist or Has Already Been Deleted.");
            system("pause");
        }
    }
    if (outputFileName != "decompressed.txt") {
        if (rename("decompressed.txt", outputFileName.c_str()) != 0) {
            system("cls");
            decompressor.LoadingAni();
            decompressor.DisplayMessage("Error Renaming The File");
            system("pause");
            return 1;
        }
        system("cls");
        decompressor.LoadingAni();
        decompressor.DisplayMessage("File Decompressed Successfully");
    }
    return 0;
}

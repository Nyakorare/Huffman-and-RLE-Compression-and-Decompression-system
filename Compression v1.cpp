#include <iostream>
#include <fstream>

// Design Purposes Only
#include <windows.h>
#include <thread>

using namespace std;

typedef struct Node {
    char data;
    int freq;
    Node* left;
    Node* right;
    Node(char data, int freq) : data(data), freq(freq), left(NULL), right(NULL) {}
} NODE;

typedef struct QUEUE {
    NODE* node;
    QUEUE* next;
    QUEUE(NODE* n) : node(n), next(NULL) {}
} Queue;

class COMPRESSOR {
private:
    Queue* head;
    string inputFileName;
    string outputFileName;
    string huffmanCodes[256];

    void enQueueSorted(NODE* node) {
        Queue* newNode = new Queue(node);
        if (!head || head->node->freq >= newNode->node->freq) {
            newNode->next = head;
            head = newNode;
        } else {
            Queue* current = head;
            while (current->next && current->next->node->freq < newNode->node->freq) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    NODE* deQueue() {
        if (!head) {
            return NULL;
        }
        Queue* temp = head;
        head = head->next;
        NODE* node = temp->node;
        delete temp;
        return node;
    }

    void GenerateTree(NODE* root, string code) {
        if (!root) {
            return;
        }
        if (root->data != '$') {
            huffmanCodes[root->data] = code;
        }
        GenerateTree(root->left, code + "0");
        GenerateTree(root->right, code + "1");
    }

        void beep() {
            Beep(440, 100);
        }

        void SetColorGreen() {
        system("color 02");
        }

        void LoadingAni() {
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
        cout << "\t\t\t\t     ";
        }

    void BuildTree(int freq[]) {
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                enQueueSorted(new NODE(char(i), freq[i]));
            }
        }
        while (head->next) {
            NODE* left = deQueue();
            NODE* right = deQueue();
            NODE* newNode = new NODE('$', left->freq + right->freq);
            newNode->left = left;
            newNode->right = right;
            enQueueSorted(newNode);
        }
        GenerateTree(head->node, "");
    }

    void Encode() {
        SetColorGreen();
        ifstream inputFile(inputFileName);
        ofstream outputFile(outputFileName + ".txt");
        ofstream asciiFile("Compressed.txt");

        if (!inputFile.is_open() || !outputFile.is_open() || !asciiFile.is_open()) {
            LoadingAni();
            DisplayMessage("Error opening files");
            return;
        }
        int freq[256] = {0};
        char currentChar;

        while (inputFile.get(currentChar)) {
            if (currentChar != ' ' && currentChar != '\n') {
                freq[int(currentChar)]++;
            }
        }
        freq[int(' ')] = 1;
        BuildTree(freq);
        int numCodes = 0;

        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                numCodes++;
            }
        }
        asciiFile << numCodes - 1 << '\n';
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0 && char(i) != ' ') {
                asciiFile << char(i) << ' ' << huffmanCodes[i] << '\n';
            }
        }
        inputFile.clear();
        inputFile.seekg(0);
        string line;
        while (getline(inputFile, line)) {
            string EncodedText = "";
            for (char ch : line) {
                if (ch == ' ') {
                    EncodedText += ' ';
                } else {
                    EncodedText += huffmanCodes[int(ch)];
                }
            }
            outputFile << EncodedText << '\n';
        }
        inputFile.close();
        outputFile.close();
        asciiFile.close();
    }

    void MoreCompress() {
        ifstream inputFile(outputFileName + ".txt");
        ofstream tempFile("temp.txt");

        if (!inputFile.is_open() || !tempFile.is_open()) {
            system("cls");
            LoadingAni();
            DisplayMessage("Error opening files");
            system("pause");
            return;
        }
        string line;
        while (getline(inputFile, line)) {
            string updatedLine = "";
            int count = 1;
            char prevChar = line[0];

            for (int i = 1; i < line.length(); ++i) {
                if (line[i] == prevChar) {
                    count++;
                } else {
                    if (count >= 3) {
                        while (count > 9) {
                            updatedLine += "9" + string(1, prevChar);
                            count -= 9;
                        }
                        updatedLine += to_string(count) + prevChar;
                    } else {
                        while (count--) {
                            updatedLine += prevChar;
                        }
                    }
                    prevChar = line[i];
                    count = 1;
                }
            }
            if (count >= 3) {
                while (count > 9) {
                    updatedLine += "9" + string(1, prevChar);
                    count -= 9;
                }
                updatedLine += to_string(count) + prevChar;
            } else {
                while (count--) {
                    updatedLine += prevChar;
                }
            }
            tempFile << updatedLine << '\n';
        }
        inputFile.close();
        tempFile.close();
        if (remove((outputFileName + ".txt").c_str()) != 0) {
            system("cls");
            LoadingAni();
            DisplayMessage("Error deleting original compressed file");
            system("pause");
            return;
        }
        if (rename("temp.txt", (outputFileName + ".txt").c_str()) != 0) {
            system("cls");
            LoadingAni();
            DisplayMessage("Error renaming temp file");
            system("pause");
            return;
        }
    }

public:
    COMPRESSOR(string& inputFile, string& outputFile) : inputFileName(inputFile), outputFileName(outputFile), head(NULL) {}

    int countLines() {
        int count = 0;
        string line;
        ifstream inputFile(inputFileName);

        if (!inputFile.is_open()) {
            system("cls");
            LoadingAni();
            DisplayMessage("Error opening file to count lines");
            system("pause");
            return -1;
        }
        while (getline(inputFile, line)) {
            count++;
        }
        inputFile.close();
        return count;
    }

    void compressFile() {
        int totalLines = countLines();
        if (totalLines <= 0) {
            system("cls");
            LoadingAni();
            DisplayMessage("Error counting lines in file");
            system("pause");
            return;
        }
        Encode();
        MoreCompress();
        system("cls");
        LoadingAni();
        DisplayMessage("File Compressed Successfully");
        MessageSpace();
        cout << "Total lines compressed from the file: " << totalLines << endl;
        system("pause");
    }

    void handleFiles() {
        ifstream inputFile(inputFileName);
        if (!inputFile.good()) {
            system("cls");
            SetColorGreen();
            LoadingAni();
            DisplayMessage("No Text File Found");
            DisplayMessage("Creating Default File");
            system("cls");
            LoadingAni();
            system("cls");
            DisplayMessage("Default File");
            MessageSpace();
            cout << "Enter text(s) to compress:";
            string userInput;
            getline(cin, userInput);

            ofstream newInputFile(inputFileName);
            if (newInputFile.is_open()) {
                newInputFile << userInput;
                newInputFile.close();
            } else {
                system("cls");
                LoadingAni();
                 DisplayMessage("Error creating default file");
                return;
            }
        }
        compressFile();
        inputFile.close();

        char deleteInput;
        system("cls");
        LoadingAni();
         DisplayMessage("Deleting Default file");
        MessageSpace();
        cout << "Do you want to delete the Default file? (Y/N): ";
        cin >> deleteInput;

        if (tolower(deleteInput) == 'y') {
            if (remove(inputFileName.c_str()) != 0) {
                system("cls");
                LoadingAni();
                DisplayMessage("Error deleting input file");
                return;
            } else {
                system("cls");
                LoadingAni();
                DisplayMessage("Default File deleted successfully");
            }
        } else {
            system("cls");
            LoadingAni();
             DisplayMessage("Default File not deleted");
        }
    }
};

int main() {
    string inputFileName = "Default.txt";
    string outputFileName = "CompressedData";
    COMPRESSOR compressor(inputFileName, outputFileName);
    compressor.handleFiles();
    return 0;
}

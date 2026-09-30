#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct Pipe {
    string name;
    double length;
    int diameter;
    bool underRepair;
};

struct CS {
    string name;
    int shopsTotal;
    int shopsWorking;
    int stationClass;
};

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

int readPositiveInt(const string& prompt) {
    while (true) {
        int value = readInt(prompt);
        if (value <= 0) {
            cout << "Value must be positive.\n";
            continue;
        }
        return value;
    }
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        double value = readDouble(prompt);
        if (value <= 0) {
            cout << "Value must be positive.\n";
            continue;
        }
        return value;
    }
}

string readNonEmptyLine(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line.empty()) {
            cout << "The string cannot be empty.\n";
            continue;
        }
        return line;
    }
}

void inputPipe(Pipe& p) {
    cout << "\n PIPE DATA INPUT \n";
    p.name = readNonEmptyLine("Kilometer mark (name): ");
    p.length = readPositiveDouble("Pipe length, km: ");
    p.diameter = readPositiveInt("Pipe diameter, mm: ");
    p.underRepair = false;
    cout << "Pipe added successfully.\n";
}

void printPipe(const Pipe& p) {
    cout << "\n PIPE \n";
    cout << "Name: " << p.name << "\n";
    cout << "Length: " << p.length << " km\n";
    cout << "Diameter: " << p.diameter << " mm\n";
    cout << "Under repair: " << (p.underRepair ? "yes" : "no") << "\n";
}

void editPipeRepair(Pipe& p, bool pipeExists) {
    if (!pipeExists) {
        cout << "Pipe has not been created yet.\n";
        return;
    }
    p.underRepair = !p.underRepair;
    cout << "New pipe status: " << (p.underRepair ? "under repair" : "not under repair") << "\n";
}

void inputCS(CS& c) {
    cout << "\n COMPRESSOR STATION DATA INPUT \n";
    c.name = readNonEmptyLine("Station name: ");
    c.shopsTotal = readPositiveInt("Number of shops: ");
    while (true) {
        c.shopsWorking = readInt("Number of working shops: ");
        if (c.shopsWorking < 0 ||
            c.shopsWorking > c.shopsTotal) {
            cout << "Number of working shops must be from 0 to " << c.shopsTotal << ".\n";
            continue;
        }
        break;
    }
    c.stationClass = readPositiveInt("Station class: ");
    cout << "Compressor station added successfully.\n";
}

void printCS(const CS& c) {
    cout << "\n COMPRESSOR STATION \n";
    cout << "Name: " << c.name << "\n";
    cout << "Total shops: " << c.shopsTotal << "\n";
    cout << "Working shops: " << c.shopsWorking << "\n";
    cout << "Station class: " << c.stationClass << "\n";
}

void editCSShops(CS& c, bool csExists) {
    if (!csExists) {
        cout << "Compressor station has not been created yet.\n";
        return;
    }
    cout << "\n1 - Start a shop\n";
    cout << "2 - Stop a shop\n";
    int choice = readInt("Your choice: ");
    if (choice == 1) {
        if (c.shopsWorking < c.shopsTotal) {
            c.shopsWorking++;
            cout << "Shop started.\n";
            cout << "Working shops: " << c.shopsWorking << "\n";
        }
        else {
            cout << "All shops are already working.\n";
        }
    }
    else if (choice == 2) {
        if (c.shopsWorking > 0) {
            c.shopsWorking--;
            cout << "Shop stopped.\n";
            cout << "Working shops: " << c.shopsWorking << "\n";
        }
        else {
            cout << "There are no working shops.\n";
        }
    }
    else {
        cout << "Invalid choice.\n";
    }
}
void savePipe(ofstream& fout, const Pipe& p, bool pipeExists) {
    fout << pipeExists << "\n";
    if (pipeExists) {
        fout << p.name << "\n";
        fout << p.length << " " << p.diameter << " " << p.underRepair << " ";
    }
}
void saveCS(ofstream& fout, const CS& c, bool csExists) {
    fout << csExists << "\n";
    if (csExists) {
        fout << c.name << "\n";
        fout << c.shopsTotal << " " << c.shopsWorking << " " << c.stationClass << "\n";
    }
}
void saveData(const Pipe& p,bool pipeExists,const CS& c,bool csExists) {
    ofstream fout("data.txt");
    if (!fout) {
        cout << "Failed to open file for writing.\n";
        return;
    }
    savePipe(fout, p, pipeExists);
    saveCS(fout, c, csExists);
    fout.close();
    cout << "Data saved to data.txt.\n";
}

void loadPipe(ifstream& fin, Pipe& p, bool& pipeExists) {
    int flag;
    fin >> flag;
    fin.ignore(10000, '\n');
    pipeExists = (flag != 0);
    if (pipeExists) {
        getline(fin, p.name);
        fin >> p.length >> p.diameter >> p.underRepair;
        fin.ignore(10000, '\n');
    }
}
void loadCS(ifstream& fin, CS& c, bool& csExists) {
    int flag;
    fin >> flag;
    fin.ignore(10000, '\n');
    csExists = (flag != 0);
    if (csExists) {
        getline(fin, c.name);
        fin >> c.shopsTotal >> c.shopsWorking >> c.stationClass;
        fin.ignore(10000, '\n');
    }
}
void loadData(Pipe& p, bool& pipeExists, CS& c, bool& csExists) {
    ifstream fin("data.txt");
    if (!fin) {
        cout << "File data.txt was not found.\n";
        return;
    }
    loadPipe(fin, p, pipeExists);
    loadCS(fin, c, csExists);
    fin.close();
    cout << "Data loaded from data.txt.\n";
}
void showMenu() {
    cout << "\n";
    cout << "        PIPELINE TRANSPORT\n";
    cout << "1. Add pipe\n";
    cout << "2. Add compressor station\n";
    cout << "3. Show all objects\n";
    cout << "4. Edit pipe (repair status)\n";
    cout << "5. Edit compressor station\n";
    cout << "6. Save to file\n";
    cout << "7. Load from file\n";
    cout << "0. Exit\n";
}

int main() {
    Pipe pipe;
    CS cs;
    bool pipeExists = false;
    bool csExists = false;
    while (true) {
        showMenu();
        int choice = readInt("Choose menu item: ");
        switch (choice) {
        case 1:
            inputPipe(pipe);
            pipeExists = true;
            break;
        case 2:
            inputCS(cs);
            csExists = true;
            break;
        case 3:
            if (!pipeExists && !csExists) {
                cout << "There are no objects yet.\n";
            }
            else {
                if (pipeExists) {
                    printPipe(pipe);
                }
                if (csExists) {
                    printCS(cs);
                }
            }
            break;
        case 4:
            editPipeRepair(pipe, pipeExists);
            break;
        case 5:
            editCSShops(cs, csExists);
            break;
        case 6:
            saveData(
                pipe,
                pipeExists,
                cs,
                csExists
            );
            break;
        case 7:
            loadData(
                pipe,
                pipeExists,
                cs,
                csExists
            );
            break;
        case 0:
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid menu item. " << "Please try again.\n";
            break;
        }
    }
    return 0;
}
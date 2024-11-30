#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include<bits/stdc++.h>
#include <iomanip>

using namespace std;

vector<vector<int>> result(5, vector<int>(2, 0));

bool search(int key, vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        {
        if (fr[i] == key)
        {
            return true;
        }
    }
    return false;
}

void displayFrames(const int* frames, int capacity) {
    cout << "Frames: ";
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1)
            cout << " - ";
        else
            cout << " " << frames[i] << " ";
    }
    cout << endl;
}

void welcomePage()
{

    system("cls"); // Clear the screen (Windows only)
    cout << "*******************************************************************************\n";
    cout << "*                                                                             *\n";
    cout << "*          WELCOME TO THE PAGE REPLACEMENT ALGORITHM SIMULATOR                *\n";
    cout << "*                                                                             *\n";
    cout << "*******************************************************************************\n";
    cout << "*                                                                             *\n";
    cout << "*   This program simulates various page replacement algorithms used in        *\n";
    cout << "*   operating systems for memory management. Choose an algorithm from         *\n";
    cout << "*   the menu to see how it performs and compare their efficiencies!           *\n";
    cout << "*                                                                             *\n";
    cout << "*   Algorithms Implemented:                                                   *\n";
    cout << "*     1. FIFO (First-In, First-Out)                                           *\n";
    cout << "*     2. Optimal Page Replacement                                             *\n";
    cout << "*     3. LRU (Least Recently Used)                                            *\n";
    cout << "*     4. LFU (Least Frequently Used)                                          *\n";
    cout << "*     5. Second Chance Algorithm                                              *\n";
    cout << "*                                                                             *\n";
    cout << "*******************************************************************************\n";
    cout << "*                                                                             *\n";
    cout << "*                  Press ENTER to continue to the program.                    *\n";
    cout << "*                                                                             *\n";
    cout << "*******************************************************************************\n";

    cin.ignore(); // Wait for the user to press Enter
    cin.get();    // Wait again to ensure the screen doesn't skip immediately

}

void helpFunction()
{
    system("cls"); // Clear the screen
    cout << "*******************************************************************************\n";
    cout << "*                                HELP MENU                                    *\n";
    cout << "*******************************************************************************\n";
    cout << "1. FIFO (First-In, First-Out):\n";
    cout << "   - The oldest page in memory is replaced when a new page needs to be loaded.\n";
    cout << "   - FIFO is one of the simplest page replacement algorithms to implement.\n";
    cout << "   - It may lead to poor performance in some cases, especially when the system is running long workloads with a high degree of locality.\n";
    cout << "   - Belady's anomaly: In some cases, increasing the number of frames leads to more page faults.\n";
    cout << "   - Usage: This algorithm is best suited for systems with low complexity where simplicity is prioritized.\n";
    cout << "\n";

    cout << "2. Optimal Page Replacement:\n";
    cout << "   - Replaces the page that will not be used for the longest period in the future.\n";
    cout << "   - This algorithm provides the best theoretical performance but requires knowledge of future memory accesses.\n";
    cout << "   - Ideal for comparison, but impractical in real-world systems because it requires predicting future page accesses.\n";
    cout << "   - In practice, this algorithm is often used to benchmark or evaluate the performance of other algorithms.\n";
    cout << "   - Usage: This is not typically used in real systems but is useful for understanding the limits of page replacement strategies.\n";
    cout << "\n";

    cout << "3. LRU (Least Recently Used):\n";
    cout << "   - Replaces the page that has not been used for the longest time, mimicking an optimal strategy based on recent activity.\n";
    cout << "   - It approximates the performance of the Optimal Page Replacement algorithm in many scenarios.\n";
    cout << "   - Requires keeping track of the time when each page was last used, which can be done using counters or hardware support.\n";
    cout << "   - LRU is effective in systems where locality of reference is high, i.e., pages that have been used recently are likely to be used again soon.\n";
    cout << "   - Usage: This algorithm is widely used in modern operating systems and hardware due to its balance of efficiency and implementation complexity.\n";
    cout << "\n";

    cout << "4. LFU (Least Frequently Used):\n";
    cout << "   - Replaces the page with the least frequency of use, meaning the page that is accessed the fewest times.\n";
    cout << "   - Tracks the frequency of access for each page and replaces the one with the lowest count.\n";
    cout << "   - LFU is effective in environments where certain pages are accessed much less frequently than others, helping to keep frequently accessed pages in memory.\n";
    cout << "   - However, LFU has a major drawback of being susceptible to 'cache pollution,' where pages that are accessed infrequently but are still important are replaced.\n";
    cout << "   - Usage: It is used in some systems that require more fine-grained memory management and can be combined with other algorithms like LRU.\n";
    cout << "\n";

    cout << "5. Second Chance (Clock Algorithm):\n";
    cout << "   - A modification of FIFO, giving pages a second chance before replacing them.\n";
    cout << "   - Each page has a reference bit. If a page's reference bit is 1, it is given a second chance, and the bit is reset. If the bit is 0, the page is replaced.\n";
    cout << "   - This algorithm provides a good compromise between performance and simplicity, balancing the advantages of FIFO and LRU.\n";
    cout << "   - It is often used in operating systems with low resource overhead, as it avoids the complexity of fully implementing LRU.\n";
    cout << "   - Usage: The second chance algorithm is often used in systems that have limited hardware resources or where simplicity is more important than absolute performance.\n";
    cout << "\n";

    cout << "*******************************************************************************\n";
    cout << "* Press ENTER to return to the main menu.                                     *\n";
    cout << "*******************************************************************************\n";

    cin.ignore();
    cin.get();
}

int predict(int pages[], vector<int>& fr, int pn, int index)
{
    int res = -1;
    int farthest = index;

    for (int i = 0; i < fr.size(); i++)
    {
        int j;
        for (j = index; j < pn; j++)
        {
            if (fr[i] == pages[j])
            {
                if (j > farthest)
                {
                    farthest = j;
                    res = i;
                }

                break;
            }
        }
        if (j == pn) { // If page not referenced in future
            return i;
        }
    }

    if(res == -1)
    {
        return 0;
    }
    else
    {
        return res;
    }
}

void FIFO(int pages[], int n, int capacity)
{
    unordered_set<int> s;
    queue<int> indexes;
    int page_faults = 0;
    int page_hits = 0;
    vector<int> frames(capacity, -1); // Represents the memory frames

    cout << "\nFIFO Simulation:\n";

    for (int i = 0; i < n; i++)
    {
        cout << "Processing page " << pages[i] << "...\n";

        if (s.size() < capacity)
        {
            if (s.find(pages[i]) == s.end())
            {
                s.insert(pages[i]);
                indexes.push(pages[i]);
                frames[indexes.size() - 1] = pages[i];
                page_faults++;
                cout << "Page fault occurred! Adding page " << pages[i] << " to memory.\n";
            }
        else
            {
                page_hits++;
                cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
            }
        }
        else
            {
            if (s.find(pages[i]) == s.end())
            {
                int val = indexes.front();
                indexes.pop();
                s.erase(val);
                s.insert(pages[i]);
                indexes.push(pages[i]);

                // Replace the page in the frame
                auto it = find(frames.begin(), frames.end(), val);
                if (it != frames.end())
                    {
                    *it = pages[i];
                    }
                page_faults++;
                cout << "Page fault occurred! Replacing page " << val << " with " << pages[i] << ".\n";
            } else
            {
                page_hits++;
                cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
            }
        }
        displayFrames(frames.data(), capacity);  // Pass raw array with .data()

    }

    result[0][0] = page_faults;
    result[0][1] = page_hits;
    cout << "\nFIFO Summary:\n";
    cout << "Number of page faults: " << result[0][0] << endl;
    cout << "Number of page hits: " << result[0][1] << endl;
    cout << "Fault Ratio: " << float(result[0][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[0][1]) / float(n) << endl;

}

void optimalPage(int pages[], int n, int capacity)
{
    vector<int> fr(capacity, -1); // Memory frames
    int hit = 0, page_faults = 0;

    cout << "\nOptimal Page Replacement Simulation:\n";

    for (int i = 0; i < n; i++)
        {
        cout << "Processing page " << pages[i] << "...\n";

        if (search(pages[i], fr))
            {
            hit++;
            cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
        }
        else
            {
            page_faults++;
            if (find(fr.begin(), fr.end(), -1) != fr.end()) {
                auto it = find(fr.begin(), fr.end(), -1);
                *it = pages[i];
                cout << "Page fault occurred! Adding page " << pages[i] << " to memory.\n";
            }
        else
            {
                int j = predict(pages, fr, n, i + 1);
                cout << "Page fault occurred! Replacing page " << fr[j] << " with " << pages[i] << ".\n";
                fr[j] = pages[i];
            }

        }
        displayFrames(fr.data(), capacity);  // Pass raw array with .data()

    }

    result[1][0] = page_faults;
    result[1][1] = hit;
    cout << "\nOptimal Page Replacement Summary:\n";
    cout << "Number of page faults: " << result[1][0] << endl;
    cout << "Number of page hits: " << result[1][1] << endl;
    cout << "Fault Ratio: " << float(result[1][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[1][1]) / float(n) << endl;

}

void LRU(int pages[], int n, int capacity)
 {
    unordered_set<int> s;
    unordered_map<int, int> indexes;
    int page_faults = 0, page_hits = 0;
    vector<int> frames(capacity, -1); // Represents the memory frames

    cout << "\nLRU Simulation:\n";

    for (int i = 0; i < n; i++)
        {
        cout << "Processing page " << pages[i] << "...\n";

        if (s.size() < capacity)
            {
            if (s.find(pages[i]) == s.end())
            {
                s.insert(pages[i]);
                frames[s.size() - 1] = pages[i];
                page_faults++;
                cout << "Page fault occurred! Adding page " << pages[i] << " to memory.\n";
            }
        else {
                page_hits++;
                cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
            }
            indexes[pages[i]] = i;
        }
        else
            {
            if (s.find(pages[i]) == s.end()) {
                int lru = INT_MAX, val;
                for (auto it = s.begin(); it != s.end(); it++) {
                    if (indexes[*it] < lru) {
                        lru = indexes[*it];
                        val = *it;
                    }
                }
                s.erase(val);
                s.insert(pages[i]);
                indexes[pages[i]] = i;

                // Replace the page in the frame
                auto it = find(frames.begin(), frames.end(), val);
                if (it != frames.end()) {
                    *it = pages[i];
                }
                page_faults++;
                cout << "Page fault occurred! Replacing least recently used page " << val << " with " << pages[i] << ".\n";
            }
            else
                {
                page_hits++;
                cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
            }

            indexes[pages[i]] = i;

        }

        displayFrames(frames.data(), capacity);
    }

    result[2][0] = page_faults;
    result[2][1] = n - page_faults;
    cout << "\nLRU Summary:\n";
    cout << "Number of page faults: " << result[2][0] << endl;
    cout << "Number of page hits: " << result[2][1] << endl;
    cout << "Fault Ratio: " << float(result[2][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[2][1]) / float(n) << endl;

}

void LFU(int pages[], int n, int capacity) {
    vector<int> frames(capacity, -1);
    unordered_map<int, int> freq;
    unordered_map<int, int> lastUsed;
    int page_faults = 0, time = 0;

    cout << "\nLFU Simulation:\n";

    for (int i = 0; i < n; i++) {
        cout << "Processing page " << pages[i] << "...\n";

        if (find(frames.begin(), frames.end(), pages[i]) != frames.end()) {
            freq[pages[i]]++;
            cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
        } else {
            page_faults++;
            if (find(frames.begin(), frames.end(), -1) != frames.end()) {
                auto it = find(frames.begin(), frames.end(), -1);
                *it = pages[i];
                cout << "Page fault occurred! Adding page " << pages[i] << " to memory.\n";
            } else {
                int lfu = INT_MAX, lru_time = INT_MAX, replace_index = -1;
                for (int j = 0; j < capacity; j++) {
                    if (freq[frames[j]] < lfu ||
                        (freq[frames[j]] == lfu && lastUsed[frames[j]] < lru_time)) {
                        lfu = freq[frames[j]];
                        lru_time = lastUsed[frames[j]];
                        replace_index = j;
                    }
                }
                cout << "Page fault occurred! Replacing page " << frames[replace_index] << " with " << pages[i] << ".\n";
                freq.erase(frames[replace_index]);
                frames[replace_index] = pages[i];
            }
            freq[pages[i]]++;
        }
        lastUsed[pages[i]] = ++time;
        displayFrames(frames.data(), capacity); // Show current state of frames
    }

    result[3][0] = page_faults;
    result[3][1] = n - page_faults;
    cout << "\nLFU Summary:\n";
    cout << "Number of page faults: " << result[3][0] << endl;
    cout << "Number of page hits: " << result[3][1] << endl;
    cout << "Fault Ratio: " << float(result[3][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[3][1]) / float(n) << endl;
}

void SecondChanceReplacement(int pages[], int n, int capacity) {
    vector<int> arr(capacity, -1);         // Memory frames
    vector<bool> second_chance(capacity); // Second chance flags
    int pointer = 0, page_faults = 0;

    cout << "\nSecond Chance Simulation:\n";

    for (int i = 0; i < n; i++) {
        cout << "Processing page " << pages[i] << "...\n";

        auto it = find(arr.begin(), arr.end(), pages[i]);
        if (it != arr.end()) {
            second_chance[it - arr.begin()] = true;
            cout << "Page " << pages[i] << " is already in memory (Page hit).\n";
        } else {
            page_faults++;
            while (second_chance[pointer]) {
                second_chance[pointer] = false;
                pointer = (pointer + 1) % capacity;
            }
            if (arr[pointer] != -1) {
                cout << "Page fault occurred! Replacing page " << arr[pointer] << " with " << pages[i] << ".\n";
            } else {
                cout << "Page fault occurred! Adding page " << pages[i] << " to memory.\n";
            }
            arr[pointer] = pages[i];
            second_chance[pointer] = false;
            pointer = (pointer + 1) % capacity;
        }
        displayFrames(arr.data(), capacity); // Show current state of frames
    }

    result[4][0] = page_faults;
    result[4][1] = n - page_faults;
    cout << "\nSecond Chance Summary:\n";
    cout << "Number of page faults: " << result[4][0] << endl;
    cout << "Number of page hits: " << result[4][1] << endl;
    cout << "Fault Ratio: " << float(result[4][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[4][1]) / float(n) << endl;
}

void MRUPageReplacement(int pages[], int n, int capacity) {
    int arr[capacity];
    bool pageFound;
    int page_faults = 0;

    // Initialize frame array
    for (int i = 0; i < capacity; i++) {
        arr[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        pageFound = false;

        // Check if page is already in memory
        for (int j = 0; j < capacity; j++) {
            if (arr[j] == pages[i]) {
                pageFound = true;
                break;
            }
        }

        if (!pageFound) {
            page_faults++;
            int replaceIndex = 0;
            for (int j = 1; j < capacity; j++) {
                // Replace the most recently used page
                if (arr[j] > arr[replaceIndex]) {
                    replaceIndex = j;
                }
            }
            cout << "Page fault occurred! Replacing page " << arr[replaceIndex] << " with " << pages[i] << ".\n";
            arr[replaceIndex] = pages[i]; // Replace page
        }

        displayFrames(arr, capacity);   // Show current state of frames
    }

    result[7][0] = page_faults;
    result[7][1] = n - page_faults;
    cout << "\nMost Recently Used (MRU) Page Replacement Summary:\n";
    cout << "Number of page faults: " << result[7][0] << endl;
    cout << "Number of page hits: " << result[7][1] << endl;
    cout << "Fault Ratio: " << float(result[7][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[7][1]) / float(n) << endl;
}

void clockPageReplacement(int pages[], int n, int capacity) {
    int arr[capacity], second_chance[capacity];
    bool pageFound;
    int page_faults = 0;
    int pointer = 0;

    // Initialize frame array and second chance array
    for (int i = 0; i < capacity; i++) {
        arr[i] = -1;
        second_chance[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        pageFound = false;

        // Check if page is in memory
        for (int j = 0; j < capacity; j++) {
            if (arr[j] == pages[i]) {
                second_chance[j] = 1; // Give the page a second chance
                pageFound = true;
                break;
            }
        }

        if (!pageFound) {
            page_faults++;
            // Find a page to replace using clock algorithm
            while (second_chance[pointer] == 1) {
                second_chance[pointer] = 0;
                pointer = (pointer + 1) % capacity;
            }
            cout << "Page fault occurred! Replacing page " << arr[pointer] << " with " << pages[i] << ".\n";
            arr[pointer] = pages[i];
            second_chance[pointer] = 0;
            pointer = (pointer + 1) % capacity; // Move to next page
        }

        displayFrames(arr, capacity);  // Use raw array, no need for .data()

    }

    result[6][0] = page_faults;
    result[6][1] = n - page_faults;
    cout << "\nClock Page Replacement Summary:\n";
    cout << "Number of page faults: " << result[6][0] << endl;
    cout << "Number of page hits: " << result[6][1] << endl;
    cout << "Fault Ratio: " << float(result[6][0]) / float(n) << endl;
    cout << "Hit Ratio: " << float(result[6][1]) / float(n) << endl;
}

void printTable() {
    cout << "\nComparison Table:\n";
    cout << "-----------------------------------------------------------\n";
    cout << "| Algorithm         | Page Faults   | Page Hits    | Hit Ratio |\n";
    cout << "-----------------------------------------------------------\n";

    string algorithms[] = {"FIFO", "Optimal", "LRU", "LFU", "Second Chance"};
    for (int i = 0; i < 5; i++)
        {
        float hitRatio = static_cast<float>(result[i][1]) / (result[i][0] + result[i][1]);
        cout << "| " << setw(16) << algorithms[i]
             << " | " << setw(12) << result[i][0]
             << " | " << setw(11) << result[i][1]
             << " | " << setw(9) << fixed << setprecision(2) << hitRatio << " |\n";
    }

    cout << "-----------------------------------------------------------\n";
}

void concludeComparison(int n) {
    cout << "\nConclusion:\n";
    int min_faults = result[0][0], best_algo = 0;
    string algorithms[] = {"FIFO", "Optimal", "LRU", "LFU", "Second Chance"};

    for (int i = 1; i < 5; i++)
    {
        if (result[i][0] < min_faults)
        {
            min_faults = result[i][0];
            best_algo = i;
        }
    }

    cout << "Best Algorithm: " << algorithms[best_algo] << " with " << min_faults << " page faults.\n";
}

void CompareAll(int pages[], int n, int capacity) {
    cout << "\n[INFO] Comparing All Page Replacement Algorithms...\n";
    FIFO(pages, n, capacity);
    optimalPage(pages, n, capacity);
    LRU(pages, n, capacity);
    LFU(pages, n, capacity);
    SecondChanceReplacement(pages, n, capacity);
    clockPageReplacement(pages, n, capacity);
    MRUPageReplacement(pages, n, capacity);

    printTable();
    concludeComparison(n);
}

int main() {

    welcomePage();

    int n, capacity;

    cout << "Enter the number of pages: ";
    cin >> n;
    int pages[n];

    cout << "Enter the pages: ";
    for (int i = 0; i < n; i++)
    {
        cin >> pages[i];
    }

    cout << "Enter the capacity of page frames: ";
    cin >> capacity;

    system("cls");

   while (true) {

    char ch;
    cout << "\n************************************** MENU **************************************\n";
    cout << "|  Press 1 for FIFO Replacement Technique                                          |\n";
    cout << "|  Press 2 for Optimal Page Replacement Technique                                  |\n";
    cout << "|  Press 3 for LRU Replacement Technique                                           |\n";
    cout << "|  Press 4 for LFU Replacement Technique                                           |\n";
    cout << "|  Press 5 for Second Chance Replacement Technique                                 |\n";
    cout << "|  Press 6 to Compare All Page Replacement Techniques                              |\n";
    cout << "|  Press 7 for Help Information About Algorithms                                   |\n";
    cout << "|  Press 8 to Exit                                                                |\n";
    cout << "**********************************************************************************\n";
    cout << "Please enter your choice: ";
    cin >> ch;

    switch (ch) {

        case '1':
            FIFO(pages, n, capacity);
            break;

        case '2':
            optimalPage(pages, n, capacity);
            break;

        case '3':
            LRU(pages, n, capacity);
            break;

        case '4':
            LFU(pages, n, capacity);
            break;

        case '5':
            SecondChanceReplacement(pages, n, capacity);
            break;

        case '6':
            CompareAll(pages, n, capacity);
            break;

        case '7':
            helpFunction();
            break;

        case '8':
            cout << "Exiting program. Thank you!" << endl;
            return 0;

        default:
            cout << "Invalid choice! Please try again." << endl;
    }

    cout << "\nEnter M to go back to the menu, or E to exit: ";
    char c;
    cin >> c;

    if (c == 'm' || c == 'M')
        {
        system("cls");
        continue;
    }
 else if (c == 'e' || c == 'E')
     {
        break;
    }
}
    return 0;
}
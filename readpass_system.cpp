#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include <limits>
#include <iomanip>
using namespace std;
// --- Data Models ---
struct User {
    string id;
    string name;
    string subscriptionTier; // "Free", "Basic", "Premium"
};
struct Article {
    string id;
    string title;
    int views;
    double engagementScore;
    double monetizationRate;
};
// --- System Class encapsulating all features ---
class ReadPassSystem {
private:
    // 1. Subscriber Directory (Hash Map)
    unordered_map<string, User> subscriberDirectory;
    // 2. Access Undo (Stack)
    struct Action {
        string userId;
        string previousTier;
        string newTier;
        string description;
    };
    stack<Action> accessHistory;
    // 3. Click Tracker (Queue)
    queue<string> clickStream;
    // 4. Coupon Check (Sorted Vector + Binary Search)
    vector<string> activeCoupons;
    // 5. Article Sorter (Merge Sort helper function)
    void merge(vector<Article>& arr, int left, int mid, int right, const string& metric) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<Article> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            bool condition = false;
            if (metric == "views") condition = L[i].views >= R[j].views;
            else if (metric == "engagement") condition = L[i].engagementScore >= R[j].engagementScore;
            else if (metric == "monetization") condition = L[i].monetizationRate >= R[j].monetizationRate;
            if (condition) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) { arr[k] = L[i]; i++; k++; }
        while (j < n2) { arr[k] = R[j]; j++; k++; }
    }
    void mergeSort(vector<Article>& arr, int left, int right, const string& metric) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, metric);
        mergeSort(arr, mid + 1, right, metric);
        merge(arr, left, mid, right, metric);
    }
    // 6. Content Web (Graph - Adjacency List)
    unordered_map<string, vector<pair<string, double>>> contentGraph; // double is distance/correlation weight (lower is more correlated)
public:
    ReadPassSystem() {
        // Initialize with some sorted coupons
        activeCoupons = {"NYT2026", "READ50", "STUDENT99", "TRIAL30"};
        sort(activeCoupons.begin(), activeCoupons.end());
    }
    // --- 1. Subscriber Directory Methods ---
    void addUser(const string& id, const string& name, const string& tier) {
        subscriberDirectory[id] = {id, name, tier};
        cout << "User added/updated: " << name << " (" << tier << ")\n";
    }
    void getUser(const string& id) {
        if (subscriberDirectory.find(id) != subscriberDirectory.end()) {
            User u = subscriberDirectory[id];
            cout << "Found User: " << u.name << ", Tier: " << u.subscriptionTier << "\n";
        } else {
            cout << "User not found.\n";
        }
    }
    void deleteUser(const string& id) {
        if (subscriberDirectory.erase(id)) {
            cout << "User " << id << " deleted successfully.\n";
        } else {
            cout << "User not found for deletion.\n";
        }
    }
    void showAllUsers() {
        cout << "All Subscribers:\n";
        for (const auto& pair : subscriberDirectory) {
            cout << pair.second.id << " " << pair.second.name << " " << pair.second.subscriptionTier << "\n";
        }
    }
    // --- 2. Access Undo Methods ---
    void grantAccess(const string& userId, const string& newTier) {
        if (subscriberDirectory.find(userId) == subscriberDirectory.end()) {
            cout << "User not found.\n";
            return;
        }
        string prevTier = subscriberDirectory[userId].subscriptionTier;
        subscriberDirectory[userId].subscriptionTier = newTier;
        
        accessHistory.push({userId, prevTier, newTier, "Upgraded to " + newTier});
        cout << "Access granted. " << subscriberDirectory[userId].name << " is now " << newTier << ".\n";
    }
    void undoLastAccessChange() {
        if (accessHistory.empty()) {
            cout << "No access changes to undo.\n";
            return;
        }
        Action last = accessHistory.top();
        accessHistory.pop();
        
        if (subscriberDirectory.find(last.userId) != subscriberDirectory.end()) {
            subscriberDirectory[last.userId].subscriptionTier = last.previousTier;
            cout << "Undo successful. User reverted to " << last.previousTier << " tier.\n";
        }
    }
    void showAccessHistory() {
        cout << "Access History Stack (Top to Bottom):\n";
        stack<Action> temp = accessHistory;
        if (temp.empty()) {
            cout << "No history.\n";
            return;
        }
        while (!temp.empty()) {
            Action a = temp.top();
            cout << "- User: " << a.userId << " | " << a.previousTier << " -> " << a.newTier << " (" << a.description << ")\n";
            temp.pop();
        }
    }
    // --- 3. Click Tracker Methods ---
    void registerClick(const string& articleId) {
        clickStream.push(articleId);
        cout << "Click registered for article: " << articleId << "\n";
    }
    void processClicks() {
        cout << "Processing click stream sequentially:\n";
        while (!clickStream.empty()) {
            cout << " -> Processed click for: " << clickStream.front() << "\n";
            clickStream.pop();
        }
    }
    // --- 4. Coupon Check Method ---
    bool checkCoupon(const string& code) {
        int left = 0, right = activeCoupons.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (activeCoupons[mid] == code) return true;
            if (activeCoupons[mid] < code) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }
    // --- 5. Article Sorter Method ---
    void sortAndDisplayArticles(vector<Article>& articles, const string& metric) {
        mergeSort(articles, 0, articles.size() - 1, metric);
        cout << "\nArticles sorted by " << metric << " (Descending):\n";
        for (const auto& a : articles) {
            cout << "- " << a.title << " | Views: " << a.views 
                 << " | Engagement: " << a.engagementScore 
                 << " | Monetization: $" << a.monetizationRate << "\n";
        }
    }
    // --- 6. Content Web Methods ---
    void addContentLink(const string& art1, const string& art2, double correlationScore) {
        // Lower correlation score implies stronger relationship (shorter path)
        contentGraph[art1].push_back({art2, correlationScore});
        contentGraph[art2].push_back({art1, correlationScore}); // Undirected
    }
    void displayContentGraph() {
        cout << "Content Graph Connections:\n";
        for (const auto& pair : contentGraph) {
            for (const auto& edge : pair.second) {
                cout << pair.first << " -> " << edge.first << " (" << edge.second << ")\n";
            }
        }
    }
    // --- 7. Reading Path (Dijkstra) ---
    void findReadingPath(const string& startId, const string& targetId) {
        unordered_map<string, double> distances;
        unordered_map<string, string> previous;
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        for (const auto& pair : contentGraph) {
            distances[pair.first] = numeric_limits<double>::infinity();
        }
        distances[startId] = 0;
        pq.push({0, startId});
        while (!pq.empty()) {
            double currentDist = pq.top().first;
            string currentId = pq.top().second;
            pq.pop();
            if (currentId == targetId) break;
            if (currentDist > distances[currentId]) continue;
            for (const auto& neighbor : contentGraph[currentId]) {
                string nextId = neighbor.first;
                double weight = neighbor.second;
                double distance = currentDist + weight;
                if (distance < distances[nextId]) {
                    distances[nextId] = distance;
                    previous[nextId] = currentId;
                    pq.push({distance, nextId});
                }
            }
        }
        if (distances[targetId] == numeric_limits<double>::infinity()) {
            cout << "No path found between " << startId << " and " << targetId << "\n";
            return;
        }
        // Reconstruct path
        vector<string> path;
        string curr = targetId;
        while (curr != startId) {
            path.push_back(curr);
            curr = previous[curr];
        }
        path.push_back(startId);
        reverse(path.begin(), path.end());
        cout << "Optimal Reading Path (Min Correlation Weight: " << distances[targetId] << "):\n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << "\n";
    }
    // --- 8. Price Modeler (Dynamic Programming - Knapsack variant) ---
    // Problem: Given a set of possible discount offers (each with a cost to the company and a conversion probability),
    // find the optimal set of offers to maximize total conversion probability within a maximum marketing budget.
    void optimizePricingModel(int maxBudget, vector<int> offerCosts, vector<int> conversionProbs) {
        int n = offerCosts.size();
        vector<vector<int>> dp(n + 1, vector<int>(maxBudget + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int w = 1; w <= maxBudget; w++) {
                if (offerCosts[i - 1] <= w) {
                    dp[i][w] = max(conversionProbs[i - 1] + dp[i - 1][w - offerCosts[i - 1]], dp[i - 1][w]);
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }
        cout << "Optimal Pricing Strategy Calculated.\n";
        cout << "Maximum conversion score achievable within budget ($" << maxBudget << ") is: " << dp[n][maxBudget] << "\n";
    }
};
int main() {
    ReadPassSystem system;
    vector<Article> db = {
        {"A1", "Tech Trends", 5000, 4.2, 120.50},
        {"A2", "Market Analysis", 12000, 3.8, 450.00},
        {"A3", "Startup Culture", 8000, 4.9, 310.20}
    };
    system.addContentLink("A1", "A2", 5.0);
    system.addContentLink("A1", "A3", 1.0);
    system.addContentLink("A3", "A2", 2.0);
    
    cout << "========================================\n";
    cout << "READPASS SUBSCRIPTION & PAYWALL SYSTEM\n";
    cout << "========================================\n";
     while(true) {
        cout << "\n===== READPASS MENU =====\n";
        cout << "1. Add User\n";
        cout << "2. Search User\n";
        cout << "3. Delete User\n";
        cout << "4. Show All Users\n";
        cout << "5. Grant Access\n";
        cout << "6. Undo Access\n";
        cout << "7. Register Click\n";
        cout << "8. Validate Coupon\n";
        cout << "9. Sort Articles\n";
        cout << "10. Display Content Graph\n";
        cout << "11. Find Reading Path\n";
        cout << "12. Optimize Pricing\n";
        cout << "13. Exit\n";
        cout << "Enter your choice: ";
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        string id, name, tier, code, artId, startArt, endArt;
        vector<Article> originalDB = db; // Copy for clean sorting demo
        switch(choice) {
            case 1:
                cout << "Enter User ID: "; cin >> id;
                cout << "Enter Name: "; cin >> ws; getline(cin, name);
                cout << "Enter Tier (Free/Premium): "; cin >> tier;
                system.addUser(id, name, tier);
                break;
            case 2:
                cout << "Enter User ID to search: "; cin >> id;
                system.getUser(id);
                break;
            case 3:
                cout << "Enter User ID to delete: "; cin >> id;
                system.deleteUser(id);
                break;
            case 4:
                system.showAllUsers();
                break;
            case 5:
                cout << "Enter User ID: "; cin >> id;
                cout << "Enter New Tier: "; cin >> tier;
                system.grantAccess(id, tier);
                break;
            case 6:
                cout << "Enter User ID to verify after undo: "; cin >> id;
                system.undoLastAccessChange();
                system.getUser(id);
                break;
            case 7:
                cout << "Enter Article ID clicked: "; cin >> artId;
                system.registerClick(artId);
                system.processClicks();
                break;
            case 8:
                cout << "Enter Coupon Code: "; cin >> code;
                cout << "Checking coupon '" << code << "': " << (system.checkCoupon(code) ? "VALID" : "INVALID") << "\n";
                break;
            case 9:
                system.sortAndDisplayArticles(db, "views");
                db = originalDB; // Restore
                system.sortAndDisplayArticles(db, "engagement");
                break;
            case 10:
                system.displayContentGraph();
                break;
            case 11:
                cout << "Enter Start Article ID: "; cin >> startArt;
                cout << "Enter Target Article ID: "; cin >> endArt;
                system.findReadingPath(startArt, endArt);
                break;
            case 12:
                {
                    vector<int> costs = {10, 20, 30};
                    vector<int> probs = {60, 100, 120};
                    system.optimizePricingModel(50, costs, probs);
                }
                break;
            case 13:
                cout << "Exiting system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}


#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <functional>

using namespace std;
// globally declared graph

struct Route {
    string destination;
    double distance;
    double cost;
};

map<string, vector<Route>> graph;

void addcity(string city){
    if(graph.find(city)==graph.end()){
        graph[city] ={};

        cout << "City added: " << city <<endl;
    }
    else{
        cout << "city already exist.\n";
    }
}

void addRoute(string cityA,string cityB);
     bool routeExists(string cityA, string cityB) {
    if (graph.find(cityA) == graph.end() ||
        graph.find(cityB) == graph.end()) {
        return false;
    }

    for (const auto &r : graph[cityA]) {
        if (r.destination == cityB) {
            return true;
        }
    }

    return false;
}

void addRoute(string cityA, string cityB,
              double distance, double cost) {
    if (graph.find(cityA) == graph.end() ||
        graph.find(cityB) == graph.end()) {
        cout << "City does not exist.\n";
        return;
    }

    if (cityA == cityB) {
        cout << "Same city route is not allowed.\n";
        return;
    }

    if (distance <= 0 || cost <= 0) {
        cout << "Distance and cost must be positive.\n";
        return;
    }

    if (routeExists(cityA, cityB)) {
        cout << "Route already exists.\n";
        return;
    }

    graph[cityA].push_back({cityB, distance, cost});
    graph[cityB].push_back({cityA, distance, cost});

    cout << "Route added successfully.\n";
}


void displayAll(){
    cout<< "All Routes:\n";
    for(auto &city:graph){
        cout << city.first << " is connected to: ";
        for(auto &route:city.second){
            cout << route.destination << " (Distance: " << route.distance << " km, Cost: $" << route.cost << ") ";
            cout << " | ";
        }
        cout << endl;
    }
}

void searchConnected(string start){
    if(graph.find(start)==graph.end()){
        cout <<"City not found";
        return;
    }

    //check if city has no routes at all
    if(graph[start].empty()){
        cout<<"No connected citites found from " << start << endl;
        return;
    }

    queue<string> q;
    set<string> visited;

    q.push(start);
    visited.insert(start);
    cout <<"Cities reachable from " << start<< ":";

    bool first = true;

    while(!q.empty()){
        string current = q.front();
        q.pop();


        if(current!=start){
            cout << current << " ";
        }

        for(auto &neighbour:graph[current]){
            if(!visited.count(neighbour.destination)){
                visited.insert(neighbour.destination);
                q.push(neighbour.destination);
            }
        }
    }
    cout << endl;

}
// Delete Flight Route


void deleteRoute(string cityA, string cityB) {
    if (graph.find(cityA) == graph.end() ||
        graph.find(cityB) == graph.end()) {
        cout << "One or both cities do not exist.\n";
        return;
    }

    if (!routeExists(cityA, cityB)) {
        cout << "Route does not exist.\n";
        return;
    }

    auto &vecA = graph[cityA];

    vecA.erase(
        remove_if(vecA.begin(), vecA.end(),
        [&](const Route &r) {
            return r.destination == cityB;
        }),
    vecA.end()
    );

    auto &vecB = graph[cityB];

         vecB.erase(
         remove_if(vecB.begin(), vecB.end(),
                 [&](const Route &r) {
            return r.destination == cityA;
        }),
            vecB.end()
         );

    cout << "Route deleted between "
         << cityA << " and " << cityB << endl;
}

//totalConnections

void totalConnections(string city){
    if(graph.find(city)==graph.end()){

        cout << "City not found\n";
        return;

    }
    cout << city<<" has " <<graph[city].size()<<" direct connections.\n";
}
//this is a shortest path ////////////////////////////////////////// 
void shortestPath(string start, string destination) {
    // Validate cities
    if (graph.find(start) == graph.end() ||
        graph.find(destination) == graph.end()) {
        cout << "One or both cities do not exist.\n";
        return;
    }

    if (start == destination) {
        cout << "Shortest path: " << start << endl;
        cout << "Total routes: 0\n";
        return;
    }

    queue<string> q;
    map<string, string> parent;

    q.push(start);
    parent[start] = start;

    // BFS traversal
    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == destination) {
            break;
        }

        for (auto &neighbour : graph[current]) {
            if (parent.find(neighbour.destination) == parent.end()) {
                parent[neighbour.destination] = current;
                q.push(neighbour.destination);
            }
        }
    }

    // Check whether destination was reached
    if (parent.find(destination) == parent.end()) {
        cout << "No path exists between "
             << start << " and " << destination << endl;
        return;
    }

    // Reconstruct the path
    vector<string> path;
    string current = destination;

    while (current != start) {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(start);
    reverse(path.begin(), path.end());

    cout << "Shortest path: ";

    for (int i = 0; i < path.size(); i++) {
        cout << path[i];

        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }

    cout << "\nTotal routes: "
         << path.size() - 1 << endl;
}
void bestRoute(string start, string destination, bool useCost) {

    if (graph.find(start) == graph.end() ||
        graph.find(destination) == graph.end()) {
        cout << "One or both cities do not exist.\n";
        return;
    }

    map<string, double> dist;
    map<string, string> parent;

    for (auto &city : graph) {
        dist[city.first] = numeric_limits<double>::infinity();
    }

    priority_queue<
        pair<double, string>,
        vector<pair<double, string>>,
        greater<pair<double, string>>
    > pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {

        double currentValue = pq.top().first;
        string currentCity = pq.top().second;
        pq.pop();

        if (currentValue > dist[currentCity])
            continue;

        for (const auto &route : graph[currentCity]) {

            double weight;

            if (useCost)
                weight = route.cost;
            else
                weight = route.distance;

            double newValue = dist[currentCity] + weight;

            if (newValue < dist[route.destination]) {

                dist[route.destination] = newValue;
                parent[route.destination] = currentCity;

                pq.push({newValue, route.destination});
            }
        }
    }

    if (dist[destination] == numeric_limits<double>::infinity()) {
        cout << "No path exists between "
             << start << " and " << destination << endl;
        return;
    }

    vector<string> path;
    string current = destination;

    while (current != start) {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(start);
    reverse(path.begin(), path.end());

    cout << "\nBest route: ";

    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i];

        if (i < (int)path.size() - 1)
            cout << " -> ";
    }

    cout << endl;

    if (useCost)
        cout << "Minimum total cost: $" << dist[destination] << endl;
    else
        cout << "Minimum total distance: "
             << dist[destination] << " km" << endl;
}


int main(){
    // Pre-added citites....
    cout << "Pre-added citites and routes and distances in km and costs......\n";
    addcity("Delhi");
    addcity("Bihar");
    addRoute("Delhi","Bihar", 100, 5000);
    cout<<"\n";

    int choice;
    string cityA,cityB;
    while(true){
        cout << "-------Flight Route Optimizer-------\n";
        cout <<"1. Add City\n2. Add Route\n3. Display All Routes\n4. Search Connected cities\n5. Delete Routes\n6. Total Connections\n7. Find Shortest Path\n8. Minimum Distance Route\n9. Minimum Cost Route\n0. Exit\n";
        cout <<"Enter choice: ";
        cin >> choice;

        if(choice==0){
            break;
        }
    
    switch(choice){
        case 1:
            cout << "Enter city name "; 
            cin>>cityA;
            addcity(cityA);
            break;

        case 2:
            cout << "Enter two cities and distance and cost: ";
            double distance, cost;
            cin >> cityA >> cityB >> distance >> cost;
            addRoute(cityA, cityB, distance, cost);
            break;
        
        case 3:
            displayAll();
            break;

        case 4:
            cout << "Enter start city: ";
            cin >> cityA;
            searchConnected(cityA);
            break;

        case 5:
            cout << "Enter two cities: ";
            cin>>cityA>>cityB;
            deleteRoute(cityA,cityB);
            break;
        
        case 6:
            cout<< "Enter city: ";
            cin >>cityA;
            totalConnections(cityA);
            break;

            
        case 7:
            cout << "Enter starting and destination cities: ";
            cin >> cityA >> cityB;
            shortestPath(cityA, cityB);
            break;

        case 8:
            cout << "Enter starting and destination cities: ";
            cin >> cityA >> cityB;
            bestRoute(cityA, cityB, false);
            break;

        case 9:
            cout << "Enter starting and destination cities: "; 
            cin >> cityA >> cityB;
            bestRoute(cityA, cityB, true);
            break;

        default:
            cout << "Invalid Choice\n";

    }
}

    return 0;
}
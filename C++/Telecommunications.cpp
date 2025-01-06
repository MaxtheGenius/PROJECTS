#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

// Define Node struct to represent each node in the network
struct Node {
    int node_id;                // Unique identifier for the node
    vector<int> neighbors;      // List of neighboring node IDs
    bool is_active;             // State of the node (active or not)

    Node(int id) : node_id(id), is_active(true) {}
};

// Define Message struct to represent a message in the system
struct Message {
    int source_id;              // Sender node ID
    int destination_id;         // Receiver node ID
    string content;             // Content of the message
    int hops;                   // Number of hops to reach the destination

    Message(int src, int dest, string msg)
        : source_id(src), destination_id(dest), content(msg), hops(0) {}
};

// MeshNetwork class to represent the mesh network system
class MeshNetwork {
public:
    vector<Node> nodes;                              // List of all nodes in the network
    map<int, vector<Message>> message_queues;       // Queues of messages per node
    map<int, map<int, int>> routing_table;          // Routing table for each node (node_id -> destination_id -> next_hop)

    MeshNetwork(int num_nodes);
    void add_edge(int node_id1, int node_id2);
    void send_message(int source_id, int destination_id, string content);
    void receive_message(int node_id);
    void update_routing_table();
    void print_routing_table();
    void display_network_state();
};

// Constructor to initialize the network with given number of nodes
MeshNetwork::MeshNetwork(int num_nodes) {
    for (int i = 0; i < num_nodes; ++i) {
        nodes.push_back(Node(i));
    }
    update_routing_table();
}

// Add bidirectional edges between nodes
void MeshNetwork::add_edge(int node_id1, int node_id2) {
    nodes[node_id1].neighbors.push_back(node_id2);
    nodes[node_id2].neighbors.push_back(node_id1);
}

// Send a message from source node to destination node
void MeshNetwork::send_message(int source_id, int destination_id, string content) {
    Message msg(source_id, destination_id, content);

    // Check if destination is directly reachable
    if (find(nodes[source_id].neighbors.begin(), nodes[source_id].neighbors.end(), destination_id) != nodes[source_id].neighbors.end()) {
        msg.hops = 1;  // Direct hop
        message_queues[destination_id].push_back(msg);
        cout << "Message sent from Node " << source_id << " to Node " << destination_id << " directly." << endl;
    } else {
        // Use routing table to find the next hop
        int next_hop = routing_table[source_id][destination_id];
        if (next_hop != -1) {
            msg.hops = 1;  // Next hop to destination
            message_queues[next_hop].push_back(msg);
            cout << "Message sent from Node " << source_id << " to Node " << next_hop << " (via routing table)." << endl;
        } else {
            cout << "No route found from Node " << source_id << " to Node " << destination_id << "." << endl;
        }
    }
}

// Receive a message at a particular node
void MeshNetwork::receive_message(int node_id) {
    if (message_queues.find(node_id) != message_queues.end()) {
        for (Message& msg : message_queues[node_id]) {
            if (msg.destination_id == node_id) {
                cout << "Node " << node_id << " received message: \"" << msg.content << "\" (Hops: " << msg.hops << ")" << endl;
            }
        }
        message_queues[node_id].clear();  // Clear the message queue for this node
    }
}

// Update routing table using a simple flood-fill algorithm (can be replaced with more complex algorithms like Dijkstra)
void MeshNetwork::update_routing_table() {
    for (Node& node : nodes) {
        // Initialize routing table with direct neighbors
        for (int neighbor : node.neighbors) {
            routing_table[node.node_id][neighbor] = neighbor;  // Direct route to neighbor
        }

        // Simulate a basic flood-fill routing algorithm to propagate routes across the network
        for (Node& other_node : nodes) {
            if (other_node.node_id != node.node_id && find(node.neighbors.begin(), node.neighbors.end(), other_node.node_id) == node.neighbors.end()) {
                // Simulate route through intermediate neighbors
                for (int neighbor : node.neighbors) {
                    if (find(other_node.neighbors.begin(), other_node.neighbors.end(), neighbor) != other_node.neighbors.end()) {
                        routing_table[node.node_id][other_node.node_id] = neighbor;
                    }
                }
            }
        }
    }
}

// Print the current routing table for all nodes
void MeshNetwork::print_routing_table() {
    for (Node& node : nodes) {
        cout << "Routing Table for Node " << node.node_id << ":\n";
        for (auto& entry : routing_table[node.node_id]) {
            cout << "  Destination " << entry.first << " -> Next Hop: " << entry.second << endl;
        }
    }
}

// Display the current state of the network (nodes and their neighbors)
void MeshNetwork::display_network_state() {
    for (Node& node : nodes) {
        cout << "Node " << node.node_id << " -> Active: " << (node.is_active ? "Yes" : "No") << " | Neighbors: ";
        for (int neighbor : node.neighbors) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Main function to demonstrate Mesh Network communication protocol
int main() {
    // Create a mesh network with 5 nodes
    MeshNetwork network(5);

    // Create bidirectional edges (connect the nodes)
    network.add_edge(0, 1);
    network.add_edge(0, 2);
    network.add_edge(1, 3);
    network.add_edge(2, 3);
    network.add_edge(3, 4);

    // Display network state
    network.display_network_state();
    
    // Send messages between nodes
    network.send_message(0, 4, "Temperature Data: 25°C");
    network.send_message(1, 4, "Temperature Data: 24°C");

    // Receive messages at node 4
    network.receive_message(4);

    // Print routing table
    network.print_routing_table();

    return 0;
}
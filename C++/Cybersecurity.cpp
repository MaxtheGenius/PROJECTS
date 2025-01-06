#include <iostream>
#include <pcap.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <thread>
#include <atomic>

using namespace std;

// Constants for attack detection thresholds
const int MAX_PACKETS_PER_SECOND = 1000;  // Threshold for potential DoS attack (packets per second)
const int MAX_CONNECTIONS = 100;          // Threshold for SYN flood detection

// Data structure to store packet information
struct PacketData {
    string src_ip;
    string dest_ip;
    int packet_size;
    time_t timestamp;
};

// Attack signature class to classify patterns
class AttackSignature {
public:
    static bool isDoSAttack(const vector<PacketData>& packets) {
        // Simple DoS detection based on packet rate
        time_t first_time = packets.front().timestamp;
        int packet_count = 0;

        for (const auto& packet : packets) {
            if (difftime(packet.timestamp, first_time) <= 1) {
                packet_count++;
            }
        }

        return packet_count > MAX_PACKETS_PER_SECOND;
    }

    static bool isSYNFloodAttack(const vector<PacketData>& packets) {
        // SYN flood detection based on repeated SYN packets from the same IP
        unordered_map<string, int> src_ip_count;
        for (const auto& packet : packets) {
            src_ip_count[packet.src_ip]++;
        }

        for (const auto& ip : src_ip_count) {
            if (ip.second > MAX_CONNECTIONS) {
                return true;
            }
        }

        return false;
    }
};

// Network traffic analysis tool class
class NetworkTrafficAnalyzer {
private:
    pcap_t* handle;
    vector<PacketData> packet_buffer;
    atomic<bool> running;

public:
    NetworkTrafficAnalyzer() : handle(nullptr), running(false) {}

    // Start capturing packets from the network interface
    void startCapture(const char* device) {
        char errbuf[PCAP_ERRBUF_SIZE];

        // Open the network device for packet capture
        handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
        if (!handle) {
            cerr << "Error opening device " << device << ": " << errbuf << endl;
            return;
        }

        running = true;
        thread capture_thread(&NetworkTrafficAnalyzer::capturePackets, this);
        capture_thread.detach();
    }

    // Stop capturing packets
    void stopCapture() {
        running = false;
        pcap_close(handle);
    }

    // Capture packets and analyze them
    void capturePackets() {
        struct pcap_pkthdr header;
        const unsigned char* packet;

        while (running) {
            packet = pcap_next(handle, &header);
            if (packet != nullptr) {
                processPacket(packet, header.ts.tv_sec);
            }
        }
    }

    // Process each captured packet and analyze it
    void processPacket(const unsigned char* packet, time_t timestamp) {
        // For simplicity, we'll assume we're using IPv4 packets.
        // In reality, this would involve parsing the Ethernet, IP, and TCP headers.
        string src_ip = "192.168.0.1";  // Placeholder: replace with real extraction from the packet
        string dest_ip = "192.168.0.2"; // Placeholder: replace with real extraction from the packet
        int packet_size = 64;            // Placeholder: replace with real packet size

        PacketData packet_data = {src_ip, dest_ip, packet_size, timestamp};
        packet_buffer.push_back(packet_data);

        // Perform attack detection based on captured packets
        if (packet_buffer.size() > 1000) {
            analyzeTraffic();
            packet_buffer.clear();  // Reset buffer after analysis
        }
    }

    // Analyze traffic and detect potential attacks
    void analyzeTraffic() {
        if (AttackSignature::isDoSAttack(packet_buffer)) {
            cout << "DoS Attack detected!" << endl;
        }
        if (AttackSignature::isSYNFloodAttack(packet_buffer)) {
            cout << "SYN Flood Attack detected!" << endl;
        }
    }
};

// Main function to run the network traffic analyzer
int main() {
    NetworkTrafficAnalyzer analyzer;

    // Start packet capture on a given network interface (replace "eth0" with your interface name)
    analyzer.startCapture("eth0");

    // Run the capture and analysis for a fixed amount of time (e.g., 30 seconds)
    this_thread::sleep_for(chrono::seconds(30));

    // Stop packet capture after the analysis period
    analyzer.stopCapture();

    return 0;
}
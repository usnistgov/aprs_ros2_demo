#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <cstdint>
#include <math.h>


union FloatUnion {
    float f;
    uint32_t i;
};

uint32_t float_to_ieee754(float value) {
    FloatUnion fu;
    fu.f = value;
    return htonl(fu.i);
}

std::vector<uint8_t> create_byte_array(std::vector<float> joint_values, float velocity, float duration) {
    uint32_t prefix_length = htonl(0x00000040);
    uint32_t msg_type = htonl(0x0000000b);
    uint32_t comm_type = htonl(0x00000002);
    uint32_t reply_code = 0x00;
    uint32_t sequence = htonl(0x00000001);

    std::vector<uint32_t> joint_data;
    for (float val : joint_values) {
        joint_data.push_back(float_to_ieee754(val));
    }
    while (joint_data.size() < 10) {
        joint_data.push_back(0x00000000);
    }

    uint32_t velocity_ieee754 = float_to_ieee754(velocity);
    uint32_t duration_ieee754 = float_to_ieee754(duration);

    std::vector<uint8_t> byte_array;
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&prefix_length), reinterpret_cast<uint8_t*>(&prefix_length) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&msg_type), reinterpret_cast<uint8_t*>(&msg_type) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&comm_type), reinterpret_cast<uint8_t*>(&comm_type) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&reply_code), reinterpret_cast<uint8_t*>(&reply_code) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&sequence), reinterpret_cast<uint8_t*>(&sequence) + 4);

    for (uint32_t data : joint_data) {
        byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + 4);
    }

    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&velocity_ieee754), reinterpret_cast<uint8_t*>(&velocity_ieee754) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&duration_ieee754), reinterpret_cast<uint8_t*>(&duration_ieee754) + 4);

    return byte_array;
}

void print_byte_array_as_hex(const std::vector<uint8_t>& byte_array) {
    for (size_t i = 0; i < byte_array.size(); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte_array[i]);
        if ((i + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }
}

bool send_byte_array(const std::vector<uint8_t>& byte_array, const std::string& ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return false;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sockfd);
        return false;
    }

    if (send(sockfd, byte_array.data(), byte_array.size(), 0) < 0) {
        std::cerr << "Send failed" << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "Data sent successfully" << std::endl;
    std::cout << std::endl;
    
    char buffer[1024] = {0};
    ssize_t received_bytes = recv(sockfd, buffer, sizeof(buffer)-1,0);
      
    if (received_bytes < 0){
    	std::cerr << "Receive failed" << std::endl;
        close(sockfd);
        return false;
    }
    
    std::cout << "Data Received is: " << std::endl;
    
    for (ssize_t i = 0; i < received_bytes; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
        if ((i + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
    float result;
    int start = 0;
    for (int i = 0; i < 6; i++) {
    	start = 20 + (4*i);
        char recv[4] = {buffer[start+3],buffer[start+2],buffer[start+1],buffer[start]};
        std::memcpy(&result,recv,sizeof(float));
        double deg = result * (180/M_PI);
        std::cout << "Joint " << i+1 << ": " << deg << std::endl;
    }

    
    
    std::cout << std::endl;

    close(sockfd);
    return true;
}

int main() {
    std::vector<float> joint_values = {0.1, 0.327742815, -0.865697324, -3.141592741, 0.705099046, -3.141592741};
    float velocity = 0.01;
    float duration = 10.0;

    std::vector<uint8_t> byte_array = create_byte_array(joint_values, velocity, duration);
    print_byte_array_as_hex(byte_array);

    std::string ip = "192.168.1.34";
    int port = 11000;

    if (!send_byte_array(byte_array, ip, port)) {
        std::cerr << "Failed to send byte array to " << ip << ":" << port << std::endl;
    }

    std::cout << "Press Enter to continue...";
    std::cin.get();

    return 0;
}

// g++ -o JOINT_TRAJ_PT_LINUX JOINT_TRAJ_PT_LINUX.cpp ./JOINT_TRAJ_PT_LINUX


#include <vector>
#include <cstring>

#include <arpa/inet.h>
#include <unistd.h>

/*
==============================================================================
NETWORK UTILITIES
==============================================================================
*/

float bytes_to_float(char* bytes)
{
  float result;

  uint32_t temp = ntohl(*(uint32_t*)bytes);
  std::memcpy(&result, &temp, sizeof(float));

  return result;
}

void insert_byte(std::vector<uint8_t> &v, int data)
{
  uint32_t d = htonl(data);

  v.insert(v.end(), reinterpret_cast<uint8_t*>(&d), reinterpret_cast<uint8_t*>(&d) + 4);

}

void insert_byte(std::vector<uint8_t> &v, float data)
{
  u_int32_t d;
  std::memcpy(&d, &data, sizeof(u_int32_t));

  d = htonl(d);

  v.insert(v.end(), reinterpret_cast<uint8_t*>(&d), reinterpret_cast<uint8_t*>(&d) + 4);
}

int get_packet_length(int socket)
{
  char *length_packet = new char[sizeof(uint32_t)];

  ssize_t ret = read(socket, length_packet, sizeof(uint32_t));

  if (ret < 0){
    return -1;
  }

  return ntohl(*(uint32_t*)length_packet);
}

char* read_from_socket(int socket, int num_bytes){
  char *data = new char[num_bytes];
  
  read(socket, data, num_bytes);

  return data;
} 

void write_to_socket(int socket, std::vector<uint8_t> byte_stream)
{
  write(socket, byte_stream.data(), byte_stream.size());
} 


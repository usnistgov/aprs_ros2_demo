#include <vector>
#include <cstring>

#include <arpa/inet.h>

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

  v.insert(v.end(), reinterpret_cast<uint8_t*>(&d), reinterpret_cast<uint8_t*>(&d) + sizeof(uint8_t));

}

void insert_byte(std::vector<uint8_t> &v, float data)
{
  u_int32_t d;
  std::memcpy(&d, &data, sizeof(u_int32_t));

  d = htonl(d);

  v.insert(v.end(), reinterpret_cast<uint8_t*>(&d), reinterpret_cast<uint8_t*>(&d) + sizeof(uint8_t));
}
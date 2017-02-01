
#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "cert.h"
#include "certt.h"
#include "nss.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./quickder <der file>" << std::endl;
    return 1;
  }
  std::ifstream in(argv[1], std::ios::in | std::ios::binary);
  assert(in && !in.bad());
  if (!in || in.bad()) {
    return 1;
  }

  std::vector<uint8_t> value;
  uint8_t buf[1024];
  while (true) {
    in.read(reinterpret_cast<char *>(buf), sizeof(buf));
    assert(!in.bad());
    if (in.bad()) {
      return 1;
    }
    std::vector<uint8_t> tmp(buf, buf + in.gcount());
    value.insert(value.end(), tmp.begin(), tmp.end());
    if (!in) {
      break;
    }
  }

  char *dest[4096];
  PORTCheapArenaPool pool;
  SECItem item = {siBuffer, const_cast<unsigned char *>(&value[0]),
                  static_cast<unsigned int>(value.size())};

  PORT_InitCheapArena(&pool, DER_DEFAULT_CHUNKSIZE);
  assert(SEC_QuickDERDecodeItem(&pool.arena, dest,
                                SEC_SignedCertificateTemplate,
                                &item) == SECSuccess);
  PORT_DestroyCheapArena(&pool);

  return 0;
}
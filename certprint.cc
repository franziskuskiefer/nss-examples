
#include <assert.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "cert.h"
#include "certt.h"
#include "nss.h"
#include "nssb64.h"

struct NSSCertDeletePolicy {
  void operator()(CERTCertificate *cert) { CERT_DestroyCertificate(cert); }
};
typedef std::unique_ptr<CERTCertificate, NSSCertDeletePolicy> NSSCert;

class NSSDatabase {
public:
  NSSDatabase() { assert(NSS_NoDB_Init(nullptr) == SECSuccess); }
  ~NSSDatabase() { assert(NSS_Shutdown() == SECSuccess); }
};

#define BUF_SIZE 1024
const std::string kPEMBeginCert = "-----BEGIN CERTIFICATE-----";
const std::string kPEMEndCert = "-----END CERTIFICATE-----";

NSSCert read_cert(const std::string &file) {
  SECItem item = {siBuffer, nullptr, 0};
  std::ifstream in(file);
  assert(!in.bad());
  if (in.bad()) {
    return nullptr;
  }

  char buf[BUF_SIZE] = {0};
  in.getline(buf, BUF_SIZE);
  assert(!in.bad());
  if (in.bad()) {
    return nullptr;
  }

  bool isCertFile =
      strncmp(buf, kPEMBeginCert.c_str(), kPEMBeginCert.size()) == 0;
  assert(isCertFile);
  if (!isCertFile) {
    return nullptr;
  }

  std::string value = "";
  while (true) {
    in.getline(buf, sizeof(buf));
    assert(!in.bad());
    if (in.bad()) {
      return nullptr;
    }

    bool done = strncmp(buf, kPEMEndCert.c_str(), kPEMEndCert.size()) == 0;
    if (done) {
      break;
    }

    value += buf;
  }

  bool decoded = NSSBase64_DecodeBuffer(nullptr, &item, value.c_str(),
                                        value.size()) != nullptr;
  assert(decoded);
  if (!decoded) {
    return nullptr;
  }

  NSSCert cert(CERT_NewTempCertificate(CERT_GetDefaultCertDB(), &item, nullptr,
                                       PR_FALSE, PR_TRUE));
  SECITEM_FreeItem(&item, PR_FALSE);

  return cert;
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./certprint <pem file>"  << std::endl;
    return 1;
  }

  // assert(SECOID_Init() == SECSuccess);
  static std::unique_ptr<NSSDatabase> db(new NSSDatabase());
  assert(db != nullptr);

  NSSCert cert = read_cert(std::string(argv[1]));
  assert(cert);

  return 0;
}

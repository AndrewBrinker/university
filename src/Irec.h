#ifndef DOCFS_SRC_IREC_H
#define DOCFS_SRC_IREC_H

#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Irec {
 public:
  Irec(std::string new_key, unsigned int new_block_id);
  inline std::string getKey() { return key; }
  inline unsigned int getBlockID() { return block_id; }

 private:
  std::string key;
  unsigned int block_id;

  DISALLOW_COPY_AND_ASSIGN(Irec);
};

#endif  // DOCFS_SRC_IREC_H

/*
 * Copyright 2013 Andrew Brinker
 */

#include "./FastTable.h"


FastTable::FastTable(std::string new_diskname,
                     std::string new_flat_file,
                     std::string new_index_file)
                   : FileSys(new_diskname),
                     flat_file(new_flat_file),
                     index_file(new_index_file) {
  makeFile(new_flat_file);
  makeFile(new_index_file);
}

void FastTable::buildRoot(Irec r1, Irec r2) {
  if (r1.getKey() > r2.getKey()) {
    Irec temp = r1;
    r1 = r2;
    r2 = temp;
  }

  std::vector<std::string> k;
  std::vector<unsigned int> b;

  // Build the left side
  k.push_back("NU");
  k.push_back(r1.getKey());

  b.push_back(0);
  b.push_back(r1.getBlockID());

  Bnode left(k, b, DEFAULT_BLOCK_SIZE);

  // Empty the vectors
  k.clear();
  b.clear();

  // Build the right side
  k.push_back("NU");
  k.push_back(r2.getKey());

  b.push_back(0);
  b.push_back(r2.getBlockID());

  Bnode right(k, b, DEFAULT_BLOCK_SIZE);

  unsigned int leftBlock  = addBlock("indexfile2", left.getBuffer());
  unsigned int rightBlock = addBlock("indexfile2", right.getBuffer());

  // Clear vectors
  k.clear();
  b.clear();

  // Build the root
  k.push_back("NU");
  k.push_back(r1.getKey());

  b.push_back(leftBlock);
  b.push_back(rightBlock);

  Bnode rt(k, b, DEFAULT_BLOCK_SIZE);

  // NOTE: root is an attribute
  root_block = addBlock("indexfile2", rt.getBuffer());
}


unsigned int FastTable::indexSearch(unsigned int node,
                                    std::string key) {
  // This is how the Search function is modified:
  // int flatfileblockid = searchbtreeindexfile(root, key);
  // in function search(string key).
  // returns the blockid of the index record
  std::string buffer;
  readBlock("indexfile2", node, buffer);
  Bnode c(buffer);
  std::vector<std::string> k = c.getKeys();
  std::vector<unsigned int> b = c.getBlockIDs();

  // External node
  if (b[0] == 0) {
    for (unsigned int i = 1; i < k.size(); ++i) {
      if (k[i] == key) {
        return i;
      }
    }
    return -1;
  } else {
    // Internal node
    unsigned int i = 1;
    while (i < k.size() && key > k[i]) {
      ++i;
    }
    // i-1 is the index of the link to visit
    return indexSearch(b[i - 1], key);
  }
}

/*
Irec FastTable::addBTree(unsigned int node, Irec r) {
  std::string buffer;
  unsigned int error = readBlock("indexfile2", node, buffer);

  Bnode c(buffer);

  std::vector<std::string> k = c.getKeys();
  std::vector<unsigned int> b = c.getBlockIDs();

  // External node
  if (b[0] == 0) {
    return addExternNode(node, r);
  } else {
    // Internal node
    unsigned int i = 1;
    while (i < k.size() && key > k[i]) {
      ++i;
    }

    // i-1 is the index of the link to visit
    Irec r2 = addBTree(b[i - 1], r);
    addInternNode(node, r);
  }
}

Irec FastTable::addExternNode(unsigned int node, Irec r) {
  std::string buffer;

  readBlock("indexfile2", node, buffer);

  Bnode b(buffer);

  std::vector<std::string> C = C.getKey();
  std::vector<unsigned int> B = C.getBlockIDs();

  key = r.getKey();
  blockid = r.getBlockID();

  // Add values to the end of the vectors
  C.push_back(key);
  B.push_back(blockid);

  unsigned int i = k.size() -1;

  while (i > 1 && k[i] < k[i - 1]) {
    unsigned int temp = k[i - 1];
    k[i - 1] =  k[i];
    k[i] = temp;
  }

  if (k.size() > maxallowed) {
    // Global variable = 5

    // Left Child
    std::vector<std::string> Lkeys;
    std::vector<unsigned int> Lblocks;

    // First block
    Lkeys.push_back("NU");
    Lblocks.push_back(0);

    // Rest of the blocks
    for (unsigned int i = 1; i < (k.size() / 2); ++i) {
      Lkeys.push_back(k[i]);
      Lblocks.push_back(b[i]);
    }

    Bnode Lchild(Lkeys, Lblocks);

    writeBlock("indexfile2", node, Lchild.getBuffer());

    // Right Child
    std::vector<std::string> Rkeys;
    vector<unsigned int> Rblocks;

    // First block
    Rkeys.push_back("NU");
    Rblocks.push_back(0);

    // Rest of the blocks
    for (unsigned int i = (k.size() / 2); i < k.size(); ++i) {
      Rkeys.push_back(k[i]);
      Rblocks.push_back(b[i]);
    }

    Bnode Rchild(Rkeys, Rblocks);
    unsigned int newnode = addBlock("indexfile2", Rchild.getBuffer());
    unsigned int j = ((k.size / 2) - 1);
    return Irec(k[j], newnode);
  }
}


Irec FastTable::addInternNode(unsigned int node, Irec r) {
  std::string buffer;
  readBlock("indexfile2", node, buffer);
  Bnode b(buffer);

  std::vector<std::string> C = C.getKey();
  std::vector<unsigned int> B = C.getBlockIDs();

  key = r.getKey();
  blockid = r.getBlockID();

  // Add values to the end of the vectors
  C.push_back(key);
  B.push_back(blockid);

  unsigned int i = k.size() - 1;

  while (i > 1 && k[i] < k[i - 1]) {
    unsigned int temp = k[i - 1];
    k[i - 1] =  k[i];
    k[i] = temp;
  }

  if (k.size() > maxallowed) {
    // Global variable = 5
    // Left Child
    std::vector<std::string> Lkeys;
    std::vector<unsigned int> Lblocks;

    for (unsigned int i = 0; i < (k.size() / 2); ++i) {
      Lkeys.push_back(k[i]);
      Lblocks.push_back(b[i]);
    }

    Bnode Lchild(Lkeys, Lblocks);
    writeBlock("indexfile2", node, Lchild.getBuffer());
    // Right Child
    std::vector<std::string> Rkeys;
    std::vector<unsigned int> Rblocks;

    // First block
    Rkeys.push_back("NU");
    Rblocks.push_back((b.size() / 2));

    // Rest of the blocks
    for (unsigned int i = ((k.size() / 2 + 1)); i < k.size(); ++i) {
      Rkeys.push_back(k[i]);
      Rblocks.push_back(b[i]);
    }
    Bnode Rchild(Rkeys, Rblocks);
    unsigned int newnode = addBlock("indexfile2", Rchild.getBuffer());
    unsigned int j = (k.size / 2);
    return Irec(k[j], newnode);
  }
}


void FastTable::buildBTree() {
  // You need to get the index records for the next line;
  std::vector<Irec> indexrecords;
  BuildRoot(indexrecords[0], indexrecords[1]);

  for (unsigned int i = 2; i < indexrecords.size(); ++i) {
    Irec r = addbtree(root_block, indexrecords[i]);

    if (r.getblockid() != 0)
      Buildnewroot(r);
    }

    std::vector<std::string> k;
    std::vector<unsigned int> b;

    k.push_back("NU");
    b.push_back(root_block);

    k.push_back(r.getKey());
    b.push_back(r.getBlockID());

    Bnode C(k, b);

    root_block = addBlock("indexfile2", C.getBuffer());
}

void FastTable::BuildNewRoot(Irec r) {
  std::vector<std::string> k;
  std::vector<unsigned int> b;
  // Build the root
  k.push_back("NU");
  k.push_back(r.getkey());

  b.push_back(this->root_block());
  b.push_back(r.getblockid());

  Bnode rt(k, b, getblocksize());

  // NOTE: root is an attribute
  root_block = addBlock("indexfile2", rt.getBuffer());
}
*/

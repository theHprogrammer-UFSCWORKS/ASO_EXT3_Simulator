//
//  sha256.hpp
//  t22
//
//  Created by Martin on /29/519.
//  Copyright © 2019 Martin. All rights reserved.
//
//  Adapted by Helder Henrique on 11/15/2022
//  "With the update of the openssl library to its version 3.1, the features of version 1.1.1 became obsolete. Although they can still be used,
//  it is advisable that these resources be adapted. More details can be seen at (https://www.openssl.org/docs/man1.1.1/man3/SHA256.html)"

#ifndef sha256_hpp
#define sha256_hpp

// ----------------------------------------------------------------------------------------------------------------------------------------------

// Para usar a versão com a biblioteca openssl/sha.h
// #include <stdio.h>
// #include <openssl/sha.h>
// #include <openssl/bio.h>
// #include <string>
// #include <iostream>
// #include <sstream>

// ----------------------------------------------------------------------------------------------------------------------------------------------

// Versão usando a biblioteca openssl/evp.h
#include <openssl/evp.h>
#include <string>

// ----------------------------------------------------------------------------------------------------------------------------------------------

// Função que retorna o hash SHA256 de um arquivo
std::string printSha256(const char *path);

#endif /* sha256_hpp */

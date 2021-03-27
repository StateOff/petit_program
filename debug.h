//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_DEBUG_H
#define PETIT_PROGRAM_DEBUG_H

#define PETIT_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define PETIT_ASSERT(value) if(!(value)) { printf("TEST FAILED at %s:%d: %s\n", PETIT_FILENAME, __LINE__, #value); return false; }

#endif//PETIT_PROGRAM_DEBUG_H

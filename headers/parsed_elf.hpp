#pragma once
#ifndef _PARSED_ELF_H

#include <elf.h>
#include <cstdio>


class ParsedElf 
{
    FILE* _f;
    int   _bitness;
    Elf64_Ehdr _ehdr;
    Elf64_Shdr* _sections;
    Elf64_Phdr* _segments;
    void* _symbols;

    void parseEhdr();
    void parseSections();
    void parseSegments();
public:
    ParsedElf(char* path);
    int getBitness() const;
    const Elf64_Ehdr* getEhdr() const;
    const Elf64_Phdr* getSegments() const;
    const Elf64_Shdr* getSections() const;
};

#endif
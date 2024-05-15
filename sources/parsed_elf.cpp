#include "parsed_elf.hpp"
#include <elf.h>
#include <cstring>

ParsedElf::ParsedElf(char* path)
{
    _f = fopen(path, "r");
    parseEhdr();
    parseSections();
    parseSegments();
}

void ParsedElf::parseEhdr()
{
    Elf32_Ehdr ehdr32;

    fread(&_ehdr, 1, EI_NIDENT, _f);

    _bitness = _ehdr.e_ident[EI_CLASS];

    if (_bitness == ELFCLASS64)
    {
        fread(&_ehdr + EI_NIDENT, 1, sizeof(Elf64_Ehdr) - EI_NIDENT, _f);
    }
    else if (_bitness == ELFCLASS32)
    {
        fread(&ehdr32  + EI_NIDENT, 1, sizeof(Elf32_Ehdr) - EI_NIDENT , _f);
        _ehdr.e_type = ehdr32.e_type;
        _ehdr.e_machine = ehdr32.e_machine;
        _ehdr.e_version = ehdr32.e_version;
        _ehdr.e_entry = ehdr32.e_entry;
        _ehdr.e_phoff = ehdr32.e_phoff;
        _ehdr.e_shoff = ehdr32.e_shoff;
        _ehdr.e_flags = ehdr32.e_flags;
        _ehdr.e_ehsize = ehdr32.e_ehsize;
        _ehdr.e_phentsize = ehdr32.e_phentsize;
        _ehdr.e_phnum = ehdr32.e_phnum;
        _ehdr.e_shentsize = ehdr32.e_shentsize;
        _ehdr.e_shnum = ehdr32.e_shnum;
        _ehdr.e_shstrndx = ehdr32.e_shstrndx;

        // Fields that differ in size need to be handled appropriately
        _ehdr.e_entry = (Elf64_Addr)ehdr32.e_entry;
        _ehdr.e_phoff = (Elf64_Off)ehdr32.e_phoff;
        _ehdr.e_shoff = (Elf64_Off)ehdr32.e_shoff;

    }
}

void ParsedElf::parseSections()
{
    _sections = new Elf64_Shdr[_ehdr.e_shnum];
    Elf32_Shdr *sections32 = NULL;


    if (_bitness == ELFCLASS64)
    {
        fseek(_f, _ehdr.e_shoff, SEEK_SET);
        fread(_sections, sizeof(Elf64_Shdr), _ehdr.e_shnum, _f);
    }
    else if (_bitness == ELFCLASS32)
    {
        sections32 = new Elf32_Shdr[_ehdr.e_shnum];
        fseek(_f, _ehdr.e_shoff, SEEK_SET);
        fread(_sections, sizeof(Elf32_Shdr), _ehdr.e_shnum, _f);       

        for (int i = 0; i < _ehdr.e_shnum; i++)
        {
            _sections[i].sh_name = sections32[i].sh_name;
            _sections[i].sh_type = sections32[i].sh_type;
            _sections[i].sh_flags = sections32[i].sh_flags;
            _sections[i].sh_addr = sections32[i].sh_addr;
            _sections[i].sh_offset = sections32[i].sh_offset;
            _sections[i].sh_size = sections32[i].sh_size;
            _sections[i].sh_link = sections32[i].sh_link;
            _sections[i].sh_info = sections32[i].sh_info;
            _sections[i].sh_addralign = sections32[i].sh_addralign;
            _sections[i].sh_entsize = sections32[i].sh_entsize;

            // Fields that differ in size need to be handled appropriately
            _sections[i].sh_addr = static_cast<Elf64_Addr>(sections32[i].sh_addr);
            _sections[i].sh_offset = static_cast<Elf64_Off>(sections32[i].sh_offset);
            _sections[i].sh_size = static_cast<Elf64_Xword>(sections32[i].sh_size);
            _sections[i].sh_flags = static_cast<Elf64_Xword>(sections32[i].sh_flags);
            _sections[i].sh_addralign = static_cast<Elf64_Xword>(sections32[i].sh_addralign);
            _sections[i].sh_entsize = static_cast<Elf64_Xword>(sections32[i].sh_entsize);

        } 
        delete[] sections32;
    }

}


void ParsedElf::parseSegments()
{
    _segments = new Elf64_Phdr[_ehdr.e_phnum];
    Elf32_Phdr *segments32 = nullptr;

    if (_bitness == ELFCLASS64)
    {
        fseek(_f, _ehdr.e_phoff, SEEK_SET);
        fread(_segments, sizeof(Elf64_Phdr), _ehdr.e_phnum, _f);
    }
    else if (_bitness == ELFCLASS32)
    {
        segments32 = new Elf32_Phdr[_ehdr.e_phnum];
        fseek(_f, _ehdr.e_phoff, SEEK_SET);
        fread(segments32, sizeof(Elf32_Phdr), _ehdr.e_phnum, _f);

        for (int i = 0; i < _ehdr.e_phnum; ++i)
        {
            _segments[i].p_type = segments32[i].p_type;
            _segments[i].p_offset = static_cast<Elf64_Off>(segments32[i].p_offset);
            _segments[i].p_vaddr = static_cast<Elf64_Addr>(segments32[i].p_vaddr);
            _segments[i].p_paddr = static_cast<Elf64_Addr>(segments32[i].p_paddr);
            _segments[i].p_filesz = static_cast<Elf64_Xword>(segments32[i].p_filesz);
            _segments[i].p_memsz = static_cast<Elf64_Xword>(segments32[i].p_memsz);
            _segments[i].p_flags = segments32[i].p_flags;
            _segments[i].p_align = static_cast<Elf64_Xword>(segments32[i].p_align);
        }

        delete[] segments32;
    }
}

// Copy fields that have the same size and meaning



















































int ParsedElf::getBitness() const
{
    return _bitness;
}

const Elf64_Ehdr* ParsedElf::getEhdr() const
{
    return &_ehdr;
}

const Elf64_Phdr* ParsedElf::getSegments() const
{
    return _segments;
}
const Elf64_Shdr* ParsedElf::getSections() const
{
    return _sections;
}


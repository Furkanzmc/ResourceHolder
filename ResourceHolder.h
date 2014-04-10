#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include "physfs.h"
#include "SFML/System/InputStream.hpp"

/**
 * @brief A custom sf::InputStream to handle input from Physfs. You don't need to call PHYSFS_init separetely
 * this class handles everything from initializing to deinitializing Physfs. This class is just an edited version of
 * Laurent Gomila's. (Source: https://github.com/LaurentGomila/SFML/wiki/Source%3A-PhysicsFS-Input-Stream)
 * Dependencies: physfs.lib (physfs-d.lib for Debug build), zlib.lib and advapi32.lib Also don't forget to copy the *.dll files to the
 * *.exe file path (physfs.dll (physfs-d.dll for Debug build), zlib1.dll)
 */
namespace zmc
{
class PhysFsStream : public sf::InputStream
{
public:
    PhysFsStream(const char *compressedFilePath = 0x0)
        : mFile(0x0)
    {
        PHYSFS_init(NULL);
        if (compressedFilePath)
            addSearchPath(compressedFilePath);
    }

    virtual ~PhysFsStream()
    {
        PHYSFS_deinit();
        close();
    }

    void deInitializePhysfs()
    {
        close();
        PHYSFS_deinit();
    }

    void initializePhysfs()
    {
        PHYSFS_init(NULL);
    }

    //Adds a compressed file to Physfs's search path
    void addSearchPath(const char *compressedFilePath)
    {
        PHYSFS_addToSearchPath(compressedFilePath,  1);
    }

    //Returns the currently opened file name
    std::string getOpenFileName()
    {
        return mLastopenFileName;
    }

    bool isOpen() const
    {
        return (mFile != 0x0);
    }

    bool open(const char* filename)
    {
        close();
        mFile = PHYSFS_openRead(filename);
        mLastopenFileName = filename;
        return isOpen();
    }

    void close()
    {
        if (isOpen())
            PHYSFS_close(mFile);
        mFile = 0x0;
    }

    virtual sf::Int64 read(void* data, sf::Int64 size)
    {
        if (!isOpen())
            return -1;

        // PHYSFS_read returns the number of 'objects' read or -1 on error.
        // We assume our objects are single bytes and request to read size
        // amount of them.
        return PHYSFS_read(mFile, data, 1, static_cast<PHYSFS_uint32>(size));
    }

    virtual sf::Int64 seek(sf::Int64 position)
    {
        if (!isOpen())
            return -1;

        // PHYSFS_seek return 0 on error and non zero on success
        if (PHYSFS_seek(mFile, position))
            return tell();
        else
            return -1;
    }

    virtual sf::Int64 tell()
    {
        if (!isOpen())
            return -1;
        // PHYSFS_tell returns the offset in bytes or -1 on error just like SFML wants.
        return PHYSFS_tell(mFile);
    }

    virtual sf::Int64 getSize()
    {
        if (!isOpen())
            return -1;
        // PHYSFS_fileLength also the returns length of file or -1 on error just like SFML wants.
        return PHYSFS_fileLength(mFile);
    }

private:
    PHYSFS_File* mFile;
    std::string mLastopenFileName;
};

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    /**
     * @brief Use this if you want to load textures with smooth parameter true, any other resource would create a problem with this
     * function
     * @param id
     * @param filename
     * @param isSmooth
     */
    void load(Identifier id, const std::string& filename, bool isSmooth);
    void load(Identifier id, const std::string& filename);
    template <typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);
    void loadFromStream(Identifier id, PhysFsStream &stream, bool isSmooth = false);
    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;
    bool remove(Identifier id);

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
}
#include "ResourceHolder.inl"
#endif // RESOURCEHOLDER_HPP

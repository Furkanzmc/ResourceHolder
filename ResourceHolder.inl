namespace zmc
{
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, bool isSmooth)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    resource->setSmooth(isSmooth);
    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::loadFromStream(Identifier id, PhysFsStream &stream, bool isSmooth)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromStream(stream))
        throw std::runtime_error("ResourceHolder::load - Failed to load from stream - " + stream.getOpenFileName());

    if (isSmooth == true)
        resource->setSmooth(true);
    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
bool ResourceHolder<Resource, Identifier>::remove(Identifier id)
{
    auto found = mResourceMap.find(id);
    //If the texture doesn't exist
    if (found == mResourceMap.end())
        return false;
    mResourceMap.erase(id);
    return true;
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    //If the resource already exist, std::map simply ignores it
    mResourceMap.insert(std::make_pair(id, std::move(resource)));
}
}

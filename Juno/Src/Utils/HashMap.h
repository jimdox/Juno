#pragma once

namespace Juno
{
template<class K, class V>
class HashNode
{
public:
    HashNode(K key, V val) : key(key), val(val)
    {

    }




private:
    uint32_t hash;
    K key;
    V val;
};


template<class K, class V>
class HashMap
{
public:
    HashMap();

private:
    std::vector<std::vector<HashNode<K, V>>> buckets(10, std::vector<HashNode>(1));
};
}
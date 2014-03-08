#ifndef INDEXKEYMAP_H
#define INDEXKEYMAP_H

#include <QtCore>

template<typename K, typename V> class IndexKeyMap
{
    private:
        QList<K> _keymap;
        QMap<K, V> _map;

    public:
        IndexKeyMap() { }

        void insert(K key, V value)
        {
            this->_keymap.append(key);
            this->_map.insert(key, value);
        }

        bool isEmpty() const
        {
            return this->_map.empty();
        }

        void clear()
        {
            this->_keymap.clear();
            this->_map.clear();
        }

        bool contains(K key) const
        {
            return this->_map.contains(key);
        }

        qint64 length() const
        {
            return this->_map.count();
        }

        void sortIndexes()
        {
            qSort(this->_keymap.begin(), this->_keymap.end());
        }

        K key(qint64 i) const
        {
            return this->_keymap[i];
        }

        V value(qint64 i) const
        {
            return this->_map.value(this->_keymap[i]);
        }

        V valueFromKey(K k) const
        {
            return this->_map.value(k);
        }

        V& valueRefFromKey(K k)
        {
            return this->_map[k];
        }

        V& valueRef(qint64 i)
        {
            return this->_map[this->_keymap[i]];
        }

        V& operator[](const K& k)
        {
            return this->_map[k];
        }
};

#endif // INDEXKEYMAP_H

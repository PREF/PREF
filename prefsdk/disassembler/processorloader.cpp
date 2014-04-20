#include "processorloader.h"

namespace PrefSDK
{
    ProcessorLoader::ProcessorLoader()
    {

    }

    void ProcessorLoader::addSegment(QString name, DisassemblerSegment::SegmentType segmenttype, quint64 startoffset, quint64 endoffset, quint64 baseaddress)
    {
        this->_segments.append(DisassemblerSegment(baseaddress, startoffset, endoffset, name, segmenttype));
    }

    void ProcessorLoader::addEntryPoint(QString name, quint64 address)
    {
        this->_entries.append(EntryPair(name, address));
    }

    quint64 ProcessorLoader::entryAddress(int i)
    {
        return this->_entries[i].second;
    }

    QString ProcessorLoader::entryName(int i)
    {
        return this->_entries[i].first;
    }

    int ProcessorLoader::entryCount()
    {
        return this->_entries.count();
    }

    DisassemblerSegment &ProcessorLoader::segment(quint64 address)
    {
        for(int i = 0; i < this->_segments.count(); i++)
        {
            if((address >= this->_segments[i].start()) && (address < this->_segments[i].end()))
                return this->_segments[i];
        }

        throw std::runtime_error("Segment Not Found");
    }

    bool ProcessorLoader::inSegment(quint64 address)
    {
        for(int i = 0; i < this->_segments.count(); i++)
        {
            if((address >= this->_segments[i].start()) && (address < this->_segments[i].end()))
                return true;
        }

        return false;
    }
}

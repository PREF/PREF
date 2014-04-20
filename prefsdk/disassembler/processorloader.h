#ifndef PREFSDK_PROCESSORLOADER_H
#define PREFSDK_PROCESSORLOADER_H

#include "disassemblersegment.h"

namespace PrefSDK
{
    class ProcessorLoader
    {
        private:
            typedef QPair<QString, quint64> EntryPair;

        public:
            ProcessorLoader();
            void addSegment(QString name, DisassemblerSegment::SegmentType segmenttype, quint64 startoffset, quint64 endoffset, quint64 baseaddress);
            DisassemblerSegment& segment(quint64 address);
            bool inSegment(quint64 address);
            void addEntryPoint(QString name, quint64 address);
            quint64 entryAddress(int i);
            QString entryName(int i);
            int entryCount();

        private:
            QVector<DisassemblerSegment> _segments;
            QVector<EntryPair> _entries;
    };
}

#endif // PREFSDK_PROCESSORLOADER_H

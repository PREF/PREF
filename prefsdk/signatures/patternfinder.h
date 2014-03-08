#ifndef PREFSDK_PATTERNFINDER_H
#define PREFSDK_PATTERNFINDER_H

#include "viewmodels/filefiltermodel/filefiltermodel.h"

namespace PrefSDK
{
    class PatternFinder
    {
        public:
            PatternFinder(FileFilterModel* filefiltermodel);
            const QByteArray& startPattern() const;
            bool analyze(QByteArray &sp);
            bool analyze();

        private:
            void analyze(QByteArray &startpattern, int idx);
            QByteArray readFile(int i, qint64 len = -1);
            void updateStartPattern(QByteArray &startpattern, qint64 startoffset, qint64 endoffset, int i);

        private:
            FileFilterModel* _filefiltermodel;
            QByteArray _startpattern;
    };
}

#endif // PREFSDK_PATTERNFINDER_H

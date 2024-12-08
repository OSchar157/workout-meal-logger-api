#ifndef FITNESS_LOG_H
#define FITNESS_LOG_H

#include <vector>

template <typename T>
class Log {
public:
    void addEntry(T entry) { entries.push_back(entry); }
    std::vector<T> getEntries() { return entries; }

protected:
    std::vector<T> entries;
};

#endif

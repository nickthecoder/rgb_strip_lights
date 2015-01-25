#ifndef HEADER_DATA
#define HEADER_DATA


int sequenceCount();

boolean load();

void save();

void reset();

void saveSequence( int index, Sequence* pSequence );

void loadSequence( int index );

void dump();

#endif


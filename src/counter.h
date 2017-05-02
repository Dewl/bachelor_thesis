#ifndef COUNTER_H
#define COUNTER_H

#include <list>
#include "blob.h"

typedef list<Blob>::const_iterator lBci;
typedef list<Point>::const_reverse_iterator lPcri;

using namespace std;

class Counter {
	private:
		int getPathValue(const Point& point);
	public:
		int lowerCount;
		int upperCount;
		bool hor;
		int bound1;
		int bound2;

		Counter(bool _hor, int _bound1, int _bound2);
		void processBlob(const Blob& blob);
		void receive(const list<Blob>& blobs);
};

#endif /* !COUNTER_H */

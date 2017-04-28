#ifndef TRACKER_H
#define TRACKER_H

#include <list>

#include "blob.h"

using namespace std;

class Tracker {
	public:
		double thres = 20;
		list<Blob> blobs;

		void receive(const list<Blob> inputBlobs);
};

#endif /* !TRACKER_H */

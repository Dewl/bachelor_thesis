#ifndef TRACKER_H
#define TRACKER_H

#include <list>

#include "blob.h"

using namespace std;

class Tracker {
	public:
		double thres = 50.0;
		list<Blob> blobs;
		list<Blob> lostBlobs;

		void receive(list<Blob> inputBlobs);
};

#endif /* !TRACKER_H */

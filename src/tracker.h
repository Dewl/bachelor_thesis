#ifndef TRACKER_H
#define TRACKER_H

#include <list>

#include "blob.h"

using namespace std;

class Tracker {
	public:
		int thres;
		list<Blob> blobList;
		Tracker(int _thres);
		void receive(const list<Blob> blobs);
}

#endif /* !TRACKER_H */

#include "counter.h"

Counter::Counter(bool _hor, int _bound1, int _bound2)
{
	hor = _hor;
	bound1 = _bound1;
	bound2 = _bound2;
	lowerCount = 0;
	upperCount = 0;
}

void Counter::receive(const list<Blob>& blobs)
{
	for (lBci it = blobs.cbegin(); it != blobs.cend(); ++it) {
		processBlob(*it);
	}
}

void Counter::processBlob(const Blob& blob)
{
	lPcri it = blob.path.crbegin();
	int initial = getPathValue(blob.path.back());
	if (initial >= bound2) {
		while (it != blob.path.crend()) {
			if (getPathValue(*it) < bound2) {
				upperCount += blob.estimation;
				// TODO: someone passing the line
				cout << "debug:counter:upper line" << endl;
				break;
			}
			++it;
		}
	} else if (initial <= bound1) {
		while (it != blob.path.crend()) {
			if (getPathValue(*it) > bound1) {
				// TODO: someone passing the line
				lowerCount += blob.estimation;
				cout << "debug:counter:lower line" << endl;
				break;
			}
			++it;
		}
	}
}

int Counter::getPathValue(const Point& point)
{
	if (hor) {
		return point.x;
	}

	return point.y;
}

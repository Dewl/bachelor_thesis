#include "drawer.h"
#include "const.h"

static void drawer_FillRectColor(Mat& canvas,
		const Rect& area,  const Scalar& color);

void drawer_DrawSensor(Mat& canvas, Sensor* _s)
{
	line(
			canvas,
			Point(_s->x - _s->w, _s->y),
			Point(_s->x + _s->w, _s->y),
			COLOR_GREEN
			);

	for (int i = _s->x - _s->w; i <= _s->x + _s->w; i += _s->divx) {
		line(
				canvas,
				Point(i, _s->y - _s->h),
				Point(i, _s->y + _s->h),
				COLOR_GREEN
			);
	}

	for (int i = 0; i < _s->no_str * 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			int rectPoints[4];
			sensor_GetRect(_s, i, j, rectPoints);
			Rect rect(
					Point(rectPoints[0], rectPoints[1]),
					Point(rectPoints[2], rectPoints[3])
					);
			if (_s->cell_state[i][j]) {
				drawer_FillRectColor(canvas, rect, COLOR_CYAN);
			} if (_s->cell_pre_state[i][j]) {
				drawer_FillRectColor(canvas, rect, COLOR_YELLOW);
			}
		}
	}

	for (int i = 0; i < _s->no_str * 2; ++i) {
	}
}

static void drawer_FillRectColor(Mat& _canvas,
		const Rect& _area,  const Scalar& _color)
{
	Mat roi = _canvas(_area);
    Mat color(roi.size(), CV_8UC3,_color); 
    double alpha = 0.4;
    addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
}

#ifndef FFMPEG_H
#define FFMPEG_H

extern "C" {

/*#define INT64_C
#define __STDC_CONSTANT_MACROS
#include <stdint.h>*/

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
//#include "libavformat/riff.h"
//#include "libavformat/metadata.h"
//#include "libavformat/utils.h"
#include "libavutil/rational.h"
//#include "options.h"
#include "libavutil/avstring.h"
//#include "libavutil/internal.h"
#include "libswscale/swscale.h"
}

#endif // FFMPEG_H

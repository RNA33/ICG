
#include <time.h>
#include "ICGStatic.h"

ICG ICGStatic :: icg ( 15485863, 213, 64, time ( NULL ) % 15485863 );


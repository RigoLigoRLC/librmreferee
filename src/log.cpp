
#include "librmreferee/log.h"

namespace RMReferee {
    namespace Log {
        TSTREAM* g_logStream = nullptr;

        void SetLogStream(TSTREAM* stream) {
            g_logStream = stream;
        }

        TSTREAM& Log() {
#ifdef STD_STREAM_LOGGER
            static std::ostream devnull(0);
            if (!g_logStream)
                return devnull;
            return *g_logStream;
#else
            return DummyStream::Instance();
#endif
        }
    };
};
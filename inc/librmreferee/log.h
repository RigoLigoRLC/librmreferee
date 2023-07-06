
#include <ios>
#ifdef STD_STREAM_LOGGER
#include <ostream>
#include <iostream>
#endif

namespace RMReferee {
    namespace Log {
#ifdef STD_STREAM_LOGGER
        using TSTREAM = std::ostream;

        static constexpr decltype(std::hex) &hex = std::hex;
        static constexpr decltype(std::dec) &dec = std::dec;
#else
        class DummyStream {
        public:
            static DummyStream& Instance() {
                static DummyStream instance;
                return instance;
            }

            template<typename T>
            DummyStream& operator<<(const T&) {
                return *this;
            }
        };

        using TSTREAM = RMReferee::DummyStream;

        constexpr int hex = 0;
        constexpr int dec = 0;
#endif
        extern TSTREAM* g_logStream;
        
#ifdef STD_STREAM_LOGGER
        void SetLogStream(TSTREAM* stream);
#endif

        TSTREAM& Log();
    };
};

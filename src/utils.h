#pragma once

namespace aquahash {
    template <typename T1, typename T2> T1 convert(T2 h) {
        T1 result;
        memcpy(&result, &h, sizeof(T1));
        return result;
    }

    

} // namespace aquahash

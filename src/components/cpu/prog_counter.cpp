#include "prog_counter.h"

void PROG_COUNTER::main() {
    if (rst.read()) {
        reg = 0;
    } else {
        if (pcSrc.read() == 0)
            reg += 4;
        else
            reg += immB.read().to_uint();
    }
    addr.write(reg);
}



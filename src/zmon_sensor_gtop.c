#include <zmon.h>
int main(int argc, char **argv) {

    zmon_gtop_t *self = zmon_gtop_new ("");
    assert (self);
    int n = 5;
    for (int i = 0; i < n; i++) {
        zmon_gtop_update (self);
//        zmon_gtop_dump (self);
        zclock_sleep (500);
    }

    zmon_gtop_destroy (&self);
    return 0;
}

#include <czmq.h>
int main(int argc, char **argv) {

    zactor_t *mon_pub = zactor_new (zgossip, "mon_pub");
    assert (mon_pub);
    zstr_sendx (mon_pub, "SET", "server/animate", "0", NULL);
    zstr_sendx (mon_pub, "BIND", "tcp://*:5670", NULL);

    zactor_t *mon_node = zactor_new (zgossip, "mon_node");
    assert (mon_node);
    zstr_sendx (mon_node, "SET", "server/animate", "0", NULL);
    zstr_sendx (mon_node, "BIND", "inproc://mon_node", NULL);
    zstr_sendx (mon_node, "CONNECT", "tcp://localhost:5670", NULL);

    zactor_t *mon_node_sensor_cpu = zactor_new (zgossip, "mon_node_sensor_cpu");
    assert (mon_node_sensor_cpu);
    zstr_sendx (mon_node_sensor_cpu, "SET", "server/animate", "0", NULL);
    zstr_sendx (mon_node_sensor_cpu, "CONNECT", "inproc://mon_node", NULL);

    zactor_t *mon_node_sensor_mem = zactor_new (zgossip, "mon_node_sensor_mem");
    assert (mon_node_sensor_mem);
    zstr_sendx (mon_node_sensor_mem, "SET", "server/animate", "0", NULL);
    zstr_sendx (mon_node_sensor_mem, "CONNECT", "inproc://mon_node", NULL);

    zactor_t *mon_node_sensor_net = zactor_new (zgossip, "mon_node_sensor_net");
    assert (mon_node_sensor_net);
    zstr_sendx (mon_node_sensor_net, "SET", "server/animate", "0", NULL);
    zstr_sendx (mon_node_sensor_net, "CONNECT", "inproc://mon_node", NULL);


    zstr_sendx (mon_node_sensor_cpu, "PUBLISH", "cpu", "83.5", NULL); //  83.5 %
    zstr_sendx (mon_node_sensor_mem, "PUBLISH", "mem", "1024", NULL); //  1 kB
    zstr_sendx (mon_node_sensor_net, "PUBLISH", "net", "500", NULL);  // 500 b/s

    zclock_sleep (1000);

    char *reply;
    zstr_sendx (mon_pub, "STATUS", NULL);
    reply = zstr_recv (mon_pub);
    zclock_log ("mon_pub=%s", reply);
    free (reply);

    zstr_sendx (mon_node, "STATUS", NULL);
    reply = zstr_recv (mon_node);
    zclock_log ("mon_node=%s", reply);
    free (reply);


    zstr_sendx (mon_node_sensor_cpu, "STATUS", NULL);
    reply = zstr_recv (mon_node_sensor_cpu);
    zclock_log ("mon_node_sensor_cpu=%s", reply);
    free (reply);

    zstr_sendx (mon_node_sensor_mem, "STATUS", NULL);
    reply = zstr_recv (mon_node_sensor_mem);
    zclock_log ("mon_node_sensor_mem=%s", reply);
    free (reply);

    zstr_sendx (mon_node_sensor_net, "STATUS", NULL);
    reply = zstr_recv (mon_node_sensor_net);
    zclock_log ("mon_node_sensor_net=%s", reply);
    free (reply);


    zactor_destroy (&mon_node_sensor_cpu);
    zactor_destroy (&mon_node_sensor_mem);
    zactor_destroy (&mon_node_sensor_net);

    zactor_destroy (&mon_node);
    zactor_destroy (&mon_pub);

    return 0;

}

/* /usr/include/libnl3

`pkg-config --cflags --libs libnl-genl-3.0`

userapce send
  kernel recv

kernel send
  userspace recv
*/

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdint.h>
#include <getopt.h>
#include <libaio.h>
#include <stdarg.h>

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nbd-netlink.h>

void err(char *format, ...)
{
        va_list va;
        char buf[1000];

        va_start(va, format);
        vsnprintf(buf, 1000, format, va);
        perror(buf);
        va_end(va);

        exit(-1);
}

static int callback(struct nl_msg *msg, void *arg) {
        struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
        struct nlattr *msg_attr[NBD_ATTR_MAX + 1];
        int ret;
        uint32_t index;

        ret = nla_parse(msg_attr, NBD_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
                        genlmsg_attrlen(gnlh, 0), NULL);
        if (ret)
                err("Invalid response from the kernel\n");
        if (!msg_attr[NBD_ATTR_INDEX])
                err("Did not receive index from the kernel\n");
        index = nla_get_u32(msg_attr[NBD_ATTR_INDEX]);
        printf("Connected /dev/nbd%d\n", (int)index);
        return NL_OK;
}

static struct nl_sock *get_nbd_socket(int *driver_id) {
        struct nl_sock *socket;

        socket = nl_socket_alloc();
        if (!socket)
                err("Couldn't allocate netlink socket\n");

        if (genl_connect(socket))
                err("Couldn't connect to the generic netlink socket\n");
        *driver_id = genl_ctrl_resolve(socket, "nbd");
        if (*driver_id < 0)
                err("Couldn't resolve the nbd netlink family, make sure the nbd module is loaded and your nbd driver supports the netlink interface.\n");
        return socket;
}

static void netlink_configure(int index, int *sockfds, int num_connects,
                              unsigned long size64, int blocksize, uint16_t flags,
                              int timeout) {
        struct nl_sock *socket;
        struct nlattr *sock_attr;
        struct nl_msg *msg;
        int driver_id, i;

        socket = get_nbd_socket(&driver_id);
        nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, callback, NULL);

        msg = nlmsg_alloc();
        if (!msg)
                err("Couldn't allocate netlink message\n");
        genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, driver_id, 0, 0,
                    NBD_CMD_CONNECT, 0);
        if (index >= 0)
                NLA_PUT_U32(msg, NBD_ATTR_INDEX, index);
        NLA_PUT_U64(msg, NBD_ATTR_SIZE_BYTES, size64);
        NLA_PUT_U64(msg, NBD_ATTR_BLOCK_SIZE_BYTES, blocksize);
        NLA_PUT_U64(msg, NBD_ATTR_SERVER_FLAGS, flags);
        if (timeout)
                NLA_PUT_U64(msg, NBD_ATTR_TIMEOUT, timeout);

        sock_attr = nla_nest_start(msg, NBD_ATTR_SOCKETS);
        if (!sock_attr)
                err("Couldn't nest the sockets for our connection\n");
        for (i = 0; i < num_connects; i++) {
                struct nlattr *sock_opt;
                sock_opt = nla_nest_start(msg, NBD_SOCK_ITEM);
                if (!sock_opt)
                        err("Couldn't nest the sockets for our connection\n");
                NLA_PUT_U32(msg, NBD_SOCK_FD, sockfds[i]);
                nla_nest_end(msg, sock_opt);
        }
        nla_nest_end(msg, sock_attr);

        if (nl_send_sync(socket, msg) < 0)
                err("Failed to setup device, check dmesg\n");
        return;
nla_put_failure:
        err("Failed to create netlink message\n");
}

static struct nl_sock *get_my_socket(int *driver_id) {
        struct nl_sock *socket;

        socket = nl_socket_alloc();
        if (!socket)
                err("Couldn't allocate netlink socket\n");

        if (genl_connect(socket))
                err("Couldn't connect to the generic netlink socket\n");
        *driver_id = genl_ctrl_resolve(socket, "my_netlink");
        if (*driver_id < 0)
                err("Couldn't resolve the nbd netlink family, make sure the nbd module is loaded and your nbd driver supports the netlink interface.\n");
        return socket;
}


static int mcast_callback(struct nl_msg *msg, void *arg)
{
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    struct nlattr *msg_attr[NBD_ATTR_MAX + 1];
    int ret;
    uint32_t index, b;

    printf("got mcast\n");

    /*
    if (gnlh->cmd != NBD_CMD_LINK_DEAD)
        return NL_SKIP;
    */
    
    ret = nla_parse(msg_attr, NBD_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
                    genlmsg_attrlen(gnlh, 0), NULL);
    if (ret) {
        err("failed\n");
        return NL_SKIP;
    }

    if (!msg_attr[NBD_ATTR_INDEX]) {
        err("Don't have the index set\n");
        return NL_SKIP;
    }
    index = nla_get_u32(msg_attr[NBD_ATTR_INDEX]);


    b = nla_get_u32(msg_attr[2]);


    printf("got mcast info %d, %d\n", index, b);
    return NL_OK;
}

static void netlink_configure_mcast()
{
	int id;
	struct nl_sock *sock ;
	int mcast_grp;

	sock = get_my_socket(&id);

	mcast_grp = genl_ctrl_resolve_grp(sock, "my_netlink",
					  "my_mcast");
	if (mcast_grp < 0)
		err("Couldn't find mcast\n");

	nl_socket_disable_seq_check(sock);
	nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, mcast_callback, 0);
	nl_socket_add_memberships(sock, mcast_grp, 0);
	while (1)
		nl_recvmsgs_default(sock);
}

static int cmd_callback(struct nl_msg *msg, void *arg) {
        return NL_OK;
}

static void send_cmd()
{
	struct nl_sock *socket;
	int family_id;

	struct nlattr *sock_attr;
        struct nl_msg *msg;


	socket = get_my_socket(&family_id);
	nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, cmd_callback, NULL);

	msg = nlmsg_alloc();
        if (!msg)
                err("Couldn't allocate netlink message\n");
        genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id, 0, 0,
                    1 /*hardcoded*/, 0 /*version*/);

	NLA_PUT_U32(msg, 1, 0x11); /* attr 1 = 0x11 */
        NLA_PUT_U64(msg, 2, 0x22); /* attr 2 = 0x22. attr id is hardcoded here */

	if (nl_send_sync(socket, msg) < 0)
                err("Failed to setup device, check dmesg\n");
        return;
nla_put_failure:
        err("Failed to create netlink message\n");

		
}

int main(int argc, char **argv)
{
	int fds[2] = {5, 0};
	int flag = 0;

	if (flag)
		netlink_configure(0, fds, 1, 1024*1024, 4096, 0, 0);

	send_cmd();

	netlink_configure_mcast();

	return 0;
}

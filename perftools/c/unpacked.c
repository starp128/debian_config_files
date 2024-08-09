#include <stdio.h>
#include <stddef.h>//offsetof
#include <assert.h>
#include "voe_ctl_cmd.h"
#include <linux/vhost_types.h>

//#define offsetof(type, member)  __builtin_offsetof (type, member)


/*
struct test_hdr {
        __u32 a;
        __u64 b;
        __u8 c;
} __attribute__((aligned(128)));
*/

struct test_hdr {
	__u8 d;
	__u16 e;
	__u32 a;
	__u64 b;
	__u8 c;
};



int main(int argc, char **argv)
{
	struct test_hdr hdr;

	assert(sizeof(struct rsp_query) % 4 == 0);
	assert(sizeof(struct rsp_query_instances) % 4 == 0);
	assert(sizeof(struct rsp_query_types) % 4 == 0);

	hdr.a = 10;
	printf("sizeof struct is %lu\n", sizeof(struct test_hdr));

	printf("offset of hdr.a %ld\n", offsetof(struct test_hdr, a));
	printf("offset of hdr.b %ld\n", offsetof(struct test_hdr, b));
	printf("offset of hdr.c %ld\n", offsetof(struct test_hdr, c));
	printf("offset of hdr.c %ld\n", offsetof(struct test_hdr, d));
	printf("offset of hdr.c %ld\n", offsetof(struct test_hdr, e));


	printf("sizeof vhost_iotlb_msg %ld\n", sizeof(struct vhost_iotlb_msg));
	printf("offset vhost_iotbl_msg.perm is %ld\n", offsetof(struct vhost_iotlb_msg, perm));
	printf("offset vhost_iotbl_msg.type is %ld\n", offsetof(struct vhost_iotlb_msg, type));
			
	printf("%d\n", hdr.a);
}

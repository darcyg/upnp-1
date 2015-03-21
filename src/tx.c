/*
 * clang tx.c -o tx -O0 -g -lubus -lubox -lblobmsg_json -levent
 */

#include <libubus.h>
#include <libubox/blobmsg_json.h>

static struct blob_buf b;

int publish(char *ip, char *name)
{
	const char *ubus_socket = "/var/run/ubus.sock";
	struct ubus_context *ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	char msg[128];
	snprintf(msg, 128, "{\"ip\":\"%s\", \"name\":\"%s\"}", ip, name);

	blob_buf_init(&b, 0);
	bool ret = blobmsg_add_json_from_string(&b, msg);
	if (!ret) {
		fprintf(stderr, "fail\n");
		return -2;
	}
	ubus_send_event(ctx, "mqtt-publish", b.head);

	blob_buf_free(&b);
	ubus_free(ctx);
	return 0;
}
